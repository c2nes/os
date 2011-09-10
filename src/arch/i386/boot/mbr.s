
bits 16
org 0x7C00

; Segment descriptors are 8 bytes each
%define NULL_SEGMENT 0
%define DATA_SEGMENT (8 * 1)
%define CODE_SEGMENT (8 * 2)

; Stack pointer
%define STACK_BASE_ADDRESS 0x7000

; Kernel information
%define KERNEL_SECTOR_COUNT 32
%define KERNEL_LOAD_ADDRESS 0x8000
%define KERNEL_ENTRY_POINT 0x8000

start:
    ; Print loading message
    push loading
    call print_string

    ; Attempt to load the kernel

    ; Initialize registers to read sectors
    mov bx, KERNEL_LOAD_ADDRESS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov di, boot_device_list
.next_device:
    ; int 0x13 clobbers ax
    mov ah, 0x02
    mov al, KERNEL_SECTOR_COUNT
    mov dl, [di]
    int 0x13

    ; Load successful
    cmp ax, (KERNEL_SECTOR_COUNT)
    je .loaded

    ; Bad boot device, try next
    inc di
    cmp di, boot_device_list_end
    jl .next_device

    ; Error
    push boot_error
    call print_string
    hlt

.loaded:
    ; Kernel has been loaded so perform a switch to 32 bit mode, jump to a
    ; 32 bit entry point, perform initialization, and then jump into the
    ; kernel

    push kernel_loaded
    call print_string

    ; Clear interrupts
    cli

    ; Load GDT
    lgdt [gdtr]

    ; Perform switch to protected mode
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    ; Perform a far jump to the 32 bit entry point
    jmp CODE_SEGMENT:start_32

; Print a string to the screen using BIOS interrupt 0x10 
print_string:
    mov bp, sp
    mov si, [bp + 2]
    mov ah, 0x0e
    mov bx, 0xff00
.loop:
    lodsb
    cmp al, 0
    je .break
    int 0x10
    jmp .loop
.break:
    ret 2

; 32 bit entry point
bits 32
start_32:
    ; NOTE: Code segment initialized by jmp call
    ;

    ; Initialize data segment
    mov ax, DATA_SEGMENT
    mov ds, ax

    ; Initialize the stack pointer
    mov ax, DATA_SEGMENT
    mov ss, ax
    mov esp, STACK_BASE_ADDRESS

    ; Initialize other segments with the null descriptor
    mov ax, NULL_SEGMENT
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Jump into the loaded kernel
    mov eax, KERNEL_ENTRY_POINT
    jmp eax

    ; This instrution is never reached
    nop

; Start of the data section
align 8

;
; This specification is copied from the "Intel 64 and IA-32 Architectures
; Software Developer's Manual", "Volume 3A: System Programming Guide, Part
; 1" section 3.4.5 "Segment Descriptors". Refer to it for more information.
;
; -----------------------------------------------------------------------------
; Location  Name    Description
; -----------------------------------------------------------------------------
;  0:15     LIMIT   Segment limit (low)
; 16:31     BASE    Base address (low)
; -----------------------------------------------------------------------------
;  0:7      BASE    Base address (middle)
;  8:11     TYPE    Segment type, see section 3.4.5.1
; 12        S       Descriptor type (0=system, 1=code/data)
; 13:14     DPL     Descriptor privilege Level
; 15        P       Segement present
; 16:19     LIMIT   Segment limit (high)
; 20        AVL     Available for use by system software
; 21        L       64-bit code segment
; 22        D/B     Default operation size (0=16 bit, 1=32 bit)
; 23        G       Granularity of limit (0=1 byte, 1=4096 bytes)
; 24:31     BASE    Base address (high)
; -----------------------------------------------------------------------------
gdt:    ; Null descriptor
        dd 0x00000000
        dd 0x00000000

        ; Data segment
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0b1_00_1_0010
        db 0b1_1_0_0_1111
        db 0x00

        ; Code segment
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0b1_00_1_1010
        db 0b1_1_0_0_1111
        db 0x00

; GDT register as passed to the lgdt instruction
gdtr    dw (gdtr - gdt)
        dd gdt

; Messages
kernel_loaded db 'Kernel loaded...', 13, 10, 0
boot_error    db 'Error', 13, 10, 0
loading       db 'Loading...', 13, 10, 0

; Boot device list (default, first floppy, first hdd, second hdd)
boot_device_list db 0xff, 0x00, 0x80, 0x81
boot_device_list_end:

; Pad out to 512 bytes and insert magic number at end
times (510 - ($-$$)) db 0
        dw 0xAA55
