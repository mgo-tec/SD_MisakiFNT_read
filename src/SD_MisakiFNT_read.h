/*
  SD_MisakiFNT_read.h - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.0
  This is the Arduino library for reading Misaki font. (For ESP8266) 
  
The MIT License (MIT)

Copyright (c) 2016 Mgo-tec
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SD_MisakiFNT_read_h_
#define SD_MisakiFNT_read_h_
#include "Arduino.h"
#include "SD.h"

class SD_MisakiFNT_read
{
public:
  SD_MisakiFNT_read();
  uint8_t Sjis_To_MisakiFNT_DotRead(File FNT_file_8x8, File FNT_file_4x8, uint8_t Direction, int16_t Angle, uint8_t SjisH, uint8_t SjisL, uint8_t* buf);
  void Sjis_To_Misaki_Font_Adrs(File f, uint8_t jisH, uint8_t jisL, uint8_t* buf);
  void SD_MisakiFontRead(File f1, uint16_t addrs, uint8_t* buf);
private:
  boolean _Hankaku_three;
};

#endif