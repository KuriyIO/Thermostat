#include "stdint.h"

#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__

/* structures definition */
typedef struct
{
    unsigned th: 4;     //first number
    unsigned tl: 4;     //second number
    unsigned frac: 4;   //number after '.' (for example 25.x, where x - is frac)
}tempValue;

/* function prototypes */
void getTemp(void);
void ADC_init(void);
uint16_t getADC(void);

/* global variables */
extern tempValue temperature;

/* define Table Size */
#define TABLE_SIZE 32
/* Celsius degree per i */
#define C_per_i 1
/* Starting Value (in Celsius degree) */
#define TEMP_START 10

#endif
