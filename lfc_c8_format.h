#ifndef _LFC_C8_FORMAT_H_
#define _LFC_C8_FORMAT_H_


#include "lfc_font_def.h"

/*
    @brief: Export a font profile to a C header file as a C8 bitmap font type

    @param font_profile: Font structure containing character bitmap data and character properties.
                         This structure is created by the Lfc_Load_Font function.
    @param export_file_name: Output C header file name that will contain the C-style font array.
                             Example: my_font.h

    @return: 0 on success, any other value indicates an error.
*/

int Lfc_Export_C8_Format(FONT_PROFILE* font_profile, const char* export_file_name);






#endif 
