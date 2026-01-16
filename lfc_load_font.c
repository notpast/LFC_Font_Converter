#define _CRT_SECURE_NO_WARNINGS

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>



#include "lfc_unicode.h"
#include "lfc_utils.h"
#include "lfc_font.h"




//Sorting call back function
static int CB_Sorting(const void* a, const void* b) {

    const UTF32* ca = (UTF32*)a;
    const UTF32* cb = (UTF32*)b;

    if (*ca > *cb) return  1;
    if (*ca < *cb) return -1;

    //a==b
    return 0;
}

//Sorting fonction
static void Sort_Small_To_Big(UTF32* char_list, int len) {
    qsort(char_list, len, sizeof(UTF32), CB_Sorting);
}




//Remove dublicate characters
//Return: new length
static int Unique_Characters(UTF32* str, int len) {
    int rlen = 0;
    if (str == NULL || len < 1) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        int j;
        for (j = 0; j < i; j++) {
            if (str[i] == str[j]) {
                break;
            }
        }
        if (i == j) {
            str[rlen++] = str[i];
        }
    }
    return rlen;
}




/**
 * Loads font file and creates font profile from specified characters
 *
 * Opens font file, renders characters at given size/mode, returns font profile.
 *
 * @param font_file Font file path (TTF/OTF)
 * @param size Font size in pixels
 * @param render_mode 1=mono (only support monocrome)
 * @param render_list UTF-32 array of chars to render
 * @return Font profile or NULL on error
 */

