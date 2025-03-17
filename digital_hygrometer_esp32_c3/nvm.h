/**
 *  @filename   :   nvm.h
 *  @brief      :   Header file for the hygrometer EEPROM (not to be mistaken for the Arduino EEPROM.h file) 
 *  @author     :   C. Guenther
 *  @details    :   The routines contained herein are simply wrapper functions 
 *                   that use the core EEPROM.h Arduino library
 *  
 *  Copyright (C) C. Guenther December 10 2024
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */

#ifndef HYGROMETENVM_H
#define HYGROMETENVM_H

#include <Arduino.h>    //This likely defines wire.h
#include <Preferences.h>
#include "app.h"

#define PREF_BUFF_ELEMENTS              32

/**
 * Keys for key-value 
 * NVM storage 
 */
#define PREF_EMAIL_AUTHOR_KEY           "auth"
#define PREF_EMAIL_RECIPIENT_KEY        "recipient"
#define PREF_EMAIL_AUTHOR_PASSWORD_KEY  "auth_pass"
#define PREF_WIFI_SSID                  "wifi_ssid"
#define PREF_WIFI_PASSWORD              "wifi_password"

#define PREF_RH_OFFSET1                 "rhoffset_1"
#define PREF_RH_OFFSET2                 "rhoffset_2"

#define PREF_TEMP_OFFSET1                "tempoffset_1"
#define PREF_TEMP_OFFSET2                "tempoffset_2"

#define PREF_CAL_KEY     "cal_stat"
#define PREF_NETWORK_VALID_KEY          "net_valid"     //TODO not sure if we'll need this

#define NVM_NAMESPACE                               "HYG_NVM"

//TODO can we do away with these?  
// TODO "keys" for preferences are defined in lan.h

/**
 * Hardcoded configuration status 
 * words
 */
#define WORD_EEPROM_INITALIZED                     0x5555  // 16bit initialization sequence //TODO not sure if we still need this
#define WORD_EEPROM_CAL_INDICATION                 0xAAAA  // 16bit calibration sequence 


// TODO: do we need to comment functions?

class NVM {
    private:
        uint8_t     characters_read_back    = 0x00;

    public:
        /**
         * @brief NVM init function
         * @param \p none 
         * @return nothing
         */
        void init(void);

        /**
         * @brief Erase the NVM 
         * @details NVM is 512 bytes
         * @param \p none
         * @return nothing
         */
        // void nvm_erase( void );

        /**
         * @brief NVM store byte
         * @details NVM is 512 bytes
         * @param \p address (uint 16 bit)  
         * @param \p Value (8 bit)
         * @return nothing
         */
        void nvm_store_byte (Preferences & pref, const char * nvmkey, uint8_t value);
        
        //TODO need to comment
        void nvm_store_int (Preferences & pref, const char * nvmkey, uint8_t value);

        /**
         * @brief NVM store float 
         * @details NVM is 512 bytes
         * @param \p address (uint 16 bit)  
         * @param \p value (float)s
         * @return nothing
         */
        void nvm_store_float (Preferences & pref, const char * nvmkey, float value);

        //TODO need to comment
        float nvm_get_float (Preferences & pref, const char * nvmkey);
        
        //TODO need to comment
        void nvm_store_string (Preferences & pref, const char * nvmkey, const char * data_buffer);
        

        //TODO need to comment 
        void nvm_read_string(Preferences & pref, const char * nvmkey, char (&arr)[PREF_BUFF_ELEMENTS]);
        
        
        //TODO need to comment 
        void load_network_parameters (void);
        
        //TODO need to comment 
        bool network_valid(Preferences & pref);
        
        
        /**
         * @brief NVM read byte
         * @details NVM is 512 bytes
         * @param \p address (uint 16 bit)  
         * @return The byte the occupies the read address
         */
        uint8_t nvm_read_byte (Preferences & pref, const char * nvmkey);
        
        //TODO need to comment 
        uint16_t nvm_read_int (Preferences & pref, const char * nvmkey);
        
        /**
         * @brief NVM store a string 
         * @param \p address (uint 16 bit)  
         * @param \p Sting (const char pointer)  
         * @return nothing 
         */
        // void nvm_store_string(uint16_t address, const char * string_in);
        
        /**
         * @brief NVM read a string 
         * @param \p address (uint 16 bit)  
         * @param \p buffer (char pointer to buffer)  
         * @return nothing 
         */
        // void nvm_read_string(uint16_t address, char * string_buffer);

        /**
         * @brief Breakdown a 32bit float and shove into a four byte array
         * @details Take a 32 bit floating point value and 
         * shove it, one byte at a time, into DataArray
         * @param \p float_value
         * @param \p four_byte_data_array
         * @return nothing 
         */
        // void nvm_parse_float(float float_value, uint8_t four_byte_data_array[]);
        
        /**
         * @brief Retrieve the 32 bit status word
         * @param \p address (uint 16b)
         * @return nothing 
         */
        // void nvm_get_status_word( uint16_t address );

        /**
         * @brief Determine if we are calibrated
         * @param \p none
         * @return bool 
         */
        // bool nvm_is_calibrated ( void );
        
        /**
         * @brief Determine if the EEPROM has been initialized 
         * @param \p none
         * @return bool 
         */
        // bool nvm_is_initalized ( void );

        
};

#endif