// fibonacci in ARM64
// TODO: fibonacci in recursion 


.arch armv8-a

.section .text
.global _start

    _start:
    mov x10, #10                     // init var = 10
    mov x3, #1
    eor x15, x15, x15
    mov x16, #1
    mov x9, #1

    print_msg:
        mov x8, #64                 // syscall write
        mov x0, #1                  // stdout
        adr x1, display_msg         // msg
        mov x2, display_msg_len     // msg len
        svc #0                      // call kernel

    request_num:
        mov x0, #0                  // stdin
        adr x1, number
        mov x2, 8
        mov x8, #63                 // syscall read
        svc #0                      // call kernel
    
    b general

    convert_to_int:    
        // 1) convert the string to integer
        stp x29, x30, [sp, #-16] !
        sub x12, x0, #2
        eor x14, x14, x14
        ldr x0, number
        str x0, [sp]
        mov x11, sp
        eor x18, x18, x18
        loop:
            eor x13, x13, x13
            ldrb w13, [x11, x12]
            sub w13, w13, #0x30
            umull x0, w13, w9
            add x18, x18, x0
            mul x9, x9, x10
            sub x12, x12, #1
            subs x12, x12, x14
            bge loop
        mov x0, x18
        ldp x29, x30, [sp], #16
        ret

    
    fibonacci:
        stp x29, x30, [sp, #-16] !
        mov x4, x0
        cmp x0, #1
        beq done
        cmp x0, #0
        beq done
        fibonacci_loop:
            mov x14, x16
            add x16, x16, x15
            mov x15, x14
            add x3, x3, #1
            cmp x3, x4
            bne fibonacci_loop
            mov x0, x16
        done:
        ldp x29, x30, [sp], #16
        ret


    // 2) start fibonacci
    general:
        bl convert_to_int
        adr x1, number
        str x0, [x1]                // store number as int in number
        bl fibonacci

    convert_to_char:
        mov x9, #1
        mov x16, #10
        eor x14, x14, x14
        sub sp, sp, 16
        mov x15, x0
        get_len:
            sdiv x15, x15, x16
            add x14, x14, #1
            cmp x15, #0x00
            bne get_len
        
        mov x10, x0
        mov w13, 0xa
        strb w13, [sp, x14]
        add x15, x14, #1
        store_char:
            sub x14, x14, #1
            udiv x12, x10, x16
            msub x13, x12, x16, x10     // x13 == reminder, x12 == result
            mov x10, x12
            add w13, w13, 0x30
            strb w13, [sp, x14]
            cmp x10, #0x00
            bne store_char
        mov x8, #64                 // syscall write
        mov x0, #1                  // stdout
        mov x1, sp                  // msg
        mov x2, x15                 // msg len
        svc #0                      // call kernel
        add sp, sp, 16   
        
    
    exit:
        // exit 
        mov x0, 0                   // status code
        mov x8, 93                  // syscall exit
        svc #0                      // call kernel



.section .bss
number:
    .space 8
    
        
.section .data

display_msg:
    .ascii  "Enter a number: "
display_msg_len = . - display_msg


