#define _CRT_SECURE_NO_WARNINGS




#include <stdio.h>
#include <string.h>

#include "lfc_utils.h"



int  Format_File_Name(const char* input_file, char* array_name, int size) {
    int i, j = 0;

    if (array_name == NULL || input_file == NULL) {
        return 1;
    }

    // Replace forign characters with underscores
    for (i = 0; input_file[i] != '\0' && input_file[i] != '.'; i++) {
        if (!((input_file[i] >= 'a' && input_file[i] <= 'z') || (input_file[i] >= 'A' && input_file[i] <= 'Z'))) {
            array_name[j++] = '_';
        }
        else {
            array_name[j++] = input_file[i];
        }
    }

    // Append the number
    j += sprintf(array_name + j, "_%d", size);

    // Terminate string
    array_name[j] = '\0';

    return 0;
}


// Extract only the file name from full path
char* Get_File_Name(char* full_path) {

    if (full_path == NULL) {
        return NULL;
    }

    char* program_name = full_path;

    // Find the last directory separator
    char* last_slash = strrchr(full_path, '/');
    if (last_slash != NULL) {
        program_name = last_slash + 1;
    }

    // On Windows
    char* last_backslash = strrchr(program_name, '\\');
    if (last_backslash != NULL) {
        program_name = last_backslash + 1;
    }

    return program_name;
}


//Check file name for length and null pointer
int Check_File_Name(const char* file_name) {

    int file_name_len;

    //Check null pointer
    if (file_name == NULL) {
        return 1;
    }

    //Check file name length
    file_name_len = strlen(file_name);
    if (file_name_len < 1 || file_name_len >= MAX_FILE_NAME_LEN) {
        return 2;
    }

    return 0;
}


//Check file exist
int Check_File_Exist(const char* file_name) {
    FILE* test_file;

    if (file_name == NULL) {
        return 2;
    }

    //Try to open
    test_file = fopen(file_name, "r");

    if (test_file == NULL) {
        return 0;
    }

    fclose(test_file);

    return 1;
}

/*
 Confirm file overwrite: 'y' or 'n'
    @return
        1 : overwrite (yes / allow)
        0 : do not overwrite (no / deny)
*/
int Confirm_File_Overwrite_Prompt(const char* file_name) {

    //Total invalid selection
    int try_choice = 3;
    char response[3];

    if (file_name == NULL) {
        return 0;
    }

        while (try_choice--) {
            printf("<%s> file already exists. Do you want to overwrite it? [Yes=y ,No=n]:",file_name);
            if (scanf("%2c", response)) {
                if (response[0] == 'Y' || response[0] == 'y') {
                    return 1;
                }
                else if (response[0] == 'N' || response[0] == 'n') {
                    return 0;
                }
                else {
                    printf("Invalid input!\n");
                }
            }
        }

        return 0;
}


long Get_File_Size(FILE * inp) {
    long file_size;
    long current_pos;

    if (inp == NULL) {
        return -1;
    }

    //Get current file position
    current_pos= ftell(inp);
  
    fseek(inp, 0L, SEEK_END);

    file_size = ftell(inp);

    fseek(inp, current_pos, SEEK_SET);

    return file_size;
}




