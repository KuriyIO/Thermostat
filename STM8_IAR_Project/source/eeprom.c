#include "eeprom.h"

#define PAttr __far

void EEPROM_Unlock()
{
  /* Unlock data memory */
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;
}

void EEPROM_Lock()
{
  FLASH_IAPSR &= 0xF7;
}

void EEPROM_ProgramByte(uint32_t Address, uint8_t Data)
{
  if (Address >= EEPROM_START_ADDRESS && Address <= EEPROM_END_ADDRESS)
  *(PAttr uint8_t*) (uint16_t)Address = Data;
}

uint8_t EEPROM_ReadByte(uint32_t Address)
{
  /* Read byte */
  return(*(PAttr uint8_t *) (uint16_t)Address);
}

void EEPROM_EraseByte(uint32_t Address)
{
  /* Check parameter */
  *(PAttr uint8_t*) (uint16_t)Address = FLASH_CLEAR_BYTE; /* Erase byte */
}

int __eeprom_wait_for_last_operation(void)
{
  int status = FLASH_WaitForLastOperation(FLASH_MemType_Data);
  return !!(status & (FLASH_Status_Write_Protection_Error | FLASH_Status_Successful_Operation));
}

void __eeprom_program_byte(unsigned char __near * dst, unsigned char v)
{
  FLASH_ProgramByte((uint32_t)dst, (uint32_t)v);
}

void __eeprom_program_long(unsigned char __near * dst, unsigned long v)
{
  FLASH_ProgramWord((uint32_t)dst, (uint32_t)v);
}