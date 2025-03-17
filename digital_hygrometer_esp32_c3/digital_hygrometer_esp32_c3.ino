/**
 *  @filename   :   epd1in54-demo.ino
 *  @brief      :   1.54inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 5 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
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
 * TODO the SW version needs to come across in email. 
 * TODO On 3/16/25 there was an attempt to get the button
 * counter to increment and work, but that proved to be unsuccessful
 * therefore, it might be nice to get this piece of the RTC memory working.
 * For now, "boot_count" is written to NVM.
 * 
 */


#include <Arduino.h>
#include <stdio.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_timer.h>
#include <Preferences.h>
#include "nvm.h"
#include "i2c.h"
#include "console.h"
#include "app.h"
#include "lan.h"

/**
 * Instantiate the Preferences class
 * which is used for NVM
 */ 
Preferences pref;   

/**
 * Global variable to 
 * keep track of how many 
 * times the module has 
 * rebooted.  
 */


// uint8_t boot_counter = 0; //TODO need to have this stored in pref
RTC_DATA_ATTR int boot_counter = 0;

// uint8_t local_boot_counter = boot_counter;
int local_boot_counter = 0;
uint8_t test_variable = 0;

/**
 * Timer parameters
 */
unsigned int    ms_ticks_50           = 0;
unsigned int    ms_ticks_100          = 0;
unsigned int    ms_ticks_500          = 0;
unsigned int    ms_ticks_1000         = 0;

bool            Timer50msFlag         = false;
bool            Timer100msFlag        = false;
bool            Timer500msFlag        = false;
bool            Timer1000msFlag       = false;

char rx_char                          = '\n';

/**
 * Time structure 
 */
hw_timer_t *Timer1_Cfg = NULL;     //TODO can we rename this?

I2C     main_i2c;
CONSOLE console;
LAN     lan;
NVM     nvm_functions;
APP     app;

/**
 * Wake from deep sleep using a timer
 * ================================
 * ESP32 Deep Sleep Mode Discussion
 * https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/
 * 
 * Some larger points from the article
 * First, you need to configure the wake up sources. 
 * This means configure what will wake up the ESP32. 
 * You can use one or combine more than one wake up source.
 * You can decide what peripherals to shut down or keep 
 * on during deep sleep. However, by default, the ESP32 automatically
 * powers down the peripherals that are not needed with the
 * wake up source you define. Finally, you use the esp_deep_sleep_start() 
 * function to put your ESP32 into deep sleep mode.
 * The ESP32 can go into deep sleep mode, and then wake up at predefined
 * periods of time. This feature is specially useful if you are
 * running projects that require time stamping or daily tasks, 
 * while maintaining low power consumption.
 * Enabling the ESP32 to wake up after a predefined amount of
 * time is very straightforward. In the Arduino IDE, you 
 * just have to specify the sleep time in microseconds in the following function:
 * esp_sleep_enable_timer_wakeup(time_in_us)
 * 
 * 
 * ================================
 * Wake from deep sleep using an IO pin
 * ================================
 * 
 * https://hutscape.com/tutorials/external-wakeup-arduino-esp32c3
 * 
 * There is some incorrect information floating around about waking the processor.  
 * The aforementioned article points out that we shall use 
 * esp_deep_sleep_enable_gpio_wakeup(1 << INTERRUPT_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);
 * instead of sp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1); 
 * to wake the processor.  
 * 
 * The ESP32-C3 will wake up from an interrupt that can be caused
 * by an RTC pin.  According to a Google search, these are pins
 * GPIO0, GPIO1, GPIO2, and GPIO3.  On the hygrometer, the calibration 
 * button feeds into IO1.
 * 
 * The wakeup IO needs to be defined as a mask,
 * it was empirically determined that this value is one based.  Therefor, the mask
 * shall be 0 for GPIO0, 1 for GPIO1, and so on and so forth.  
 * 
 * 
 // This is how to place the ESP32 into deep sleep 
 //                               Value in uS  
 //                                 |  
 // esp_sleep_enable_timer_wakeup(1000);
 // esp_deep_sleep_start();  //This will put the module into deep sleep
 */

/**
 * Note for attaching an interrupt 
 * ================================
 * These are notes on attaching a button interrupt.
 * https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
 * 
 */

/**
 * Note for using a timer interrupt  
 * ================================
 * https://circuitdigest.com/microcontroller-projects/esp32-timers-and-timer-interrupts
 *  
 */

/**
 * @brief Timer interrupt
 * @details This function has to 'live' 
 * up above the setup routine
 * 
 */
