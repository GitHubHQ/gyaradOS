
#include "keyboard.h"

// array of ASCII codes, indexed by the corresponding characters scan code
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

uint8_t special_keys[] = {};

int special_key = 0;

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

    // if its a break key, ignore
    if(key_code > MAX_SCANCODE) {
        send_eoi(IRQ_KEYBOARD_CTRL);
        sti();
        return;
    }

    // if its a key that we want, convert it to ascii and print it
    uint8_t key_ascii = code_to_ascii[key_code];
    if(key_ascii != ASCII_PLACEHOLDER) {
        printf("%c", code_to_ascii[key_code]);
    }

    // send eoi and allow interrupts again
    send_eoi(IRQ_KEYBOARD_CTRL);
    sti();
}
