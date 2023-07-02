# How to create an engine patch

Tested under Windows.

## Preparation of the required software.

Install MSYS2: https://www.msys2.org/
Install GCC x32/x64 via MSYS console:
x32: pacman -S mingw-w64-i686-gcc
x64: pacman -S mingw-w64-x86_64-gcc
Optional pacman -S mingw-w64-i686-make

The latest release of the EXE patcher: https://github.com/FAForever/FA_Patcher

Basic EXE of the game: https://content.faforever.com/build/ForgedAlliance_base.exe

Optionally install Git: https://git-scm.com

Engine patches: https://github.com/FAForever/FA-Binary-Patches

Put the EXE of the patcher, the base EXE of the game, the patches of the engine in one folder.

To compile a patch, just run FaP.exe
By default, a new file is created in the same folder and named ForgedAlliance_exxt.exe
In config.txt you can specify paths and other patcher settings.

## What's where?

/hooks contains only ASM.
/section contains ASM/C/C++(the standard library is not available).
define.h(created by the patcher) contains all addresses from /section.

All or almost all known engine structures: /section/include/moho.h
Pointers to common functions and main classes: /section/include/global.h
Everything you need to work with LUA: /section/include/LuaAPI.h
List of all engine functions that are called from LUA: /section/include/LuaCFuncs.h

## How do I add a new LUA function?

New features register here: /section/LuaFuncRegs.cpp

Only global(_G) UI and SIM functions are supported.
All functions of one group are connected in a chain.
When adding and deleting, you need to keep the chain connected.
The beginning and end of the chain are marked as // * chain entry, // End * chain
(recommend placing functions between them)

Example of a new function: /section/SimSetCommandSource.cpp

## The subtleties of working with the section.

You can mix everything in the section files(very don't recommend it).
Example: /section/gpg_net.cpp

If you need to work with the processor register from C, you can do this: register int eax asm("eax");

## Subtleties of working with hooks.

The main rules are described in the patch repo.

It is critically important to make sure that the hooks are embedded exactly(bytes, registers, stack) and do not break the rest of the code.
If the hook does not fit, you can transfer part of the instructions to the /section file.
Example: /hooks/HUpgradeProgressFix.cpp , /section/UpgradeProgressFix.cpp

You can also place the code in several unoccupied places.
Example: /hooks/HFix4GB.cpp

GCC always compiles JMP/CALL with a long(4 bytes) offset.
The way to make it use a short(1 byte): JMP .-0x73;
If you use CALL, you need to return via RET or fix the stack before returning.

Global variables can be declared in both hooks and section files.

Example of string patching: /hooks/LuaMessages.cpp
Example of loading a DLL: /hooks/HFAExtLoad.cpp , /section/FAExtLoad.cpp