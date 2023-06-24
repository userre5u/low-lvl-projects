# Dynamic packer

The program takes a filename (that contains a shellcode) and a key as standard inputs 
and generates a packer version of that's shellcode.
A packer is used to decode sequence of bytes in memory during runtime, which means the shellcode will be placed in memory encooded with your key
and will be decoded during runtime of the program. 

# how it works ?
The shellcode part (encoded with a key) is pushed into the stack, the program gets initialized and starts
decoding the encoded shellcode, when its done it jumps to its memory address.
This requires disabling the 'NX' bit.

* NX bit -> mark the stack as non-executable area (which prevents from our shellcode to be executed)

1) insert the shellcode into a file (you can take the 'payload.txt' as an example, it simply opens a shell, you can view its asm insturction in 'local_shell.asm' file)
2) usage: make build
3) execute the executable
4) enter filename
5) enter keyname
6) the full stub shellcode will be generated



Files: 
*********************************
payload            -> local shellcode ( instructions are in 'local_shell.asm' )
local_shell.asm    -> simple local shellcode that executes /bin/sh
depack.asm         -> hardcoded version of 'generate_payload.c'
generate_payload.c -> code for generating payload with a stub
makefile           -> automate the compile process
*********************************
