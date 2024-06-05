bits 32

section .multiboot               ;according to multiboot spec
        dd 0x1BADB002            ;set magic number for bootloader
        dd 0x0                   ;set flags
        dd - (0x1BADB002 + 0x0)  ;set checksum

section .text
global start
extern kmain                      ; the C

start:
        cli                      ;block interrupts
		xor ebp, ebp
		mov esp, stack_space        ; set Stack pointer
		push ebx
        call kmain
		    jmp $
    	; hlt                         ; Halt CPU

global gdt_flush
gdt_flush:
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer
   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov fs, ax
   mov gs, ax
   mov es, ax
   mov ax, 0x18
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret

section .bss
    ; resb 8192                         ; stack size : 8KB
    resb 4096                           ; stack size : 4KB
stack_space:
