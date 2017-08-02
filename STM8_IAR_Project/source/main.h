#ifndef __MAIN_H__
#define __MAIN_H__

#include "stdint.h"
#include "delay.h"
#include "HD44780.h"
#include "buttons.h"
#include "RTC.h"
#include "thermistor.h"
#include "power.h"
#include "menu.h"

typedef struct {
    unsigned mode: 2;
    unsigned pwr: 1;
    unsigned sleep: 1;
} sysStatus;

/* global variables */
extern sysStatus status;

/* function prototypes */
void Clock_init(void);

#endif
