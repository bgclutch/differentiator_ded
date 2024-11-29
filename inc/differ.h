
#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "../../lib_buffer_proc/buffer.h"
#include "../../lib_file_proc/file.h"
#include "bintree.h"

#define AKN_ERR(result, error) if(akinator_is_err(result, __FILE__, __LINE__) == MACRO_AK_ERR) return error


enum Differ_Err
{
    DIFFER_IS_OKAY               = 0x00,
    DIFFER_IS_DEAD               = 0x01,
    DIFFER_BUFFER_CTOR_ERR       = 0x02,
    DIFFER_BUFFER_FILE_OPEN_ERR  = 0x03,
    DIFFER_BUFFER_FILE_CLOSE_ERR = 0x04,
    DIFFER_FILE_ERROR            = 0x05,
    CHOSEN_WORD_GETLINE_ERR      = 0x06,


    MACRO_AK_GOOD = 0x20,
    MACRO_AK_ERR  = 0x30,

};

enum Text_Colors
{
    REDTEXT,
    BLUETEXT,
    GREENTEXT,
    MAGENTATEXT,
    YELLOWTEXT,
};


Differ_Err differ_is_err(const Differ_Err result, const char* name, const size_t line);

#endif // AKINATOR_H_
