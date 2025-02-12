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
#include <esp_sleep.h>    //This likely defines wire.h
#include "nvm.h"
#include "i2c.h"
#include "epdif.h"
#include "driver/rtc_io.h"
#include "epd1in54_V2.h"
#include "epdpaint.h"
#include "imagedata.h"

// ==============================
// ==============================
#define     SW_VER_STRING       "0.2.0" 
// ==============================
// ==============================

/**
 * Set to true to 
 * enable logging
 */
#define ENABLE_LOGGING                true

/**
 * Health LED
 */
#define HEALTH_LED                10

/**
 * Interrupt / button pin
 * 
 */
#define INTERRUPT_PIN             LOCAL_BTN_GPIO_PIN    //RTC pins are GPIO0-GPIO3; the button ties to IO1, so the mask shall be 1

/**
 * Analog and battery parameters
 */
#define MIN_BATT_VOLTAGE          3.0



/**
 * Serial parameters
 */
#define SERIAL_BAUD_RATE          115200

/**
 * Button related
 */

#define LOCAL_BTN_GPIO_PIN        1
#define SHORT_PRESS_50MS_EVENTS   10
#define LONG_PRESS_50MS_EVENTS    20
#define WAKEUP_GPIO               GPIO_NUM_1   

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
#define nSENSOR_PWR_EN              3
#define SENSOR_MUX_RST_LINE         9

typedef enum State {
  STATE_SLEEP,
  STATE_READ_DATA,
  STATE_UPDATE_DISPLAY,
  STATE_SEND_EMAIL
};

/**
 * ESP 32 Analog related parameters
 * It seems that that informaiton available online 
 * is incorrect.  
 * Some of the following values had to be 
 * empirically derived. 
 * We know the attenuation is 11db, which works 
 * out to a voltage gain of 3.548.  Knowing 
 * this, we demanded the digital code be printed 
 * out from the ADC during the "battery read"
 * routein.  Knowing the voltage feeding the pin, 
 * the internal attenuation value, and the fact that
 * we are dealing with 4096 steps (12 bit), we are 
 * able to discern the analog reference voltage. 
 */
#define ANALOG_BATT_PIN             A0      // The analog pin on the ESP32-C that is 'watching' the battery voltage
#define HYG_ADC_REFERENCE           0.50    // ESP32-C3 ADC reference (calculated)
#define HYG_ADC_BIT_VALUE           4096    // ESP32-C3 Full scale digital reading +1
#define HYG_PCB_ATTEN               5.435   // Attenuation that is applied on the hygrometer PCB
#define HYG_ESP32_INTERNAL_ATTEN    3.548   // The ESP32-C3 employs 11db of attenuation


// typedef struct network_info  //TODO I think we can put these down in the class
// {
//   char hyg_name[HYG_NAME_STR_LEN];
//   char wifi_ssid[WIFI_SSID_STR_LEN];
//   char wifi_password[WIFI_PASS_STR_LEN];
//   char recipient_email_address [RECIPIENT_EMAIL_STR_LEN];
//   char sender_email_address[HYG_SENDER_EMAIL_USER_NAME_STR_LEN ];

//   bool enable_email;                                      //Flag to determine if emails shall be sent
// };



class APP
{
    public:
        State state                     = STATE_SLEEP;
        
        uint16_t btn_press_ctr          = 0x000;
        bool btn_interrupt_triggered    = false;
        bool btn_short_press_flag       = false;
        bool btn_long_press_flag        = false;
        bool calibrate_sensors          = false;  

        uint16_t seconds_counter        = 0x0000;

        float battery_voltage           = 0.0;

        bool heartbeat_enabled          = true;

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
        
        //TODO: need to comment
        void display_post_message( void );

        /**
         * @brief Display splash screen (i.e. SW ver)
         * @param \p none 
         * @return nothing
         */
        void full_screen_refresh(void);
        
        /**
         * @brief Update the Eink display 
         * @param \p none 
         * @return nothing
         */
        void update_display( void );
        
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
        float get_battery_voltage ( void ); 

        /**
         * @brief Handle button press   
         * @param \p none 
         * @return nothing 
         */
        void button_handler ( void );
        
        //TODO need to comment
        void state_handler( State current_state );


        
};

#endif