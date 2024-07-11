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
		
section .bss
    ; resb 8192                         ; stack size : 8KB
    resb 4096                           ; stack size : 4KB
stack_space:
