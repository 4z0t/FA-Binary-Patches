#include "include/moho.h"

struct luaFuncDescReg
{                         // 0x1C bytes
    void **RegisterFunc;  // call for register lua function
    char *FuncName;       // lua name function
    char *ClassName;      // lua class name. <global> if class none
    char *FuncDesc;       // for log
    luaFuncDescReg *Next; // reg func of chain
    void *FuncPtr;        // code address
    void *ClassPtr;       // C++ class type address. NULL if class none
};
VALIDATE_SIZE(luaFuncDescReg, 0x1C)

int SimSessionIsReplay(lua_State *L);             // End Sim chain
luaFuncDescReg SSIRRegDesc = {0x00E45E90,         // Std register func
                              0x00E4AFBC,         // "SessionIsReplay"
                              0x00E00D90,         // "<global>"
                              0x00E4AF84,         // "Return true if the active session is a replay session."
                              0x010B8AE8,         // Next reg desc: ArmyGetHandicap
                              SimSessionIsReplay, // Func ptr
                              0x00000000};        // C++ class vtable ptr

int SimSetCommandSource(lua_State *L);
luaFuncDescReg SSCSRegDesc = {0x00E45E90,
                              "SetCommandSource",
                              0x00E00D90,
                              "(targetArmyIndex, sourceHumanIndex, Set or Unset)",
                              &SSIRRegDesc,
                              SimSetCommandSource,
                              0x00000000};

#define s_GDAPName "GetDepositsAroundPoint"
#define s_GDAPDesc "(X, Z, Radius, Type)"
int SimGetDepositsAroundPoint(lua_State *L);
luaFuncDescReg SGDAPRegDesc = {0x00E45E90,
                               s_GDAPName,
                               0x00E00D90,
                               s_GDAPDesc,
                               &SSCSRegDesc,
                               SimGetDepositsAroundPoint,
                               0x00000000};

#define s_GTFPName "GetTimeForProfile"
#define s_GTFPDesc "(OriginTime)"
int GetTimeForProfile(lua_State *L);
luaFuncDescReg SGTFPRegDesc = {0x00E45E90,
                               s_GTFPName,
                               0x00E00D90,
                               s_GTFPDesc,
                               &SGDAPRegDesc,
                               GetTimeForProfile,
                               0x00000000};

luaFuncDescReg SGMWPRegDesc = {0x00E45E90,
                               0x00E451A4, // "GetMouseWorldPos"
                               0x00E00D90,
                               0x00E45188,
                               &SGTFPRegDesc,
                               0x00842BB0,
                               0x00000000};

int SimSetFocusArmy(lua_State *L);             // Sim chain entry
luaFuncDescReg SSFARegDesc = {0x00E45E90,      // Std register func
                              0x00E43408,      // "SetFocusArmy"
                              0x00E00D90,      // "<global>"
                              0x00E451FC,      // "SetFocusArmy(armyIndex or -1)"
                              &SGMWPRegDesc,   // Next reg desc
                              SimSetFocusArmy, // Func ptr
                              0x00000000};     // C++ class vtable ptr

luaFuncDescReg UGTFPRegDesc = {0x00E45E90, // UI chain end
                               s_GTFPName,
                               0x00E00D90,
                               s_GTFPDesc,
                               0x010C3CA4, // Next reg desc: SetFocusArmy
                               GetTimeForProfile,
                               0x00000000};

int SetInvertMidMouseButton(lua_State *L);
luaFuncDescReg USIMMBRegDesc = {0x00E45E90,
                                "SetInvertMidMouseButton",
                                0x00E00D90,
                                "(bool)",
                                &UGTFPRegDesc,
                                SetInvertMidMouseButton,
                                0x00000000};

int GetInterpolatedPosition(lua_State *L);
luaFuncDescReg UUserUnitGetInterpolatedPosition = {0x00E4DA64,
                                                   "GetInterpolatedPosition",
                                                   "UserUnit",
                                                   "UserUnit:GetGetInterpolatedPosition()",
                                                   &USIMMBRegDesc,
                                                   GetInterpolatedPosition,
                                                   0x00F8D89C};

int GetFractionComplete(lua_State *l);
luaFuncDescReg UUserUnitGetFractionComplete = {0x00E4DA64,
                                               "GetFractionComplete",
                                               "UserUnit",
                                               "UserUnit:GetFractionComplete()",
                                               &UUserUnitGetInterpolatedPosition,
                                               GetFractionComplete,
                                               0x00F8D89C};

int LuaDrawRect(lua_State *l);
luaFuncDescReg DrawRectDesc = {0x00E45E90, // UI chain entry
                               "DrawRect",
                               0x00E00D90,
                               "DrawRect()",
                               &UUserUnitGetFractionComplete, // Next reg desc
                               LuaDrawRect,
                               0x00000000};

luaFuncDescReg UGDAPRegDesc = {0x00E45E90, // UI chain entry
                               s_GDAPName,
                               0x00E00D90,
                               s_GDAPDesc,
                               &DrawRectDesc, // Next reg desc
                               SimGetDepositsAroundPoint,
                               0x00000000};
