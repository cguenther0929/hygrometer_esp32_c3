#include "nvm.h"


// nvm_32b_union convert;  //TODO: do we need something like this
Preferences pref;

void NVM::init(void) 
{
    //TODO: need to define this function 
    __asm__("nop\n\t");
}

// void NVM::nvm_erase ( void )  
// {
    // __asm__("nop\n\t");
    // uint16_t i = 0;             
    // for(i = 0; i < EEPROM_SIZE_IN_BYTES; i++) 
    // {
        
    //     EEPROM.write(i, NULL);  // Write NULL to the EEPROM, one byte at a time
    // }
    

// }

/**
 * The nvmkey parameter is arg [1] while the value
 * is arg[0]
 *                                   Key of key:value pair
 *                                       |            Value to write to NVM  
 *                                       |               |
 */
void NVM::nvm_store_byte (const char * nvmkey, uint8_t value)
{
    // __asm__("nop\n\t");
    
    // Preferences pref;

    pref.begin("My Test", false); //Pass in false to use the algorithm in read/write mode
    pref.clear();
    
    pref.putBytes(nvmkey, &value, 1);      //Store the value.  The function used will depend on the data type
    
    pref.end();              //Use to close

}

uint8_t NVM::nvm_read_byte (const char * nvmkey)
{
    // __asm__("nop\n\t");
    
    // Preferences pref;
    uint8_t uint8t_data_buffer[4];

    pref.begin("My Test", true); //Pass in false to use the algorithm in read/write mode
    
    pref.getBytes(nvmkey, uint8t_data_buffer, 1);
    
    pref.end();              //Use to close
    

//     // return uint8t_data_buffer[0];
}

// void NVM::nvm_store_float (uint16_t nvmkey, uint8_t value)
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");

// }


// void NVM::nvm_store_string(uint16_t nvmkey, const char * string_in)
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");

// }

// void NVM::nvm_read_string(uint16_t nvmkey, char * string_buffer)
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");

// }

// void NVM::nvm_parse_float(float float_value, uint8_t four_byte_data_array[])
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");

// }

// void NVM::nvm_get_status_word( uint16_t nvmkey ) //TODO: can we remove this function?  I don't think we need this wrapper
// {
        
//     //TODO: need to define this function 
//     __asm__("nop\n\t");
   

// }

// bool NVM::nvm_is_calibrated ( void )
// {
//     __asm__("nop\n\t");  //TODO: need to define this function
//     /**
//      * Read the configuration word 
//      * to see if we are calibrated
//     */
//     //    nvm_get_status_word();

//     // uint8_t i = 0;             
//     // for(i = 0; i < 4; i++)
//     //  {
        
//     //     convert.nvm_byte_buffer[i] = EEPROM.read(EEPROM_ADDR_STAT_DWORD + i);  // Grabs one byte of info from the EEPROM
//     // }
    
//     // if (convert.ulong_nvm_number == DWORD_EEPROM_CAL_INDICATION)
//     // {
//     //     return true;
//     // }
//     // else 
//     // {
//     //     return false;
//     // }
// }

// bool NVM::nvm_is_initalized ( void )
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");
//     /**
//      * Read the configuration word 
//      * to see if we are calibrated
//     */
//     //    nvm_get_status_word();

//     // uint8_t i = 0;             
//     // for(i = 0; i < 4; i++) {
        
//     //     convert.nvm_byte_buffer[i] = EEPROM.read(EEPROM_ADDR_STAT_DWORD + i);    // Grabs one byte of info from the EEPROM           
//     // }
    
//     // if (convert.ulong_nvm_number == DWORD_EEPROM_INITALIZED)
//     // {
//     //     return true;
//     // }
//     // else 
//     // {
//     //     return false;
//     // }
// }

