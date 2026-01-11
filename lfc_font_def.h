#ifndef _LFC_FONT_DEF_H_
#define _LFC_FONT_DEF_H_


#include "lfc_unicode.h"
 

#define MSB_BYTE(n)  (((n)>>8)&0xff)
#define LSB_BYTE(n)  ((n)&0xff)





typedef struct{
    // Character Unicode code point (4 bytes)
    UTF32 char_code;

    // Character bitmap width in pixels
    unsigned char bitmap_width;

    // Character bitmap height in pixels
    unsigned char bitmap_height;

    // Vertical offset from baseline to top of bitmap
    unsigned char bitmap_top;

    // Horizontal offset from origin to left edge of bitmap
    unsigned char bitmap_left;

    // Horizontal advance (space to next character) in pixels
    // This can be larger than bitmap_width
    unsigned char advance;

    // Bitmap data length in bytes
    unsigned int bitmap_data_len;

    // Pointer to bitmap data (monochrome 1-bit per pixel)
    unsigned char* bitmap_data;
} CHAR_DATA;




typedef struct {
    // Font file name/path
    char* font_name;

    // Font size in pixels
    int font_size;

    // Rendering type (version 1: 1-bit monochrome)
    int render_type;

    // Font height in pixels (maximum character height)
    int char_height;

    // Number of characters in the structure
    unsigned int char_data_len;

    // Array of character data structures
    CHAR_DATA* char_data;
} FONT_PROFILE;


#endif
