#ifndef _LFC_FONT_H_
#define _LFC_FONT_H_

#include "lfc_font_def.h"
#include "lfc_unicode.h"


/*
    @brief: Export a font profile to a C header file as a C8 bitmap font type

    @param font_profile: Font structure containing character bitmap data and character properties.
                         This structure is created by the Lfc_Load_Font function.
    @param size : Font size in pixel

    @param char_list_file: UTF-8 text file containing characters to export
                            (Sample file content : abcdefghjklmnopqrstvwxyz )

    @param export_file_name: Output C header file name that will contain the C-style font array.
                             Example: my_font.h

    @return: 0 on success, any other value indicates an error.
*/
int Lfc_Convert_C8_Format(const char* font_file, unsigned int size, const char* char_list_file, const char* export_file_name);


/*
    @brief: Export a font profile to a C header file as a C8 bitmap font type

    @param font_profile: Font structure containing character bitmap data and character properties.
                         This structure is created by the Lfc_Load_Font function.
    @param size : Font size in pixel

    @param char_list_file: Text file containing characters UTF-32 hexcode to export
                            (Sample file content : 0x9A5F9 ) should have a number on each line;
                            short names describing the characters can also be written on the same line.
                            

    @param export_file_name: Output C header file name that will contain the C-style font array.
                             Example: my_font.h

    @return: 0 on success, any other value indicates an error.
*/
int Lfc_Convert_C8_Format_Utf32(const char* font_file, unsigned int size, const char* char_list_file, const char* export_file_name);

#endif //_LFC_FONT_H_
