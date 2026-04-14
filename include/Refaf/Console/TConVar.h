#pragma once
#include "ConCommand.h"

namespace Refaf
{
    template <typename T>
    class TConVar : public ConCommand
    {
        T *value;

        // Direct handing doesn't work
        static void __thiscall HandleInternal(const TConVar *var, const STD::Vector<STD::String> &args);

    public:
        TConVar(const char *name, const char *description, T *value) : ConCommand(name, description), value{value} {}

        virtual void Handle(const STD::Vector<STD::String> &args) const override { HandleInternal(this, args); }
    };

    static_assert(sizeof(TConVar<bool>) == 0x10);

    template <>
    void __thiscall TConVar<bool>::HandleInternal(const TConVar<bool> *var, const STD::Vector<STD::String> &args) asm("0x41F9C0");

    static_assert(sizeof(TConVar<int>) == 0x10);

    template <>
    void __thiscall TConVar<int>::HandleInternal(const TConVar<int> *var, const STD::Vector<STD::String> &args) asm("0x41FA10");

    static_assert(sizeof(TConVar<float>) == 0x10);

    template <>
    void __thiscall TConVar<float>::HandleInternal(const TConVar<float> *var, const STD::Vector<STD::String> &args) asm("0x41FB50");
}
