
x64_assemble:
	@nasm -f elf64 reverse_shell-x64.asm


x64_link: x64_assemble
	@ld reverse_shell-x64.o -o reverse_shell-x64

x64_shell: x64_link
	

a64_assemble:
	@aarch64-linux-gnu-as reverse_shell-a64.s -o reverse_shell-a64.o


a64_link: a64_assemble
	@aarch64-linux-gnu-ld reverse_shell-a64.o -o reverse_shell-a64

a64_shell: a64_link