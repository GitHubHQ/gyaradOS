
#include "keyboard.h"

/**
 * Reads a scan code from the keyboard port
 * @return  The scan code sent from the keyboard (Not ASCII)
 */
uint8_t kbrd_read_scan_code() {
	return inb(KEYBOARD_D_PORT);
}

void kbrd_print_keypress() {
    uint8_t scancode = kbrd_read_scan_code();
    uint8_t key_pressed = 0x00;

    switch(scancode) {
        case KEY_MAKE_A:
            key_pressed = ASCII_A;
            break;
        case KEY_MAKE_B:
            key_pressed = ASCII_B;
            break;
        case KEY_MAKE_C:
            key_pressed = ASCII_C;
            break;
        case KEY_MAKE_D:
            key_pressed = ASCII_D;
            break;
        case KEY_MAKE_E:
            key_pressed = ASCII_E;
            break;
        case KEY_MAKE_F:
            key_pressed = ASCII_F;
            break;
        case KEY_MAKE_G:
            key_pressed = ASCII_G;
            break;
        case KEY_MAKE_H:
            key_pressed = ASCII_H;
            break;
        case KEY_MAKE_I:
            key_pressed = ASCII_I;
            break;
        case KEY_MAKE_J:
            key_pressed = ASCII_J;
            break;
        case KEY_MAKE_K:
            key_pressed = ASCII_K;
            break;
        case KEY_MAKE_L:
            key_pressed = ASCII_L;
            break;
        case KEY_MAKE_M:
            key_pressed = ASCII_M;
            break;
        case KEY_MAKE_N:
            key_pressed = ASCII_N;
            break;
        case KEY_MAKE_O:
            key_pressed = ASCII_O;
            break;
        case KEY_MAKE_P:
            key_pressed = ASCII_P;
            break;
        case KEY_MAKE_Q:
            key_pressed = ASCII_Q;
            break;
        case KEY_MAKE_R:
            key_pressed = ASCII_R;
            break;
        case KEY_MAKE_S:
            key_pressed = ASCII_S;
            break;
        case KEY_MAKE_T:
            key_pressed = ASCII_T;
            break;
        case KEY_MAKE_U:
            key_pressed = ASCII_U;
            break;
        case KEY_MAKE_V:
            key_pressed = ASCII_V;
            break;
        case KEY_MAKE_W:
            key_pressed = ASCII_W;
            break;
        case KEY_MAKE_X:
            key_pressed = ASCII_X;
            break;
        case KEY_MAKE_Y:
            key_pressed = ASCII_Y;
            break;
        case KEY_MAKE_Z:
            key_pressed = ASCII_Z;
            break;
        case KEY_MAKE_0:
            key_pressed = ASCII_0;
            break;
        case KEY_MAKE_1:
            key_pressed = ASCII_1;
            break;
        case KEY_MAKE_2:
            key_pressed = ASCII_2;
            break;
        case KEY_MAKE_3:
            key_pressed = ASCII_3;
            break;
        case KEY_MAKE_4:
            key_pressed = ASCII_4;
            break;
        case KEY_MAKE_5:
            key_pressed = ASCII_5;
            break;
        case KEY_MAKE_6:
            key_pressed = ASCII_6;
            break;
        case KEY_MAKE_7:
            key_pressed = ASCII_7;
            break;
        case KEY_MAKE_8:
            key_pressed = ASCII_8;
            break;
        case KEY_MAKE_9:
            key_pressed = ASCII_9;
            break;
        case KEY_MAKE_ASTERISK:
            key_pressed = ASCII_ASTERISK;
            break;
        case KEY_MAKE_HYPHEN:
            key_pressed = ASCII_HYPHEN;
            break;
        case KEY_MAKE_EQUALS:
            key_pressed = ASCII_EQUALS;
            break;
        case KEY_MAKE_BACKWARDS_SLASH:
            key_pressed = ASCII_BACKWARDS_SLASH;
            break;
        case KEY_MAKE_BKSP:
            printf("BKSP\n");
            break;
        case KEY_MAKE_SPACE:
            key_pressed = ASCII_SPACE;
            break;
        case KEY_MAKE_TAB:
            printf("TAB\n");
            break;
        case KEY_MAKE_CAPS:
            printf("CAPS");
            break;
        case KEY_MAKE_L_SHIFT:
            break;
        case KEY_MAKE_L_CTRL:
            break;
        case KEY_MAKE_L_ALT:
            break;
        case KEY_MAKE_R_SHIFT:
            break;
        case KEY_MAKE_ENTER:
            break;
        case KEY_MAKE_ESC:
            break;
        case KEY_MAKE_F1:
            break;
        case KEY_MAKE_F2:
            break;
        case KEY_MAKE_F3:
            break;
        case KEY_MAKE_F4:
            break;
        case KEY_MAKE_F5:
            break;
        case KEY_MAKE_F6:
            break;
        case KEY_MAKE_F7:
            break;
        case KEY_MAKE_F8:
            break;
        case KEY_MAKE_F9:
            break;
        case KEY_MAKE_F10:
            break;
        case KEY_MAKE_F11:
            break;
        case KEY_MAKE_F12:
            break;
        case KEY_MAKE_SCROLL:
            break;
        case KEY_MAKE_LEFT_BRACKET:
            key_pressed = ASCII_LEFT_BRACKET;
            break;
        case KEY_MAKE_NUM:
            break;
        case KEY_MAKE_RIGHT_BRACKET:
            key_pressed = ASCII_RIGHT_BRACKET;
            break;
        case KEY_MAKE_SEMICOLON:
            key_pressed = ASCII_SEMICOLON;
            break;
        case KEY_MAKE_TICK:
            key_pressed = ASCII_TICK;
            break;
        case KEY_MAKE_COMMA:
            key_pressed = ASCII_COMMA;
            break;            
        case KEY_MAKE_PERIOD:
            key_pressed = ASCII_PERIOD;
            break;
        case KEY_MAKE_FORWARDS_SLASH:
            key_pressed = ASCII_FORWARDS_SLASH;
            break;
        default:
            break;
    }

    if(key_pressed) {
        printf("%c\n", key_pressed);
    }
}