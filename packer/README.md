# mini-packer

Used to decode sequence of bytes in memory during runtime

for now the shellcode and the key are hardcoded (this will be changed...)

usage: make build

# how it works ?
In the data section there is a shellcode (encoded with a key), the program initialized several variables and starts
decoding the encoded shellcode, when its done it jumps to its memory address (the stack).
This requires disabling the 'NX' bit.

* NX bit -> mark the stack as non-executable area (which prevents from our shellcode to be executed)

