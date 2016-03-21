
#include "keyboard.h"

// array of lowercase ASCII codes, indexed by the corresponding characters scan code
uint8_t code_to_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, ASCII_1, ASCII_2, ASCII_3, ASCII_4,
    ASCII_5, ASCII_6, ASCII_7, ASCII_8, ASCII_9, ASCII_0, ASCII_HYPHEN,
    ASCII_EQUALS, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_Q, ASCII_W,
    ASCII_E, ASCII_R, ASCII_T, ASCII_Y, ASCII_U, ASCII_I, ASCII_O, ASCII_P,
    ASCII_LEFT_BRACKET, ASCII_RIGHT_BRACKET, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_A, ASCII_S, ASCII_D, ASCII_F, ASCII_G, ASCII_H, ASCII_J, ASCII_K,
    ASCII_L, ASCII_SEMICOLON, '\'', '`', ASCII_PLACEHOLDER,
    ASCII_BACKWARDS_SLASH, ASCII_Z, ASCII_X, ASCII_C, ASCII_V, ASCII_B, ASCII_N,
    ASCII_M, ASCII_COMMA, ASCII_PERIOD, ASCII_FORWARDS_SLASH, ASCII_PLACEHOLDER,
    ASCII_KP_ASTERISK, ASCII_PLACEHOLDER, ASCII_SPACE, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_KP_7, ASCII_KP_8, ASCII_KP_9, ASCII_KP_HYPHEN, ASCII_KP_4, ASCII_KP_5,
    ASCII_KP_6, ASCII_KP_PLUS, ASCII_KP_1, ASCII_KP_2, ASCII_KP_3, ASCII_KP_0,
    ASCII_KP_PERIOD, ASCII_NULL_CHAR, ASCII_NULL_CHAR, ASCII_NULL_CHAR,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER
};

// array of capitalized letters
uint8_t caps_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, '!', '@', '#', '$',
    '%', '^', '&', '*', '(', ')', '_',
    '+', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, 'Q', 'W',
    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
    'L', ':', '"', '~', ASCII_PLACEHOLDER,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?', ASCII_PLACEHOLDER,
    ASCII_KP_ASTERISK, ASCII_PLACEHOLDER, ASCII_SPACE, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0',
    '.', ASCII_NULL_CHAR, ASCII_NULL_CHAR, ASCII_NULL_CHAR,
    ASCII_PLACEHOLDER, ASCII_PLACEHOLDER
};

int8_t keyboard_buf[MAX_CHARS_IN_BUF];
uint8_t num_chars_in_buf = 0;

// variables designating status of special keys
int cntrl_l_on = 0;
int cntrl_r_on = 0;

int caps_on = 0;
int caps_btn_pressed = 0;

int shift_l_on = 0;
int shift_r_on = 0;

int32_t terminal_open (const uint8_t * filename) {
    int i = 0;

    for(i = 0; i < num_chars_in_buf; i++) {
        keyboard_buf[i] = NULL;
    }
    num_chars_in_buf = 0;

    return 0;
}

int32_t terminal_close (int32_t fd) {
    return 0;
}

int32_t terminal_read (int32_t fd, uint8_t * buf, int32_t nbytes) {
    int bytes_read = 0;
    int i = 0;

    for(i = 0; i < nbytes; i++) {
        buf[i] = keyboard_buf[i];
        bytes_read++;
    }

    return bytes_read;
}

int32_t terminal_write (int32_t fd, const uint8_t * buf, int32_t nbytes) {
    int num_printed = 0;
    int i = 0;

    for(i = 0; i < nbytes; i++) {
        print_char(buf[i]);
        num_printed++;
    }

    return num_printed;
}

void reset_term() {
    int i = 0;

    for(i = 0; i < num_chars_in_buf; i++) {
        keyboard_buf[i] = NULL;
    }
    num_chars_in_buf = 0;

    clear_screen();
}

void add_char_to_buffer(uint8_t new_char) {
    // if we haven't reached the buffer limit, add the char to the buffer and print the key
    if(num_chars_in_buf < MAX_CHARS_IN_BUF) {
        keyboard_buf[num_chars_in_buf] = new_char;
        num_chars_in_buf++;
        print_char(new_char);
    }
}

void handle_enter() {
    int i = 0;
    
    for(i = 0; i < num_chars_in_buf; i++) {
        keyboard_buf[i] = NULL;
    }
    num_chars_in_buf = 0;

    new_line();
}

