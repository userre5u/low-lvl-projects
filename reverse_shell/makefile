
assemble:
	@nasm -f elf64 reverse_shell-x64.asm


link: assemble
	@ld reverse_shell-x64.o -o reverse_shell-x64

build: link
	@echo "Done"
