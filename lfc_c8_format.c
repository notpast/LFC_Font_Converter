#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#include "lfc_font_def.h"
#include "lfc_font_info.h"
#include "lfc_unicode.h"
#include "lfc_utils.h"
#include "lfc_c8_format.h"
#include "lfc_load_font.h"



/*


    C8 data structure information


    |------------------------------------------------------------------------------------------------------------------|...
    |---------------------    FONT HEADER   -------------------------| First Char UTF32 Code  | First character offset |...
    |  Font type  |Header size| Font height|      Cracters count     |   (Little Endian)      |   LSB     |   MSB      |...
    |-------------|-----------|------------|------------|------------|------------------------|-----------|------------|...
    |   1 byte    | 1 byte    |   1 byte   | 1 byte LSB | 1 byte MSB |        4 bytes         |   1 byte  |   1 byte   |...
    |-------------|-----------|------------|------------|------------|------------------------|-----------|------------|...


    ...|---------------------------------------------------------------------------------------------------------------------|...
    ...|  Last Char UTF32 Code  | Last character offset|           First character header        |-- Character bitmap data-- |...
    ...|   (Little Endian)      |     LBS   |   MBS    | Width | Height|  Top  | Left  | Advance |                           |...
    ...|------------------------|-----------|----------|-------|-------|-------|-------|---------|  width x height (bits)    |...
    ...|        4 bytes         |  1 byte   | 1 byte   | 1byte | 1byte | 1byte | 1byte | 1Byte   |  result is byte-aligned   |...
    ...|------------------------|-----------|----------|-------|-------|-------|-------|---------|---------------------------|...


    ...|---------------------------------------------------------------------| 
    ...|           Last character header         |-- Character bitmap data-- |
    ...| Width | Height|  Top  | Left  | Advance |                           |
    ...|-------|-------|-------|-------|---------|  width x height (bits)    |
    ...| 1byte | 1byte | 1byte | 1byte | 1Byte   |  result is byte-aligned   |
    ...|-------|-------|-------|-------|---------|---------------------------|



*/



/*
    @brief: Export a font profile to a C header file as a C8 bitmap font type

    @param font_profile: Font structure containing character bitmap data and character properties.
                         This structure is created by the Lfc_Load_Font function.
    @param export_file_name: Output C header file name that will contain the C-style font array.
                             Example: my_font.h

    @return: 0 on success, any other value indicates an error.
*/

int Lfc_Export_C8_Format(FONT_PROFILE* font_profile, const char* export_file_name) {

    char base_name[MAX_FILE_NAME_LEN + 10];
    char array_name[MAX_FILE_NAME_LEN + 10];
    char def_name[MAX_FILE_NAME_LEN + 10];
    char out_file[MAX_FILE_NAME_LEN + 10];

    FILE* export_file;
    int size;
    int character_count;
    int total_bytes;



    if (font_profile == NULL) {
        printf("\n Error: font profile null pointer.");
        return 1;
    }

    // Check char data
    if (font_profile->char_data == NULL || font_profile->char_data_len == 0) {
        printf("\n Error: font profile char data null pointer.");
        return 1;
    }


    const char* font_file_name = font_profile->font_name;
    if (font_file_name == NULL) {
        printf("\n Error: font profile font name null pointer.");
        return 1;
    }


    // Font size
    size = font_profile->font_size;


    // Create base name
    if (strlen(font_file_name) < 1024) {
        Format_File_Name(font_file_name, base_name, size);
    }
    else {
        sprintf(base_name, "font_%d", size);
    }

    // Create array name for use in C files
    strcpy(array_name, base_name);


    // Create header file for define name
    sprintf(def_name, "_%s_H_", base_name);


    // Create output file name form font file name
    if (export_file_name) {
        if (strlen(export_file_name) > MAX_FILE_NAME_LEN) {
            return 1;
        }
        strcpy(out_file, export_file_name);
    }
    else {
        sprintf(out_file, "%s.h", base_name);
    }


    // Create output file
    export_file = fopen(out_file, "w+b");
    if (export_file == NULL) {
        printf("\n Error: Export file create error.");
        return 1;
    }

    // C8 format header
    const int header_size = 5;

    // Per char code (4 bytes) + offset address (2 bytes)
    const int char_index_data = 4 + 2;

    // Length of char bitmap info
    const int char_info = 5;

    // Calculate C8 format array length
    total_bytes = header_size;
    for (int i = 0; i < font_profile->char_data_len; i++) {
        total_bytes += (char_info + char_index_data);
        total_bytes += font_profile->char_data[i].bitmap_data_len;
    }


    // Write font info to file as a comment
    fprintf(export_file, "#ifndef %s\n", def_name);
    fprintf(export_file, "#define %s\n\n\n", def_name); 

    // Write font info to the file as a comment lines
    Lfc_Write_Font_Info(export_file, font_profile, total_bytes, 0);

    // Array type 
    fprintf(export_file, "static const uint8_t %s[]={ ", array_name);

    // Font structure type 0xC8 (This is the signature for this font type)
    fprintf(export_file, "0x%02X,", 0xC8);

    // Size of header
    fprintf(export_file, "0x%02X,", 0x05);

    // Write Font height
    fprintf(export_file, "0x%02X,", font_profile->char_height);

    // Write cracters count
    character_count = font_profile->char_data_len;

    // LSB LSB_BYTE
    fprintf(export_file, "0x%02X,", LSB_BYTE(character_count));

    // MSB MSB_BYTE
    fprintf(export_file, "0x%02X,", MSB_BYTE(character_count));


    // Calculate first offset address
    int offset_pos = header_size + char_index_data * character_count;

    // Write file: character utf32 code and character data offset
    for (int i = 0; i < character_count; i++) {
        const CHAR_DATA* char_data = &(font_profile->char_data[i]);
        UTF32 char_code = char_data->char_code;

        // Write character UTF-32 code (4 bytes little endian)
        for (int j = 0; j < 4; j++) {
            fprintf(export_file, "0x%02X,", (unsigned int)((char_code >> (j * 8)) & 0xff));
        }

        // Write character data offset (2 bytes little endian)
        // LSB
        fprintf(export_file, "0x%02X,", LSB_BYTE(offset_pos));
        // MSB
        fprintf(export_file, "0x%02X,", MSB_BYTE(offset_pos));

        // Update offset position
        offset_pos += (char_data->bitmap_data_len + char_info);
    }


    // Write characters data
    for (int i = 0; i < character_count; i++) {

        const CHAR_DATA* char_data = &(font_profile->char_data[i]);

        // Write char bitmap info
        fprintf(export_file, "0x%02X,", char_data->bitmap_width);
        fprintf(export_file, "0x%02X,", char_data->bitmap_height);
        fprintf(export_file, "0x%02X,", char_data->bitmap_top);
        fprintf(export_file, "0x%02X,", char_data->bitmap_left);
        fprintf(export_file, "0x%02X,", char_data->advance);

        // Write bitmap data
        for (int j = 0; j < char_data->bitmap_data_len; j++) {
            fprintf(export_file, "0x%02X,", char_data->bitmap_data[j]);
        }
    }

    // Remove last comma in the file
    fseek(export_file, -1, SEEK_END);

    // Close array
    fprintf(export_file, " };\n\n");

    fprintf(export_file, "#endif \n");
    fclose(export_file);


    return 0;
}



