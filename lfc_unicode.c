#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <stdlib.h>

#include "lfc_unicode.h"




/*
U + 0000 	U + 007F 	    0xxxxxxx
U + 0080 	U + 07FF 	    110xxxxx 	10xxxxxx
U + 0800 	U + FFFF 	    1110xxxx 	10xxxxxx 	10xxxxxx
U + 10000 	U + 1FFFFF  	11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
U + 200000 	U + 3FFFFFF     111110xx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
U + 4000000 U + 7FFFFFFF 	1111110x 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
*/


/*
 * @brief Converts a single UTF-32 code point to UTF-8 byte sequence.
 *
 * @param utf32_c UTF-32 Unicode code point to convert
 * @param utf8_s  Output buffer for UTF-8 bytes ( must be at least 7 bytes)
 * @return int    Number of UTF-8 bytes written (1-6), or 0 on error
 *
 */

int Utf32_To_Utf8(UTF32 utf32_c, char* utf8_s) {
    UTF32 us8_len;
    UTF32 i;

    if (utf8_s == NULL) {
        return 0;
    }

    unsigned char rot = 0xFF;

    for (i = 0; i < 7; i++) {
        utf8_s[i] = 0;
    }

    us8_len = 0;

    if (utf32_c < 0x80) {
        utf8_s[us8_len++] = utf32_c;
    }
    else if (utf32_c < 0x800) {
        rot = rot << 6;
        utf8_s[us8_len++] = (((utf32_c) >> 6) & 0x1F) | rot;
        utf8_s[us8_len++] = (utf32_c & 0x3F) | 0x80;
    }
    else if (utf32_c < 0x10000) {
        rot = rot << 5;
        utf8_s[us8_len++] = (((utf32_c) >> 12) & 0x0F) | rot;
        utf8_s[us8_len++] = (((utf32_c) >>  6) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (utf32_c & 0x3F) | 0x80;
    }
    else if (utf32_c < 0x200000) {
        rot = rot << 4;
        utf8_s[us8_len++] = (((utf32_c) >> 18) & 0x07) | rot;
        utf8_s[us8_len++] = (((utf32_c) >> 12) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >>  6) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (utf32_c & 0x3F) | 0x80;
    }
    else if(utf32_c < 0x4000000) {
        rot = rot << 3;
        utf8_s[us8_len++] = (((utf32_c) >> 24) & 0x03) | rot;
        utf8_s[us8_len++] = (((utf32_c) >> 18) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >> 12) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >>  6) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (utf32_c& 0x3F) | 0x80;
    }
    else if (utf32_c < 0x80000000) {
        rot = rot << 2;
        utf8_s[us8_len++] = (((utf32_c) >> 30) & 0x01) | rot;
        utf8_s[us8_len++] = (((utf32_c) >> 24) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >> 18) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >> 12) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (((utf32_c) >>  6) & 0x3F) | 0x80;
        utf8_s[us8_len++] = (utf32_c & 0x3F) | 0x80;
    }

    return us8_len;
}


/*
 * @brief Converts a single UTF-8 character to UTF-32 code point.
 *
 * @param utf8_string Pointer to UTF-8 byte sequence (null-terminated)
 * @return            UTF-32 code point, or 0 on error/invalid input
 *
 */

UTF32 U8_To_U32(const char* utf8_string) {

    if (utf8_string == NULL) {
        return 0;
    }

    unsigned int i = 0, u_32_char, len = 0, w = 0;
    const unsigned char* s;

    //The pair number to compare UTF-8 first byte for get byte count
    const unsigned char u8[5] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0 };
    const unsigned char c8[5] = { 0xFC, 0xF8, 0xF0, 0xE0, 0xC0 };

    s = utf8_string;

    if (utf8_string == NULL) {
        return 0;
    }

        u_32_char = 0;
        if ((*s) >127) {
            w = 0;
            for (i = 0; i < 5; i++) {
                if ((*s & u8[i]) == c8[i]) {
                    w = 5 - i;
                    u_32_char = (*s) & (0xff >> (7 - i));
                    break;
                }
            }
            if (w == 0) {
                return 0;
            }
            for (i = 0; i < w; i++) {
                s++;
                if (*s == 0) {
                    return 0;
                }
                if ((*s & 0xC0) != 0x80) {
                    return 0;
                }
                u_32_char = (u_32_char << 6) | ((*s) & 0x3F);
            }
        }
        else {
            u_32_char = *s;
        }

         return u_32_char;

}


//UTF-8 character count
int Utf8_Char_Count(const char* utf8_string) {

    int i = 0, len = 0, w = 0;
    const char* s = utf8_string;
    //The pair number to compare UTF-8 first byte for get byte count
    const unsigned char u8[5] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0 };
    const unsigned char c8[5] = { 0xFC, 0xF8, 0xF0, 0xE0, 0xC0 };

    if (utf8_string == NULL) {
        return -1;
    }
    len = 0;
    while (*s) {
        if ((*s) > 127) {
            w = 0;
            for (i = 0; i < 5; i++) {
                if ((*s & u8[i]) == c8[i]) {
                    w = 5 - i;
                    break;
                }
            }
            if (w == 0) {
                return -1;
            }
            do {
                s++;
                if ((*s & 0xC0) != 0x80) {
                    return -1;
                }
            } while (--w);
        }
        s++;
        len++;
    }
    return len;
}


