#pragma once
#include <exception>
#include "global.h"

// struct __std_exception_data
// {
//     char const *_What;
//     bool _DoFree;
// };

extern "C"
{
    void __cdecl __std_exception_copy(
        __std_exception_data const *_From,
        __std_exception_data *_To)
    {
        _To->_DoFree = _From->_DoFree;
        const char *_What = _From->_What;
        if (!_To->_DoFree)
        {
            _To->_What = _What;
        }
        else if (_What)
        {
            size_t len = strlen(_What) + 1;
            _To->_What = (char *)malloc(len);
            if (_To->_What)
                strcpy_s((char *)_To->_What, len, _From->_What);
        }
        else
        {
            _To->_What = nullptr;
        }
    }

    void __cdecl __std_exception_destroy(
        __std_exception_data *_Data)
    {
        if (_Data->_DoFree)
        {
            free((void *)_Data->_What);
        }
    }
}