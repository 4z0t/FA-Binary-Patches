#include "include/moho.h"
#include <type_traits>

struct ConDescReg
{
	uintptr_t vftable = 0x0E01700/*Moho::CConCommand::`vftable'*/;
	const char *name;
	const char *description;
	void *value;
};
VALIDATE_SIZE(ConDescReg, 0x10)

using TConFunction = void(*)(vector<string>*);
template<typename T>
inline void RegisterTCon(ConDescReg *descCon)
{
    reinterpret_cast<void(__thiscall *)(void *, ConDescReg *)>(0x41E390)(reinterpret_cast<void *(__stdcall *)()>(0x41BEB0)(), descCon);
    if constexpr (std::is_same_v<T, bool>)
        descCon->vftable = 0xE01790; // Moho::TConVar<bool>::`vftable'
    else if constexpr (std::is_same_v<T, unsigned char>)
        descCon->vftable = 0xE017A0; // Moho::TConVar<unsigned char>::`vftable'
    else if constexpr (std::is_same_v<T, int>)
        descCon->vftable = 0xE01798; // Moho::TConVar<int>::`vftable'
    else if constexpr (std::is_same_v<T, unsigned int>)
        descCon->vftable = 0xE40BB0; // Moho::TConVar<unsigned int>::`vftable'
    else if constexpr (std::is_same_v<T, float>)
        descCon->vftable = 0xE017A8; // Moho::TConVar<float>::`vftable'
    else if constexpr (std::is_same_v<T, string>)
        descCon->vftable = 0xE017B0; // Moho::TConVar<std::string>::`vftable'
    else if constexpr (std::is_same_v<T, TConFunction>)
        descCon->vftable = 0xE01708; // Moho::CConFunc::`vftable'
    else
        static_assert(!std::is_same_v<T, T>, "Not supported type!");
}

ConDescReg myConFunction{0x0E01700, "name", "description", [](vector<string>* vec_) {
        auto& vec = *vec_;
        WarningF("Command: %s", vec[0].data());
        if (vec.size() >= 2)
            for (auto it = vec.begin + 1; it != vec.end; it++)
                WarningF("param[%d]: %s", it - vec.begin, it->data());
    }};

int v;
ConDescReg myConIntV{0x0E01700, "name2", "description", &v};

void OnRegisterTCon()
{
    RegisterTCon<TConFunction>(&myConFunction);
    RegisterTCon<int>(&myConIntV);

}