UTF32* Utf8_To_Utf32(const char* utf8_string, int char_count) {

    unsigned int i = 0, u_32_char, len = 0, w = 0;
    const unsigned char* s;
    UTF32* utf32_string;

    //The pair number to compare UTF-8 first byte for get byte count
    const unsigned char u8[5] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0 };
    const unsigned char c8[5] = { 0xFC, 0xF8, 0xF0, 0xE0, 0xC0 };

    s = utf8_string;

    if (utf8_string == NULL || char_count<1) {
        return NULL;
    }

    utf32_string = (UTF32*)malloc((char_count + 1) * sizeof(UTF32));
    if (utf32_string == NULL) {
        return NULL;
    }

    len = 0;
    while (*s) {
        u_32_char = 0;
        if ((*s) > 127) {
            w = 0;
            for (i = 0; i < 5; i++) {
                if ((*s & u8[i]) == c8[i]) {
                    w = 5 - i;
                    u_32_char = (*s) & (0xff >> (7 - i));
                    break;
                }
            }
            if (w == 0) {
                free(utf32_string);
                return NULL;
            }
            for (i = 0; i < w; i++) {
                s++;
                if (*s == 0) {
                    free(utf32_string);
                    return NULL;
                }
                if ((*s & 0xC0) != 0x80) {
                    free(utf32_string);
                    return NULL;
                }
                u_32_char = (u_32_char << 6) | ((*s) & 0x3F);
            }
        }
        else {
            u_32_char = *s;
        }


        utf32_string[len] = u_32_char;
        len++;
        s++;

    }

    utf32_string[len] = 0;

    return utf32_string;
}



int UTF32_Len(UTF32* str) {
    int len=0;
    if (str == NULL) {
        return 0;
    }

    while (str[len]) { len++; }

    return len;
}





/*
 * @brief This function reads a file containing UTF-8 encoded characters, validates the UTF-8
 *        encoding, counts the characters, and converts them to a UTF-32 Unicode code point array.
 *        The input file should contain valid UTF-8 encoded text. Each UTF-8 character sequence
 *        in the file will be converted to its corresponding UTF-32 code point.
 *
 * @param list_file Path to the UTF-8 character list file.
 *
 * @return  Pointer to a dynamically allocated array of UTF-32 code points.
 *          The array is terminated with a null character (0).
 *          Returns NULL on any error condition
 */

 UTF32* Read_Char_List_File(const char* list_file) {

    FILE* utf8_file;
    long utf8_list_byte_len;
    unsigned char* utf8_list;

    int char_count;
    UTF32* u32_list;

    size_t read_len = 0;


    if (list_file == NULL) {
        return NULL;
    }

    utf8_file = fopen(list_file, "rb");

    if (utf8_file == NULL) {
        printf(" File <%s> UTF-8 charakter list file not found.", list_file);
        return NULL;
    }

    //Get file size
    utf8_list_byte_len = Get_File_Size(utf8_file);

    if (utf8_list_byte_len < 1) {
        fclose(utf8_file);
        return NULL;
    }


    //Memory allocation for the list file content
    utf8_list = malloc(utf8_list_byte_len + 1);
    if (utf8_list == NULL) {
        fclose(utf8_file);
        return NULL;
    }

    //Read char list in the file
    read_len = fread(utf8_list, 1, utf8_list_byte_len, utf8_file);

    if (read_len != utf8_list_byte_len) {
        free(utf8_list);
        fclose(utf8_file);
        return NULL;
    }

    utf8_list[utf8_list_byte_len] = '\0';

    //Close utf8 list file
    fclose(utf8_file);


    //Convert utf8 to utf32 code
    char_count = Utf8_Char_Count(utf8_list);

    //This list not a utf-8. 
    if (char_count < 0) {
        free(utf8_list);
        return NULL;
    }
    else {
        u32_list = Utf8_To_Utf32(utf8_list, char_count);
    }

    if (u32_list == NULL) {
        free(utf8_list);
        return NULL;
    }

    free(utf8_list);

    return u32_list;

}




static int CK_Separator(unsigned char chr) {
    unsigned char clist[] = "\x20\n\r\t\f,;.:";
    for (int i = 0; clist[i]; i++) {
        if (chr == clist[i]) return 1;
    }
    return 0;
}









/*
* @brief Reads a file containing hex-encoded UTF-32 codepoints and converts them to an array.
*
* @param file_name Path to the file containing hex values (e.g., "0x1F600", "0041")
* @return UTF32* Dynamically allocated array of UTF-32 codepoints, NULL-terminated.
*         Returns NULL on failure.
*
* @note The file should contain hex values separated by whitespace or other separators.
*       Maximum capacity is 64K codepoints (65536).
*       Caller is responsible for freeing the returned memory.
*/

UTF32* Utf32_List_File(const char* file_name) {

    char line[1024];

    FILE* u32_file;
    char* comment_mark = NULL;
    UTF32 u32_code;

    unsigned int list_len = 0;

    FILE* list_file;
    list_file = fopen(file_name, "rb");

    // Return NULL if file cannot be opened
    if (list_file == NULL) {
        printf("Error: File '%s' cannot be opened!\n", file_name);
        return NULL;
    }


    // Allocate memory for 64K UTF-32 codes (+1 for null terminator)
    UTF32* u32_list = malloc(1024 * 64 * sizeof(UTF32) + 1);
    if (u32_list == NULL) {
        printf("Error: Memory allocation failed (u32_list)!\n");
        fclose(list_file);
        return NULL;
    }

    while (fgets(line, 1024, list_file)) {

        if (sscanf(line, "%x", &u32_code) > 0) {
            //printf("\n %x", u32_code);
            u32_list[list_len++] = u32_code;
        }
    }

    u32_list[list_len] = 0;

    return u32_list;
}
