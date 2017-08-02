#define SYSFREQ 16000000

#include "iostm8l051f3.h"
#include "intrinsics.h"
#include "source\main.h"

sysStatus status = {0,0,0};

void Clock_init(void)
{
    CLK_CKDIVR = 0;
}

inline void currentMode()
{
    for(int i=0; i<4; i++)
    {
        if(RTC_TR3_bit.HT > mode[i].sth)
            if(RTC_TR3_bit.HU > mode[i].stl)
                if(RTC_TR3_bit.HT < mode[i].eth)
                    if(RTC_TR3_bit.HU < mode[i].etl)
                    {
                        status.mode = i;
                        break;
                    }
    }
}

uint8_t getMode()
{
    currentMode();
    return status.mode;
}

void main(void)
{   
    /* unlock eeprom */
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;
    
    /* System init */
    Clock_init();
    LCD_init();
    Power_init(); 
    ADC_init();
    KBD_init();       
    RTC_init();
    __enable_interrupt();    
  
    while (1)
    {        
        getTemp();        
        uint8_t curMode = getMode();
        
        uint8_t currentTemp = (temperature.th * 10) + temperature.tl;
        uint8_t modeTemp = (mode[curMode].th * 10) + mode[curMode].tl;
        if(currentTemp < (modeTemp - mode[curMode].hyst))
            powerOn();
        if(currentTemp > (modeTemp + mode[curMode].hyst))
            powerOff();
        
        menu_upd();
        delay_ms(500);        
    }
}
