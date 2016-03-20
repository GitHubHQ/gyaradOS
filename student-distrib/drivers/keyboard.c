
#include "keyboard.h"

// array of lowercase ASCII codes, indexed by the corresponding characters scan code
uint8_t code_to_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, ASCII_1, ASCII_2, ASCII_3, ASCII_4,
    ASCII_5, ASCII_6, ASCII_7, ASCII_8, ASCII_9, ASCII_0, ASCII_HYPHEN,
    ASCII_EQUALS, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, ASCII_Q, ASCII_W,
    ASCII_E, ASCII_R, ASCII_T, ASCII_Y, ASCII_U, ASCII_I, ASCII_O, ASCII_P,
    ASCII_LEFT_BRACKET, ASCII_RIGHT_BRACKET, ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    ASCII_A, ASCII_S, ASCII_D, ASCII_F, ASCII_G, ASCII_H, ASCII_J, ASCII_K,
    ASCII_L, ASCII_SEMICOLON, ASCII_TICK, ASCII_ASTERISK, ASCII_PLACEHOLDER,
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

uint8_t caps_ascii[] = {
    ASCII_NULL_CHAR, ASCII_PLACEHOLDER, '!', '@', '#', '$',
    '%', '^', '&', '*', '(', ')', '_',
    '+', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER, 'Q', 'W',
    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}', ASCII_PLACEHOLDER, ASCII_PLACEHOLDER,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
    'L', ':', '~', ASCII_ASTERISK, ASCII_PLACEHOLDER,
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

// variables designating status of special keys
int cntrl_l_on = 0;
int cntrl_r_on = 0;
int caps_on = 0;
int shift_l_on = 0;
int shift_r_on = 0;

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
                        clear();
                        break;
                    default:
                        break;
                }
            } else if (caps_on || shift_l_on || shift_r_on) {
                // print caps version
                key_ascii = caps_ascii[key_code];
                print_char(key_ascii);
            } else {
                // print char normally
                print_char(key_ascii);
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
                    caps_on = !caps_on;
                    break;
                case KEY_MAKE_ENTER:
                    new_line();
                    break;
                case KEY_MAKE_BKSP:
                    del_last_char();
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
                    // only make toggles caps, so do nothing
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
