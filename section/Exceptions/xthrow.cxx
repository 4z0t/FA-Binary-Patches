#include <stdexcept>

_STD_BEGIN
[[noreturn]] _CRTIMP2_PURE void __CLRCALL_PURE_OR_CDECL _Xlength_error(_In_z_ const char* const _Message) {
    _THROW(length_error(_Message));
}
_STD_END