
void *memcpy(void *dest, void const *src, int count);
void *memsetw(void *dest, short int val, int count);
int strlen(const char *s);
void outb(unsigned short port, unsigned char value);

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
    unsigned blank, temp;

    /* Background color for blank */
    blank = 0x20 | (attrib << 8);

    if (csr_y >= ROWS)
    {
        temp = csr_y - ROWS + 1;
        memcpy(screens[current_screen].buffer, textmemptr + temp * WIDTH, (ROWS - temp) * WIDTH * 2);

        memsetw(textmemptr + (ROWS - temp) * WIDTH, blank, WIDTH);
        csr_y = ROWS - 1;
    }
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

void print(char *text)
{
    int i;
    for (i = 0; i < strlen(text); i++)
        putc(text[i], current_screen);
    memcpy(textmemptr, screens[current_screen].buffer,
           WIDTH * ROWS * sizeof(unsigned short));
    move_csr();
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
    "Welcome to KFS, Use the F1-F2-F3-F4 to switch screen\n"
    "====================================================\n";

/* Function to copy logo data into each screen's buffer */
void init()
{
    for (int i = 0; i < NUM_SCREENS; i++)
    {
        cls(i);
        switch_screen_color(i);
        puts(logo, i);
        screens[i].csr_y = 15;
    }
    switch_screen(0);
}