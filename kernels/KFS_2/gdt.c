/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:18:09 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:18:10 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;

} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;

} __attribute__((packed));

#define GDT_ADDRESS 0x00000800
#define GDT_ENTRIES 7

struct gdt_entry gdt_entries[GDT_ENTRIES];
struct gdt_ptr *gp;

extern void gdt_flush(unsigned int gdt_addr);

static void gdt_set_gate(int num, unsigned int base, unsigned int limit,
                         unsigned char access, unsigned char gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

void gdt_init(void)
{
	log("Initialize gdt...");
    gp->limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gp->base = (unsigned int)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                 // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Kernel code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Kernel data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x96, 0xCF);  // Kernel stack

    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User code
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User data
    gdt_set_gate(6, 0, 0xFFFFFFFF, 0xF6, 0xCF);  // User stack

    gdt_flush((unsigned int)(gp));
    log("Initialize gdt DONE");
}