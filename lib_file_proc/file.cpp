#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "file.h"

Files_Errors file_read_open(FILE** file, const char* filename)
{
    assert(!*file);
    assert(filename);

    *file = fopen(filename,  "r");

    if(*file == nullptr)
    {
        fprintf(stderr, "file read couldn't open fread open\n");
        return FILE_R_OPEN_ERR;
    }

    return ALL_GOOD_RET_F;
}


Files_Errors file_write_open(FILE** file, const char* filename)
{
    assert(!*file);

    *file = fopen(filename, "w");

    if(*file == nullptr)
    {
        fprintf(stderr, "file write couldn't open\n");
        return FILE_W_OPEN_ERR;
    }

    return ALL_GOOD_RET_F;
}


Files_Errors file_close(FILE* file)
{
    int file_cl_res = fclose(file);

    if(file_cl_res != 0)
    {
        fprintf(stderr, "file close error\n");
        return FILE_CLOSE_ERR;
    }

    return ALL_GOOD_RET_F;
}

Files_Errors file_is_err(Files_Errors result, const char* name, const size_t line)
{
    assert(name);

    if(result != ALL_GOOD_RET_F)
    {
        fprintf(stderr, "Error here: %s:%lu\n\n", name, line);
        return MACRO_FILE_ERR;
    }

    return MACRO_FILE_GOOD;
}