# Dynamic packer

The program creates a packer using generated secret key ( 8 bytes ) and a given filename that contains shellcode
A packer is used to decode sequence of bytes in memory during runtime, which means the shellcode will be placed in memory encooded with a generated key
and will be decoded during the runtime of the program.

# how it works ?
The shellcode part (encoded with a key - random 8 bytes each run) is pushed into the stack, the program gets initialized and starts
decoding the encoded shellcode, when its done it jumps to its memory address.
This requires disabling the 'NX' bit.

* NX bit -> mark the stack as non-executable area (which prevents from our shellcode to be executed)

1) insert the shellcode into a file (you can take the 'payload.txt' as an example, it simple open a local shell, you can view its assembly insturction in 'local_shell.asm' file)
2) make build
3) execute the executable
4) enter filename (which contains the shellcode)
6) the full stub shellcode will be generated



Files: 
*********************************
payload.txt        -> local shell (/bin/sh) ( instructions are in 'local_shell.asm' )

local_shell.asm    -> local shell that executes /bin/sh

depack.asm         -> hardcoded version of 'generate_payload.c'

generate_payload.c -> code for generating payload with a stub

makefile           -> automate the compile process
*********************************
