asm:
	@nasm -f elf64 poison.asm
	@ld poison.o -o poison

build: asm
	@gcc main.c -o main -fno-stack-protector -z execstack -w

run: build
	@./main binaries poison
