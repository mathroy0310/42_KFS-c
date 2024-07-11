

#include "kernel.h"

char scancode_to_char(unsigned char scancode, int is_shift_pressed)
{
    static char keymap[] = {
        0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
        '9',  '0', '-', '=', '\b',                           /* Backspace */
        '\t',                                                /* Tab */
        'q',  'w', 'e', 'r',                                 /* 19 */
        't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      /* Enter key */
        0, /* 29   - Control */
        'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
        '\'', '`', 0,                                        /* Left shift */
        '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 /* 49 */
        'm',  ',', '.', '/', 0,                              /* Right shift */
        '*',  0,                                             /* Alt */
        ' ',                                                 /* Space bar */
        0,                                                   /* Caps lock */
        0, /* 59 - F1 key ... > */
    };

    static char shift_keymap[128] = {
        0,    27,  '!', '@', '#',  '$', '%', '^', '&',  '*', /* 9 */
        '(',  ')', '_', '+', '\b',                           /* Backspace */
        '\t',                                                /* Tab */
        'Q',  'W', 'E', 'R',                                 /* 19 */
        'T',  'Y', 'U', 'I', 'O',  'P', '{', '}', '\n',      /* Enter key */
        0, /* 29   - Control */
        'A',  'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L',  ':', /* 39 */
        '"',  '~', 0,                                        /* Left shift */
        '|',  'Z', 'X', 'C', 'V',  'B', 'N',                 /* 49 */
        'M',  '<', '>', '?', 0,                              /* Right shift */
        '*',  0,                                             /* Alt */
        ' ',                                                 /* Space bar */
        0,                                                   /* Caps lock */
        0, /* 59 - F1 key ... > */
    };
    if (scancode < 128)
    {
        if (scancode >= 0x3B && scancode <= 0x44)
        {
            int fn_key = (scancode - 0x3A) - 1;
            switch_screen(fn_key);
        }
        if (is_shift_pressed)
            return shift_keymap[scancode];
        else
            return keymap[scancode];
    }
    return 0;
}

void handle_special_keys(unsigned char scancode, int* is_ctrl_pressed,
                         int* is_shift_pressed)
{
    switch (scancode)
    {
        case 0x1D:  // Left Control pressed
            *is_ctrl_pressed = true;
            break;
        case 0x9D:  // Left Control released
            *is_ctrl_pressed = false;
            break;
        case 0x2A:  // Left Shift pressed
        case 0x36:  // Right Shift pressed
            *is_shift_pressed = true;
            break;
        case 0xAA:  // Left Shift released
        case 0xB6:  // Right Shift released
            *is_shift_pressed = false;
            break;
    }
}

unsigned char read_scancode()
{
    while (1)
    {
        if (inb(KEYBOARD_PORT_STATUS) & 0x01)
        {
            return inb(KEYBOARD_PORT_DATA);
        }
    }
}

void move_cursor_right(void);
void move_cursor_left(void);

void get_input(char* buffer, int buffer_limit)
{
    unsigned char scancode;
    int buffer_index = 0;

    int is_shift_pressed = false;
    int is_ctrl_pressed = false;
    while (1)
    {
        scancode = read_scancode();
        handle_special_keys(scancode, &is_ctrl_pressed, &is_shift_pressed);
        unsigned char c = scancode_to_char(scancode, is_shift_pressed);

        // log("scancode = %d", scancode);
        // log("C = %d", c);
        if (c == 0)
        {
            /*left Arrow*/
            if (scancode == 0x4B)
            {
                if (buffer_index > 0)
                {
                    move_cursor_left();
                    buffer_index--;
                }
            }
            /* Right Arrow */
            else if (scancode == 0x4D)
            {
                if (buffer_index < strlen(buffer))
                {
                    move_cursor_right();
                    buffer_index++;
                }
            }
            continue;
        }
        if (is_ctrl_pressed)
        {
            /* CTRL + L */
            if (c == 'l')
            {
                memset(buffer, 0, 256);
                clear();
                return;
            }
        }
        if (c == '\b')  // Handle backspace
        {
            if (buffer_index > 0)
            {
                buffer[--buffer_index] = '\0';
                printstr("\b \b");
            }
        }
        else if (c == '\n')
        {
            buffer[buffer_index] = '\0';
            printchar('\n');
            return;
        }
        else if (buffer_index < 64 - 1)
        {
            if (strlen(buffer) == buffer_limit)
                return;
            buffer[buffer_index++] = c;
            printchar(c);
        }
    }
}

void keyboard_handler(void)
{
    while (1)
    {
        char buffer[64];
        memset(buffer, 0, 64);
        get_input(buffer, 64);
        shell(buffer);
        printchar('>');
    }
}

void init_keyboard(void) { clear(); }