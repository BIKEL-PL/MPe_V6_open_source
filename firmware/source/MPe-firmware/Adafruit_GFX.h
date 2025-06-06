/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include "gfxfont.h"

class Adafruit_GFX : public Print
{

public:
  Adafruit_GFX(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void startWrite(void);
  virtual void writePixel(int16_t x, int16_t y, uint16_t color);
  virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void endWrite(void);

  // CONTROL API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void setRotation(uint8_t r);
  virtual void invertDisplay(boolean i);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void
  // It's good to implement those, even if using transaction API
  drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
      drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
      fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
      fillScreen(uint16_t color),
      // Optional and probably not necessary to change
      drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
      drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void
  drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
      drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                       uint16_t color),
      fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
      fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                       int16_t delta, uint16_t color),
      drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                   int16_t x2, int16_t y2, uint16_t color),
      fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                   int16_t x2, int16_t y2, uint16_t color),
      drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                    int16_t radius, uint16_t color),
      fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                    int16_t radius, uint16_t color),
      drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                 int16_t w, int16_t h, uint16_t color),
      drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                 int16_t w, int16_t h, uint16_t color, uint16_t bg),
      drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
                 int16_t w, int16_t h, uint16_t color),
      drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
                 int16_t w, int16_t h, uint16_t color, uint16_t bg),
      drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                  int16_t w, int16_t h, uint16_t color),
      drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
               uint16_t bg, uint8_t size),
      setCursor(int16_t x, int16_t y),
      setTextColor(uint16_t c),
      setTextColor(uint16_t c, uint16_t bg),
      setTextSize(uint8_t s),
      setTextWrap(boolean w),
      cp437(boolean x = true),
      setFont(const GFXfont *f = NULL),
      getTextBounds(char *string, int16_t x, int16_t y,
                    int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h),
      getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y,
                    int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

#if ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void write(uint8_t);
#endif

  int16_t height(void) const;
  int16_t width(void) const;

  uint8_t getRotation(void) const;

  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
  int16_t getCursorX(void) const;
  int16_t getCursorY(void) const;

protected:
  const int16_t
      WIDTH,
      HEIGHT; // This is the 'raw' display w/h - never changes
  int16_t
      _width,
      _height, // Display w/h as modified by current rotation
      cursor_x, cursor_y;
  uint16_t
      textcolor,
      textbgcolor;
  uint8_t
      textsize,
      rotation;
  boolean
      wrap,   // If set, 'wrap' text at right edge of display
      _cp437; // If set, use correct CP437 charset (default is off)
  GFXfont
      *gfxFont;
};

class Adafruit_GFX_Button
{

public:
  Adafruit_GFX_Button(void);
  // "Classic" initButton() uses center & size
  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y,
                  uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                  uint16_t textcolor, char *label, uint8_t textsize);
  // New/alt initButton() uses upper-left corner & size
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1,
                    uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                    uint16_t textcolor, char *label, uint8_t textsize);
  void drawButton(boolean inverted = false);
  boolean contains(int16_t x, int16_t y);

  void press(boolean p);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();

private:
  Adafruit_GFX *_gfx;
  int16_t _x1, _y1; // Coordinates of top-left corner
  uint16_t _w, _h;
  uint8_t _textsize;
  uint16_t _outlinecolor, _fillcolor, _textcolor;
  char _label[10];

  boolean currstate, laststate;
};

class GFXcanvas1 : public Adafruit_GFX
{

public:
  GFXcanvas1(uint16_t w, uint16_t h);
  ~GFXcanvas1(void);
  void drawPixel(int16_t x, int16_t y, uint16_t color),
      fillScreen(uint16_t color);
  uint8_t *getBuffer(void);

private:
  uint8_t *buffer;
};

class GFXcanvas16 : public Adafruit_GFX
{
  GFXcanvas16(uint16_t w, uint16_t h);
  ~GFXcanvas16(void);
  void drawPixel(int16_t x, int16_t y, uint16_t color),
      fillScreen(uint16_t color);
  uint16_t *getBuffer(void);

private:
  uint16_t *buffer;
};

#endif // _ADAFRUIT_GFX_H
