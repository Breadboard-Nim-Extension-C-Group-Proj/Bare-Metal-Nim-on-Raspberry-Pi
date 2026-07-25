.section ".text.boot"
.global _start

_start:
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, master_core
halt:
    wfe
    b       halt

master_core:
    // 1. 设置栈指针
    ldr     x1, =_start
    mov     sp, x1

    // 2. 安全清空 .bss 段
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    mov     x3, #0
clear_bss:
    cmp     x1, x2
    b.hs    run_main       // 如果 x1 >= x2 (Higher or Same)，则清零结束，跳出循环
    str     x3, [x1], #8   // 写入 0 并将指针加 8
    b       clear_bss

run_main:
    bl      main
    b       halt