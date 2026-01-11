#ifndef _LFC_PRINT_ALL_H_
#define _LFC_PRINT_ALL_H_



/**

    @brief Writes all characters from a font file to a text file

    @param font_file: Path to the input font file (e.g., "arial.ttf")
    @param out_file_name: Path to the output text file (e.g., "output.txt")

    @return: 0 on success, other error code on failure

    This function reads the specified font file and writes all contained
    characters to the specified output text file. If the output
    file already exists, it will be overwritten.

    */


int Print_All(const char* font_file, const char* out);




#endif