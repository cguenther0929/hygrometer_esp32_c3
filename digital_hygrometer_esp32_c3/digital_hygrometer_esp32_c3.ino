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
 * furished to do so, subject to the following conditions:
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
 * TODO: Here are a lit of TODOs
 * TODO: Handle the calibration routine (read button and store values)
 * TODO: choose sensors via mux
 * TODO: read temperature
 * TODO: read relative humidity
 * TODO: wake up processor from deep sleep with timer
 * TODO: interrupt times and a tick system
 * TODO: button press algorithm 
 * TODO: establish the command line interface
 * TODO: adc for measuring battery voltage
 * TODO: get email features tested and working
 * TODO: paint line at bottom of screen
 * 
 * 
 * 
 */

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include "epd1in54.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "i2c.h"

#define COLORED                   0
#define UNCOLORED                 1

#define SENSOR_1                  1
#define SENSOR_2                  2

#define SW_VER_STRING             "0.0.1"
#define SERIAL_BAUD_RATE          57600

#define LOCAL_BTN_GPIO_PIN        1
#define WAKEUP_GPIO               GPIO_NUM_1
#define GPIO_EXPANDER_HLTH_LED    8

/**
 * Uncomment the following 
 * to enable logging
 */
// #define ENABLE_LOGGING

/**
 * Timer parameters
 */
//TODO: remove? // unsigned int    tmr1_write_val  = 3030;   // Empirically derived to generate a 1ms tick timer.
// unsigned int    ms_ticks_1      =0;
unsigned int    ms_ticks_50           =0;
unsigned int    ms_ticks_100          =0;
unsigned int    ms_ticks_500          =0;
unsigned int    ms_ticks_1000         =0;

// bool            Timer1msFlag        = false;
bool            Timer50msFlag         = false;
bool            Timer100msFlag        = false;
bool            Timer500msFlag        = false;
bool            Timer1000msFlag       = false;
// bool            timer_running       = false;

// long            seconds_counter     = 0;        //32bit value 4.264....e9
// long            tick_1ms_counter    = 0;        //32bit value.  At 20ms, this can count to 8.5899e7 seconds

// uint16_t        timeout_seconds     = 4.0;
// uint16_t        timeout_1ms_ticks   = (uint16_t)(timeout_seconds/0.001);


hw_timer_t *IntTmr = NULL;

//TODO: here are notes on using the EEPROM

/**
 * Note for using Deep Sleep Mode
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
 * Here is an article about waking the processor from deep sleep, note that the 
 * GPIO source shall be of the RTC type -- which I don't know what that means for 
 * the version of ESP32 we are using.  
 * https://randomnerdtutorials.com/esp32-external-wake-up-deep-sleep/
 * 
 * According to a Google search, these are the RTC pins on the ESP32-C3
 * GPIO0, GPIO1, GPIO2, and GPIO3 -- I assume this means we are good 
 * since our button feeds into IO1
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
 * 
 **/


//TODO: is the image buffer used or can it be deleted?  
/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
I2C sensor;



/**
 * The name of this function cannot be 
 * changed
 */
void IRAM_ATTR onTimer()
{
  // timer1_write(tmr1_write_val);

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






void setup() {

 /**
  * Attach the button interrupt
  */
  //           IO Pin number that shall trigger the interrupt                              
  //                      |         Name of the call back function               
  //                      |               |     Type of signal edge to detect    
  //                      |               |         |
  // attachInterrupt(LOCAL_BTN_GPIO_PIN, button_press, RISING); //TODO: we want this back in

  /**
   * This will allow this pin to 
   * wake the processor from deep sleep mode
   * A high value (1) will wake the processor from 
   * deep sleep.  It's unclear if allowing the processor to be
   * awoken from deep sleep in the manor eats more 
   * power, so we may need to look into this.  
   */
  // esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);  //TODO: we want this back in

  Serial.begin(SERIAL_BAUD_RATE);
  Serial.print("Reset.");
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

  paint.SetRotate(ROTATE_0);  

  paint.SetWidth(200);
  paint.SetHeight(36);
  epd.SetFrameMemory(IMAGE_DATA);  
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);   
  epd.DisplayFrame();

  /* The following was written by me and seems to somewhat work*/
  paint.SetWidth(4);
  paint.SetHeight(80);
  paint.Clear(UNCOLORED);
  paint.DrawLine(0, 0, 1, 160, COLORED);
  paint.DrawLine(1, 0, 2, 160, COLORED);
  paint.DrawLine(2, 0, 3, 160, COLORED);
  paint.DrawLine(3, 0, 4, 160, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 100, 100, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(56);
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Humidity", &Font12, COLORED);    // Font12 is seven pixels wide
  epd.SetFrameMemory(paint.GetImage(), 17, 112, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(77);
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Temperature", &Font12, COLORED);    // Font12 is seven pixels wide
  epd.SetFrameMemory(paint.GetImage(), 112, 112, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(64);
  paint.SetHeight(36);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "68", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 17, 142 , paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(64);
  paint.SetHeight(36);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "75", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 115, 142 , paint.GetWidth(), paint.GetHeight());
  /* END OF WORKING EXAMPLE WRITTEN BY CJG*/

  epd.DisplayFrame();

  // delay(2000);

  /**
   * The ESP32-C3 close frequency is 
   * assumed to be 160MHz, so to get
   * to a frequency of 1000000, the 
   * prescaler needs to be 160
   */

  //Initialize timer interrupt
  //                       Timer to use (0 through 3)
  //                           | Prescaler of 160 to run the timer at 1MHz (see note above)       
  //                           |   |   true = count up
  //                           |   |     |  
  IntTmr = timerBegin(0, 160, true);
  
  //                 Name of timer (from above) 
  //                     |      Name of callback function       
  //                     |        |     true (the tutorial did not indicate what this mans)
  //                     |        |        |     
  timerAttachInterrupt(IntTmr, &onTimer, true);

  //              Name of timer (from above) 
  //                  |   Interrupt time value in microseconds (I think we want the timer to run at 1MHz (see notes above))       
  //                  |     |    true = to tell the timer to reload 
  //                  |     |      |  
  timerAlarmWrite(IntTmr, 50000, true);
  
  // Enable the timer 
  timerAlarmEnable(IntTmr);
}

void loop() {
  
  //TODO: this is just a test.  Need to move/update/etc.
  // esp_sleep_enable_timer_wakeup(1000);
  // esp_deep_sleep_start();  //This will put the module into deep sleep

  //TODO: This is in just for testing!! 
  EEPROM.write(0, 9);
  EEPROM.read(0);
  
  if(Timer50msFlag == true) 
  {
    Timer50msFlag = false;
  }
  
  if(Timer100msFlag == true) 
  {
    Timer100msFlag = false;
  }

  if(Timer500msFlag == true) 
  {
    Timer500msFlag = false;
    // toggle_io_expander(GPIO_EXPANDER_HLTH_LED); //TODO: we wan this line in 
  }

  if(Timer1000msFlag == true) 
  {
    Timer1000msFlag = false;
  }


}



//TODO: we want this function back in.
// void IRAM_ATTR button_press() 
// {
//   //TODO: need statements here
//   __asm__("nop\n\t");  //TODO: eventually need to remove this line
// }


