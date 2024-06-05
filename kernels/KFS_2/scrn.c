
void *memcpy(void *dest, void const *src, int count);
void *memsetw(void *dest, short int val, int count);
int strlen(const char *s);
void outb(unsigned short port, unsigned char value);
void printf(char *str, ...);
void move_csr(void);


/* This values are set for 80x25 QEMU mode */
#define WIDTH 80
#define ROWS 25
#define NUM_SCREENS 4


unsigned short *textmemptr = (unsigned short *)0xB8000;

typedef struct
{
    unsigned short buffer[WIDTH * ROWS];
    int csr_x;
    int csr_y;
} Screen;
Screen screens[NUM_SCREENS];
int current_screen = 0;


int attrib = 0x0F;
int csr_x = 0;
int csr_y = 0;

void scroll(void)
{
    unsigned short blank = 0x20 | (attrib << 8);
    int i;

    if (csr_y >= ROWS)
    {
        for (i = 0; i < (ROWS - 1) * WIDTH; i++)
        {
            screens[current_screen].buffer[i] =
                screens[current_screen].buffer[i + WIDTH];
        }
        for (i = (ROWS - 1) * WIDTH; i < ROWS * WIDTH; i++)
            screens[current_screen].buffer[i] = blank;
        csr_y = ROWS - 1;

        memcpy(textmemptr, screens[current_screen].buffer,
               WIDTH * ROWS * sizeof(unsigned short));
    }
}

void move_cursor_right(void)
{
    csr_x++;
    if (csr_x >= WIDTH)
    {
        csr_x = 0;
        csr_y++;
        if (csr_y >= ROWS)
        {
            scroll();
        }
    }
    move_csr();
}

void move_cursor_left(void)
{
    if (csr_x > 0)
    {
        csr_x--;
    }
    else if (csr_y > 0)
    {
        csr_y--;
        csr_x = WIDTH - 1;
    }
    move_csr();
}

