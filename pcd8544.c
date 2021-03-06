#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "fontx.h"
#include "pcd8544.h"


// LCD port variables
static int8_t _din, _sclk, _dc, _rst, _cs;

// small font bitmap 8X5
static unsigned char  font[] = {
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
		0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
		0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
		0x18, 0x3C, 0x7E, 0x3C, 0x18,
		0x1C, 0x57, 0x7D, 0x57, 0x1C,
		0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
		0x00, 0x18, 0x3C, 0x18, 0x00,
		0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
		0x00, 0x18, 0x24, 0x18, 0x00,
		0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
		0x30, 0x48, 0x3A, 0x06, 0x0E,
		0x26, 0x29, 0x79, 0x29, 0x26,
		0x40, 0x7F, 0x05, 0x05, 0x07,
		0x40, 0x7F, 0x05, 0x25, 0x3F,
		0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
		0x7F, 0x3E, 0x1C, 0x1C, 0x08,
		0x08, 0x1C, 0x1C, 0x3E, 0x7F,
		0x14, 0x22, 0x7F, 0x22, 0x14,
		0x5F, 0x5F, 0x00, 0x5F, 0x5F,
		0x06, 0x09, 0x7F, 0x01, 0x7F,
		0x00, 0x66, 0x89, 0x95, 0x6A,
		0x60, 0x60, 0x60, 0x60, 0x60,
		0x94, 0xA2, 0xFF, 0xA2, 0x94,
		0x08, 0x04, 0x7E, 0x04, 0x08,
		0x10, 0x20, 0x7E, 0x20, 0x10,
		0x08, 0x08, 0x2A, 0x1C, 0x08,
		0x08, 0x1C, 0x2A, 0x08, 0x08,
		0x1E, 0x10, 0x10, 0x10, 0x10,
		0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
		0x30, 0x38, 0x3E, 0x38, 0x30,
		0x06, 0x0E, 0x3E, 0x0E, 0x06,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x5F, 0x00, 0x00,
		0x00, 0x07, 0x00, 0x07, 0x00,
		0x14, 0x7F, 0x14, 0x7F, 0x14,
		0x24, 0x2A, 0x7F, 0x2A, 0x12,
		0x23, 0x13, 0x08, 0x64, 0x62,
		0x36, 0x49, 0x56, 0x20, 0x50,
		0x00, 0x08, 0x07, 0x03, 0x00,
		0x00, 0x1C, 0x22, 0x41, 0x00,
		0x00, 0x41, 0x22, 0x1C, 0x00,
		0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
		0x08, 0x08, 0x3E, 0x08, 0x08,
		0x00, 0x80, 0x70, 0x30, 0x00,
		0x08, 0x08, 0x08, 0x08, 0x08,
		0x00, 0x00, 0x60, 0x60, 0x00,
		0x20, 0x10, 0x08, 0x04, 0x02,
		0x3E, 0x51, 0x49, 0x45, 0x3E,
		0x00, 0x42, 0x7F, 0x40, 0x00,
		0x72, 0x49, 0x49, 0x49, 0x46,
		0x21, 0x41, 0x49, 0x4D, 0x33,
		0x18, 0x14, 0x12, 0x7F, 0x10,
		0x27, 0x45, 0x45, 0x45, 0x39,
		0x3C, 0x4A, 0x49, 0x49, 0x31,
		0x41, 0x21, 0x11, 0x09, 0x07,
		0x36, 0x49, 0x49, 0x49, 0x36,
		0x46, 0x49, 0x49, 0x29, 0x1E,
		0x00, 0x00, 0x14, 0x00, 0x00,
		0x00, 0x40, 0x34, 0x00, 0x00,
		0x00, 0x08, 0x14, 0x22, 0x41,
		0x14, 0x14, 0x14, 0x14, 0x14,
		0x00, 0x41, 0x22, 0x14, 0x08,
		0x02, 0x01, 0x59, 0x09, 0x06,
		0x3E, 0x41, 0x5D, 0x59, 0x4E,
		0x7C, 0x12, 0x11, 0x12, 0x7C,
		0x7F, 0x49, 0x49, 0x49, 0x36,
		0x3E, 0x41, 0x41, 0x41, 0x22,
		0x7F, 0x41, 0x41, 0x41, 0x3E,
		0x7F, 0x49, 0x49, 0x49, 0x41,
		0x7F, 0x09, 0x09, 0x09, 0x01,
		0x3E, 0x41, 0x41, 0x51, 0x73,
		0x7F, 0x08, 0x08, 0x08, 0x7F,
		0x00, 0x41, 0x7F, 0x41, 0x00,
		0x20, 0x40, 0x41, 0x3F, 0x01,
		0x7F, 0x08, 0x14, 0x22, 0x41,
		0x7F, 0x40, 0x40, 0x40, 0x40,
		0x7F, 0x02, 0x1C, 0x02, 0x7F,
		0x7F, 0x04, 0x08, 0x10, 0x7F,
		0x3E, 0x41, 0x41, 0x41, 0x3E,
		0x7F, 0x09, 0x09, 0x09, 0x06,
		0x3E, 0x41, 0x51, 0x21, 0x5E,
		0x7F, 0x09, 0x19, 0x29, 0x46,
		0x26, 0x49, 0x49, 0x49, 0x32,
		0x03, 0x01, 0x7F, 0x01, 0x03,
		0x3F, 0x40, 0x40, 0x40, 0x3F,
		0x1F, 0x20, 0x40, 0x20, 0x1F,
		0x3F, 0x40, 0x38, 0x40, 0x3F,
		0x63, 0x14, 0x08, 0x14, 0x63,
		0x03, 0x04, 0x78, 0x04, 0x03,
		0x61, 0x59, 0x49, 0x4D, 0x43,
		0x00, 0x7F, 0x41, 0x41, 0x41,
		0x02, 0x04, 0x08, 0x10, 0x20,
		0x00, 0x41, 0x41, 0x41, 0x7F,
		0x04, 0x02, 0x01, 0x02, 0x04,
		0x40, 0x40, 0x40, 0x40, 0x40,
		0x00, 0x03, 0x07, 0x08, 0x00,
		0x20, 0x54, 0x54, 0x78, 0x40,
		0x7F, 0x28, 0x44, 0x44, 0x38,
		0x38, 0x44, 0x44, 0x44, 0x28,
		0x38, 0x44, 0x44, 0x28, 0x7F,
		0x38, 0x54, 0x54, 0x54, 0x18,
		0x00, 0x08, 0x7E, 0x09, 0x02,
		0x18, 0xA4, 0xA4, 0x9C, 0x78,
		0x7F, 0x08, 0x04, 0x04, 0x78,
		0x00, 0x44, 0x7D, 0x40, 0x00,
		0x20, 0x40, 0x40, 0x3D, 0x00,
		0x7F, 0x10, 0x28, 0x44, 0x00,
		0x00, 0x41, 0x7F, 0x40, 0x00,
		0x7C, 0x04, 0x78, 0x04, 0x78,
		0x7C, 0x08, 0x04, 0x04, 0x78,
		0x38, 0x44, 0x44, 0x44, 0x38,
		0xFC, 0x18, 0x24, 0x24, 0x18,
		0x18, 0x24, 0x24, 0x18, 0xFC,
		0x7C, 0x08, 0x04, 0x04, 0x08,
		0x48, 0x54, 0x54, 0x54, 0x24,
		0x04, 0x04, 0x3F, 0x44, 0x24,
		0x3C, 0x40, 0x40, 0x20, 0x7C,
		0x1C, 0x20, 0x40, 0x20, 0x1C,
		0x3C, 0x40, 0x30, 0x40, 0x3C,
		0x44, 0x28, 0x10, 0x28, 0x44,
		0x4C, 0x90, 0x90, 0x90, 0x7C,
		0x44, 0x64, 0x54, 0x4C, 0x44,
		0x00, 0x08, 0x36, 0x41, 0x00,
		0x00, 0x00, 0x77, 0x00, 0x00,
		0x00, 0x41, 0x36, 0x08, 0x00,
		0x02, 0x01, 0x02, 0x04, 0x02,
		0x3C, 0x26, 0x23, 0x26, 0x3C,
		0x1E, 0xA1, 0xA1, 0x61, 0x12,
		0x3A, 0x40, 0x40, 0x20, 0x7A,
		0x38, 0x54, 0x54, 0x55, 0x59,
		0x21, 0x55, 0x55, 0x79, 0x41,
		0x21, 0x54, 0x54, 0x78, 0x41,
		0x21, 0x55, 0x54, 0x78, 0x40,
		0x20, 0x54, 0x55, 0x79, 0x40,
		0x0C, 0x1E, 0x52, 0x72, 0x12,
		0x39, 0x55, 0x55, 0x55, 0x59,
		0x39, 0x54, 0x54, 0x54, 0x59,
		0x39, 0x55, 0x54, 0x54, 0x58,
		0x00, 0x00, 0x45, 0x7C, 0x41,
		0x00, 0x02, 0x45, 0x7D, 0x42,
		0x00, 0x01, 0x45, 0x7C, 0x40,
		0xF0, 0x29, 0x24, 0x29, 0xF0,
		0xF0, 0x28, 0x25, 0x28, 0xF0,
		0x7C, 0x54, 0x55, 0x45, 0x00,
		0x20, 0x54, 0x54, 0x7C, 0x54,
		0x7C, 0x0A, 0x09, 0x7F, 0x49,
		0x32, 0x49, 0x49, 0x49, 0x32,
		0x32, 0x48, 0x48, 0x48, 0x32,
		0x32, 0x4A, 0x48, 0x48, 0x30,
		0x3A, 0x41, 0x41, 0x21, 0x7A,
		0x3A, 0x42, 0x40, 0x20, 0x78,
		0x00, 0x9D, 0xA0, 0xA0, 0x7D,
		0x39, 0x44, 0x44, 0x44, 0x39,
		0x3D, 0x40, 0x40, 0x40, 0x3D,
		0x3C, 0x24, 0xFF, 0x24, 0x24,
		0x48, 0x7E, 0x49, 0x43, 0x66,
		0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
		0xFF, 0x09, 0x29, 0xF6, 0x20,
		0xC0, 0x88, 0x7E, 0x09, 0x03,
		0x20, 0x54, 0x54, 0x79, 0x41,
		0x00, 0x00, 0x44, 0x7D, 0x41,
		0x30, 0x48, 0x48, 0x4A, 0x32,
		0x38, 0x40, 0x40, 0x22, 0x7A,
		0x00, 0x7A, 0x0A, 0x0A, 0x72,
		0x7D, 0x0D, 0x19, 0x31, 0x7D,
		0x26, 0x29, 0x29, 0x2F, 0x28,
		0x26, 0x29, 0x29, 0x29, 0x26,
		0x30, 0x48, 0x4D, 0x40, 0x20,
		0x38, 0x08, 0x08, 0x08, 0x08,
		0x08, 0x08, 0x08, 0x08, 0x38,
		0x2F, 0x10, 0xC8, 0xAC, 0xBA,
		0x2F, 0x10, 0x28, 0x34, 0xFA,
		0x00, 0x00, 0x7B, 0x00, 0x00,
		0x08, 0x14, 0x2A, 0x14, 0x22,
		0x22, 0x14, 0x2A, 0x14, 0x08,
		0xAA, 0x00, 0x55, 0x00, 0xAA,
		0xAA, 0x55, 0xAA, 0x55, 0xAA,
		0x00, 0x00, 0x00, 0xFF, 0x00,
		0x10, 0x10, 0x10, 0xFF, 0x00,
		0x14, 0x14, 0x14, 0xFF, 0x00,
		0x10, 0x10, 0xFF, 0x00, 0xFF,
		0x10, 0x10, 0xF0, 0x10, 0xF0,
		0x14, 0x14, 0x14, 0xFC, 0x00,
		0x14, 0x14, 0xF7, 0x00, 0xFF,
		0x00, 0x00, 0xFF, 0x00, 0xFF,
		0x14, 0x14, 0xF4, 0x04, 0xFC,
		0x14, 0x14, 0x17, 0x10, 0x1F,
		0x10, 0x10, 0x1F, 0x10, 0x1F,
		0x14, 0x14, 0x14, 0x1F, 0x00,
		0x10, 0x10, 0x10, 0xF0, 0x00,
		0x00, 0x00, 0x00, 0x1F, 0x10,
		0x10, 0x10, 0x10, 0x1F, 0x10,
		0x10, 0x10, 0x10, 0xF0, 0x10,
		0x00, 0x00, 0x00, 0xFF, 0x10,
		0x10, 0x10, 0x10, 0x10, 0x10,
		0x10, 0x10, 0x10, 0xFF, 0x10,
		0x00, 0x00, 0x00, 0xFF, 0x14,
		0x00, 0x00, 0xFF, 0x00, 0xFF,
		0x00, 0x00, 0x1F, 0x10, 0x17,
		0x00, 0x00, 0xFC, 0x04, 0xF4,
		0x14, 0x14, 0x17, 0x10, 0x17,
		0x14, 0x14, 0xF4, 0x04, 0xF4,
		0x00, 0x00, 0xFF, 0x00, 0xF7,
		0x14, 0x14, 0x14, 0x14, 0x14,
		0x14, 0x14, 0xF7, 0x00, 0xF7,
		0x14, 0x14, 0x14, 0x17, 0x14,
		0x10, 0x10, 0x1F, 0x10, 0x1F,
		0x14, 0x14, 0x14, 0xF4, 0x14,
		0x10, 0x10, 0xF0, 0x10, 0xF0,
		0x00, 0x00, 0x1F, 0x10, 0x1F,
		0x00, 0x00, 0x00, 0x1F, 0x14,
		0x00, 0x00, 0x00, 0xFC, 0x14,
		0x00, 0x00, 0xF0, 0x10, 0xF0,
		0x10, 0x10, 0xFF, 0x10, 0xFF,
		0x14, 0x14, 0x14, 0xFF, 0x14,
		0x10, 0x10, 0x10, 0x1F, 0x00,
		0x00, 0x00, 0x00, 0xF0, 0x10,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
		0xFF, 0xFF, 0xFF, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
		0x38, 0x44, 0x44, 0x38, 0x44,
		0x7C, 0x2A, 0x2A, 0x3E, 0x14,
		0x7E, 0x02, 0x02, 0x06, 0x06,
		0x02, 0x7E, 0x02, 0x7E, 0x02,
		0x63, 0x55, 0x49, 0x41, 0x63,
		0x38, 0x44, 0x44, 0x3C, 0x04,
		0x40, 0x7E, 0x20, 0x1E, 0x20,
		0x06, 0x02, 0x7E, 0x02, 0x02,
		0x99, 0xA5, 0xE7, 0xA5, 0x99,
		0x1C, 0x2A, 0x49, 0x2A, 0x1C,
		0x4C, 0x72, 0x01, 0x72, 0x4C,
		0x30, 0x4A, 0x4D, 0x4D, 0x30,
		0x30, 0x48, 0x78, 0x48, 0x30,
		0xBC, 0x62, 0x5A, 0x46, 0x3D,
		0x3E, 0x49, 0x49, 0x49, 0x00,
		0x7E, 0x01, 0x01, 0x01, 0x7E,
		0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
		0x44, 0x44, 0x5F, 0x44, 0x44,
		0x40, 0x51, 0x4A, 0x44, 0x40,
		0x40, 0x44, 0x4A, 0x51, 0x40,
		0x00, 0x00, 0xFF, 0x01, 0x03,
		0xE0, 0x80, 0xFF, 0x00, 0x00,
		0x08, 0x08, 0x6B, 0x6B, 0x08,
		0x36, 0x12, 0x36, 0x24, 0x36,
		0x06, 0x0F, 0x09, 0x0F, 0x06,
		0x00, 0x00, 0x18, 0x18, 0x00,
		0x00, 0x00, 0x10, 0x10, 0x00,
		0x30, 0x40, 0xFF, 0x01, 0x01,
		0x00, 0x1F, 0x01, 0x01, 0x1E,
		0x00, 0x19, 0x1D, 0x17, 0x12,
		0x00, 0x3C, 0x3C, 0x3C, 0x3C,
		0x00, 0x00, 0x00, 0x00, 0x00,
};

