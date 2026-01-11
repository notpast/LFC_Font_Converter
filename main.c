/*******************************************************************************
 * LFC Font Converter
 *
 * DESCRIPTION:
 * A specialized tool for converting TrueType font files (.ttf) into C8 bitmap
 * font format optimized for embedded systems and low-memory environments.
 *
 * FEATURES:
 * - Converts TTF/OTF fonts to compact C8 bitmap format
 * - Supports selective character set conversion via UTF-8 character lists
 * - Generates complete font character maps for easy character selection
 * - Produces ready-to-use C header files for embedded projects
 *
 * TYPICAL WORKFLOW:
 * 1. Use the tool to list all available characters in a font
 * 2. Create a custom character set file with desired characters
 * 3. Convert the selected characters to C8 bitmap format
 * 4. Include the generated header file in your embedded project
 *
 * USAGE:
 * Command Line: lfc -f <font.ttf> -o <output.h> -c <chars.txt> -s <size>
 * Developer Mode: Use the test section in main() for rapid prototyping
 *
 * OUTPUT FORMAT:
 * Generates C arrays in "C8" format - monochrome bitmaps
 * optimized for memory-constrained embedded systems and displays.
 *
 *
 ******************************************************************************/




#define _CRT_SECURE_NO_WARNINGS

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#include "lfc_unicode.h"
#include "lfc_font.h"
#include "lfc_utils.h"

#include "lfc_c8_format.h"
#include "lfc_load_font.h"

#include "lfc_print_all.h"




void print_help(char* full_path) {
    if (full_path == NULL) {
        return;
    }
    const char* program_name = Get_File_Name(full_path);

    printf("\n");
    printf(" LFC Font Converter: Convert font files to C8 bitmap font format\n");
    printf(" MIT License\n");
    printf(" Copyright(c) 2026 M.Cetin Atila\n\n");

    printf(" Usage for Conversion : %s -f <filename> -o <filename> -c <filename> -s <size>\n", program_name);
    printf(" Example              : %s -f Arial.ttf -o ssd_arial.h -c chars_list.txt -s 24 \n\n", program_name);

    printf(" Usage for All Chars  : %s -f <filename>  -a <filename>\n", program_name);
    printf(" Example              : %s -f Arial.ttf   -a all_chars.txt\n\n", program_name);
    
    printf("\n");

    printf(" Typical Workflow:\n\n");
    printf("  1. Use -a to list all characters to a file (e.g., all_chars.txt) (for utf-8 character file)\n");
    printf("  2. Review all_chars.txt and create a smaller file with desired characters\n");
    printf("     (e.g., my_chars.txt).\n");
    printf("  3. Use -f, -s, -c or -u (with my_chars.txt), and -o to generate the final font header.\n");
    printf("  4. Use only one of either -c or -u \n");
    printf("\n");

    printf(" Parameters:\n\n");
    printf("  -f <file>  : Input font file name (*.ttf required)\n\n");
    printf("  -o <file>  : Output header file to be created (*.h required)\n");
    printf("               If blank, the output file will be named after the input font file\n\n");
    printf("  -c <file>  : UTF-8 character set file (*.txt required)\n");
    printf("               File containing the list of UTF-8 characters to be converted\n\n");
    printf("  -u <file>  : UTF-32 character set file (*.txt required)\n");
    printf("               File containing the list of UTF-32 code number to be converted\n\n");
    printf("  -s <size>  : Font size in pixels (required, positive integer)\n\n");
    printf("  -a <file>  : Export all characters available in the font to a UTF-8 text file\n");
    printf("               (*.txt required, file will be created)\n");
    printf("               When using this parameter, only -f is required; others are optional.\n");
    printf("               Use this file to check available characters and create your custom character \n");
    printf("               list for the -c parameter.\n\n");
    printf("  -h  or  ?  : Show this help message\n");
    printf("\n");
}



