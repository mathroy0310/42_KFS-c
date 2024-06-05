#include <klib/string.h>


void outb(unsigned short port, unsigned char value);

#define DEBUGCON_SERIAL_PORT 0xe9 /*DEBUG CON*/
#define WHITE_TXT 0x07            /* light gray on black text */


void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);


void log(char *str, ...)
{
    int *args;
    char *format;
    int i;

    args = (int *)(&str);
    format = (char *)(*args++);
    i = 0;
    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == 'c')
                outb(DEBUGCON_SERIAL_PORT, *args++);
            else if (format[i] == 's')
            {
                char *s = (char *)(*args++);
                while (*s)
                    outb(DEBUGCON_SERIAL_PORT, *s++);
            }
            else if (format[i] == 'd')
            {
                int num = *args++;
                char buf[32];
                int j = 0;
                if (num < 0)
                {
                    outb(DEBUGCON_SERIAL_PORT, '-');
                    num = -num;
                }
                do
                {
                    buf[j++] = '0' + num % 10;
                    num /= 10;
                } while (num);
                while (j > 0)
                    outb(DEBUGCON_SERIAL_PORT, buf[--j]);
            }
            else
            {
                outb(DEBUGCON_SERIAL_PORT, '%');
                outb(DEBUGCON_SERIAL_PORT, format[i]);
            }
        }
        else
            outb(DEBUGCON_SERIAL_PORT, format[i]);
        i++;
    }
}


void kmain(void)
{
    outb(0x3D4, 0x0A); /*DISABLE CURSOR*/
    outb(0x3D5, 0x20); /*this too*/

    log("example du logger %d", 42);

    k_clear_screen();
    k_printf("Hello, world! Welcome to KFS", 0);

    k_printf("42 42 yo", 5);

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