/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     July 28 2017
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

#ifndef EPDPAINT_H
#define EPDPAINT_H

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3

#define COLORED     0
#define UNCOLORED   1

#define TEMP_X_START            20
#define TEMP_Y_START            130
#define HUM_X_START             120
#define HUM_Y_START             130
#define TWOUP_ROW_X_START       12
#define TWOUP_ROW_Y_START       168     //182 (top of bottom row) - 12 pixels tall text - a little slop
#define BOT_ROW_X_START         12
#define BOT_ROW_Y_START         182     //200 total pixels high - 12 pixels tall text - a little slop

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

// #include <String.h>
#include "fonts.h"

class Paint {
public:
    Paint(unsigned char* image, int width, int height);
    ~Paint();
    void Clear(int colored);
    int  GetWidth(void);
    void SetWidth(int width);
    int  GetHeight(void);
    void SetHeight(int height);
    int  GetRotate(void);
    void SetRotate(int rotate);
    unsigned char* GetImage(void);
    void DrawAbsolutePixel(int x, int y, int colored);
    void DrawPixel(int x, int y, int colored);
    void DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored);
    void DrawStringAt(int x, int y, const char* text, sFONT* font, int colored);
    void DrawLine(int x0, int y0, int x1, int y1, int colored);
    void DrawHorizontalLine(int x, int y, int width, int colored);
    void DrawVerticalLine(int x, int y, int height, int colored);
    void DrawRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored);
    void DrawCircle(int x, int y, int radius, int colored);
    void DrawFilledCircle(int x, int y, int radius, int colored);
    
    //TODO need to comment this function 
    /**
     * Font 12 is seven pixels wide.  Therefore, we can
     * have a total of 28 characters, as this will yield 
     *  28*7 (196) pixels of width
     */
    void eink_put_string_bottom(const char * string_to_print);
    //TODO need to comment this function 
    void eink_put_string_twoup(const char * string_to_print);

private:
    unsigned char* image;
    int width;
    int height;
    int rotate;
};

#endif

/* END OF FILE */

