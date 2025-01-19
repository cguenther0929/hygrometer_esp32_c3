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
 * TODO: Wake up processor from deep sleep with timer
 * TODO: Button press interrupt working  
 * TODO: adc for measuring battery voltage
 * TODO: get email features tested and working
 * TODO: Establish text notification at bottom of screen 
 * TODO: Need to be able to read the HW revision
 * TODO: need to be able to read the the "charging status"
 * 
 */


#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <SPI.h>
#include <Wire.h>
#include <esp_timer.h>
#include <Preferences.h>
#include "epd1in54_V2.h"
#include "nvm.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "i2c.h"
#include "console.h"
#include "lan.h"
#include "app.h"


// ==============================
// ==============================
// Value last updated 1/12/25 
// SW version string 
String SW_VER_STRING = "0.1.3";
// ==============================
// ==============================

// TODO: how much of all of this can we put in app.h?

/* Instantiate the Preferences class*/
Preferences pref;

/**
 * Display parameters
 */    
#define COLORED     0
#define UNCOLORED   1


/**
 * Button related
 */
 uint16_t btn_press_ctr           = 0x000;
 bool btn_interrupt_triggered     = false;
 bool btn_short_press_flag        = false;
 bool btn_long_press_flag         = false;
#define LOCAL_BTN_GPIO_PIN        1
#define SHORT_PRESS_50MS_EVENTS   10
#define LONG_PRESS_50MS_EVENTS    20

//TODO: this is just for testing.  The following can be removed
uint16_t counter                = 0;

/**
 * Health LED
 */
#define HEALTH_LED                10

/**
 * Sensor parameters
 */
//TODO: can this be placed in app.h?
// #define SENSOR_1                  1
// #define SENSOR_2                  2
// #define SENSOR_MUX_RST_LINE       9
bool calibrate_sensors            = false;  //TODO we may want to put this in the sensor struct



/**
 * Interrupt / button pin
 * 
 */
//TODO: how much of this can we put in app.h?
#define INTERRUPT_PIN             PUSH_BUTTON    //RTC pins are GPIO0-GPIO3; the button ties to IO1, so the mask shall be 1

/**
 * Analog and battery parameters
 */
#define MIN_BATT_VOLTAGE          3.0

/**
 * Global variables 
 * for testing
 */
 //TODO -- this is for testing only ....
 float battery_voltage            = 0.0;

typedef enum State {
  STATE_SLEEP,
  STATE_READ_DATA,
  STATE_UPDATE_DISPLAY,
  STATE_SEND_EMAIL
};

/** 
 * E-ink Parameters
*/
#define TEMP_X_START            20
#define TEMP_Y_START            130
#define HUM_X_START             120
#define HUM_Y_START             130
#define BOT_ROW_X_START         12
#define BOT_ROW_Y_START         182     //200 total pixels high - 12 pixels tall text - a little slop



/**
 * Set to true to 
 * enable logging
 */
#define ENABLE_LOGGING                true

/**
 * Timer parameters
 */
unsigned int    ms_ticks_50           =0;
unsigned int    ms_ticks_100          =0;
unsigned int    ms_ticks_500          =0;
unsigned int    ms_ticks_1000         =0;

bool            Timer50msFlag         = false;
bool            Timer100msFlag        = false;
bool            Timer500msFlag        = false;
bool            Timer1000msFlag       = false;

char rx_char                          = '\n';

/**
 * Time structure 
 */
hw_timer_t *Timer1_Cfg = NULL;     

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint   paint(image, 0, 0);    // width should be the multiple of 8 
Epd     epd;
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
  /**
   * If the button is pushed
   * update the button counter
   */
  //TODO: need to update the button counter
  //TODO: and act accordingly
  btn_interrupt_triggered  = true;


}

/**
 * @brief Arduino Setup routine
 */
