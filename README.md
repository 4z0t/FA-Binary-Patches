# FA Engine Patches Repository

This are some binary patches for Supreme Commander Forged Alliance.

To apply them build or get the patcher itself from here

<https://github.com/FAForever/FA_Patcher>

# Change List

## Signature patches

- See `SigPatches.txt`

## Technical patches

- Adds `lua_createtable`
  - section/lua_createtable.cpp
- Adds registrators for Sim and UI Lua funcs.
  - hooks/LuaFuncRegs.cpp
  - section/LuaFuncRegs.cpp

## Fixes

- Allows multiple collisions to be processed during collision checks.
  - hooks/FixCollisions.cpp
- Fix `CMauiControl:SetAlpha`: don't change color part and check for 3rd argument as boolean.
  - hooks/SetAlpha.cpp
  - section/SetAlpha.cpp
- Fix `UserDecal:SetPosition` argument.
  - hooks/DecalSetPositionFix.cpp
- Fix `ForkThread` function. Allow only lua function to be passed as first argument.
  - hooks/ForkThreadFix.cpp
  - section/ForkThreadFix.cpp
- Fix `Unit:SetStat` function, crashed before. Now returns true if value must be set.
  - hooks/SetStatFix.cpp
  - section/SetStatFix.cpp
- Upgrade Progress Fix
  - hooks/HUpgradeProgressFix.cpp
  - section/UpgradeProgressFix.cpp
- Allows to use 4GB on x64
  - hooks/HFix4GB.cpp
- Reduce call sim beat of UI
  - hooks/HUIReduceCallSimBeat.cpp
- Removing CTRL formations
  - hooks/HRemovingCTRLFormations.cpp
- Ignore empty reclaim orders
  - hooks/HIgnoreEmptyReclaim.cpp
- Zeroing GuardScanRadius if the unit's order is not Patrol/AttackMove. Increases performance.
  - hooks/HGuardFix.cpp
  - section/GuardFix.cpp
- Prevents the crash with 'None' collisions of air. #3235
  - hooks/AirNoneCollisionFix.cpp
- Prevents commander exploding for no reason. #3406
  - hooks/NegativeIncomeFix.cpp
- Range ring performance improvement (reducing height cylinders)
  - hooks/HRangeRings2.cpp
  - section/RangeRings2.cpp
