#define _CRT_SECURE_NO_WARNINGS

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lfc_unicode.h"
#include "lfc_utils.h"
#include "lfc_font.h"

/**

    @brief Writes all characters from a font file to a text file

    @param font_file: Path to the input font file (e.g., "arial.ttf")
    @param out_file_name: Path to the output text file (e.g., "output.txt")

    @return: 0 on success, other error code on failure

    This function reads the specified font file and writes all contained
    characters to the specified output text file. If the output
    file already exists, it will be overwritten.

    */

int Print_All(const char* font_file, const char* out_file_name) {

    FT_Face face  = NULL;
    FT_Library ft = NULL;
    FT_UInt hinting_engine = 1; //FT_CFF_HINTING_ADOBE=1

    FILE* export_file = NULL;
    FT_UInt gid;
    unsigned long u32;
    char u8[7];

    UTF32 max = 0;

    //Create output file
    export_file = fopen(out_file_name, "w+b");
    if (export_file == NULL) {
        printf("\n Error: Unable to create output file!\n");
        goto secure_exit;
    }

    //Init freetype lib
    if (FT_Init_FreeType(&ft))
    {
        printf("\n FreeType init error!\n");
        goto secure_exit;
    }

    //Set hinting engine
    if (FT_Property_Set(ft, "cff", "hinting-engine", &hinting_engine)) {
        printf("\n Hinting error");
    }

    //Open font file
    if (FT_New_Face(ft, font_file, 0, &face))
    {
        printf("\n Error: Unable to open font file\n");
        goto secure_exit;
    }

    //Set parameters
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);


    u32 = FT_Get_First_Char(face, &gid);
    while (gid != 0)
    {
        Utf32_To_Utf8(u32, u8);
        fprintf(export_file, "%s", u8);


        u32 = FT_Get_Next_Char(face, u32, &gid);
        if (max < u32) {
            max = u32;
        }
    }


    printf("\n All characters saved to file successfully!\n");

    return 0;

    secure_exit:

        if (export_file) {
            fclose(export_file);
        }
        if (face) {
            FT_Done_Face(face);
        }
        if (ft) {
            FT_Done_FreeType(ft);
        }

        return 1;

}
