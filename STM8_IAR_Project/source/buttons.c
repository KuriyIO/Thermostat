#include "iostm8l051f3.h"
#include "stdint.h"
#include "buttons.h"
#include "HD44780.h"
#include "main.h"
#include "menu.h"

uint8_t btn = 0;

void KBD_init(void)
{
    /* set pins as output */    
    PC_DDR &= ~((1<<0)|(1<<1)|(1<<4));
    PA_DDR &= ~(1<<3);
    
    /* enable pull-up */    
    PC_CR1 |= ((1<<0)|(1<<1)|(1<<4));
    PA_CR1 |= (1<<3);
    
    /* Timer init */
    CLK_PCKENR1_bit.PCKEN12 = 1;        //enable clock for TIM4
    TIM4_PSCR_bit.PSC = 0x0F;           //set prescaler 32768
    TIM4_ARR = 49;                      //ticks from 0 to 49
    TIM4_IER_bit.UIE = 1;               //enable update interupt
    TIM4_CR1_bit.ARPE = 1;              //get interrupt when counter reach ARR value
    TIM4_CR1_bit.CEN = 1;               //run timer
}

static uint8_t press = 0, ticks = 0, lastbtn = 0, skips = 0;
uint8_t isPressed = 0;

#pragma vector=TIM4_UIF_vector
__interrupt void TIM4_UIF_IRQHandler(void)
{
    TIM4_SR1_bit.UIF = 0;   //clear update flag
    
    if(skips != 0)
        skips--;    
    
    if(MENU_BTN) { 
        press = MENU; 
        isPressed = 0xFF; 
        if(led_t != off) {
            LCD_led_on();
        }
        else {
            LCD_led_off();
        }
    }
    if(SELECT_BTN) { 
        press = SEL;  
        isPressed = 0xFF; 
        if(led_t != off) {
            LCD_led_on();
        }
        else {
            LCD_led_off();
        }
    }
    if(UP_BTN) { 
        press = UP;   
        isPressed = 0xFF; 
        if(led_t != off) {
            LCD_led_on();
        }
        else {
            LCD_led_off();
        }
    }
    if(DOWN_BTN) { 
        press = DOWN; 
        isPressed = 0xFF; 
        if(led_t != off) {
            LCD_led_on();
        }
        else {
            LCD_led_off();
        }
    }
        
    switch(press)
    {
    case MENU:
    {
        if(lastbtn == MENU && skips!=0)
        {
            press = 0;
            break;
        }
        else
        {
            if(!MENU_BTN || ticks > LTICK)
            {
                if(ticks > LTICK) {
                    btn = LMENU; skips = SKIPS * 2;
                }
                else {
                    btn = MENU; skips = SKIPS;
                }
                lastbtn = MENU;
                ticks = 0;
                press = 0;
            }
            else
                ticks++;
        }
        break;
    }
    case SEL:
    {
        if(lastbtn == SEL && skips!=0)
        {
            press = 0;
            break;
        }
        else
        {
            if(!SELECT_BTN || ticks > LTICK)
            {
                if(ticks > LTICK) {
                    btn = LSEL; skips = SKIPS*2;
                }
                else {
                    btn = SEL; skips = SKIPS;
                }
                lastbtn = SEL;
                ticks = 0;
                press = 0;
            }
            else
                ticks++;
        }
        break;
    }
    case UP:
    {
        if(lastbtn == UP && skips!=0)
        {
            press = 0;
            break;
        }
        else
        {
            btn = UP;
            lastbtn = UP;
            ticks = 0;
            skips = SKIPS+(SKIPS/2);
            press = 0;           
        }
        break;
    }
    case DOWN:
    {
        if(lastbtn == DOWN && skips!=0)
        {
            press = 0;
            break;
        }
        else
        {
            btn = DOWN;
            lastbtn = DOWN;
            ticks = 0;
            skips = SKIPS+(SKIPS/2);
            press = 0;  
        }
        break;
    }
    default: {}
    }
}