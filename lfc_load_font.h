#ifndef _LFC_LOAD_FONT_H_
#define _LFC_LOAD_FONT_H_

#include <stdio.h>

#include"lfc_font_def.h"
#include"lfc_unicode.h"


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
 * 
 */

FONT_PROFILE* Lfc_Load_Font(const char* font_file, int size, int render_mode, UTF32* render_list);




#endif
 
