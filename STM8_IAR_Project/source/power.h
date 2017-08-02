#include "stdint.h"

#ifndef __POWER_H__
#define __POWER_H__

/* Function prototypes */
void Power_init(void);
void powerOn(void);
void powerOff(void);
void powerToggle(void);

/* define power pins */

#define PWR_DDR_bit PA_DDR_bit.DDR2
#define PWR_CR1_bit PA_CR1_bit.C12
#define PWR_ODR_bit PA_ODR_bit.ODR2

#endif
