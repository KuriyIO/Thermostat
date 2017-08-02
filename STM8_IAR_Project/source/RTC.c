#include "iostm8l051f3.h"
#include "stdint.h"

void RTC_init(void)
{    
    CLK_CRTCR_bit.RTCSEL3 = 1;          //set clock source
    while(CLK_CRTCR_bit.RTCSWBSY);
    CLK_PCKENR2_bit.PCKEN22 = 1;        //enable clock
           
    RTC_WPR = 0xCA;     //unlock registers
    RTC_WPR = 0x53;
        
    RTC_ISR1_bit.INIT = 1;      //enable init mode
    while(!RTC_ISR1_bit.INITF);  //wait for init unlock
    
    RTC_APRER = 127;            //
    RTC_SPRERL = 255;           //set prescallers
    RTC_SPRERH = 0;             //
    
    RTC_CR1 = 0x14;             //BYPSHAD = 1, WUCKSEL = 100
    
    RTC_WUTRH = 0;              //periodic wakeup
    RTC_WUTRL = 30;             //every 30sec
    
    RTC_CR2_bit.WUTIE = 1;      //enable wakeup interrupt
    RTC_CR2_bit.WUTE = 1;       //enable wakeup    
    
    RTC_DR1 = 0x01;             //set data
    RTC_DR2 = 0x01;             //as 01.01.2001
    RTC_DR3 = 0x01;             //
    
    RTC_ISR1_bit.INIT = 0;      //exit init mode
    RTC_ISR1_bit.RSF = 0;       //clear RSF bit
}