
binary:
	@gcc example.c -o example && ./example

assemble:
	@nasm -f elf64 depack.asm


link: assemble
	@gcc depack.o -z execstack -no-pie -o execute_Me

build: link
	./execute_Me

