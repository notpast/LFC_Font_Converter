#ifndef _LFC_UTILS_H_
#define _LFC_UTILS_H_

#include <stdio.h>




#define MAX_FILE_NAME_LEN   1000


//Remove spacial charakters in the file name and add size as a suffix 
int Format_File_Name(const char* input_file, char* array_name, int size);


// Extract only the file name from full path
char* Get_File_Name(char* full_path);

//Check file name for length and null pointer
int Check_File_Name(const char* file_name);


int Check_File_Exist(const char* file_name);


long Get_File_Size(FILE* inp);


/*
 Confirm file overwrite: 'y' or 'n'
	@return
		1 : overwrite (yes / allow)
		0 : do not overwrite (no / deny)
*/
int Confirm_File_Overwrite_Prompt(const char* file_name);







#endif