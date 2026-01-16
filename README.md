# LFC Font Converter



**LFC Font Converter** is a specialized command-line tool that converts TrueType/OpenType fonts into the C8 bitmap font format optimized for embedded systems. Using the FreeType library, it converts only specified characters into efficient C arrays ready for integration into C-based embedded projects.



## ✨ Key Features



- **FreeType-Based Conversion**: Accurate font rendering using the FreeType library

- **Selective Character Conversion**: Convert only needed characters to minimize memory footprint

- **C8 Bitmap Format**: Optimized custom format for embedded systems

- **C Language Integration**: Outputs directly as C arrays (.h and .c files)

- **UTF-8 & UTF-32 Support**: Full Unicode character support

- **Command Line Interface**: Easy integration into build pipelines

- **Visual Studio Project**: Pre-configured for easy compilation



## 📦 Quick Start



## 📋 Complete Parameter Reference



### Export all characters to UTF-8 text file (not conversion)

| Parameter   | Description                              | Required   | Example               |
|-------------|------------------------------------------|------------|-----------------------|
|  -f <file>  | Input font file (TTF format required)    | **Yes**    |  -f Arial.ttf         |
|  -a <file>  | Export all characters to UTF-8 text file | **Yes**    |  -a all_chars.txt     |

Example: Converter.exe -f Arial.ttf -a all_char.txt  
all_char.txt file will be create





### Convert from UTF-8 char list file

| Parameter   | Description                              | Required   | Example               |
|-------------|------------------------------------------|------------|-----------------------|
|  -f <file>  | Input font file (TTF format required)    | **Yes**    |  -f Arial.ttf         |
|  -o <file>  | Output header file (.h extension)        | **Yes**    |  -o my_font.h         |
|  -c <file>  | UTF-8 character set file (.txt)          | **Yes**    |  -c my_chars.txt      |
|  -s <size>  | Font size in pixels (positive integer)   | **Yes**    |  -s 16                |

Example: Converter.exe -f Arial.ttf -o arial.h -c chars.txt -s 24  
arial.h will be create  



### Convert from UTF-32 code list file


| Parameter   | Description                              | Required   | Example               |
|-------------|------------------------------------------|------------|-----------------------|
|  -f <file>  | Input font file (TTF format required)    | **Yes**    |  -f Arial.ttf         |
|  -o <file>  | Output header file (.h extension)        | Optional   |  -o my_font.h         |
|  -u <file>  | UTF-32 character set file (.txt)         | Optional** |  -u utf32_codes.txt   |
|  -s <size>  | Font size in pixels (positive integer)   | **Yes**    |  -s 16                |

Example: Converter.exe -f Arial.ttf -o arial.h -u utf32_codes.txt -s 24  
arial.h will be create

### Show help message

| Parameter   | Description                              | Required   | Example               |
|-------------|------------------------------------------|------------|-----------------------|
|  -h  or  ?  | Show help message                        | Optional   |  -h                   |

Video : https://www.youtube.com/embed/kiy8hUHDtfQ?si=wGdkTaGpn4vWaZG1

