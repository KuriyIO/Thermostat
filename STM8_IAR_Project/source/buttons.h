#include "stdint.h"

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

/* function prototypes */
void KBD_init(void);

/* global variables */
extern uint8_t isPressed;
extern uint8_t btn;

/* define long tick */
#define LTICK 7
/* define skips */
#define SKIPS 6

/* define buttons */
#define LMENU 0x80
#define LDOWN 0x40
#define LUP 0x20
#define LSEL 0x10
#define MENU 0x08
#define DOWN 0x04
#define UP 0x02
#define SEL 0x01

/* buttons name defines */
#define MENU_BTN        !PC_IDR_bit.IDR4
#define SELECT_BTN      !PA_IDR_bit.IDR3
#define UP_BTN          !PC_IDR_bit.IDR1
#define DOWN_BTN        !PC_IDR_bit.IDR0

#endif