void IRAM_ATTR onTimer()
{
  Timer50msFlag = true;

  if(ms_ticks_50 == 2) {
    ms_ticks_50 = 0;
    Timer100msFlag = true;
    if(ms_ticks_100 == 5) {
      ms_ticks_50 = 0;               //Reset centi-seconds
      Timer500msFlag = true;
    
      if(ms_ticks_500 == 2) {         //See if we need to roll over
        ms_ticks_500 = 0;
        Timer1000msFlag = true;  
      }
      else {
        ms_ticks_500++;              //Increase 500ms timer
      }

    }
    else {
        ms_ticks_100++;
    }
    
  }
  else {
    ms_ticks_50++;
  }
}

/**
 * @brief Button Press Interrupt 
 * @details This function has to 'live' 
 * up above the setup routine
 */
void IRAM_ATTR button_press()    
{
  if(ENABLE_LOGGING)
  {
    Serial.println("^Button interrupt triggered.");
  }
  
  /**
   * All we need this function to 
   * do is to wake up the processor.
   * The buttonhandler routine will
   * increment the button counter
   * and act accordingly 
   */
  app.btn_interrupt_triggered  = true;
  detachInterrupt(digitalPinToInterrupt(LOCAL_BTN_GPIO_PIN));  
}

/**
 * @brief Arduino Setup routine
 */
void setup() {
  
  memset(& local_boot_counter, boot_counter, sizeof(local_boot_counter));
  
  // if(local_boot_counter == 0)
  // {
    Serial.begin(SERIAL_BAUD_RATE);
    
    pinMode(HEALTH_LED,OUTPUT);
    digitalWrite(HEALTH_LED, HIGH);
    
    pinMode(nSENSOR_PWR_EN,OUTPUT);
    digitalWrite(nSENSOR_PWR_EN,HIGH);   // Default is to keep sensor power off 
    
    /**
     * @brief Define IO interrupt for push button input 
     */
    //           IO Pin number that shall trigger the interrupt                              
    //                      |         Name of the callback function               
    //                      |               |     Type of signal edge to detect    
    //                      |               |         |
    attachInterrupt(LOCAL_BTN_GPIO_PIN, button_press, RISING); 

    /**
     * @brief Function for Defining which IO shall wake the MCU from deep sleep
     * @details This function will allow an IO pin (RTC1-5) 
     * to wake the processor from deep sleep mode
     * It's unclear if allowing the processor to be
     * awoken from deep sleep in this manner eats more 
     * power. 
     * TODO If the mask version is used, the device will almost 
     * TODO immediately wake up (BAD).  The third option allows for 
     * TODO proper sleep operation, however, that routine doesn't
     * TODO wake up the processor like we want.  
     * TODO I also learned that the ESP32C3 does not support 
     * TODO RTC wakeup, so we need to be careful when pulling examples
     * TODO from online
     * TODO The ESP32 Wiki mentions that GPIO wakeup is for ***light sleep***
     * TODO If we want to wake the processor from deep sleep, then
     * TODO we'll have to use a different processor
     */
    //                                  A mask value needs to be passed in (empirically found to be one-based)
    //                                     |                  Parameter for the input signal   
    //                                     |                     |
    //                                     |                     |
    // esp_deep_sleep_enable_gpio_wakeup(1 << (INTERRUPT_PIN + 1), ESP_GPIO_WAKEUP_GPIO_HIGH);  
    // esp_deep_sleep_enable_gpio_wakeup(1 << 2, ESP_GPIO_WAKEUP_GPIO_HIGH);  
    // esp_deep_sleep_enable_gpio_wakeup(GPIO_NUM_1, ESP_GPIO_WAKEUP_GPIO_HIGH);
    
    
    
    /**
     * Remaining initialization functions
     */
    if(ENABLE_LOGGING)
    {
      Serial.println("^Calling remaining initialization functions");
    }
    
    main_i2c.init();          
    console.init();
    lan.init();
    nvm_functions.init();
    app.init();
    
    
    if(ENABLE_LOGGING)
    {
      Serial.println("^Boot Count equal to 0");
    }
    
    Serial.println("===================================================");
    Serial.println("====================== Reset ======================");
    Serial.println("===================================================");
    
    //Initialize timer interrupt
    //                     The frequency of the timer   
    //                       |     
    // Timer1_Cfg = timerBegin(1000000);   
    
    //                 Name of timer (from above) 
    //                     |      Name of callback function       
    //                     |        |     true (the tutorial did not indicate what this mans)
    //                     |        |        |     
    timerAttachInterrupt(Timer1_Cfg,&onTimer);    
    
    //       This is the timer struct 
    //           |        This is the alarm value (so alarm when we count up to this value)       
    //           |          |    true = to tell the timer to reload 
    //           |          |      |  Value to reload into the timer when auto reloading
    //           |          |      |   |
    timerAlarm(Timer1_Cfg, 50000, true,0);   
    
    /**
     * Print inital POST screen and 
     * then refresh the display
     */
    if(ENABLE_LOGGING)
    {
      Serial.println("^Printing splash screen.");
    }
    
    /**
     * Get the temperature offset from memory
     */
    //TODO we want to turn the following back on
    main_i2c.temp_offset = nvm_functions.nvm_get_float(pref,PREF_TEMP_OFFSET1); //TODO 1 and 2 uses the same offset
    if(ENABLE_LOGGING)
    {
      Serial.print("^Temp offset: ");
      Serial.println(main_i2c.temp_offset);
    }
    
    app.display_post_message();
    app.full_screen_refresh(pref);

    app.state = STATE_SLEEP;



    //TODO TEST CODE STARTS HERE
    
    if(ENABLE_LOGGING)
    {
      Serial.println("\t*** DEBUG Sleep test routine");
    } 
    
    Serial.print("^Boot counter: ");
    Serial.println(boot_counter);
    
    boot_counter = boot_counter + 1 ;   //TODO here is where we increment boot count
    
    //                           Value in uS  
    //                             |  
    esp_sleep_enable_timer_wakeup(5000000);     // This value is a uint64_t
    
    /**
     * According to the doc
     * the serial buffer has to be flushed 
     * before deep sleep.  Also, WIFI
     * needs to be shutoff
     */
    // WiFi.mode(WIFI_OFF);
    Serial.flush();
    
    esp_deep_sleep_start();  //This will put the module into deep sle

    // TODO TEST CODE ENDS HERE



    
  // }
  
  /**
   * Here is the case if we've booted already
   */
  // else 
  // {
  //   if(ENABLE_LOGGING)
  //   {
  //     Serial.println("^Awake, but boot count is greater than 0");
  //   }
    
  // }

}

