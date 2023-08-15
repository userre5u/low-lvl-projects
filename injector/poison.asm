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


        ;fn db "/proc/self/maps", 0
		mov rax, 0x007370616d2f666c
		push rax

		mov rax, 0x65732f636f72702f
		push rax

        ; open
        mov rdi, rsp       		; filename
        mov rsi, 0              ; flags (READ ONLY)
        mov rax, 2              ; open syscall
        syscall                 ; call kernel

		add rsp, 16


        ; read
        mov rdi, rax            ; save fd to first arg
        mov rsi, rsp            ; store msg
        mov rdx, 12             ; msg len
        mov rax, 0              ; read syscall
        syscall                 ; call kernel

		
		; convert base address string to number
		mov rcx, 11
		mov r8, 1
		xor rbx, rbx

		is_number:
			xor rax, rax
			mov al, BYTE[rsp+rcx]
			cmp al, 0x3a
			jge is_hexdigit
			sub rax, 0x30
			jmp general
		is_hexdigit:
			sub rax, 87
		general:
			mul r8
			add rbx, rax
			imul r8, r8, 16
			dec rcx
			cmp rcx, 0
			jge is_number

		; rbx contains here the base address in hex
		mov r8, 0x4141414141414141
		add rbx, r8

        ; msg

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

		jmp rbx
