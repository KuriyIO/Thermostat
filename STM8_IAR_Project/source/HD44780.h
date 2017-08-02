#ifndef __HD44780_H__
#define __HD44780_H__

/* function prototypes */
void LCD_led_on(void);
void LCD_led_off(void);
void LCD_send_byte(uint8_t data);
void LCD_send_cmd(uint8_t cmd);
void LCD_send_char(char chr);
void print(void);
void printstr(char* str);
uint8_t strcmp(char* str1, char* str2);
void strcpy(char* str1, char* str2);
void LCD_init(void);

/* global variables */
extern char LCD_str[16];

/* define port registers */
#define LCD_DDR PB_DDR
#define LCD_CR1 PB_CR1
#define LCD_ODR PB_ODR

#define LCD_DDR_bit PB_DDR_bit
#define LCD_CR1_bit PB_CR1_bit
#define LCD_ODR_bit PB_ODR_bit

/* define port pins */
#define LCD_D0 ODR0
#define LCD_D1 ODR1
#define LCD_D2 ODR2
#define LCD_D3 ODR3
#define LCD_E ODR4
#define LCD_RS ODR5
#define LCD_LED ODR6

enum lcd_pins 
{
    lcd_D0,
    lcd_D1,
    lcd_D2,
    lcd_D3,
    lcd_E,
    lcd_RS,
    lcd_LED,
};

/* strobe macro */
#define LCD_STROBE() LCD_ODR_bit.LCD_E = 0;             \
                     delay_us(1);                       \
                     LCD_ODR_bit.LCD_E = 1;
                     
/* define menu symbols */
#define LCD_UP 1
#define LCD_DOWN 2
#define LCD_UP_DOWN 3
                     
/* define display commands */
#define LCD_4_BIT       (0x20)
#define LCD_ON          (0x0C)
#define LCD_OFF         (0x08)
#define LCD_HOME        (0x02)
#define LCD_CLEAR       (0x01)
#define LCD_SET_GC      (0x40)
#define LCD_SET_DD      (0x80)
                     
/* Led on macro */
#define LCD_led_on() LCD_ODR_bit.LCD_LED = 1;
                     
/* Led off macro */
#define LCD_led_off() LCD_ODR_bit.LCD_LED = 0;
                     
#endif
