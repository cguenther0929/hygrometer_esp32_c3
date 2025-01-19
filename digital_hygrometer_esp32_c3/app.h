/**
 *  @filename   :   app.h
 *  @brief      :   Header file for general hygrometer application functions  
 *  @author     :   C. Guenther
 *  @details    :   The routines contained herein are very specific to the 
 *                  Digital Hygrometer project
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

#ifndef HYGROMETERFUNCTIONS_H
#define HYGROMETERFUNCTIONS_H

#include <Arduino.h>    //This likely defines wire.h
#include "nvm.h"
#include "epdif.h"

/**
 * Serial parameters
 */
#define SERIAL_BAUD_RATE          115200

/**
 * General parameters related 
 * to the hygrometer application
 */

/**
 * Sensor related
 */
#define SENSOR_1                    1
#define SENSOR_2                    2

/* Define IO */
#define PUSH_BUTTON                 1
#define nSENSOR_PWR_EN              3
#define SENSOR_MUX_RST_LINE         9

/**
 * Analog related parameters
 */
#define ANALOG_BATT_PIN             A0      // The analog pin on the ESP32-C that is 'watching' the battery voltage
#define HYG_ADC_REFERENCE           1.10    // ESP32-C3 ADC reference
#define HYG_ADC_BIT_VALUE           4096    // ESP32-C3 Full scale digital reading +1
#define HYG_PCB_ATTEN               5.435   // Attenuation that is applied on the hygrometer PCB
#define HYG_ESP32_INTERNAL_ATTEN    2.34    // The ESP32-C3 employes some amount of internal attenuation (this was empirically derived)


typedef struct sensor_info  //TODO we might be able to remove this ...
{
    uint8_t     rh_reading_sensor_one;
    uint8_t     rh_reading_sensor_two;
    uint8_t     temp_reading_sensor_one;
    uint8_t     temp_reading_sensor_two;
};

typedef struct network_info
{
    char hyg_name[HYG_NAME_STR_LEN];
    char wifi_ssid[WIFI_SSID_STR_LEN];
    char wifi_password[WIFI_PASS_STR_LEN];
    char recipient_email_address [RECIPIENT_EMAIL_STR_LEN];
    char sender_email_address[HYG_SENDER_EMAIL_USER_NAME_STR_LEN ];

    // uint32_t counts_20ms_between_emails; //TODO: remove? 

    bool enable_email;                                      //Flag to determine if emails shall be sent
};



class APP{
    //TODO: remove the following lines? 
    // private:
    //     uint8_t user_selection      = 0;

    public:
        /**
         * @brief APP init function
         * @param \p none 
         * @return nothing
         */
        void init(void);

        //TODO: need to comment
        void sensor_power_on(void);
        
        //TODO: need to comment
        void sensor_power_off(void);
        
        /**
         * @brief Display splash screen (i.e. SW ver)
         * @param \p none 
         * @return nothing
         */
        void display_splash_screen(void);
        
        /**
         * @brief Determins if network parameters are valid 
         * @param \p none 
         * @return bool
         */
        bool network_parameters_valid( void );

        /**
         * @brief Get battery voltage  
         * @param \p none 
         * @return float value of battery voltage
         */
        float get_battery_voltage (void); 


        
};

#endif