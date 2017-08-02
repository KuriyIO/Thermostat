#include "iostm8l051f3.h"
#include "intrinsics.h"
#include "stdint.h"
#include "HD44780.h"
#include "main.h"
#include "thermistor.h"

extern sysStatus status;

void sleep()
{
    /* disable interrupt */
    TIM4_IER_bit.UIE = 0;
    status.sleep = 1;
    
    __halt();   //go to active halt mode
}

void reinit()
{
    Clock_init();
    LCD_init();
}

void wakeup()
{
    ADC_init();         //init ADC
    getADC();           //get Value (if 0 it will sleep anyway)
    
    if(status.sleep)         //if we was sleeping 
        asm("jp 0x8000");    //calling software reset
}