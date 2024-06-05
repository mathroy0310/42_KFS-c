/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:34:00 by maroy             #+#    #+#             */
/*   Updated: 2024/06/05 00:45:14 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void outb(unsigned short port, unsigned char value);

#define WHITE_TXT 0x07 /* light gray on black text */

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);
void log(char *str, ...);


void kmain(void)
{
    outb(0x3D4, 0x0A); /*DISABLE CURSOR*/
    outb(0x3D5, 0x20); /*this too*/

    /*log("example du logger %d", 42);*/

    k_clear_screen();
    k_printf("Hello, world! Welcome to KFS-1 test", 0);
    log("Hello, world! Welcome to KFS-1 test");
    while (1)
    {
    }
}

void k_clear_screen()
{
    char *vidmem = (char *)0xb8000;
    unsigned int i = 0;
    while (i < (80 * 25 * 2))
    {
        vidmem[i] = ' ';
        i++;
        vidmem[i] = WHITE_TXT;
        i++;
    };
};

unsigned int k_printf(char *message, unsigned int line)
{
    char *vidmem = (char *)0xb8000;
    unsigned int i = 0;

    i = (line * 80 * 2);

    while (*message != 0)
    {
        if (*message == '\n')
        {
            line++;
            i = (line * 80 * 2);
            message++;
        }
        else
        {
            vidmem[i] = *message;
            message++;
            i++;
            vidmem[i] = WHITE_TXT;
            i++;
        };
    };

    return (1);
}