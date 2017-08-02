#include "iostm8l051f3.h"
#include "intrinsics.h"
#include "stdint.h"
#include "thermistor.h"
#include "delay.h"
#include "sleep.h"

static const uint16_t Table[TABLE_SIZE] = {
    4095, 4061, 3994, 3926, 3858, 3790, 3721, 3653,
    3584, 3515, 3445, 3376, 3308, 3239, 3170, 3102,
    3035, 2967, 2901, 2834, 2769, 2704, 2640, 2576,
    2514, 2452, 2391, 2331, 2272, 2214, 2157, 2101 };

tempValue temperature = {0,0,0};

static uint8_t binarySearch(uint16_t Value)
{
    uint8_t i, left = 0, right = TABLE_SIZE-1;
    while(1)
    {
        /* Set current element */
        i = (left+right)>>1;   
        
        /* Return current element was found */
        if(Value < Table[i-1] && Value >= Table[i])
            break;
        
        /* Check IndexOutOfBound */
        if(i == 0)
            if(Value < Table[i+1])
                break;
        if(i == TABLE_SIZE-1)
            if(Value > Table[i-1])
                break;
        
        /* Check for last Value */
        if(i == TABLE_SIZE-2)
            if(Value < Table[i])
            { i++; break; }
        
        /* Check for depth */
        if(left == right)
            break;
        
        /* Otherwise chose left or right */
        if(Value < Table[i])
            left = i;
        else
            right = i;
    }
    
    return i;
}

uint16_t getADC()
{
    uint16_t ADC_Value = 0;
    
    if(!ADC1_CR1_bit.ADON)      //if ADC sleeping
    {
        ADC1_CR1_bit.ADON = 1;  //wake up
        delay_us(5);            //wait for ADC wake up
    }
    
    __disable_interrupt();
    ADC1_CR1_bit.START = 1;     //Start measuring
    while(!ADC1_SR_bit.EOC);     //wait for ADC measurment
    
    ADC_Value = ADC1_DRH << 8;  //get result
    ADC_Value |= ADC1_DRL;
    __enable_interrupt();
    
    if(ADC_Value == 0)
        sleep();
    
    return ADC_Value;
}

void getTemp()
{    
    uint16_t ADC_Value = getADC();
    uint8_t i = binarySearch(ADC_Value);
    
    if(i == 0)
    {
        temperature.th = 1;
        temperature.tl = 0;
        temperature.frac = 0;
        return;
    }
    
    if(i == TABLE_SIZE-1)
    {
        temperature.th = 4;
        temperature.tl = 1;
        temperature.frac = 0;
        return;
    }
    
    uint8_t temp = (i*C_per_i + TEMP_START);
    temperature.th = temp/10;
    temperature.tl = temp-(temperature.th * 10);
    
    uint16_t round = ((ADC_Value - Table[i])*100)/(Table[i-1] - Table[i]);
    if(round-((round/10)*10) > 4)
    {
        if((round/10) < 9)
            temperature.frac = (round/10)+1;
        else
            temperature.frac = (round/10); 
    }
    else
        temperature.frac = (round/10); 

}

void ADC_init()
{
    CLK_PCKENR2_bit.PCKEN20 = 1; //Enable clock for ADC
    
    ADC1_CR1_bit.ADON = 1;      //Enable ADC
    delay_us(5);                //wait for ADC wake up
    ADC1_CR1_bit.RES = 0;       //12-bit resolution
    ADC1_CR2_bit.SMTP1 = 0;     //4 clocks sampling time
    ADC1_SQR1_bit.DMAOFF = 1;   //Disable DMA
    
    ADC1_TRIGR2_bit.TRIG22 = 1;     //Deisable trigger for ADC pin
    ADC1_SQR2_bit.CHSEL_S22 = 1;    //select pin ADC_IN22

}