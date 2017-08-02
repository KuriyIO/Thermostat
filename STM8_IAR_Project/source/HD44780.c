#include "iostm8l051f3.h"
#include "stdint.h"
#include "delay.h"
#include "HD44780.h"

char LCD_str[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void LCD_send_byte(uint8_t data)
{
    uint8_t temp = data;
    
    LCD_ODR &= 0xF0;
    LCD_ODR |= (data>>4);

    LCD_STROBE();       //send high half-byte
    
    LCD_ODR &= 0xF0;
    temp &= 0x0F;
    LCD_ODR |= temp;
    LCD_STROBE();       //send low half-byte
    
    delay_us(50);       //delay between bytes 
}

void LCD_send_cmd(uint8_t cmd)
{
    LCD_ODR_bit.LCD_RS = 0;     //clear RS (sending command)
    LCD_send_byte(cmd);         //send command byte
}

void LCD_send_char(char chr)
{
    LCD_ODR_bit.LCD_RS = 1;     //set RS (sending data)
    LCD_send_byte(chr);         //send data byte
}

void print()
{
    LCD_send_cmd(LCD_HOME);     //set cursor home
    
    uint8_t endstr = 0x00;      //end of string flag
    
    for(uint8_t i=0; i<8;i++)   //printing first 8 chars
    {
        if(LCD_str[i] == 0)         //if strings end
            endstr = 0xFF;      //set flag
        
        if(endstr)                      //if string was ended
            LCD_send_char(' ');         //print rest spase as whitespaces
        else
            LCD_send_char(LCD_str[i]);      //otherwise, print text
    }
    
    LCD_send_cmd(LCD_SET_DD+40);        //set cursor on 9th symbol
    
    for(uint8_t i=8; i<16;i++)  //continue print
    {
        if(LCD_str[i] == 0)
            endstr = 0xFF;
        
        if(endstr)
            LCD_send_char(' ');
        else
            LCD_send_char(LCD_str[i]);
    }
}

uint8_t strcmp(char* str1, char* str2)
{
    for(int i=0; i<16; i++)
    {
        if(str1[i] != str2[i])
            return 0;
        if(str1[i] == 0 && str2[i] == 0)
            return 0xFF;
    }
    return 0xFF;
}

void strcpy(char* str1, char* str2)
{
    for(int i=0; i<16; i++)
    {
        str1[i] = str2[i];
    }
}

void printstr(char* str)
{
    if(!strcmp(LCD_str, str))
    {
        strcpy(LCD_str, str);
        print();
    }
}

void LCD_init(void)
{
    /* init LCD pins */
    LCD_DDR |= 0x7F;    //set pins 0-6 as output
    LCD_CR1 |= 0x7F;
    LCD_ODR_bit.LCD_E = 1;  //set strobe
    LCD_ODR_bit.LCD_RS = 0;
        
    /* init 4-bit mode */
    delay_ms(100);       //wait for power supply
    LCD_ODR &= 0xD0;
    //LCD_ODR |= (1<<lcd_D1);
    LCD_ODR |= (1<<lcd_D0) | (1<<lcd_D1);
    LCD_STROBE();
    
    delay_ms(5);
    
    LCD_ODR &= 0xD0;
    LCD_ODR |= (1<<lcd_D1);
    LCD_STROBE();
    delay_us(100);
    
    LCD_STROBE();
    delay_us(100);
    
    LCD_send_cmd(LCD_4_BIT+8);
    delay_us(40);
    
    /* Display ON */
    LCD_send_cmd(LCD_ON);
    delay_us(40);
    
    /* Drawing menu arrows */
    LCD_send_cmd(LCD_SET_GC+8);
    delay_us(40);
    
    /* Draw Up arrow */
    LCD_send_char(0x04);
    LCD_send_char(0x0E);
    LCD_send_char(0x1F);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    
    /* Draw Down arrow */
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0x1F);
    LCD_send_char(0x0E);
    LCD_send_char(0x04);
    
    /* Draw "Up and Down" arrow */
    LCD_send_char(0x04);
    LCD_send_char(0x0E);
    LCD_send_char(0x1F);
    LCD_send_char(0);
    LCD_send_char(0);
    LCD_send_char(0x1F);
    LCD_send_char(0x0E);
    LCD_send_char(0x04);
    
    /*Draw '+-' symbol */
    LCD_send_char(0x00);
    LCD_send_char(0x04);
    LCD_send_char(0x04);
    LCD_send_char(0x1F);
    LCD_send_char(0x04);
    LCD_send_char(0x04);
    LCD_send_char(0x1F);
    LCD_send_char(0x00);
    
    /* Clear display */
    LCD_send_cmd(LCD_CLEAR);
    delay_ms(2);
}