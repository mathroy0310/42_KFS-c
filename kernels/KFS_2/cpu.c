/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:51:42 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:17:58 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void outb(unsigned short port, unsigned char value)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(unsigned short port, unsigned short value)
{
    __asm__ __volatile__("outw %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char inb(unsigned short port)
{
    unsigned char value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void lgdt(void* gdt_ptr)
{
    __asm__ __volatile__(
        "movl %0, %%eax; "      // Move the GDT pointer from the stack into EAX
        "lgdt (%%eax); "        // Load the GDT
        "movw $0x10, %%ax; "    // Load data segment selector into AX
        "movw %%ax, %%ds; "     // Update DS
        "movw %%ax, %%es; "     // Update ES
        "movw %%ax, %%fs; "     // Update FS
        "movw %%ax, %%ss; "     // Update SS
        "movw $0x18, %%ax; "    // Load the GS segment selector into AX
        "movw %%ax, %%gs; "     // Update GS
        "ljmp $0x08, $flush; "  // Far jump to flush the instruction pipeline
        "flush: ret; "          // Return from the function
        :
        : "m"(gdt_ptr)  // Input operand: GDT pointer
        : "eax"         // Clobbered registers
    );
}
