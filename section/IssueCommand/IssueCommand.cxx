#include "IssueCommand.h"
#include "CObject.h"
#include "magic_classes.h"
#include "utility.h"
#include <cmath>

SHARED void UNIT_IssueCommand(Moho::EntitySet *units, void *moho,
                              Moho::SSTICommandIssueData *command, bool a5);

SHARED void *GetBlueprintByName(LuaState *ls, int index,
                                const char *name) noexcept(false);

SHARED Moho::AddResult *
EntitySetAddItem(Moho::EntitySet *set, Moho::AddResult *insert_res, void *unit);

int IssueStop(lua_State *L) {
  int top = lua_gettop(L);
  if (top != 1)
    L->LuaState->Error("%s\n  expected %d args, but got %d", __FUNCTION__, 1,
                       top);

  {
    Moho::SSTICommandIssueData command{1};
    Moho::EntitySet units;
    LuaStackObject obj{L->LuaState, 1};
    CheckUnitList(&units, &obj, L->LuaState, "IssueStop");
    void *sim = lua_getglobaluserdata(L);
    UNIT_IssueCommand(&units, sim, &command, 0);
  }
  return 0;
}

int IssueKillSelf(lua_State *L) {
  int top = lua_gettop(L);
  if (top != 1)
    L->LuaState->Error("%s\n  expected %d args, but got %d", __FUNCTION__, 1,
                       top);

  {
    Moho::SSTICommandIssueData command{30};
    Moho::EntitySet units;
    LuaStackObject obj{L->LuaState, 1};
    CheckUnitList(&units, &obj, L->LuaState, "IssueKillSelf");
    void *sim = lua_getglobaluserdata(L);
    UNIT_IssueCommand(&units, sim, &command, 0);
  }
  return 0;
}

static SimRegFunc IssueStopReg{"IssueStop2", "", IssueStop};
static SimRegFunc IssueKillSelfReg{"IssueKillSelf2", "", IssueKillSelf};