void setup() {
  
  pinMode(HEALTH_LED,OUTPUT);
  digitalWrite(HEALTH_LED, HIGH);
  
  pinMode(nSENSOR_PWR_EN,OUTPUT);
  digitalWrite(nSENSOR_PWR_EN,HIGH);   // Default is to keep sensor power off 

  app.sensor_power_on();    //TODO: need to remove this line.  Turn power on only when we need it


  State STATE_READ_DATA;
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
   * awoken from deep sleep in the manor eats more 
   * power. 
   * For more information on this routine, see
   * the notes and the web link that is pased up 
   * above 
   */
  //                                  A mask value needs to be passed in
  //                                     |                Parameter for the input signal   
  //                                     |                   |
  //                                     |                   |
  esp_deep_sleep_enable_gpio_wakeup(1 << INTERRUPT_PIN, ESP_GPIO_WAKEUP_GPIO_HIGH);    

  Serial.begin(SERIAL_BAUD_RATE);

 
  // if(ENABLE_LOGGING)
  // {
  //   Serial.println("Initializing e-paper display");
  // }
  // if (epd.Init(lut_full_update) != 0) 
  // { 
  //   if(ENABLE_LOGGING)
  //   {
  //     Serial.println("e-Paper init failed");
  //   }
  //   while(true);
  // }

  /**
   * Remaining initialization functions
   */
  if(ENABLE_LOGGING)
  {
    Serial.println("Calling remaining initialization functions");
  }

  main_i2c.init();          
  console.init();
  lan.init();
  nvm_functions.init();
  app.init();

  // if(ENABLE_LOGGING)
  // {
  //   Serial.println("Printing default display image");
  // }
  /** 
   *  There are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  // epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  // epd.DisplayFrame();
  // epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  // epd.DisplayFrame();

  // paint.SetRotate(ROTATE_0);  

  //TODO: how do we want to initialize the display

  // paint.SetWidth(200);
  // paint.SetHeight(36);
  // epd.SetFrameMemory(IMAGE_DATA);  
  // epd.DisplayFrame();
  // epd.SetFrameMemory(IMAGE_DATA);   
  // epd.DisplayFrame();

  if(ENABLE_LOGGING)
  {
    Serial.println("====================== Reset ======================");
  }



  /*~~~~~~~~~~~~~~~~~~~~~~~ ORIGINAL DISPLAY STUFF ~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /* The following was written by me and seems to somewhat work*/
  // paint.SetWidth(4);
  // paint.SetHeight(80);
  // paint.Clear(UNCOLORED);
  // paint.DrawLine(0, 0, 1, 160, UNCOLORED);
  // paint.DrawLine(1, 0, 2, 160, UNCOLORED);
  // paint.DrawLine(2, 0, 3, 160, UNCOLORED);
  // paint.DrawLine(3, 0, 4, 160, UNCOLORED);
  // epd.SetFrameMemory(paint.GetImage(), 100, 100, paint.GetWidth(), paint.GetHeight());

  // paint.SetWidth(56);         // 7 pixels wide x 8 characters 
  // paint.SetHeight(12);        // 12 pixels tall
  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "Humidity", &Font12, UNCOLORED);    // Font12 is seven pixels wide

  //                                   It seems that this is the absolute X   
  //                                      |   It seems that this is the absolute y
  //                                      |    |  
  // epd.SetFrameMemory(paint.GetImage(), 17, 112, paint.GetWidth(), paint.GetHeight());
  
  // paint.SetWidth(77);         // 7 pixels wide x 11 characters 
  // paint.SetHeight(12);        // 12 pixels tall
  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "Temperature", &Font12, UNCOLORED);    // Font12 is seven pixels wide
  // epd.SetFrameMemory(paint.GetImage(), 112, 112, paint.GetWidth(), paint.GetHeight());
  
  // paint.SetWidth(64);         // 32 pixels wide x 2 characters = 64 
  // paint.SetHeight(36);        // 36 pixels tall
  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "68", &SevenSeg_Font36, UNCOLORED);
  // epd.SetFrameMemory(paint.GetImage(), 17, 142 , paint.GetWidth(), paint.GetHeight());
  
  // paint.SetWidth(64);
  // paint.SetHeight(36);
  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "75", &SevenSeg_Font36, UNCOLORED);
  // epd.SetFrameMemory(paint.GetImage(), 115, 142 , paint.GetWidth(), paint.GetHeight());
  /* END OF WORKING EXAMPLE WRITTEN BY CJG*/

  // epd.DisplayFrame();

  /*~~~~~~~~~~~~~~~~~~~~~~~ ORIGINAL DISPLAY STUFF ~~~~~~~~~~~~~~~~~~~~~~~*/






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



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**   HERE WE ARE JUST TESTING DISPLAY STUFF */


 if(ENABLE_LOGGING)
  {
    Serial.println("Initializing e-paper display");
  }
  
  epd.LDirInit();
  epd.Display(IMAGE_DATA);


  paint.SetWidth(77);         //7 pixels wide * 11 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Temperature", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 12, 112, paint.GetWidth(), paint.GetHeight());
  
  
  paint.SetWidth(56);           //7 pixels wide * 8 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Humidity", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 112, 112, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "75", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), TEMP_X_START, TEMP_Y_START, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "68", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(84);           //7 pixels wide * 12 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "TEST BOT ROW", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), BOT_ROW_X_START, BOT_ROW_Y_START, paint.GetWidth(), paint.GetHeight());

  /** 
   * Print the divider line 
   */
  paint.SetWidth(4);
  paint.SetHeight(132);
  paint.Clear(UNCOLORED);
  paint.DrawLine(0, 0, 1, 132, COLORED);
  paint.DrawLine(1, 0, 2, 132, COLORED);
  paint.DrawLine(2, 0, 3, 132, COLORED);
  paint.DrawLine(3, 0, 4, 132, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 100, 100, paint.GetWidth(), paint.GetHeight());
  
  epd.DisplayFrame();



  /**   END OF DISPLAY TESTING */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



}

