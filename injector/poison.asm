

section .text
	global _start

	_start:
		push rax
		push rdi
		push rsi
		push rcx
		push rdx
		push r8
		push r9

		mov rax, 0x0a3c3c3c2064656b
		push rax

		mov rax, 0x63616a6968207361
		push rax

		mov rax, 0x77206d6172676f72
		push rax

		mov rax, 0x50203e3e3e090909
		push rax

		mov rdi, 1
		mov rsi, rsp
		mov rdx, 32
		mov rax, 1
		syscall
		
		add rsp, 32
		pop r9
		pop r8
		pop rdx
		pop rcx
		pop rsi
		pop rdi
		pop rax

		mov rbx, 0x4141414141414141
		jmp rbx
