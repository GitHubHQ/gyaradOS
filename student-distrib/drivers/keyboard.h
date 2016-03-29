#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "../libs/lib.h"
#include "i8259.h"

// Keyboard port
#define KEYBOARD_D_PORT          0x60

// Keyboard Return values
#define KEYBOARD_D_ACK			 0xFA
#define KEYBOARD_LED_V			 0xED

// Number of keys we want to detect
#define MAX_MAKE_SCANCODE        0x59

// max number of chars in the keyboard buffer
#define MAX_CHARS_IN_BUF         127

// Make keys
#define KEY_MAKE_ESC             0x01
#define KEY_MAKE_1               0x02
#define KEY_MAKE_2               0x03
#define KEY_MAKE_3               0x04
#define KEY_MAKE_4               0x05
#define KEY_MAKE_5               0x06
#define KEY_MAKE_6               0x07
#define KEY_MAKE_7               0x08
#define KEY_MAKE_8               0x09
#define KEY_MAKE_9               0x0A
#define KEY_MAKE_0               0x0B
#define KEY_MAKE_HYPHEN          0x0C
#define KEY_MAKE_EQUALS          0x0D
#define KEY_MAKE_BKSP            0x0E
#define KEY_MAKE_TAB             0x0F
#define KEY_MAKE_Q               0x10
#define KEY_MAKE_W               0x11
#define KEY_MAKE_E               0x12
#define KEY_MAKE_R               0x13
#define KEY_MAKE_T               0x14
#define KEY_MAKE_Y               0x15
#define KEY_MAKE_U               0x16
#define KEY_MAKE_I               0x17
#define KEY_MAKE_O               0x18
#define KEY_MAKE_P               0x19
#define KEY_MAKE_LEFT_BRACKET    0x1A
#define KEY_MAKE_RIGHT_BRACKET   0x1B
#define KEY_MAKE_ENTER           0x1C
#define KEY_MAKE_L_CTRL          0x1D
#define KEY_MAKE_A               0x1E
#define KEY_MAKE_S               0x1F
#define KEY_MAKE_D               0x20
#define KEY_MAKE_F               0x21
#define KEY_MAKE_G               0x22
#define KEY_MAKE_H               0x23
#define KEY_MAKE_J               0x24
#define KEY_MAKE_K               0x25
#define KEY_MAKE_L               0x26
#define KEY_MAKE_SEMICOLON       0x27
#define KEY_MAKE_TICK            0x28
#define KEY_MAKE_ASTERISK        0x29
#define KEY_MAKE_L_SHIFT         0x2A
#define KEY_MAKE_BACKWARDS_SLASH 0x2B
#define KEY_MAKE_Z               0x2C
#define KEY_MAKE_X               0x2D
#define KEY_MAKE_C               0x2E
#define KEY_MAKE_V               0x2F
#define KEY_MAKE_B               0x30
#define KEY_MAKE_N               0x31
#define KEY_MAKE_M               0x32
#define KEY_MAKE_COMMA           0x33
#define KEY_MAKE_PERIOD          0x34
#define KEY_MAKE_FORWARDS_SLASH  0x35
#define KEY_MAKE_R_SHIFT         0x36
#define KEY_MAKE_KP_ASTERISK     0x37
#define KEY_MAKE_L_ALT           0x38
#define KEY_MAKE_SPACE           0x39
#define KEY_MAKE_CAPS            0x3A
#define KEY_MAKE_F1              0x3B
#define KEY_MAKE_F2              0x3C
#define KEY_MAKE_F3              0x3D
#define KEY_MAKE_F4              0x3E
#define KEY_MAKE_F5              0x3F
#define KEY_MAKE_F6              0x40
#define KEY_MAKE_F7              0x41
#define KEY_MAKE_F8              0x42
#define KEY_MAKE_F9              0x43
#define KEY_MAKE_F10             0x44
#define KEY_MAKE_NUM             0x45
#define KEY_MAKE_SCROLL          0x46
#define KEY_MAKE_KP_7            0x47
#define KEY_MAKE_KP_8            0x48
#define KEY_MAKE_KP_9            0x49
#define KEY_MAKE_KP_HYPHEN       0x4A
#define KEY_MAKE_KP_4            0x4B
#define KEY_MAKE_KP_5            0x4C
#define KEY_MAKE_KP_6            0x4D
#define KEY_MAKE_KP_PLUS         0x4E
#define KEY_MAKE_KP_1            0x4F
#define KEY_MAKE_KP_2            0x50
#define KEY_MAKE_KP_3            0x51
#define KEY_MAKE_KP_0            0x52
#define KEY_MAKE_KP_PERIOD       0x53
#define KEY_MAKE_F11             0x57
#define KEY_MAKE_F12             0x58

// Break keys
#define KEY_BREAK_L_CTRL          0x9D
#define KEY_BREAK_L_SHIFT         0xAA
#define KEY_BREAK_R_SHIFT         0xB6
#define KEY_BREAK_CAPS            0xBA

// special make keys
#define SPECIAL_KEY              0xE0
#define KEY_MAKE_R_CTRL          0x1D
#define KEY_BREAK_R_CTRL         0x9D
#define KEY_MAKE_R_ALT           0x38
#define KEY_BREAK_R_ALT          0xB8
#define KEY_MAKE_L_ARROW         0x4B
#define KEY_MAKE_R_ARROW         0x4D
#define KEY_MAKE_U_ARROW         0x48
#define KEY_MAKE_D_ARROW         0x50