/**
 * @brief Arduino main loop
 */
void loop() 
{
  //TODO: This is how we put the the unit in to deep sleep mode
  //TODO: this note was confirmed on 10/24/24
  //                               Value in uS  
  //                                 |  
  // esp_sleep_enable_timer_wakeup(1000);
  // esp_deep_sleep_start();  //This will put the module into deep sleep


  /**
   * Determine if the EEPROM has been
   * initialized.  If not, it shall be erased 
   */
  //TODO: we'll need something here, but it's not the EEPROM! 
  // if(!nvm_functions.nvm_is_initalized() && !nvm_functions.nvm_is_calibrated())   
  // {
  //   nvm_functions.nvm_erase();
  // }
  
  if(Timer50msFlag == true) 
  {
    Timer50msFlag = false;
    rx_char = Serial.read();
    if (rx_char == 'z'){
      if(ENABLE_LOGGING)
      {
        Serial.println("User wishes to enter the console");
      }
      console.console(pref);       
      Timer100msFlag = false;
      Timer500msFlag = false;
      Timer1000msFlag = false;
    }

    button_handler();

  }
  
  if(Timer100msFlag == true) 
  {
    Timer100msFlag = false;
    digitalWrite(HEALTH_LED,!digitalRead(HEALTH_LED));
  }

  if(Timer500msFlag == true) 
  {
    Timer500msFlag = false;


  }

  if(Timer1000msFlag == true) 
  {
    Timer1000msFlag = false;

    
    //TODO: the following sensor read is in just for testing


    /**
     * For debugging the 
     * analog battery reading
     */
    // Serial.println("-----------------------------------------------------------");
    // Serial.println("--------------- Testing Battery Voltage Sensor ------------");
    // battery_voltage = app.get_battery_voltage();
    // Serial.print("Battery voltage: ");
    // Serial.println(battery_voltage);
    

    /**
     * For sensor debugging
     * Get readings from the first sensor
     */
    // Serial.println("-----------------------------------------------------------");
    // Serial.println("-------------------- Testing Sensors ----------------------");
    // main_i2c.choose_sensor(SENSOR_1);   //TODO: this is just in for testing
    
    // main_i2c.get_sensor_data();
    // Serial.print("Humidity #1: ");
    // Serial.println(main_i2c.hum_val1);
    // Serial.print("Temperature #1: ");
    // Serial.println(main_i2c.temp_val1);
    // Serial.println(' ');
    
    
    // main_i2c.choose_sensor(SENSOR_2);   //TODO: this is just in for testing
    
    // main_i2c.get_sensor_data();
    // Serial.print("\tHumidity #2: ");
    // Serial.println(main_i2c.hum_val2);
    // Serial.print("\tTemperature #2: ");
    // Serial.println(main_i2c.temp_val2);

    
    
    // main_i2c.disable_mux();   //TODO: this is just in for testing
  }

}
