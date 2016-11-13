/*
  SD_MisakiFNT_read.cpp - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.1
  This is the Arduino library for reading Misaki font. (For ESP8266 & SD card) 
  
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

#include "Arduino.h"
#include "SD_MisakiFNT_read.h"
#include "SD.h"

uint8_t bridge_buf1[8] = {0,0,0,0,0,0,0,0};
boolean Zenkaku_bridge = false;

SD_MisakiFNT_read::SD_MisakiFNT_read(){}

//*******************美咲フォントメインクラス*************************************************************
uint8_t SD_MisakiFNT_read::Sjis_To_MisakiFNT_DotRead(File FNT_file_8x8, File FNT_file_4x8, uint8_t Direction, int16_t Angle, uint8_t SjisH, uint8_t SjisL, uint8_t* buf)
{
  uint16_t fnt_adrs_half;
  uint8_t cp;
  uint8_t dummy_buf1[8] = {0,0,0,0,0,0,0,0};
  uint8_t dummy_buf2[8] = {0,0,0,0,0,0,0,0};
  uint8_t dummy_buf3[8] = {0,0,0,0,0,0,0,0};
  uint8_t i;
  
  if((SjisH>=0x20 && SjisH<=0x7F) || (SjisH>=0xA1 && SjisH<=0xDF)){
  
    fnt_adrs_half = 0x110 + (SjisH - 0x20)*8;
    SD_MisakiFNT_read::SD_MisakiFontRead(FNT_file_4x8, fnt_adrs_half, dummy_buf1);
    
    cp = 1;
    
    if((SjisL>=0x20 && SjisL<=0x7F) || (SjisL>=0xA1 && SjisL<=0xDF)){
      fnt_adrs_half = 0x110 + (SjisL - 0x20)*8;
      
      SD_MisakiFNT_read::SD_MisakiFontRead(FNT_file_4x8, fnt_adrs_half, dummy_buf2);
      _Hankaku_three = true;
      switch(Angle){
        case 0:
          switch(Direction){
            default:
              for(i=0; i<8; i++){
                dummy_buf2[i] = dummy_buf2[i]>>4;
                buf[i] = dummy_buf1[i] | dummy_buf2[i];
              }
              break;
            case 1:
              for(i=0; i<8; i++){
                dummy_buf1[i] = dummy_buf1[i]>>4;
                buf[i] = dummy_buf2[i] | dummy_buf1[i];
              }
              break;
          }
          break;
        case 90:
          switch(Direction){
            default:
              for(i=0; i<8; i++){
                dummy_buf2[i] = dummy_buf2[i]>>4;
                buf[i] = dummy_buf1[i] | dummy_buf2[i];
              }
              break;
            case 2:
              for(i=0; i<8; i++){
                dummy_buf1[i] = dummy_buf1[i]>>4;
                buf[i] = dummy_buf2[i] | dummy_buf1[i];
              }
              break;
          }
          break;
        case -90:
          switch(Direction){
            default:
              for(i=0; i<8; i++){
                dummy_buf2[i] = dummy_buf2[i]>>4;
                buf[i] = dummy_buf1[i] | dummy_buf2[i];
              }
              break;
            case 3:
              for(i=0; i<8; i++){
                dummy_buf1[i] = dummy_buf1[i]>>4;
                buf[i] = dummy_buf2[i] | dummy_buf1[i];
              }
              break;
          }
          break;
        case 180:
          switch(Direction){
            default:
              for(i=0; i<8; i++){
                dummy_buf2[i] = dummy_buf2[i]>>4;
                buf[i] = dummy_buf1[i] | dummy_buf2[i];
              }
              break;
            case 0:
              for(i=0; i<8; i++){
                dummy_buf1[i] = dummy_buf1[i]>>4;
                buf[i] = dummy_buf2[i] | dummy_buf1[i];
              }
              break;
          }
          break;
      }
      cp = 2;
    }else{
Serial.print('(');
Serial.print(_Hankaku_three);
Serial.print(')');
      switch(_Hankaku_three){
        case false:
          for(i=0; i<8; i++){
            buf[i] = dummy_buf1[i]>>3;
          }
          break;
        case true:
          for(i=0; i<8; i++){
            buf[i] = dummy_buf1[i];
          }
          _Hankaku_three = false;
          break;
      }
      cp = 1;
    }
  }else{
    SD_MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(FNT_file_8x8, SjisH, SjisL, dummy_buf3);
    for( uint8_t col = 0; col < 8; col++ ) {
      for( uint8_t row = 0; row < 8; row++ ) {
        bitWrite( buf[7-row], 7-col , bitRead( dummy_buf3[col], 7-row ) );
      }
    }
    cp = 2;
    _Hankaku_three = false;
  }
  return cp;
}
//*******************美咲フォント一括変換*************************************************************
uint16_t SD_MisakiFNT_read::Sjis_To_MisakiFNT_Read_ALL(File FNT_file_8x8, File FNT_file_4x8, uint8_t Direction, int16_t Angle, uint8_t sj_txt[], uint16_t sj_length, uint8_t buf[][8])
{
	uint16_t fnt_adrs_half;
  uint8_t i;
	uint16_t cnt = 0;
	uint16_t buf_cnt = 0;
  
	for(cnt=0; cnt<sj_length; cnt++){
		uint8_t dummy_buf1[8] = {0,0,0,0,0,0,0,0};
  	uint8_t dummy_buf2[8] = {0,0,0,0,0,0,0,0};
  	uint8_t dummy_buf3[8] = {0,0,0,0,0,0,0,0};
		uint8_t dummy_buf4[8] = {0,0,0,0,0,0,0,0};
		
		if((Zenkaku_bridge == true) || (sj_txt[cnt]>=0x20 && sj_txt[cnt]<=0x7F) || (sj_txt[cnt]>=0xA1 && sj_txt[cnt]<=0xDF)){
			if(Zenkaku_bridge == true){
				for(i=0; i<8; i++){
					dummy_buf1[i] = bridge_buf1[i] << 4;
				}
			}else{
				fnt_adrs_half = 0x110 + (sj_txt[cnt] - 0x20)*8;
				SD_MisakiFNT_read::SD_MisakiFontRead(FNT_file_4x8, fnt_adrs_half, dummy_buf1);
			}
			Zenkaku_bridge = false;
			
			if(cnt + 1 < sj_length){
				cnt++;
			}else{
				for(i=0; i<8; i++){
					buf[buf_cnt][i] = dummy_buf1[i];
				}
				return buf_cnt + 1;
				break;
			}
			
			if((sj_txt[cnt]>=0x20 && sj_txt[cnt]<=0x7F) || (sj_txt[cnt]>=0xA1 && sj_txt[cnt]<=0xDF)){
				fnt_adrs_half = 0x110 + (sj_txt[cnt] - 0x20)*8;

				SD_MisakiFNT_read::SD_MisakiFontRead(FNT_file_4x8, fnt_adrs_half, dummy_buf2);

				for(i=0; i<8; i++){
					dummy_buf2[i] = dummy_buf2[i]>>4;
					buf[buf_cnt][i] = dummy_buf1[i] | dummy_buf2[i];
				}			

				Zenkaku_bridge = false;
				
			}else if((sj_txt[cnt]>=0x81 && sj_txt[cnt]<=0x9F) || (sj_txt[cnt]>=0xE0 && sj_txt[cnt]<=0xEA)){
				SD_MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(FNT_file_8x8, sj_txt[cnt], sj_txt[cnt+1], dummy_buf3);
				for( uint8_t col = 0; col < 8; col++ ) {
					for( uint8_t row = 0; row < 8; row++ ) {
						bitWrite( dummy_buf4[7-row], 7-col , bitRead( dummy_buf3[col], 7-row ) );
					}
				}
				for(i=0; i<8; i++){
					bridge_buf1[i] = dummy_buf4[i];
					buf[buf_cnt][i] = dummy_buf1[i] | dummy_buf4[i]>>4;
				}

				Zenkaku_bridge = true;
			}
			
			buf_cnt++;
			
		}else if((sj_txt[cnt]>=0x81 && sj_txt[cnt]<=0x9F) || (sj_txt[cnt]>=0xE0 && sj_txt[cnt]<=0xEA)){
			SD_MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(FNT_file_8x8, sj_txt[cnt], sj_txt[cnt+1], dummy_buf3);
			for( uint8_t col = 0; col < 8; col++ ) {
				for( uint8_t row = 0; row < 8; row++ ) {
					bitWrite( buf[buf_cnt][7-row], 7-col , bitRead( dummy_buf3[col], 7-row ) );
				}
			}
			cnt++;
			buf_cnt++;
			Zenkaku_bridge = false;
		}
	}
	return buf_cnt;
}
//*******************Shift_JISコードから美咲フォントアドレス計算********************************************
void SD_MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(File f, uint8_t jisH, uint8_t jisL, uint8_t* buf) 
{    // S-JISコードからMisakiフォントファイル上のバイト位置をポインタで返す。
  uint16_t SjisCode;
  int16_t adj;
  uint16_t fnt_adrs;

  if( jisH != '\0'){  //'\0'ならば読み込まない。
    if((jisH >= 0x81 && jisH <=0x9f) || (jisH >= 0xe0 && jisH <=0xef)){  //全角の場合
      SjisCode = ((uint16_t)jisH << 8 )+jisL;
      if(SjisCode>=0x8140 && SjisCode <=0x88fc){
        if     (SjisCode>=0x8140 && SjisCode<=0x817e) adj =  16;  // 一般記号
        else if(SjisCode>=0x8180 && SjisCode<=0x81fc) adj =  15;  // 一般記号
        else if(SjisCode>=0x824f && SjisCode<=0x8279) adj = -52;  // 数字、英語大文字
        else if(SjisCode>=0x8281 && SjisCode<=0x82f1) adj = -53;  // 英小文字、ひらがな
        else if(SjisCode>=0x8340 && SjisCode<=0x837e) adj = -120;  // カタカナ
        else if(SjisCode>=0x8380 && SjisCode<=0x83d6) adj = -121;  // カタカナ
        else if(SjisCode>=0x8440 && SjisCode<=0x847e) adj = -188;  // 外国大文字
        else if(SjisCode>=0x8480 && SjisCode<=0x84be) adj = -189;  // 外国小文字、罫線、※丸囲み文字やその他特殊文字は美咲フォントには無い。
        else if(SjisCode>=0x8740 && SjisCode<=0x877e) adj = -768; // １３区
        else if(SjisCode>=0x8780 && SjisCode<=0x879c) adj = -769; // １３区
        else if(SjisCode>=0x889f && SjisCode<=0x88fc) adj = -837;  // 第一水準漢字 亜～蔭まで
        fnt_adrs = ((SjisCode-0x8140)+adj)*8;
      }else if(SjisCode>=0x8940 && SjisCode<=0x9ffc){ //院～滌
        if(jisL <= 0x7e){
          fnt_adrs = ((SjisCode-0x8140)-(836+(jisH-0x88))-67*(jisH-0x88))*8;
        }else if(jisL >= 0x80 ){
          fnt_adrs = ((SjisCode-0x8140)-(837+(jisH-0x88))-67*(jisH-0x88))*8;
        }
      }else if(SjisCode>=0xe040 && SjisCode <=0xeaa4){ //漾～熙
        if(jisL <= 0x7e ){
          fnt_adrs = ((SjisCode-0x8140)-(836+(jisH-0x88))-67*(jisH-0x88)-12032)*8;
        }else if(jisL >= 0x80 ){
          fnt_adrs = ((SjisCode-0x8140)-(837+(jisH-0x88))-67*(jisH-0x88)-12032)*8;
        }
      }else{
        fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
      }
    }else {
      fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
    }
  }else {
    fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
  }
  
  SD_MisakiFNT_read::SD_MisakiFontRead(f, fnt_adrs, buf);
}
//*****************フォントファイル読み込み**************************************
void SD_MisakiFNT_read::SD_MisakiFontRead(File f1, uint16_t addrs, uint8_t* buf)
{
  if(f1){
    f1.seek(addrs);
		f1.read(buf, 8);
  }else{
    Serial.println(F(" Misaki font file has not been uploaded to the SD"));
    delay(30000);
  }
}