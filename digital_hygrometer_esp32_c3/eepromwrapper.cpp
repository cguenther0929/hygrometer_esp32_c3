#include "eepromwrapper.h"


eeprom_32b_union convert;

void MYPROM::init(void) 
{
    //TODO: need to define this function 
    __asm__("nop\n\t");
}

void MYPROM::eeprom_erase ( void )  
{
    uint16_t i = 0;             
    for(i = 0; i < EEPROM_SIZE_IN_BYTES; i++) 
    {
        
        EEPROM.write(i, NULL);  // Write NULL to the EEPROM, one byte at a time
    }
    

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

void MYPROM::eeprom_get_status_word( uint16_t address ) //TODO: can we remove this function?  I don't think we need this wrapper
{
        
    //TODO: need to define this function 
    __asm__("nop\n\t");
   

}

bool MYPROM::eeprom_is_calibrated ( void )
{
    /**
     * Read the configuration word 
     * to see if we are calibrated
    */
//    eeprom_get_status_word();

    uint8_t i = 0;             
    for(i = 0; i < 4; i++)
     {
        
        convert.eeprom_byte_buffer[i] = EEPROM.read(EEPROM_ADDR_STAT_DWORD + i);  // Grabs one byte of info from the EEPROM
    }
    
    if (convert.ulong_eeprom_number == DWORD_EEPROM_CAL_INDICATION)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool MYPROM::eeprom_is_initalized ( void )
{
    /**
     * Read the configuration word 
     * to see if we are calibrated
    */
//    eeprom_get_status_word();

    uint8_t i = 0;             
    for(i = 0; i < 4; i++) {
        
        convert.eeprom_byte_buffer[i] = EEPROM.read(EEPROM_ADDR_STAT_DWORD + i);    // Grabs one byte of info from the EEPROM           
    }
    
    if (convert.ulong_eeprom_number == DWORD_EEPROM_INITALIZED)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

