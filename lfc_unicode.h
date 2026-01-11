#ifndef _UNICODE_H_
#define _UNICODE_H_



// 4 bytes length!
typedef unsigned long UTF32;


//UTF-8 character count
int Utf8_Char_Count(const char* utf8_string);


/*
 * @brief Converts a single UTF-8 character to UTF-32 code point.
 *
 * @param utf8_string Pointer to UTF-8 byte sequence (null-terminated)
 * @return            UTF-32 code point, or 0 on error/invalid input
 *
 */

UTF32* Utf8_To_Utf32(const char* utf8_string, int char_count);


/*
 * @brief Converts a single UTF-32 code point to UTF-8 byte sequence.
 *
 * @param utf32_c UTF-32 Unicode code point to convert
 * @param utf8_s  Output buffer for UTF-8 bytes ( must be at least 7 bytes)
 * @return int    Number of UTF-8 bytes written (1-6), or 0 on error
 *
 */
int Utf32_To_Utf8(UTF32 utf32_c, char* utf8_s);


// Return length of utf-32 null terminadet string
int UTF32_Len(UTF32* str);


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

UTF32* Read_Char_List_File(const char* list_file);



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
UTF32* Utf32_List_File(const char* file_name);



#endif