int main(int argc, char* argv[]) {

    unsigned int size=0;
    char* font_file=NULL;
    char* all_chars_file = NULL;
    char* utf8_character_set=NULL;
    char* utf32_character_set = NULL;
    char* out_file_user=NULL;
    int i;




    //  TEST/DEMO SECTION
    //  *******************************************************************************************************
    //  DEVELOPER TESTING AREA - Normally disabled in production builds
    //  
    //  PURPOSE:
    //  This section allows developers to bypass command-line arguments and test the font converter
    //  with predefined parameters. Useful for rapid testing, debugging, and development.
    //
    //  USAGE INSTRUCTIONS:
    //  1. Enable this section by uncommenting when you want to use hardcoded values
    //  2. Disable the regular command-line processing logic (argc/argv handling)
    //  3. Modify the test parameters below as needed
    //  4. Compile and run - the program will use these values directly
    //
    //  IMPORTANT NOTES:
    //  - Remember to disable this section and re-enable command-line processing for release builds
    //  - Ensure file paths are correct and accessible from your working directory
    //  - This section provides immediate feedback without typing command-line parameters
    //  
    //  TEST PARAMETERS - Configure these values for your testing needs:
    //  *******************************************************************************************************
    



    // UNCOMMENT: Print all characters to export file
    // ============================================================================================================
    
    /*
        const char* u_font_file = "ChakraPetch-SemiBold.ttf";
        return Print_All("", "all_char_list.txt");
    */



    // UNCOMMENT: LFC converter with UTF-8 char list file
    // ============================================================================================================

    /*
        const char* u_font_file = "ChakraPetch-SemiBold.ttf"; // Font file
        const char* u_char_list = "char_list.txt";            // Desired UTF-8 character list file to process
        const char* u_out_file = "c8_font.h";                 // Output C8 bitmap font header file
        unsigned int u_font_size = 24;                        // Font size in pixels (positive integer)

        int res = Lfc_Convert_C8_Format(u_font_file, u_font_size, u_char_list, u_out_file);

        if (res == 0) {
            printf("\nConversion successfully completed!\n");
            printf("Output file: %s\n\n", u_out_file);
            return 0;
        }
        else {
            printf("\nConversion failed! (Error code: %d)\n", res);
            printf("Some errors occurred during the conversion process\n\n");
            return 0;
        }

    */


    // UNCOMMENT: LFC converter with UTF-32 Code list file
    // ============================================================================================================
    /*
        const char* u_font_file = "Font Awesome 7 Free-Regular-400.otf"; // Font file
        const char* u_u32_list = "font_awesome.txt";         // UTF-8 character list file to process
        const char* u_out_file = "c8_font.h";                // Output C8 bitmap font header file
        unsigned int u_font_size = 40;                       // Font size in pixels (positive integer)

        int res = Lfc_Convert_C8_Format_Utf32(u_font_file, u_font_size, u_u32_list, u_out_file);

        if (res == 0) {
            printf("\nConversion successfully completed!\n");
            printf("Output file: %s\n\n", u_out_file);
            return 0;
        }
        else {
            printf("\nConversion failed! (Error code: %d)\n", res);
            printf("Some errors occurred during the conversion process\n\n");
            return 0;
        }
    */


    
    //  *******************************************************************************************************
    //  END OF TEST SECTION - Re-enable command-line processing for normal operation  
    



    if (argc < 2 || argv==NULL) {
        print_help(argv[0]);
        return 0;
    }

    

    for (i = 1; i < argc; i++) {
        if (argv[i] == NULL) {
            return 0;
        }

        if (strcmp(argv[i], "-f") == 0) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing filename after -f parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                font_file = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing filename after -o parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                out_file_user = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing filename after -a parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                all_chars_file = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing filename after -c parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                utf8_character_set = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing filename after -u parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                utf32_character_set = argv[++i];
            }
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            // Check if we have reached the end of arguments
            if (i + 1 >= argc) {
                printf("Error: Missing size value after -s parameter\n");
                print_help(argv[0]);
                return 1;
            }
            else {
                size = atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "?") == 0) {
            print_help(argv[0]);
            return 0;
        }
        else {
            printf("Error: Invalid parameter: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }



    if (font_file == NULL) {
        printf("\n Font file not found.");
            return 1;
    }

    if (all_chars_file!=NULL) {
        return Print_All(font_file, all_chars_file);
    }

    if (size < 1 || size>255) {
        printf("Font size error. Font size 1-255");
        return 1;
    }

    if (utf8_character_set == NULL && utf32_character_set == NULL) {
        printf("\n Charakter set file error.");
        return 1;
    }


    if (Check_File_Exist(out_file_user)) {
        if (Confirm_File_Overwrite_Prompt(out_file_user) == 0) {
            return 1;
        }
    }


    if (utf32_character_set) {
        if (!Lfc_Convert_C8_Format_Utf32(font_file, size, utf32_character_set, out_file_user)) {
            printf("\n Conversion successfully completed \n\n");
            return 0;
        }
        else {
            printf("\n\n Conversion fail!\n Some errors occurred during the Conversion\n\n");
            return 1;
        }
    }

    else if (utf8_character_set) {
        if (!Lfc_Convert_C8_Format(font_file, size, utf8_character_set, out_file_user)) {
            printf("\n Conversion successfully completed \n\n");
            return 0;
        }
        else {
            printf("\n\n Conversion fail!\n Some errors occurred during the Conversion\n\n");
            return 1;
        }
    }

    printf("\n\n Some parameters are missing.\n");
    return 1;
    
}
