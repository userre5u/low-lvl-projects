#include "elf.h"
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
char fileError[50] = "Error on target binary file: ";



// global vars
unsigned long int target_binary_size;
unsigned long int poison_size;
void *target_memory_address;
Elf64_Off poison_offset;
Elf64_Addr poison_address;



void fatal_msg(char *msg){
    fprintf(stdout, "%s\n", msg);
    exit(1);
}


void load_target_binary_in_memory(char *target_binary){
    // mmap target binary into memory
    int fd = open(target_binary, O_RDWR);
    if (fd < 0){
        fatal_msg(strcat(fileError, strerror(errno)));
    }
    struct stat buf;
    fstat(fd, &buf);
    target_binary_size = buf.st_size;
    target_memory_address = mmap(0, target_binary_size, PROT_READ | PROT_WRITE, PROT_READ, fd, 0);
    if (target_memory_address == MAP_FAILED){
        fatal_msg(strcat(mmapError, strerror(errno)));
    }
    fprintf(stdout, "[DEBUG] address of target binary: %p\n", target_memory_address); 
}



void load_poison_binary_in_memory(char *poison_binary){
    // load poison binary into the memory heap
    int fd = open(poison_binary, O_RDONLY);
    if (fd < 0){
        fatal_msg(strcat(fileError, strerror(errno)));
    }
    struct stat buf;
    fstat(fd, &buf);
    poison_size = buf.st_size;
    char* poison_address = malloc(sizeof(poison_size));

    fprintf(stdout, "[DEBUG] address of target binary: %p\n", poison_address); 
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
            poison_address = target_binary_program_header->p_vaddr + target_binary_program_header->p_filesz;
            
            target_binary_program_header->p_filesz += poison_size;
            target_binary_program_header->p_memsz += poison_size;
        }
        else if (1 && target_binary_program_header->p_type == PT_LOAD && target_binary_program_header->p_flags == (PF_R | PF_W)){
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


void patch_poison(){
    
}




int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: <%s> <target_binary> <poison_binary>\n", argv[0]);
        exit(0);
    }
    char *target_binary = argv[1];
    char *poison_binary = argv[2];
    load_target_binary_in_memory(target_binary);
    load_poison_binary_in_memory(poison_binary);

    Elf64_Ehdr* target_binary_header = (Elf64_Ehdr*)target_memory_address;
    int pad_size = get_padding_size(target_binary_header);
    
    // save original entry point
    Elf64_Addr original_entry_point = target_binary_header->e_entry;

    // change target binary entry point
    target_binary_header->e_entry = poison_offset;

    patch_section_header(target_binary_header);

    patch_poison(target_binary_header);
    
    
}