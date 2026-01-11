#ifndef _LFC_FONT_INFO_H_
#define _LFC_FONT_INFO_H_


#include <stdio.h>

#include "lfc_font_def.h"




/**
 * @brief Write font metadata header to an output C header file
 *
 * This function generates a formatted comment block containing font information
 * at the beginning of the output C header file. The header includes essential
 * metadata about the font for documentation and debugging purposes.
 *
 * @param export_file Pointer to the output FILE stream where the font header
 *                    will be written. Must be opened in write mode.
 * @param font_profile Pointer to the FONT_PROFILE structure containing font
 *                     metadata and character data.
 * @param total_bytes Total number of bytes occupied by the font bitmap data
 *                    in the output array. Used to calculate memory footprint.
 * @param write_char_codes Boolean flag indicating whether to include the list
 *                         of character code points in the header.
 *                         - 0: Do not write character codes
 *                         - 1: Write hexadecimal character codes
 *
 * @return int Returns 0 on success, 1 if invalid parameters are provided
 *
 */

int Lfc_Write_Font_Info(FILE* export_file, FONT_PROFILE* font_profile, unsigned int total_bytes, int write_char_codes);






#endif