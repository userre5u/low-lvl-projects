#include <stdio.h>

int main(int argc, char **argv) {
char code[] = "<GENERATED SHELLCODE HERE>";
 int (*func)();
 func = (int (*)()) code;
 (int)(*func)();
}