// the memory buffer for the LCD
// pcd8544 have 6 Page * 84 Column
//
// +-------------------------------------+ <-
// |               page 0                |  |
// +-------------------------------------+  |
// |               page 1                |  |
// +-------------------------------------+  |
// |               page 2                |  |
// +-------------------------------------+  | 8*6=48 Dot
// |               page 3                |  |
// +-------------------------------------+  |
// |               page 4                |  |
// +-------------------------------------+  |
// |               page 5                |  |
// +-------------------------------------+ <-
// ^                                     ^
// 0<---------------- 84 dot ----------->83 
//
// each page have 84 byte
// +-------------------------------------+
// |         page n = 84 byte            |
// +-------------------------------------+
//

uint8_t pcd8544_buffer[6][LCDWIDTH] = {0};

void LCDspiwrite(uint8_t c)
{
  shiftOut(_din, _sclk, MSBFIRST, c);
}

void LCDcommand(uint8_t c)
{
  digitalWrite( _dc, LOW);
  LCDspiwrite(c);
}

void LCDdata(uint8_t c)
{
  digitalWrite(_dc, HIGH);
  LCDspiwrite(c);
}


void LCDInit(uint8_t SCLK, uint8_t DIN, uint8_t DC, uint8_t CS, uint8_t RST, uint8_t contrast)
{
  _din = DIN;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;

  // set pin directions
  pinMode(_din, OUTPUT);
  pinMode(_sclk, OUTPUT);
  pinMode(_dc, OUTPUT);
  pinMode(_rst, OUTPUT);
  pinMode(_cs, OUTPUT);

  // toggle RST low to reset; CS low so it'll listen to us
  if (_cs > 0)
    digitalWrite(_cs, LOW);

  digitalWrite(_rst, LOW);
  delay(500);
  digitalWrite(_rst, HIGH);

  // get into the EXTENDED mode!
  LCDcommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION ); // 0x21+0x01

  // LCD bias select (4 is optimal?)
  LCDcommand(PCD8544_SETBIAS | 0x4); // 0x10+0x04

  // set VOP
  if (contrast > 0x7f) contrast = 0x7f;
  LCDcommand( PCD8544_SETVOP | contrast); // 0x80+contrast

  // normal mode
  LCDcommand(PCD8544_FUNCTIONSET); // 0x20

  // Set display to Normal
  LCDcommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL); // 0x08+0x04
}