- Range ring performance improvement (don't render each ring twice)
  - hooks/RangeRings.cpp
  - section/RangeRings.cpp
- Camera performance improvements
  - hooks/CameraPerf.cpp
- Fix replays desyncing when a player leaves the game
  - hooks/DesyncFix.cpp
  - section/gpg_net.cpp
  - section/moho_decode.cpp
  - section/moho_input.cpp
  - section/include/desync_fix_global.h
- Kill exception during map loading
  - hooks/Kill_maploader_except.cpp
- Make xact3d the error message print once
  - hooks/xact_3dapply.cpp
  - section/xact_3d_fix.cpp
- Prevent blueprint editor being used without cheat mode
  - hooks/FixOpenBPEditor.cpp
- Can't become an observer while your human allies are still alive
  - hooks/ObserverFix.cpp
- Stop the engine calling lua every time a plane turns
  - hooks/OnMotionTurnEvent.cpp
- Optimised some AI actions
  - hooks/aiinitattack.cpp
- Fix a crash when units arrive at waypoints
  - hooks/WayPointArrive.cpp
- Improvements to lua messages
  - hooks/LuaMessages.cpp
- Stops reclaim if unit is paused (similar to 'build', 'assist' etc.)
  - hooks/StopReclaimWhenPaused.cpp
  - section/StopReclaimWhenPaused.cpp

## Additions

| Name                                   | Description                                                                                                                                                                                                                                                                                                                                                                                                                                             | Sections                                                 | Hooks                                    |
| -------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ---------------------------------------- |
| Custom World Rendering                 | Adds Custom World Rendering functions:<br> - `UI_DrawRect(pos:vector, size:float, color:string, thickness?=0.15:float)`<br> - `UI_DrawCircle(pos:vector, radius:float, color:string, thickness?=0.15:float)` <br>Both functions must be called within `OnRenderWorld(delta_time)` of `gamemain.lua`. CWR can be enabled with console command `ui_CustomWorldRendering | DrawFunc.cpp<br>DrawCircleSetColor.cpp                   | HDraw.cpp<br>GetFPS.cpp |                                                          |                                          |
| Strategic icon scale support           |                                                                                                                                                                                                                                                                                                                                                                                                                                                         | IconScale.cpp                                            | IconScale.cpp                            |
| New method to `WorldView`              | `ProjectMultiple` projects multiple vectors at once                                                                                                                                                                                                                                                                                                                                                                                                     | WorldView.cpp                                            |                                          |
| New methods for `UserUnit`             | `GetInterpolatedPosition` returns position of a unit interpolated by current frame <br> `GetFractionComplete` returns float from 0 to 1                                                                                                                                                                                                                                                                                                                 | UserUnit.cpp                                             |                                          |
| New method for `MauiBitmap`            | `SetColorMask` - sets color mask for bitmap                                                                                                                                                                                                                                                                                                                                                                                                             | MauiBitmap.cpp                                           |                                          |
| 5th argument for `IssueMobileBuild`    | adds 5th argumrnt as flag whether to pick all units for build order                                                                                                                                                                                                                                                                                                                                                                                     | IssueMobileBuild.cpp                                     | IssueMobileBuild.cpp                     |
| Affinity Mask  manipulation            | Adds `GetProcessAffinityMask`, `SetProcessAffinityMask`, `SetProcessPriority` to "init.lua"                                                                                                                                                                                                                                                                                                                                                             | OnCreateInitLuaState.cpp                                 | OnCreateInitLuaState.cpp                 |
| `Unit:GetCommandQueue` adjustment      | Adds the commandType, position, targetId and blueprintId to sim `Unit:GetCommandQueue`                                                                                                                                                                                                                                                                                                                                                                  | SimGetCommandQueue.cpp                                   | SimGetCommandQueue.cpp                   |
| New categories                         | `CANLANDONWATER` for air units to be able to land on water <br> `OBSTRUCTSBUILDING` for props to block buildings from being build on top of those                                                                                                                                                                                                                                                                                                       | Categories.cpp                                           | CanLandOnWater.cpp <br>  Reclaimable.cpp |
| Changing army of ACUs                  | Allows changing army of ACUs                                                                                                                                                                                                                                                                                                                                                                                                                            |                                                          | HTransferACUs.cpp                        |
| Team colors                            | Allows customize colors for team color mode                                                                                                                                                                                                                                                                                                                                                                                                             | TeamColorMode.cpp                                        | TeamColorMode.cpp                        |
| `UserUnit:GetCommandQueue` adjustments | Adds the order ID to the UserUnit:GetCommandQueue output                                                                                                                                                                                                                                                                                                                                                                                                | OrderIDOutput.cpp                                        | HOrderIDOutput.cpp                       |
| Sim speed                              | GetSessionClients also output maximum sim speed(maxSP)                                                                                                                                                                                                                                                                                                                                                                                                  | OutputMaxSP.cpp                                          | HOutputMaxSP.cpp                         |
| `cam_DefaultMiniLOD 0`                 | Console command: "cam_DefaultMiniLOD 0" now disable mesh renderer for minimap                                                                                                                                                                                                                                                                                                                                                                           | MinimapMesh.cpp                                          | MinimapMesh.cpp                          |
| Walls selection                        | Allow players to double-click to select Walls                                                                                                                                                                                                                                                                                                                                                                                                           |                                                          | WallSelection.cpp                        |
| New function `GetTimeForProfile`       | Adds GetTimeForProfile to Sim and UI. Allows to deal with the loss of accuracy                                                                                                                                                                                                                                                                                                                                                                          | GetTimeForProfile.cpp                                    |                                          |
| `table` functions                      | Adds optimized table.getsize2(~25 times faster) and table.empty2 <br> Adds getn2 as alias getn. Entry point to math functions queue                                                                                                                                                                                                                                                                                                                     |                                                          | GetTableSize.cpp                         |
| Sim rate                               | Maximum sim rate up to 50                                                                                                                                                                                                                                                                                                                                                                                                                               |                                                          | DelClampMaxSimRate.cpp                   |
| Salem amphibious mode                  | Adds an amphibious mode toggle to the Salem                                                                                                                                                                                                                                                                                                                                                                                                             | luaSimGetStat.cpp                                        | luaSimGetStat.cpp                        |
| `LOWSELECTPRIO` category               | Make `LOWSELECTPRIO` apply to units under construction                                                                                                                                                                                                                                                                                                                                                                                                  | selectionPriority.cpp                                    | selectionPriority.cpp                    |
| `GetDepositsAroundPoint`               | Adds `GetDepositsAroundPoint` to Sim and UI                                                                                                                                                                                                                                                                                                                                                                                                             | SimGetDepositsAroundPoint.cpp                            |                                          |
| `SetInvertMidMouseButton`              | Adds `SetInvertMidMouseButton` to UI                                                                                                                                                                                                                                                                                                                                                                                                                    | InvertMidMouseButton.cpp                                 |                                          |
| **Ultra** graphics preset              | Unlock an "Ultra" graphics preset                                                                                                                                                                                                                                                                                                                                                                                                                       |                                                          | UnlockUltraPreset.cpp                    |
| `SetCommandSource`                     | Adds `SetCommandSource` to Sim. Allow armies to be shared by multiple players                                                                                                                                                                                                                                                                                                                                                                           | SimSetCommandSource.cpp                                  |                                          |
| `GetMouseWorldPos`                     | Adds GetMouseWorldPos to Sim                                                                                                                                                                                                                                                                                                                                                                                                                            | SimGetMouseWorldPos.cpp                                  |                                          |
| `SessionIsReplay`                      | Adds `SessionIsReplay` to Sim                                                                                                                                                                                                                                                                                                                                                                                                                           | SimIsReplay.cpp                                          |                                          |
| `SetFocusArmy`                         | Adds `SetFocusArmy` to Sim. Sets focus without restrictions                                                                                                                                                                                                                                                                                                                                                                                             | SimSetFocusArmy.cpp                                      |                                          |
| Upgrades of units under construction   | Allow upgrades to be queued on units under construction                                                                                                                                                                                                                                                                                                                                                                                                 | BuildUnit.cpp <br> SelectUnit.cpp <br> SimArmyCreate.cpp | BuildUnit.cpp                            |