/**
 * @brief Arduino main loop
 */
void loop() 
{

  if(local_boot_counter == 0)   //TODO this is in just for testing ....
  {
    if (ENABLE_LOGGING)
    {
      Serial.println("Boot count is 0");
    }
  }

  
  if(Timer50msFlag == true) 
  {
    
    app.state_handler(app.state, pref); 
    
    
    Timer50msFlag = false;
    rx_char = Serial.read();
    
    if(app.heartbeat_enabled)
    {
      if(!digitalRead(HEALTH_LED))  //IF ON TURN OFF
      {
        digitalWrite(HEALTH_LED,HIGH);
      }
    }
      
    if (rx_char == 'z'){
      if(ENABLE_LOGGING)
      {
        Serial.println("^User wishes to enter the console");
      }
      console.console(pref);    
      Timer100msFlag = false;
      Timer500msFlag = false;
      Timer1000msFlag = false;
    }
    
    app.button_handler();

  }
  
  if(Timer100msFlag == true) 
  {
    Timer100msFlag = false;
    if(app.calibrate_sensors == true)  //TODO need code here
    {
      app.calibrate_sensors = false;
      if(ENABLE_LOGGING)
      {
        Serial.println("^User wishes to calibrate sensors");
      }

    }
  }
  
  if(Timer500msFlag == true) 
  {
    Timer500msFlag = false;
    
  }
  
  if(Timer1000msFlag == true) 
  {
    Timer1000msFlag = false;
    
    if(app.heartbeat_enabled)
    {
      if(digitalRead(HEALTH_LED))   //IF OFF turn LED ON
      {
        digitalWrite(HEALTH_LED,LOW);
      }
    }
    
    app.seconds_counter++;
    
    /* The following is nice for longer delays */
    if(app.seconds_counter >= 30)  //TODO probably need to remove / rework the following
    {
      if(ENABLE_LOGGING)
      {
        Serial.println("^Updating the display");
      }
      app.seconds_counter = 0;
      main_i2c.get_sensor_data(pref);
      app.update_display(pref);
    }

    if(app.btn_interrupt_triggered && !digitalRead(LOCAL_BTN_GPIO_PIN) &&
      !app.btn_short_press_flag && !app.btn_long_press_flag)
    {
        attachInterrupt(LOCAL_BTN_GPIO_PIN, button_press, RISING); 
        app.btn_interrupt_triggered  = false;
    }
  }

}
