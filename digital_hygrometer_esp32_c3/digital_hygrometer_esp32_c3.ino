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
 * TODO somewhere we need to indicate the SW version.  
 * TODO the SW version needs to come across in email. 
 * TODO if a key already exists, then we need to remove if from memory
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
hw_timer_t *Timer1_Cfg = NULL;     

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
 * Since the wakeup IO needs to be defined as a mask,
 * the "INTERRUPT PIN" value mentioned above, I assume, would be 0 for GPIO0, 1 for GPIO1, 
 * and so on and so forth.  
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
 * Notes for using the EEPROM
 * ================================
 * To use the EEPROM library, you can include 
 * it with the line #include <EEPROM.h>.
 * 
 * To write data to the EEPROM, you 
 * can use the EEPROM.write(). 
 * For example, to write 9 to address 0, 
 * the instruction EEPROM.write(0, 9) shall
 * be used.
 * 
 * To read data from the EEPROM, instruction 
 * EEPROM.read() shall be used.  
 * The argument that must be passed in
 * is the address.  
 * For example, to read the byte stored
 * at address 0, the instruction EEPROM.read(0)
 * shall be used.
 * 
 **/


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
 * 
 */
void IRAM_ATTR button_press()    
{
  //TODO: need statements here
  __asm__("nop\n\t");  //TODO: eventually need to remove this line
  Serial.println("\t***DEBUG Button interrupt triggered!");
  /**
   * If the button is pushed
   * update the button counter
   */
  //TODO: need to update the button counter
  //TODO: and act accordingly
  app.btn_interrupt_triggered  = true;
  detachInterrupt(digitalPinToInterrupt(LOCAL_BTN_GPIO_PIN)); 
}

/**
 * @brief Arduino Setup routine
 */
void setup() {
  
  pinMode(HEALTH_LED,OUTPUT);
  digitalWrite(HEALTH_LED, HIGH);
  
  pinMode(nSENSOR_PWR_EN,OUTPUT);
  digitalWrite(nSENSOR_PWR_EN,HIGH);   // Default is to keep sensor power off 

  // State state = STATE_READ_DATA;

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
   */
  //                                  A mask value needs to be passed in (empirically found to be one-based)
  //                                     |                Parameter for the input signal   
  //                                     |                   |
  //                                     |                   |
  esp_deep_sleep_enable_gpio_wakeup(1 << (INTERRUPT_PIN + 1), ESP_GPIO_WAKEUP_GPIO_HIGH);    

  Serial.begin(SERIAL_BAUD_RATE);
 
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


  
  Serial.println("===================================================");
  Serial.println("====================== Reset ======================");
  Serial.println("===================================================");
  
  //Initialize timer interrupt
  //                 The frequency of the timer   
  //                   |     
  Timer1_Cfg = timerBegin(1000000);   
  
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
 
  app.display_post_message();
  delay(2000);
  
  app.full_screen_refresh();

}

/**
 * @brief Arduino main loop
 */
void loop() 
{
  /**
   * Determine if the EEPROM has been
   * initialized.  If not, it shall be erased 
   */
  //TODO: we'll need something here, but it's not the EEPROM! 
  // if(!nvm_functions.nvm_is_initalized() && !nvm_functions.nvm_is_calibrated())   
  // {
  //   nvm_functions.load_network_parameters();

  //  nvm_functions.clear()
  // }

  
  
  if(Timer50msFlag == true) 
  {
    
    // app.state_handler(app.state); //TODO not sure if we want this hear
    
    
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
      main_i2c.get_sensor_data();
      app.update_display();
    }

    if(app.btn_interrupt_triggered && !digitalRead(LOCAL_BTN_GPIO_PIN) &&
      !app.btn_short_press_flag && !app.btn_long_press_flag)
    {
        attachInterrupt(LOCAL_BTN_GPIO_PIN, button_press, RISING); 
        app.btn_interrupt_triggered  = false;
    }
  }

}