void LCDsetContrast(uint8_t val)
{
  if (val > 0x7f) val = 0x7f;
  LCDcommand(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
  LCDcommand( PCD8544_SETVOP | val);
  LCDcommand(PCD8544_FUNCTIONSET);
}

void LCDdisplay(void)
{
  uint8_t col, maxcol, p;

  for(p = 0; p < 6; p++) {

    LCDcommand(PCD8544_SETYADDR | p);

    // start at the beginning of the row
    col = 0;
    maxcol = LCDWIDTH-1;

    LCDcommand(PCD8544_SETXADDR | col);

    for(; col <= maxcol; col++) {
      LCDdata(pcd8544_buffer[p][col]);
    }
  }

  LCDcommand(PCD8544_SETYADDR );  // no idea why this is necessary but it is to finish the last byte?

}

// clear everything
void LCDclear(void) {
  memset(pcd8544_buffer, 0, sizeof(pcd8544_buffer));
}

int LCDdrawString(uint8_t x, uint8_t page, char *str, uint8_t reverse, uint8_t enhance) {
  int i;
  uint8_t _x = x;
  for(i=0;i<strlen(str);i++) {
    _x = LCDdrawChar(_x, page, str[i], reverse, enhance);
  }
  return _x;
}

int LCDdrawChar(uint8_t x, uint8_t page, char c, uint8_t reverse, uint8_t enhance) {
  if (page  > 5) return 0;
  if ((x+5) >= LCDWIDTH) return 0;
  uint8_t i;
  for ( i =0; i<5; i++ ) {
    uint8_t d = *(font+(c*5)+i);
    pcd8544_buffer[page][x+i] = d;
  }
  return x + 6;

}

// Draw SJIS Character
int LCDdrawSJISChar(FontxFile *fx, uint8_t x, uint8_t page, uint16_t sjis, uint8_t reverse, uint8_t enhance){
  unsigned char fonts[32]; // font pattern
  uint8_t bitmap[128];
  unsigned char pw, ph;
  bool rc;

  if (page > 5) return x;
  if (x >= LCDWIDTH) return x;
  uint8_t _p = page;
  uint8_t _x = x;
  uint8_t xx,yy;

  rc = GetFontx(fx, sjis, fonts, &pw, &ph); // SJIS -> Font
#ifdef _DEBUG_
  printf("GetFontx rc=%d pw=%d ph=%d\n",rc,pw,ph);
#endif
  if (!rc) return x;

  Font2Bitmap(fonts, bitmap, pw, ph, 1);
  if(enhance) UnderlineBitmap(bitmap, pw, ph);
  if(reverse) ReversBitmap(bitmap, pw, ph);

  for (yy=0; yy<(ph/8); yy++) {
    for (xx=0; xx<pw; xx++ ) {
#ifdef _DEBUG_
      printf("_p+yy=%d _x+xx=%d fotn=%x\n",_p+yy,_x+xx,bitmap[yy*32+xx]);
#endif
//      pcd8544_buffer[_p+yy][_x+xx] = bitmap[yy*32+xx];
      if (_p+yy <= 5 && _x+xx <= LCDWIDTH) {
        pcd8544_buffer[_p+yy][_x+xx] = bitmap[yy*32+xx];
      } // end if
    } // end for
  } // end for
  return x + pw;
}

int LCDdrawUTF8Char(FontxFile *fx, int x,int y,uint8_t *utf8,uint8_t reverse,
                 uint8_t enhance){
  uint16_t sjis;

  // convert UTF8 to SJIS
  sjis = UTF2SJIS(utf8);
  return LCDdrawSJISChar(fx, x, y, sjis, reverse, enhance);
}

int LCDdrawUTF8String(FontxFile *fx,int x,int y,unsigned char *utfs,
                   uint8_t reverse,uint8_t enhance){
  int i;
  int spos;
  uint16_t sjis[16];
  spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 16);
  for(i=0;i<spos;i++) {
    x = LCDdrawSJISChar(fx,x,y,sjis[i],reverse,enhance);
  }
  return x;
}