FONT_PROFILE* Lfc_Load_Font(const char* font_file, int size, int render_mode, UTF32* render_list) {

    FONT_PROFILE* font_profile=NULL;
    CHAR_DATA* char_data_array = NULL;

    //Freetype variables
    FT_Face face = NULL;
    FT_Library ft = NULL;


    //FT_CFF_HINTING_ADOBE=1,FT_HINTING_FREETYPE=0
    FT_UInt hinting_engine = 1;

    unsigned int render_list_len;

    //Chaeck the file name of the font
    if (Check_File_Name(font_file)) {
        printf("Error: The font file name error");
        goto error_secure_exit;
    }


    //Calculate length of list
    render_list_len = UTF32_Len(render_list);

    //Remove dublicate characters
    render_list_len= Unique_Characters(render_list, render_list_len);

    //Sorting for binary search
    Sort_Small_To_Big(render_list, render_list_len);


    //Init freetype lib
    if (FT_Init_FreeType(&ft))
    {
        printf("\n FreeType init error\n");
        goto error_secure_exit;
    }



    //Set hinting engine
    if (FT_Property_Set(ft, "cff", "hinting-engine", &hinting_engine)) {
        printf("\n Hinting error\n");
        //This is not a fatal error, don't exit!
    }


    // Open font file
    if (FT_New_Face(ft, font_file, 0, &face))
    {
        printf("\n Error: Unable to open font file\n");
        goto error_secure_exit;
    }



    // Set freetype parameters
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, 0, size);



    // Check validated list len
    int valid_list_len = 0;

    for (int i = 0; i < render_list_len; i++)
    {
        UTF32 utf32_char_code = render_list[i];

        if (FT_Get_Char_Index(face, utf32_char_code)) {
            render_list[valid_list_len++] = utf32_char_code;
        }
        else {
            printf("MISSING CODE: 0X%x\n", utf32_char_code);
        }
    }


    // Check empty list
    if (valid_list_len == 0) {
        printf("\n Error: valid_list_len==0");
        goto error_secure_exit;
    }
    // char_data_array
    char_data_array = malloc(sizeof(CHAR_DATA) * valid_list_len);
    if (char_data_array == NULL) {
        printf("\n Error: char_data_array = malloc(sizeof(CHAR_DATA) * valid_list_len);");
        goto error_secure_exit;
    }


    // Initialize char array structure
    for (int i = 0; i < valid_list_len; i++)
    {
        char_data_array[i].char_code    = render_list[i];
        char_data_array[i].bitmap_data  = NULL;
    }


    // Allocate memory for the font profile 
    font_profile = malloc(sizeof(FONT_PROFILE));
    if (font_profile == NULL) {
        printf("\n Error: font_profile = malloc(sizeof(FONT_PROFILE));");
        goto error_secure_exit;
    }

    // Initialize
    font_profile->font_name = NULL;
    font_profile->char_data = NULL;


    // Get font name from the full path
    const char* font_file_name = Get_File_Name(font_file);

    // Get name lengt
    unsigned int file_name_length = strlen(font_file_name);

    // Create memory for copy name
    char* font_name = malloc(file_name_length +1);

    if (font_name == NULL) {
        printf("Error :  char* font_name = malloc(file_name_length); \n");
        goto error_secure_exit;
    }

    // Copy font name
    strcpy(font_name, font_file_name);
    
    // Fill the font properties
    font_profile->font_name     = font_name;
    font_profile->render_type   = render_mode;
    font_profile->font_size     = size;
    font_profile->char_height   = (face->size->metrics.height >> 6) & 0xff;
    font_profile->char_data_len = valid_list_len;
    font_profile->char_data     = char_data_array;


    

    //Load characters to the font profile

    for (int i = 0; i < valid_list_len; i++)
    {

        UTF32 utf32_char_code = char_data_array[i].char_code;

        printf(" Char: 0x%X \n", utf32_char_code);

        //Passing missing characters
        FT_UInt char_index = FT_Get_Char_Index(face, utf32_char_code);
        if (char_index == 0) {
            printf(" Missing char: 0x%X \n", utf32_char_code);
            continue;
        }

        //Freetype character render  FT_LOAD_RENDER | FT_LOAD_MONOCHROME |FT_LOAD_TARGET_MONO
        FT_Load_Glyph(face, char_index , FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);



        if (face->glyph->bitmap.buffer == NULL) {
            printf("ERROR: Bitmap buffer is NULL!\n");
            goto  error_secure_exit;
        }


        //Copy bitmap properties
        char_data_array[i].bitmap_width   = face->glyph->bitmap.width;
        char_data_array[i].bitmap_height  = face->glyph->bitmap.rows;        
        char_data_array[i].bitmap_top     = face->glyph->bitmap_top;
        char_data_array[i].bitmap_left    = face->glyph->bitmap_left;

        unsigned char space = face->glyph->advance.x >> 6;

        char_data_array[i].advance = space;

        //Get char width and height form the ttf file
        int width  = face->glyph->bitmap.pitch;
        int height = face->glyph->bitmap.rows;

        int real_width = face->glyph->bitmap.width;
        int bitmap_data_len = (real_width * height +7) / 8;

        //Bitmap data length
        char_data_array[i].bitmap_data_len = bitmap_data_len;

        char_data_array[i].bitmap_data = calloc(bitmap_data_len,1);


        //Copy bitmap data
        int bit_index = 0;
        if (char_data_array[i].bitmap_data) {
            for (int ri = 0; ri < height; ri++) {
                for (int rj = 0; rj < real_width; rj++) {

                    int bi = (ri * width + rj / 8);

                    unsigned char bit_mask_src = 0x80 >> (rj & 0x07);
                    unsigned char bit_mask_dst = 0x80 >> (bit_index & 0x07);

                    unsigned char r_bit = face->glyph->bitmap.buffer[bi];
                    unsigned char d_bit = char_data_array[i].bitmap_data[bit_index/8];

                    if ((r_bit & bit_mask_src)) {
                        d_bit = (d_bit & ~bit_mask_dst) | bit_mask_dst;
                    }
                    else {
                        d_bit = (d_bit & ~bit_mask_dst);
                    }

                    char_data_array[i].bitmap_data[bit_index / 8] =  d_bit;

                    bit_index++;
                }
            }
        }
        else {
            printf("\n Error: char_data_array[i].bitmap_data = calloc(bitmap_data_len,1);");
            goto error_secure_exit;
        }
   
    }
    
    

    //Clean freetype memory
    if (face) FT_Done_Face(face);
    if (ft)   FT_Done_FreeType(ft);


    return font_profile;    


 //Error proccess 
error_secure_exit:


    if (render_list) {
        free(render_list);
    }

    //Clean all allocated momory if they aren't NULL
    if (font_profile) {
        if (font_profile->font_name) {
            free(font_profile->font_name);
        }
        free(font_profile);
    }

    if (char_data_array) {
        for (int i = 0; i < valid_list_len; i++) {
            if (char_data_array[i].bitmap_data) {
                free(char_data_array[i].bitmap_data);
            }
        }
        free(char_data_array);
    }

    //Clean freetype memory
    if (face) FT_Done_Face(face);
    if (ft)   FT_Done_FreeType(ft);

    return NULL;
}

