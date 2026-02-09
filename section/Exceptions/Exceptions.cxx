#pragma once

#include <exception>

// struct __std_exception_data
// {
//     char const *_What;
//     bool _DoFree;
// };

extern "C" void __cdecl __std_exception_copy(
    __std_exception_data const *_From,
    __std_exception_data *_To)
{
    if (_From->_What != nullptr)
    {
        _To->_What = new char[strlen(_From->_What) + 1];
        strcpy((char *)_To->_What, _From->_What);
    }
    _To->_DoFree = _From->_DoFree;
}

extern "C" void __cdecl __std_exception_destroy(
    __std_exception_data *_Data)
{
    if (_Data->_DoFree)
    {
        delete[] _Data->_What;
    }
}
