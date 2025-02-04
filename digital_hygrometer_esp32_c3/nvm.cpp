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

void NVM::nvm_store_string(Preferences & pref, const char * nvmkey, const char * data_buffer)
{
    char buffer_for_readback[32] = {NULL}; //TODO this is in just for testing


    Serial.print("***DEBUG buffer passed in: ");
    Serial.println(data_buffer);

    // data_buffer = data_buffer.c_str();

    pref.begin(NVM_NAMESPACE, false);       // Pass in false to use the algorithm in read/write mode
    // pref.remove(nvmkey);             //TODO can we put this back in?
    
    pref.putString(nvmkey, data_buffer);     // Store the string value. 
    
    pref.end();                         // Use to close


    //TODO the following is just for testing
    delay(500);

    pref.begin(NVM_NAMESPACE, true);    
    pref.getString(nvmkey,buffer_for_readback,sizeof(buffer_for_readback));
    pref.end();    

    Serial.print ("***DEBUG String read back in this function: ");
    Serial.println(buffer_for_readback);


}

void NVM::nvm_read_string(Preferences & pref, const char * nvmkey, char (&arr)[32])
{

    char local_data_buffer[15]; //TODO this is just for testing
    
    
    pref.begin(NVM_NAMESPACE, true);            // for read only, we pass in false

    // Serial.print("The size of the data buffer: "); //TODO this is just for testing
    // Serial.println(sizeof(data_buffer));            //TODO this is just for testing

    
    // this -> characters_read_back = pref.getString(nvmkey, data_buffer, sizeof(data_buffer));
    this -> characters_read_back = pref.getString(nvmkey, arr, sizeof(arr));

    // Serial.print("***DEBUG value of local data buffer: ");
    // Serial.println(local_data_buffer);

    
    pref.end();                             // Use to close
    
    Serial.print("***DEBUG chars read in function: ");
    Serial.println(this->characters_read_back);
    
    if(this -> characters_read_back <= 0)
    {
        Serial.println("***DEBUG FUNCTION **FAILED**");
    }
    else
    {
        Serial.println("***DEBUG FUNCTION PASSED");
    }

    // Serial.print("***DEBUG buffer in function: ");
    // Serial.println(data_buffer);

    
    
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

