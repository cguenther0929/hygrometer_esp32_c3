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

void NVM::nvm_store_float (Preferences & pref, const char * nvmkey, float value)
{
    pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    pref.putFloat(nvmkey, value);
    pref.end();                         // Use to close
}

float NVM::nvm_get_float (Preferences & pref, const char * nvmkey)
{
    float temp_float    = 0.0;
    pref.begin(NVM_NAMESPACE, true);       // Pass in false to use the algorithm in read/write mode
    temp_float = pref.getFloat(nvmkey);
    pref.end();                         // Use to close
    
    return(temp_float);

}

void NVM::nvm_store_string(Preferences & pref, const char * nvmkey, const char * data_buffer)
{
    pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    
    pref.putString(nvmkey, data_buffer);     // Store the string value. 
    
    pref.end();                         // Use to close

}

void NVM::nvm_read_string(Preferences & pref, const char * nvmkey, char (&arr)[PREF_BUFF_ELEMENTS])
{
    pref.begin(NVM_NAMESPACE, true);            // for read only, we pass in false

    this -> characters_read_back = pref.getString(nvmkey, arr, sizeof(arr));

    pref.end();                             // Use to close
    
    if(this -> characters_read_back <= 0)
    {
        if(ENABLE_LOGGING)
        {
            Serial.println("NVM read function failed");
        }
    }
    else
    {
        if(ENABLE_LOGGING)
        {
            Serial.println("NVM read function passed");
        }
    }
}
    

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


