#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <math.h>
#include <assert.h>

#define FILE_ERR(result, error) if(file_is_err(result, __FILE__, __LINE__) == MACRO_FILE_ERR) return error

enum Files_Errors
{
    ALL_GOOD_RET_F   = 0x00,
    FILE_R_OPEN_ERR  = 0x01,
    FILE_W_OPEN_ERR  = 0x02,
    FILE_CLOSE_ERR   = 0x03,
    MACRO_FILE_ERR   = 0x04,
    MACRO_FILE_GOOD  = 0x05,
};


Files_Errors file_read_open(FILE** file, const char* file_read_open);

Files_Errors file_write_open(FILE** file, const char* file_write_open);

Files_Errors file_close(FILE* file);

Files_Errors file_is_err(Files_Errors result, const char* name, const size_t line);

#endif // FILE_H_