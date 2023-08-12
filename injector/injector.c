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
unsigned long int target_memory_size;
unsigned long int poison_memory_size;
void *target_memory_address;




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
    target_memory_size = buf.st_size;
    target_memory_address = mmap(0, target_memory_size, PROT_READ | PROT_WRITE, PROT_READ, fd, 0);
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
    poison_memory_size = buf.st_size;
    char* poison_address = malloc(sizeof(poison_memory_size));

    fprintf(stdout, "[DEBUG] address of target binary: %p\n", poison_address); 
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
    
}