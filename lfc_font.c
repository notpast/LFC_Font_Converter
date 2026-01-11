
#include <stdlib.h>

#include "lfc_font.h"
#include "lfc_font_def.h"
#include "lfc_load_font.h"
#include "lfc_c8_format.h"


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


int Lfc_Convert_C8_Format(const char* font_file, unsigned int size, const char *char_list_file, const char* export_file_name) {

	UTF32* render_list;
	FONT_PROFILE* font_profile;
	int success_value = 0;


	//Get characters list from the input file
	render_list = Read_Char_List_File(char_list_file);

	//Check characters list (empty or not)
	if (UTF32_Len(render_list) < 1) {
		return 1;
	}

	//Load font file and create font profile
	font_profile = Lfc_Load_Font(font_file, size, 1, render_list);

	if (font_profile == NULL) {
		if (render_list) {
			free(render_list);
		}
		return 1;
	}


	//The conversion process is performed and the result is saved to the export file.
	success_value=Lfc_Export_C8_Format(font_profile, export_file_name);


	//Clean up, release all memory
	if (render_list) {
		free(render_list);
	}

	if (font_profile) {
		for (int i = 0; i < font_profile->char_data_len; i++) {
			CHAR_DATA* c_data = &(font_profile->char_data[i]);
			if (c_data) {
				if (c_data->bitmap_data) {
					free(c_data->bitmap_data);
				}
			}
		}
		if (font_profile->char_data) {
			free(font_profile->char_data);
		}
		free(font_profile);
	}


	return success_value;
}



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

int Lfc_Convert_C8_Format_Utf32(const char* font_file, unsigned int size, const char* char_list_file, const char* export_file_name) {

	UTF32* render_list;
	FONT_PROFILE* font_profile;
	int success_value = 0;


	//Get characters list from the input file
	render_list = Utf32_List_File(char_list_file);

	//Check characters list (empty or not)
	if (UTF32_Len(render_list) < 1) {
		return 1;
	}

	
	//Load font file and create font profile
	font_profile = Lfc_Load_Font(font_file, size, 1, render_list);

	if (font_profile == NULL) {
		if (render_list) {
			free(render_list);
		}
		return 1;
	}


	//The conversion process is performed and the result is saved to the export file.
	success_value = Lfc_Export_C8_Format(font_profile, export_file_name);


	//Clean up, release all memory
	if (render_list) {
		free(render_list);
	}

	if (font_profile) {
		for (int i = 0; i < font_profile->char_data_len; i++) {
			CHAR_DATA* c_data = &(font_profile->char_data[i]);
			if (c_data) {
				if (c_data->bitmap_data) {
					free(c_data->bitmap_data);
				}
			}
		}
		if (font_profile->char_data) {
			free(font_profile->char_data);
		}
		free(font_profile);
	}


	return success_value;
}




