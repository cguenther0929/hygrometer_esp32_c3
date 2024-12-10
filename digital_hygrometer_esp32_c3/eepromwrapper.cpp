#include "eepromwrapper.h"


void MYPROM::init(void) 
{
    //TODO: need to define this function 
    __asm__("nop\n\t");
}

uint8_t eeprom_erase ( void )  
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

void MYPROM::eeprom_store_byte (uint16_t address, uint8_t value)
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}
void MYPROM::eeprom_store_float (uint16_t address, uint8_t value)
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

uint8_t MYPROM::eeprom_read_byte (uint16_t address)
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

void MYPROM::eeprom_store_string(uint16_t address, const char * string_in)
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

void MYPROM::eeprom_read_string(uint16_t address, char * string_buffer)
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

void MYPROM::eeprom_parse_float(float float_value, uint8_t four_byte_data_array[])
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

void MYPROM::eeprom_get_status_word( void )
{
    //TODO: need to define this function 
    __asm__("nop\n\t");

}