void move_csr(void)
{
    unsigned temp;

    temp = csr_y * WIDTH + csr_x;

    // Update the character at the cursor position with the current attribute
    textmemptr[temp] = (textmemptr[temp] & 0x00FF) | (attrib << 8);

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

void cls(int screen_index)
{
    int i = 0;
    for (i = 0; i < WIDTH * ROWS; i++)
        screens[screen_index].buffer[i] = (attrib << 8) | 0x20;
    memcpy(textmemptr, screens[screen_index].buffer,
           WIDTH * ROWS * sizeof(unsigned short));
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

void clear(void)
{
    int i = 0;
    for (i = 0; i < WIDTH * ROWS; i++)
        screens[current_screen].buffer[i] = (attrib << 8) | 0x20;
    memcpy(textmemptr, screens[current_screen].buffer,
           WIDTH * ROWS * sizeof(unsigned short));
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

void putc(char c, int screen_index)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if (c == 0x08)
    {
        if (csr_x != 0)
            csr_x--;

        where = screens[screen_index].buffer + (csr_y * WIDTH + csr_x);
        *where = 0x20 | att; /* Replace with a space and current attributes */
    }
    else if (c == '\t')
        csr_x = (csr_x + 4) & ~(4 - 1);
    else if (c == '\r')
        csr_x = 0;
    else if (c == '\n')
    {
        csr_x = 0;
        csr_y++;
        if (csr_y >= ROWS)
            scroll();
    }
    else if (c >= ' ')
    {
        where = screens[screen_index].buffer + (csr_y * WIDTH + csr_x);
        *where = c | att;
        csr_x++;
        if (csr_x >= WIDTH)
        {
            csr_x = 0;
            csr_y++;
            if (csr_y >= ROWS)
                scroll();
        }
    }
    if (csr_x >= WIDTH)
    {
        csr_x = 0;
        csr_y++;
    }
}

void printstr(char *text)
{
    int i;
    for (i = 0; i < strlen(text); i++)
        putc(text[i], current_screen);
    memcpy(textmemptr, screens[current_screen].buffer,
           WIDTH * ROWS * sizeof(unsigned short));
    move_csr();
}

void printchar(char c)
{
    putc(c, current_screen);
    memcpy(textmemptr, screens[current_screen].buffer,
           WIDTH * ROWS * sizeof(unsigned short));
    move_csr();
}

void itoa(int n, char *str);
int intlen(int n);

void printnbr(int n)
{
    char str[intlen(n) + 1];

    itoa(n, str);
    printstr(str);
}


void puts(char *text, int screen_index)
{
    int i;
    for (i = 0; i < strlen(text); i++)
        putc(text[i], screen_index);
}


void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void switch_screen_color(int new_screen)
{
    switch (new_screen)
    {
        case 0:
            settextcolor(0xF, 0x0);
            break;
        case 1:
            settextcolor(0x5, 0x0);
            break;
        case 2:
            settextcolor(0x2, 0x0);
            break;
        case 3:
            settextcolor(0x3, 0x0);
            break;
        default:
            break;
    }
}

void switch_screen(int new_screen)
{
    if (new_screen >= 0 && new_screen < NUM_SCREENS)
    {
        switch_screen_color(new_screen);
        if (new_screen != current_screen)
        {
            memcpy(screens[current_screen].buffer, textmemptr,
                   WIDTH * ROWS * sizeof(unsigned short));
        }
        screens[current_screen].csr_x = csr_x;
        screens[current_screen].csr_y = csr_y;

        memcpy(textmemptr, screens[new_screen].buffer,
               WIDTH * ROWS * sizeof(unsigned short));
        current_screen = new_screen;

        csr_x = screens[current_screen].csr_x;
        csr_y = screens[current_screen].csr_y;
        move_csr();
    }
}


static char logo[] =
    "====================================================\n"
    "\t ___         .-.             \n"
    "\t(   )       /    \\           \n"
    "\t | |   ___  | .`. ;  .--.    \n"
    "\t | |  (   ) | |(___)  _  \\   \n"
    "\t | |  ' /   | |_  . .' `. ;  \n"
    "\t | |,' /   (   __)| '   | |  \n"
    "\t | .  '.    | |   _\\_`.(___) \n"
    "\t | | `. \\   | |  (   ). '.   \n"
    "\t | |   \\ \\  | |   | |  `\\ |  \n"
    "\t | |    \\ . | |   ; '._,' '  \n"
    "\t(___ ) (___|___)   '.___.'   \n"
    "====================================================\n"
    "Welcome to KFS-2! \n";


void init()
{
    for (int i = 0; i < NUM_SCREENS; i++)
    {
        cls(i);
        switch_screen_color(i);
        puts(logo, i);
        puts("Use the F1-F2-F3-F4 to change TTY. Current:", i);
        putc((char)(i + 1 + '0'), i);
        puts("\n====================================================\n", i);
        putc('>', i);
        screens[i].csr_y = 16;
        screens[i].csr_x = 1;
    }
    switch_screen(0);
}

void hex_to_str(unsigned int value, char *str, unsigned long str_size);

void printaddr(unsigned int addr, int limit)
{
    char *c = (char *)addr;
    char str_addr[9];
    int i;
    unsigned int previous;

    if (limit <= 0)
    {
        printstr("\n");
        return;
    }

    for (i = 0; i < limit; i++)
    {
        // Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0)
        {
            // Not the first line
            if (i != 0)
            {
                previous = addr - 16;
                while (previous < addr)
                {
                    if (*(char *)previous <= 32)
                        printf("%c", '.');
                    else
                        printf("%c", *(char *)previous);
                    previous++;
                }
                printstr("\n");
            }
            // hex_to_str(addr, str_addr, sizeof(str_addr));
            printf("%p: ", addr);
        }
        hex_to_str((unsigned int)c[i], str_addr, 3);
        printf("%s ", str_addr);
        addr++;
    }
    // Last line
    for (i = 0; i < ((limit % 16) * 3); i++)
        printstr(" ");
    if ((limit % 16) == 0)
        previous = addr - 16;
    else
        previous = addr - (limit % 16);
    while (previous < addr)
    {
        if (*(char *)previous <= 32)
            printf("%c", '.');
        else
            printf("%c", *(char *)previous);
        previous++;
    }
    printstr("\n");
}

void hex_to_str(unsigned int value, char *str, unsigned long str_size)
{
    static const char hex_digits[] = "0123456789ABCDEF";
    unsigned long i = str_size - 1;

    // Ensure null termination
    str[i] = '\0';

    // Fill the string with hex digits
    while (i > 0)
    {
        i--;
        str[i] = hex_digits[value & 0xF];
        value >>= 4;
    }
}
