
assemble:
	@nasm -f elf64 reverse_shell.asm 


link: assemble
	@ld reverse_shell.o -o reverse_shell

build: link
	@echo "Done"