// ASCII keys
#define ASCII_A               0x61
#define ASCII_B               0x62
#define ASCII_C               0x63
#define ASCII_D               0x64
#define ASCII_E               0x65
#define ASCII_F               0x66
#define ASCII_G               0x67
#define ASCII_H               0x68
#define ASCII_I               0x69
#define ASCII_J               0x6A
#define ASCII_K               0x6B
#define ASCII_L               0x6C
#define ASCII_M               0x6D
#define ASCII_N               0x6E
#define ASCII_O               0x6F
#define ASCII_P               0x70
#define ASCII_Q               0x71
#define ASCII_R               0x72
#define ASCII_S               0x73
#define ASCII_T               0x74
#define ASCII_U               0x75
#define ASCII_V               0x76
#define ASCII_W               0x77
#define ASCII_X               0x78
#define ASCII_Y               0x79
#define ASCII_Z               0x7A
#define ASCII_0               0x30
#define ASCII_1               0x31
#define ASCII_2               0x32
#define ASCII_3               0x33
#define ASCII_4               0x34
#define ASCII_5               0x35
#define ASCII_6               0x36
#define ASCII_7               0x37
#define ASCII_8               0x38
#define ASCII_9               0x39
#define ASCII_KP_0            0x30
#define ASCII_KP_1            0x31
#define ASCII_KP_2            0x32
#define ASCII_KP_3            0x33
#define ASCII_KP_4            0x34
#define ASCII_KP_5            0x35
#define ASCII_KP_6            0x36
#define ASCII_KP_7            0x37
#define ASCII_KP_8            0x38
#define ASCII_KP_9            0x39
#define ASCII_HYPHEN          0x2D
#define ASCII_EQUALS          0x3D
#define ASCII_LEFT_BRACKET    0x5B
#define ASCII_RIGHT_BRACKET   0x5D
#define ASCII_SEMICOLON       0x3B
#define ASCII_TICK            0x27
#define ASCII_ASTERISK        0x2A
#define ASCII_BACKWARDS_SLASH 0x5C
#define ASCII_COMMA           0x2C
#define ASCII_PERIOD          0x2E
#define ASCII_FORWARDS_SLASH  0x2F
#define ASCII_KP_ASTERISK     0x2A
#define ASCII_SPACE           0x20
#define ASCII_KP_HYPHEN       0x2D
#define ASCII_KP_PLUS         0x2B
#define ASCII_KP_PERIOD       0x2E
#define ASCII_NEW_LINE        0x0A
#define ASCII_NULL_CHAR       0x00

#define ASCII_CAPS_A          0x41
#define ASCII_CAPS_B          0x42
#define ASCII_CAPS_C          0x43
#define ASCII_CAPS_D          0x44
#define ASCII_CAPS_E          0x45
#define ASCII_CAPS_F          0x46
#define ASCII_CAPS_G          0x47
#define ASCII_CAPS_H          0x48
#define ASCII_CAPS_I          0x49
#define ASCII_CAPS_J          0x4A
#define ASCII_CAPS_K          0x4B
#define ASCII_CAPS_L          0x4C
#define ASCII_CAPS_M          0x4D
#define ASCII_CAPS_N          0x4E
#define ASCII_CAPS_O          0x4F
#define ASCII_CAPS_P          0x50
#define ASCII_CAPS_Q          0x51
#define ASCII_CAPS_R          0x52
#define ASCII_CAPS_S          0x53
#define ASCII_CAPS_T          0x54
#define ASCII_CAPS_U          0x55
#define ASCII_CAPS_V          0x56
#define ASCII_CAPS_W          0x57
#define ASCII_CAPS_X          0x58
#define ASCII_CAPS_Y          0x59
#define ASCII_CAPS_Z          0x5A

#define ASCII_PLACEHOLDER     0xFE

// not currently handled:
// #define KEY_MAKE_ESC             0x01
// #define KEY_MAKE_TAB             0x0F
// #define KEY_MAKE_F1              0x3B
// #define KEY_MAKE_F2              0x3C
// #define KEY_MAKE_F3              0x3D
// #define KEY_MAKE_F4              0x3E
// #define KEY_MAKE_F5              0x3F
// #define KEY_MAKE_F6              0x40
// #define KEY_MAKE_F7              0x41
// #define KEY_MAKE_F8              0x42
// #define KEY_MAKE_F9              0x43
// #define KEY_MAKE_F10             0x44
// #define KEY_MAKE_F11             0x57
// #define KEY_MAKE_F12             0x58
// #define KEY_MAKE_L_ALT           0x38
// #define KEY_MAKE_NUM             0x45
// #define KEY_MAKE_SCROLL          0x46

int32_t terminal_open (const uint8_t * filename);
int32_t terminal_close (int32_t fd);
int32_t terminal_write (int32_t fd, const uint8_t * buf, int32_t nbytes);
void reset_term();
uint32_t add_char_to_buffer(uint8_t new_char);
void handle_enter();
void handle_backspace();
void handle_keypress(void);

/* Tester functions */
void test_open(void);
void test_close(void);
void test_write(void);
void test_read(void);

#endif  /* _KEYBOARD_H */