void handle_backspace() {
    if(num_chars_in_buf > 0) {
        num_chars_in_buf--;
        keyboard_buf[num_chars_in_buf] = NULL;
        del_last_char();
    }
}

/**
 * Wait for a keyboard ACK
 */
void kbd_ack(void){ 
  while(!(inb(KEYBOARD_D_PORT)==KEYBOARD_D_ACK));
}

/**
 * Set the keyboard LED status 
 * @param caps   0 or 1 If caps lock is enabled
 * @param num    0 or 1 If num lock is enabled
 * @param scroll 0 or 1 If scroll lock is enabled
 */
void kbd_led_handling(int caps, int num, int scroll) {
    // Tell keyboard we want to set LED's and wait for ack
    outb(KEYBOARD_LED_V, KEYBOARD_D_PORT);
    kbd_ack();

    // If nothing is enabled, then all LEDs will be set to 0 (off)
    int ledstatus = 0x0;

    // Check which LED to enable and set based on bit masks
    if (caps) {
        ledstatus = ledstatus | 0x4;
    }

    if (num) {
        ledstatus = ledstatus | 0x2;
    }

    if (scroll) {
        ledstatus = ledstatus | 0x1;
    }

    outb(ledstatus, KEYBOARD_D_PORT);
}

/**
 * handle_keypress()
 * 
 * Description: Prints the key pressed by the keyboard to the screen
 *
 * Inputs: None (keycode is read from the keyboard port)
 * Outputs: Prints the corresponding character to the screen
 * Returns: None
 */
void handle_keypress() {
    // block interrupts
    cli();

    // get scan code
    uint8_t key_code = inb(KEYBOARD_D_PORT);

    if(key_code < MAX_MAKE_SCANCODE) {
        // make key
        uint8_t key_ascii = code_to_ascii[key_code];

        if(key_ascii != ASCII_PLACEHOLDER) {
            // if its a valid char that we can print, print it
            if(cntrl_l_on || cntrl_r_on) {
                switch (key_code) {
                    case KEY_MAKE_L:
                        reset_term();
                        break;
                    default:
                        break;
                }
            } else if (caps_on || shift_l_on || shift_r_on) {
                // print caps version
                key_ascii = caps_ascii[key_code];
                add_char_to_buffer(key_ascii);
            } else {
                // print char normally
                add_char_to_buffer(key_ascii);
            }
        } else {
            switch(key_code) {
                case KEY_MAKE_L_CTRL:
                    cntrl_l_on = 1;
                    break;
                case KEY_MAKE_L_SHIFT:
                    shift_l_on = 1;
                    break;
                case KEY_MAKE_R_SHIFT:
                    shift_r_on = 1;
                    break;
                case KEY_MAKE_CAPS:
                    if(!caps_btn_pressed) {
                        caps_on = !caps_on;
                        kbd_led_handling(caps_on, 0, 0);
                    }
                    caps_btn_pressed = 1;
                    break;
                case KEY_MAKE_ENTER:
                    handle_enter();
                    break;
                case KEY_MAKE_BKSP:
                    handle_backspace();
                    break;
                default:
                    break;
            }
        }
    } else {
        // break or special key
        if(key_code == SPECIAL_KEY) {
            // get next code
            key_code = inb(KEYBOARD_D_PORT);
            switch(key_code) {
                case KEY_MAKE_R_CTRL:
                    cntrl_r_on = 1;
                    break;
                case KEY_BREAK_R_CTRL:
                    cntrl_r_on = 0;
                    break;
                default:
                    // we don't care about the rest of the special keys
                    // missing R ALT
                    break;
            }
        } else {
            // get break and set correct key status
            switch(key_code) {
                case KEY_BREAK_L_CTRL:
                    cntrl_l_on = 0;
                    break;
                case KEY_BREAK_L_SHIFT:
                    shift_l_on = 0;
                    break;
                case KEY_BREAK_R_SHIFT:
                    shift_r_on = 0;
                    break;
                case KEY_BREAK_CAPS:
                    caps_btn_pressed = 0;
                    break;
                default:
                    // we don't care about the rest of the breaks
                    break;
            }
        }
    }

    // send eoi and allow interrupts again
    send_eoi(IRQ_KEYBOARD_CTRL);
    sti();
}
