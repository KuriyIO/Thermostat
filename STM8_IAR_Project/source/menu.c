#include "iostm8l051f3.h"
#include "stdint.h"
#include "menu.h"
#include "buttons.h"
#include "thermistor.h"
#include "HD44780.h"
#include "main.h"
#include "sleep.h"
#include "power.h"

#pragma location=0x1000
__no_init modes mode[4];
#pragma location=0x1016
__no_init uint8_t led_t;

static uint8_t pos = 0, tpos = 0, page = 0, blink = 0;

void menu_upd(void)
{
menu_upd:
    switch(page)
    {
    case home:
        {
            asm("nop");
            switch(btn)
            {
            case MENU:
                {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP:
                {
                    //go to current mode page mode[0..3]
                    page = mode1 + status.mode;
                    btn = 0;
                    pos = pos_th;
                    goto menu_upd;
                }
            case DOWN:
                {
                    //go to current mode page mode[0..3]                    
                    page = mode1 + status.mode;
                    btn = 0;
                    pos = pos_th;
                    goto menu_upd;
                }
            case LSEL:
                {
                    //toggle power
                    powerToggle();
                    btn = 0;
                    goto menu_upd;
                }
            default:
                {
                    //refresh if needed
                    char tempstr[16] = {'0','0',':','0','0',' ','+','2','5','.','5',0xDF,'C',' ',' ',0xD9};
                    tempstr[0] = RTC_TR3_bit.HT + 48;
                    tempstr[1] = RTC_TR3_bit.HU + 48;
                    tempstr[3] = RTC_TR2_bit.MNT + 48;
                    tempstr[4] = RTC_TR2_bit.MNU + 48;
                    if(temperature.th == 0)
                    {
                        tempstr[6] = ' ';
                        tempstr[7] = '+';
                    }
                    else
                    {
                        tempstr[7] = temperature.th + 48;
                    }
                    tempstr[8] = temperature.tl + 48;
                    tempstr[10] = temperature.frac + 48;
                    if(status.pwr)
                        tempstr[15] = 0xD9;
                    else
                        tempstr[15] = 0xDA;
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case sets_mode:
        {
            switch(btn)
            {
            case LMENU:
                {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case DOWN:
                {
                    //go to sets_time page
                    page = sets_time;
                    btn = 0;
                    goto menu_upd;
                }
            case SEL:
                {
                    //go to mode1 page
                    page = mode1;
                    btn = 0;
                    pos = 0;
                    goto menu_upd;
                }
            default:
                {
                    //refresh if needed
                    char tempstr[16] = {'H','a','c',0xBF,'p','.',' ','p','e',0xB6,0xB8,0xBC,'o',0xB3,' ',2};
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case sets_time:
        {
            switch(btn)
            {
            case LMENU:
                {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case UP:
                {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case DOWN:
                {
                    //go to sets_led page
                    page = sets_led;
                    btn = 0;
                    goto menu_upd;
                }
            case SEL:
                {
                    //go to time page
                    page = time;
                    btn = 0;
                    goto menu_upd;
                }
            default:
                {
                    //refresh if needed
                    char tempstr[16] = {'H','a','c',0xBF,'p','.',' ',0xB3,'p','e',0xBC,0xC7,' ',' ',' ',3};
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case sets_led:
        {
            switch(btn)
            {
            case LMENU:
                {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case UP:
                {
                    //go to sets_time page
                    page = sets_time;
                    btn = 0;
                    goto menu_upd;
                }
            case SEL:
                {
                    //go to led page
                    page = led;
                    btn = 0;
                    goto menu_upd;
                }
            default:
                {
                    //refresh if needed
                    char tempstr[16] = {0xA8,'o','g','c',0xB3,'e',0xBF,0xBA,'a',' ',' ',' ',' ',' ',' ',1};
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case mode1:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                    modeUpBtn(0);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                    modeDownBtn(0);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case SEL: {
                    modeSelBtn();
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {0,0,'-',0,0,0xC0,' ',0,0,0xDF,'C',' ',4,0,' ',2};
                    modeUpdStr(tempstr, 0, 2);
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case mode2:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                    modeUpBtn(1);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                    modeDownBtn(1);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case SEL: {
                    modeSelBtn();
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {0,0,'-',0,0,0xC0,' ',0,0,0xDF,'C',' ',4,0,' ',3};
                    modeUpdStr(tempstr, 1, 3);                    
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case mode3:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                    modeUpBtn(2);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                    modeDownBtn(2);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case SEL: {
                    modeSelBtn();
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {0,0,'-',0,0,0xC0,' ',0,0,0xDF,'C',' ',4,0,' ',3};
                    modeUpdStr(tempstr, 2, 3);                    
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case mode4:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                    modeUpBtn(3);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                    modeDownBtn(3);
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case SEL: {
                    modeSelBtn();
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {0,0,'-',0,0,0xC0,' ',0,0,0xDF,'C',' ',4,0,' ',1};
                    modeUpdStr(tempstr, 3, 1);                    
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case time:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    tpos = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    tpos = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                
                    RTC_ISR1_bit.INIT = 1;       //enable init mode
                    while(!RTC_ISR1_bit.INITF);  //wait for init unlock
                    
                    switch(tpos)
                    {
                    case pos_hh: { if(RTC_TR3_bit.HT == 2) RTC_TR3_bit.HT=0;
                                   else RTC_TR3_bit.HT++; break;}
                    case pos_hl: { if(RTC_TR3_bit.HU == 3 && RTC_TR3_bit.HT == 2) RTC_TR3_bit.HU=0;
                                   else if(RTC_TR3_bit.HU == 9) RTC_TR3_bit.HU = 0;
                                   else RTC_TR3_bit.HU++; break;}
                    case pos_mh: { if(RTC_TR2_bit.MNT == 5) RTC_TR2_bit.MNT=0;
                                   else RTC_TR2_bit.MNT++; break;}
                    case pos_ml: { if(RTC_TR2_bit.MNU == 9) RTC_TR2_bit.MNU=0;
                                   else RTC_TR2_bit.MNU++; break;}
                    default: {}
                    }
                    
                    RTC_ISR1_bit.INIT = 0;      //exit init mode
                    blink = 0;
                    btn = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                
                    RTC_ISR1_bit.INIT = 1;       //enable init mode
                    while(!RTC_ISR1_bit.INITF);  //wait for init unlock
                    
                    switch(tpos)
                    {
                    case pos_hh: { if(RTC_TR3_bit.HT == 0) RTC_TR3_bit.HT=2;
                                   else RTC_TR3_bit.HT--; break;}
                    case pos_hl: { if(RTC_TR3_bit.HU == 0 && RTC_TR3_bit.HT == 2) RTC_TR3_bit.HU=3;
                                   else if(RTC_TR3_bit.HU == 0) RTC_TR3_bit.HU = 9;
                                   else RTC_TR3_bit.HU--; break;}
                    case pos_mh: { if(RTC_TR2_bit.MNT == 0) RTC_TR2_bit.MNT=5;
                                   else RTC_TR2_bit.MNT--; break;}
                    case pos_ml: { if(RTC_TR2_bit.MNU == 0) RTC_TR2_bit.MNU=9;
                                   else RTC_TR2_bit.MNU--; break;}
                    default: {}
                    }
                    
                    RTC_ISR1_bit.INIT = 0;      //exit init mode
                    blink = 0;
                    btn = 0;
                    goto menu_upd;
                }
            case SEL: {
                    switch(tpos)
                    {
                    case pos_hh:       { tpos=pos_hl; break; }
                    case pos_hl:       { tpos=pos_mh; break; }
                    case pos_mh:       { tpos=pos_ml; break; }
                    case pos_ml:       { tpos=pos_hh; break; }
                    default: {}
                    }
                    blink = 0;
                    btn = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {'2','3',':','4','5',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
                    tempstr[pos_hh] = blinkCharTime((RTC_TR3_bit.HT+48), pos_hh);
                    tempstr[pos_hl] = blinkCharTime((RTC_TR3_bit.HU+48), pos_hl);
                    tempstr[pos_mh] = blinkCharTime((RTC_TR2_bit.MNT+48), pos_mh);
                    tempstr[pos_ml] = blinkCharTime((RTC_TR2_bit.MNU+48), pos_ml);
                    printstr(tempstr);
                }
            }
            
            break;
        }
    case led:
        {
            switch(btn)
            {
            case LMENU: {
                    //go to home page
                    page = home;
                    btn = 0;
                    goto menu_upd;
                }
            case MENU: {
                    //go to sets_mode page
                    page = sets_mode;
                    btn = 0;
                    goto menu_upd;
                }
            case UP: {
                    //inc selected value
                    if(led_t == on)
                        led_t = off;
                    else
                        led_t++;
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            case DOWN: {
                    //dec selected value
                    if(led_t == off)
                        led_t = on;
                    else
                        led_t--;
                    btn = 0;
                    blink = 0;
                    goto menu_upd;
                }
            default: {
                    //refresh if needed
                    char tempstr[16] = {0xA8,'o','g','c',0xB3,'e',0xBF,0xBA,'a',' ',' ',' ',' ',' ',' ',' '};
                    switch(led_t)
                    {
                    case off:   { tempstr[10] = blinkChar('B');
                                  tempstr[11] = blinkChar(0xC3);
                                  tempstr[12] = blinkChar(0xBA);
                                  tempstr[13] = blinkChar(0xBB);
                                  tempstr[14] = ' ';
                                  break;
                                }
                    case on:    { tempstr[10] = blinkChar('B');
                                  tempstr[11] = blinkChar(0xBA);
                                  tempstr[12] = blinkChar(0xBB);
                                  tempstr[13] = ' ';
                                  tempstr[14] = ' ';
                                  break;
                                }
                    case sec30: { tempstr[10] = blinkChar('3');
                                  tempstr[11] = blinkChar('0');
                                  tempstr[12] = blinkChar('c');
                                  tempstr[13] = blinkChar('e');
                                  tempstr[14] = blinkChar(0xBA);
                                  break;
                                }
                    case min1:  { tempstr[10] = blinkChar('1');
                                  tempstr[11] = blinkChar(0xBC);
                                  tempstr[12] = blinkChar(0xB8);
                                  tempstr[13] = blinkChar(0xBD);
                                  tempstr[14] = ' ';
                                  break;
                                }
                    }                    
                    printstr(tempstr);
                }
            }
            
            break;
        }
    default:
        {
            char tempstr[16] = "Mystery page\0";
            printstr(tempstr);
        }
    }
    
    btn = 0;
    blink++;
}

static uint8_t wkup_counter = 0;

static char blinkChar(char symbol)
{
    if(blink > 6)
        blink = 0;
    
    if(blink < 3)
        return symbol;
    return ' ';
}

static char blinkCharTime(char symbol, uint8_t position)
{
    if(position == tpos)
    {
        return blinkChar(symbol);
    }
    return symbol;
}

static char blinkCharMode(char symbol, uint8_t position)
{
    if(position == pos)
    {
        return blinkChar(symbol);
    }
    return symbol;
}

static void modeUpBtn(uint8_t i)
{
    switch(pos)
    {
    case pos_sth:       { if(mode[i].sth==2)mode[i].sth=0;
                          else mode[i].sth++; break; }
    case pos_stl:       { if(mode[i].stl==9 && mode[i].sth<2) mode[i].stl=0;
                          else if(mode[i].stl==3 && mode[i].sth==2) mode[i].stl=0;
                          else mode[i].stl++; break; }
    case pos_eth:       { if(mode[i].eth==2)mode[i].eth=0;
                          else mode[i].eth++; break; }
    case pos_etl:       { if(mode[i].etl==9 && mode[i].eth<2) mode[i].etl=0;
                          else if(mode[i].etl==3 && mode[i].eth==2) mode[i].etl=0;
                          else mode[i].etl++; break; }
    case pos_th:        { if(mode[i].th==4)mode[i].th=0;
                          else mode[i].th++; break; }
    case pos_tl:        { if(mode[i].tl==9)mode[i].tl=0;
                          else mode[i].tl++; break; }
    case pos_hyst:      { if(mode[i].hyst==9)mode[i].hyst=0;
                          else mode[i].hyst++; break; }
    case pos_nav:       { page = mode3; break; }
    default: {}
    }
}

static void modeDownBtn(uint8_t i)
{
    switch(pos)
    {
    case pos_sth:       { if(mode[i].sth==0)mode[i].sth=2;
                          else mode[i].sth--; break; }
    case pos_stl:       { if(mode[i].stl==0) {if(mode[i].sth==2) mode[i].stl=3;
                                              else mode[i].stl=9;}
                          else mode[i].stl--; break; }
    case pos_eth:       { if(mode[i].eth==0)mode[i].eth=2;
                          else mode[i].eth--; break; }
    case pos_etl:       { if(mode[i].etl==0) {if(mode[i].eth==2) mode[i].etl=3;
                                              else mode[i].etl=9;}
                          else mode[i].etl--; break; }
    case pos_th:        { if(mode[i].th==0)mode[i].th=4;
                          else mode[i].th--; break; }
    case pos_tl:        { if(mode[i].tl==2)mode[i].tl=9;
                          else mode[i].tl--; break; }
    case pos_hyst:      { if(mode[i].hyst==0)mode[i].hyst=9;
                          else mode[i].hyst--; break; }
    default: {}
    }
}

static void modeSelBtn()
{
    switch(pos)
    {
    case pos_sth:       { pos=pos_stl; break; }
    case pos_stl:       { pos=pos_eth; break; }
    case pos_eth:       { pos=pos_etl; break; }
    case pos_etl:       { pos=pos_th; break; }
    case pos_th:        { pos=pos_tl; break; }
    case pos_tl:        { pos=pos_hyst; break; }
    case pos_hyst:      { pos=pos_nav; break; }
    case pos_nav:       { pos=pos_sth; break; }
    default: {}
    }
}

static void modeUpdStr(char* str, uint8_t i, char nav)
{
    str[pos_sth] = blinkCharMode((mode[i].sth + 48), pos_sth);
    str[pos_stl] = blinkCharMode((mode[i].stl + 48), pos_stl);
    str[pos_eth] = blinkCharMode((mode[i].eth + 48), pos_eth);
    str[pos_etl] = blinkCharMode((mode[i].etl + 48), pos_etl);
    str[pos_th] = blinkCharMode((mode[i].th + 48), pos_th);
    str[pos_tl] = blinkCharMode((mode[i].tl + 48), pos_tl);
    str[pos_hyst] = blinkCharMode((mode[i].hyst + 48), pos_hyst);
    str[pos_nav] = blinkCharMode(nav, pos_nav);
}

#pragma vector=RTC_WAKEUP_vector
__interrupt void RTC_WakeUp(void)
{
    RTC_ISR2_bit.WUTF = 0;      //clear wakeup interrupt flag
    wakeup();   //wake up if was sleep, and reinit if needed
    
    switch(led_t)
    {
    case off:                           //if led always off
        {
            LCD_led_off();
            break;
        }
    case on:                            //if led always on
        {
            LCD_led_on();
            break;
        }
    case sec30:                         //off led every 30 sec (if no buttons pressed)
        {
            if(isPressed == 0)
                LCD_led_off();
            break;
        }
    case min1:                          //off led every 1 min (if no buttons pressed)
        {
            if(wkup_counter == 2)
            {
                if(isPressed == 0)
                    LCD_led_off();
                
                wkup_counter = 0;
            }
            else
                wkup_counter++;
            break;
        }        
    }
    
    if(isPressed == 0)
        page = home;
    else
        isPressed = 0;
}