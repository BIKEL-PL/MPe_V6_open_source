// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t DejaVu_LGC_Sans_Mono_Bold_20Bitmaps[] PROGMEM = {

	// Bitmap Data:
	0x1E,0x1F,0xE7,0x3B,0x87,0xE1,0xF8,0x7F,0xDF,0xF7,0xE1,0xF8,0x7E,0x1D,0xCE,0x7F,0x87,0x80, // '0'
	0x7F,0xFF,0xFF,0xFF,0xFF,0x80, // '1'
    0x7E,0x7F,0xB1,0xE0,0x70,0x38,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x0F,0xFF,0xFC, // '2'
	0x7F,0x1F,0xE4,0x3C,0x07,0x01,0xC7,0xC1,0xF0,0x0E,0x01,0xC0,0x70,0x1F,0x0F,0xFF,0x9F,0xC0, // '3'
	0x07,0x80,0xF0,0x3E,0x0F,0xC1,0xB8,0x77,0x1C,0xE3,0x1C,0xE3,0x9F,0xFF,0xFF,0x81,0xC0,0x38,0x07,0x00, // '4'
	0x7F,0x9F,0xE7,0x01,0xC0,0x7F,0x1F,0xE4,0x38,0x07,0x01,0xC0,0x70,0x1E,0x0E,0xFF,0x9F,0x80, // '5'
	0x1F,0x8F,0xE7,0x03,0x80,0xEF,0x3F,0xEF,0x3F,0xC7,0xE1,0xF8,0x7F,0x1D,0xCF,0x7F,0x87,0xC0, // '6'
	0xFF,0xFF,0xC0,0xE0,0xE0,0x70,0x38,0x38,0x1C,0x1C,0x0E,0x07,0x07,0x03,0x83,0x80, // '7'
	0x3E,0x1F,0xCE,0x3B,0x8E,0xE3,0x8F,0xC3,0xF1,0xCE,0xE1,0xF8,0x7E,0x1F,0xCF,0x7F,0x8F,0xC0, // '8'
	0x3E,0x1F,0xEF,0x3B,0x87,0xE1,0xF8,0x7F,0x3D,0xFF,0x3D,0xC0,0x70,0x39,0x0E,0x7F,0x1F,0x80, // '9'
  //0xFF,0x80,0x07,0xFC // ':'
};

const GFXglyph DejaVu_LGC_Sans_Mono_Bold_20Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {   0,  10,  14,  13,    1,  -14 }, // '0'
	  {   18,   3,  14,  13,    5,  -14 }, // '1'
	  {   24,   9,  14,  13,    1,  -14 }, // '2'
	  {   40,  10,  14,  13,    1,  -14 }, // '3'
	  {   58,  11,  14,  13,    1,  -14 }, // '4'
	  {   78,  10,  14,  13,    1,  -14 }, // '5'
	  {   96,  10,  14,  13,    1,  -14 }, // '6'
	  {   114,   9,  14,  13,    1,  -14 }, // '7'
	  {   130,  10,  14,  13,    1,  -14 }, // '8'
	  {   148,  10,  14,  13,    1,  -14 },  // '9'
    //{   166,   3,  10,  5,    1,  -10 } // ':'
};
const GFXfont DejaVu_LGC_Sans_Mono_Bold_20 PROGMEM = {
(uint8_t  *)DejaVu_LGC_Sans_Mono_Bold_20Bitmaps,(GFXglyph *)DejaVu_LGC_Sans_Mono_Bold_20Glyphs,0x30, 0x3A, 16};

