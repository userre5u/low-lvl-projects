asm:
	@nasm -f elf64 poison.asm
	@ld poison.o -o poison

build: asm
	@gcc main.c -o main -w
	@gcc test.c

test: build
	@rm poison.o

clean:
	@rm a.out inject main poison