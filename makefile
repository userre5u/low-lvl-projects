
assemble:
	@nasm -f elf64 reverse_shell-x64.asm


link: assemble
	@ld reverse_shell-x64.asm -o reverse_shell-x64.asm

build: link
	@echo "Done"
