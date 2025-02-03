#include "nvm.h"



void NVM::init(void) 
{
    __asm__("nop\n\t");
}

/**
 *                                                     Key of key:value pair
 *                                                         |            Value to write to NVM  
 *                                                         |               |
 */
void NVM::nvm_store_byte (Preferences & pref, const char * nvmkey, uint8_t value)
{

    pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    // pref.clear(); //TODO remove this line?
    pref.remove(nvmkey);            
    
    pref.putBytes(nvmkey, &value, 1);    // Store the string value. Define the length to be one
    
    pref.end();                         // Use to close

}

uint8_t NVM::nvm_read_byte (Preferences & pref, const char * nvmkey)
{
    uint8_t uint8t_data_buffer[4];

    pref.begin(NVM_NAMESPACE, true);            // for readOnly, we pass in true
    
    pref.getBytes(nvmkey, uint8t_data_buffer, 1);
    
    pref.end();                             // Use to close

    return uint8t_data_buffer[0];
}

void NVM::nvm_store_string(Preferences & pref, const char * nvmkey, const char * value)
{

    pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    pref.remove(nvmkey);            
    
    pref.putString(nvmkey, value);     // Store the string value. 
    
    pref.end();                         // Use to close
}

void NVM::nvm_read_string(Preferences & pref, const char * nvmkey, char * data_buffer)
{
    //TODO this needs a lot of work
    // char test_read_buffer[32] = {NULL};
    // char test_write_buffer[5] = {'7','7','7','7','\0'};
    

    // String test_write_string    = "77777";
    // String test_read_string     = "";

    // getString(const char* key, char* value, const size_t maxLen)

    // pref.begin(NVM_NAMESPACE, false);      
    
    // TODO this is the line we want 
    // pref.begin(NVM_NAMESPACE, true);            // for read only, we pass in true
    
    // pref.putString(nvmkey, test_write_string);     // Store the string value. 
    // delay(500);
    
    
    // if(pref.getString(nvmkey, data_buffer, 5))
    // if(pref.getString(nvmkey, test_read_buffer, sizeof(test_write_buffer)))
    // if(pref.getString(nvmkey, test_read_buffer, sizeof(test_read_buffer)))
    // test_read_string = pref.getString(nvmkey,"");
    // {
    //     Serial.println("\t***DEBUG FUNCTION PASSED");
    // }
    // else{
    //     Serial.println("\t***DEBUG FUNCTION FAILED");
    // }

    // Serial.print("***DEBUG buffer in function: ");
    // Serial.println(test_read_string);

    
    // pref.end();                             // Use to close
    
}
    
// void NVM::nvm_read_string(uint16_t nvmkey, char * string_buffer)
// {
//     //TODO: need to define this function 
//     __asm__("nop\n\t");

// }


// void NVM::nvm_store_float (Preferences & pref, const char * nvmkey, float value)
// {
    // pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    // pref.remove(nvmkey);            
        // pref.putFloat(nvmkey, value);
    // pref.end();                         // Use to close

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

