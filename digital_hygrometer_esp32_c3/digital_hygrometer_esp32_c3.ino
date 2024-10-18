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
 */

#include <SPI.h>
#include <Wire.h>
#include "epd1in54.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "hygrometeri2c.h"

#define COLORED               0
#define UNCOLORED             1

#define SENSOR_1              1
#define SENSOR_2              2

#define SW_VER_STRING         "0.0.1"

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
Sensor sensor;

void setup() {
    
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  epd.SetFrameMemory(IMAGE_DATA);  //TODO: this line needs to be put back in!
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);   //TODO: this line needs to be put back in!
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
  paint.DrawStringAt(0, 0, "68", &Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 17, 142 , paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(64);
  paint.SetHeight(36);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "75", &Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 115, 142 , paint.GetWidth(), paint.GetHeight());
  /* END OF WORKING EXAMPLE WRITTEN BY CJG*/

  epd.DisplayFrame();

  delay(2000);
  
  
  
}

void loop() {
  uint16_t i;
  // put your main code here, to run repeatedly:
  
  for(i=0;i++;i<65000);

}

