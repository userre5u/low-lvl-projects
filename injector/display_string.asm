

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

		mov rax, 0x0a2e2e2e2e2e2e2e
		push rax

		mov rax, 0x6e6f697475636578
		push rax

		mov rax, 0x6520676e696d7573
		push rax

		mov rax, 0x657220293a206465
		push rax

		mov rax, 0x7463657269646572
		push rax

		mov rax, 0x20736177206d6172
		push rax

		mov rax, 0x676f727020656874
		push rax

		mov rax, 0x206567617373656d
		push rax

		mov rax, 0x2073696874206565
		push rax

		mov rax, 0x7320756f79206649
		push rax

		mov rdi, 1
		mov rsi, rsp
		mov rdx, 80
		mov rax, 1
		syscall
		
		add rsp, 80
		pop r9
		pop r8
		pop rdx
		pop rcx
		pop rsi
		pop rdi
		pop rax

		mov rbx, 0x4141414141414141
		jmp rbx
