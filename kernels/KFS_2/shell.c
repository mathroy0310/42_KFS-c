/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:20:28 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:20:28 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void qemu_shutdown(void) { outw(0x604, 0x2000); }
void reboot(void)
{
    unsigned char good = 0x02;

    __asm__ __volatile__("cli");
    while (good & 0x02)
        good = inb(0x64);

    outb(0x64, 0xFE);
    __asm__ __volatile__("hlt");
}

void halt()
{
    // Disable interrupts
    __asm__ __volatile__("cli");

    // Infinite loop to halt the CPU
    while (1)
    {
        __asm__ __volatile__("hlt");
    }
}

void shell(const char *cmd)
{
    if (cmd[0] == '\0')
        return;
    if (strcmp(cmd, "clear") == 0)
    {
        clear();
    }
    else if (strcmp(cmd, "reboot") == 0)
    {
        reboot();
    }
    else if (strcmp(cmd, "halt") == 0)
    {
        halt();
    }
    else if (strcmp(cmd, "shutdown") == 0)
    {
        qemu_shutdown();
    }
    else if (strcmp(cmd, "stack") == 0)
    {
        unsigned int esp;
        unsigned int ebp;
        GET_ESP(esp);
        GET_EBP(ebp);
        printaddr(esp, ebp - esp);
    }
    else if (strcmp(cmd, "stack_test") == 0)
    {
        char test[128] = "ZIG tu me manques";
        unsigned int esp;
        unsigned int ebp;
        GET_ESP(esp);
        GET_EBP(ebp);
        printaddr(esp, ebp - esp);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        printf("List of commands: \n");
        printf("- clear\n");
        printf("- reboot\n");
        printf("- halt\n");
        printf("- shutdown\n");
        printf("- stack\n");
        printf("- stack_test\n");
    }
    else
    {
        printf("Command : %s not found.\n", cmd);
    }
}