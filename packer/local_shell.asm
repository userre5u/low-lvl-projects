

section .text
	global _start
	
	_start:
		xor rax, rax 		; pull null byte at end of string
		push rax
		mov rax, 0x68732f2f6e69622f ; /bin//sh in LE
		push rax
		mov rdi, rsp
		xor rsi, rsi		; 0 second arg
		xor rdx, rdx 		; 0 third arg
		xor rax, rax
		add rax, 0x3b		; execve systemcall
		syscall
