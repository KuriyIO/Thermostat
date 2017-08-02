#include "iostm8l051f3.h"
#include "stdint.h"
#include "main.h"

void Power_init(void)
{
    PWR_DDR_bit = 1;    //set power pin as output
    PWR_CR1_bit = 1;    //set push-pull mode
}

void powerOn(void)
{
    PWR_ODR_bit = 1;
    status.pwr = 1;    
}

void powerOff(void)
{
    PWR_ODR_bit = 0;
    status.pwr = 0;
}

void powerToggle(void)
{
    if(status.pwr)
        powerOff();
    else
        powerOn();
}