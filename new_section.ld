OUTPUT_FORMAT(pei-i386)
OUTPUT(section.pe)

_atexit = 0xA8211E;

SECTIONS {
  . = __image_base__ + 0x1000;
  .text : {
    *(.text*)
    *(.data*)
    *(.bss*)
    *(.rdata)
  }
  /DISCARD/ : {
    *(.text.startup)
    *(.rdata$zzz)
    *(.eh_frame*)
    *(.ctors)
    *(.reloc)
    *(.idata*)
  }
}__ZN8LuaState8CheckAnyEi = 0x923f20;
__ZN8LuaState8ArgErrorEiPKc = 0x90bf70;
__ZN8LuaState5ErrorEPKcz = 0x90c1d0;
__ZN8LuaState15GetActiveCStateEv = 0x90bef0;
__ZN4Moho6KillMe4FuncEP9lua_State = 0x696969;
__Z13lua_tobooleanP9lua_Statei = 0x90ca40;
