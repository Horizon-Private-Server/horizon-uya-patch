/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * 		Unhooks patch file and spawns menu popup telling user patch is downloading.
 * NOTES :
 * 		Each offset is determined per game region.
 * 		This is to ensure compatibility between versions of UYA PAL/NTSC.
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#include <tamtypes.h>

#include "module.h"
#include "messageid.h"
#include <libuya/uya.h>
#include <libuya/game.h>
#include <libuya/time.h>
#include <libuya/string.h>
#include <libuya/stdio.h>
#include <libuya/ui.h>
#include <libuya/net.h>
#include <libuya/graphics.h>
#include <libuya/pad.h>
#include <libuya/player.h>
#include <libuya/gamesettings.h>
#include <libuya/moby.h>

void FlushCache(int operation);

// int selfDestruct __attribute__((section(".config"))) = 0;

/*
	-1 = Both
	0 = Not In Game
	1 = In Game
	40-49 = Specific in-game level
*/
const int patches[][3] = {
#if UYA_PAL
	// patch
	{ 0, 0x005760e4, 0x0c19f270 }, // menu loop hook 1
	{ 0, 0x0057611c, 0x0c19f270 }, // menu loop hook 2
	{ 0, 0x0047cfec, 0x0069a630 }, // patchCreateGame
	{ 0, 0x0047d124, 0x0069ce50 }, // patchAdvancedOptions
	{ 0, 0x0047e9ac, 0x006c1730 }, // patchStaging
	{ 0, 0x0047dbac, 0x006aaa08 }, // patchKeyboard
	{ 0, 0x0047d86c, 0x006a3ab0 }, // patchGameDetails
	{ 0, 0x0047d944, 0x006a3690 }, // patchGameDetailsOpen
	{ 0, 0x0068261c, 0x0c18460e }, // details player text color
	{ 0, 0x006c40e0, 0x0c1b3a86 }, // patchLoadingPopup
	{ 0, 0x006c234c, 0x0040f809 }, // staging headset color
	{ 0, 0x00240608, 0x00000000 }, // custom DME message receiver
	{ 0, 0x0024060c, 0x00000000 }, // late-join loading input receiver
	{ 0, 0x002405fc, 0x00000000 }, // late-join loading state receiver
	{ 0, 0x006a2d50, 0xa220005e }, // search name truncation
	{ 0, 0x006a4694, 0xa0600146 }, // details name truncation
	{ 0, 0x006a47bc, 0x0040f809 }, // details map setter
	{ 0, 0x006a4978, 0x0060f809 }, // details mode setters
	{ 0, 0x006a49a0, 0x0060f809 },

	// Resident late-join team and world-report hooks.
	{ -1, 0x0019ac1c, 0x0c066712 }, // joinSelectTeam
	{ -1, 0x0019a650, 0x0c066712 }, // joinKeepTeam
	{ -1, 0x00195b38, 0x0c066de2 }, // joinIsReportHost: update
	{ -1, 0x001964b8, 0x0c066de2 }, // joinIsReportHost: send
	{ -1, 0x00196048, 0x0c066de2 }, // joinShouldCloseWorld
	{ 0, 0x006d3518, 0x0c1b5352 }, // joinBeginClient
	{ 0, 0x006d38fc, 0x0c1b502c }, // joinEndClient
	{ 0, 0x006c4520, 0x0c16695e }, // joinLoadClient

	// late-join admission gates (also present outside the lobby overlay)
	{ -1, 0x0019a750, 0x28420007 },
	{ -1, 0x0019a564, 0x28420007 },
	{ -1, 0x0019a5e0, 0x28420007 },

	// maploader
	{ 0, 0x005a41c8, 0x0c170a90 }, // LOAD_LEVEL_READ_WAD_HOOK
	{ -1, 0x00194f8c, 0x0c0653b6 }, // LOAD_LEVEL_READ_LEVEL_TOC_HOOK
	{ 0, 0x005a4070, 0x0c06528e }, // LOAD_LEVEL_CD_SYNC_HOOK
	{ 0, 0x006787a8, 0x03e00008 }, // LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK
	{ -1, 0x00193340, 0x0c064bda }, // LEVEL_EXIT_FUNCTION_HOOK
	{ 0, 0x005c1354, 0x0c064e22 }, // LOAD_LEVEL_SOUND_BANK_HOOK
#else
	// patch
	{ 0, 0x005753a4, 0x0c19e7c2 }, // menu loop hook 1
	{ 0, 0x005753dc, 0x0c19e7c2 }, // menu loop hook 2
	{ 0, 0x0047d0ac, 0x00697e20 }, // patchCreateGame
	{ 0, 0x0047d1e4, 0x0069a640 }, // patchAdvancedOptions
	{ 0, 0x0047ea6c, 0x006bec18 }, // patchStaging
	{ 0, 0x0047dc6c, 0x006a80b8 }, // patchKeyboard
	{ 0, 0x0047d92c, 0x006a12a0 }, // patchGameDetails
	{ 0, 0x0047da04, 0x006a0e80 }, // patchGameDetailsOpen
	{ 0, 0x0067fafc, 0x0c183c48 }, // details player text color
	{ 0, 0x006c15c8, 0x0c1b30a4 }, // patchLoadingPopup
	{ 0, 0x006bf834, 0x0040f809 }, // staging headset color
	{ 0, 0x00240788, 0x00000000 }, // custom DME message receiver
	{ 0, 0x0024078c, 0x00000000 }, // late-join loading input receiver
	{ 0, 0x0024077c, 0x00000000 }, // late-join loading state receiver
	{ 0, 0x006a0540, 0xa220005e }, // search name truncation
	{ 0, 0x006a1e84, 0xa0600146 }, // details name truncation
	{ 0, 0x006a1fac, 0x0040f809 }, // details map setter
	{ 0, 0x006a2168, 0x0060f809 }, // details mode setters
	{ 0, 0x006a2190, 0x0060f809 },

	// Resident late-join team and world-report hooks.
	{ -1, 0x0019ad5c, 0x0c066762 }, // joinSelectTeam
	{ -1, 0x0019a790, 0x0c066762 }, // joinKeepTeam
	{ -1, 0x00195c28, 0x0c066e32 }, // joinIsReportHost: update
	{ -1, 0x001965a8, 0x0c066e32 }, // joinIsReportHost: send
	{ -1, 0x00196138, 0x0c066e32 }, // joinShouldCloseWorld

	// Restore calls before clearing/replacing the patch they currently target.
	{ 0, 0x006d0d90, 0x0c1b4970 }, // joinBeginClient: native 0x006d25c0
	{ 0, 0x006d1174, 0x0c1b464a }, // joinEndClient: native 0x006d1928
	{ 0, 0x006c1a08, 0x0c1664aa }, // joinLoadClient: native 0x005992a8
	{ -1, 0x0019a890, 0x28420007 }, // late-join admission gates
	{ -1, 0x0019a6a4, 0x28420007 },
	{ -1, 0x0019a720, 0x28420007 },

	// maploader
	{ 0, 0x005a2560, 0x0c17027e }, // LOAD_LEVEL_READ_WAD_HOOK
	{ -1, 0x0019507c, 0x0c0653f2 }, // LOAD_LEVEL_READ_LEVEL_TOC_HOOK
	{ 0, 0x005a2408, 0x0c0652ca }, // LOAD_LEVEL_CD_SYNC_HOOK
	{ 0, 0x00675dc0, 0x03e00008 }, // LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK
	{ -1, 0x00193430, 0x0c064c16 }, // LEVEL_EXIT_FUNCTION_HOOK
	{ 0, 0x005bf36c, 0x0c064e5e }, // LOAD_LEVEL_SOUND_BANK_HOOK
#endif
#if UYA_PAL
	// Bakisi Isles
	{ 40, 0x00240654, 0x00538f48 }, // remote bot leave callback
	{ 40, 0x00407388, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 40, 0x00407394, 0x0c117674 }, // GetSniperShotDirectionHook
	{ 40, 0x00407398, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 40, 0x00408508, 0xa2400020 }, // SniperShotCreatedHook
	{ 40, 0x00408bc0, 0x0c11344e }, // SniperShotCollLineFixHook
	{ 40, 0x00408bc4, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 40, 0x00424d20, 0x0c1468be }, // EnableTurretSync_Addr
	{ 40, 0x00430568, 0x0c122f90 }, // vaFlagEventUiPopup_Hook
	{ 40, 0x004428c8, 0x0c133998 }, // FpsCounter_Hooks
	{ 40, 0x00442928, 0x0c1158ca }, // FpsCounter_Hooks +0x00000060
	{ 40, 0x00442eac, 0x0c125a96 }, // GameTimeUpdate_Hook
	{ 40, 0x00447d6c, 0x03e00008 }, // cameraRender +0x0000040c
	{ 40, 0x00448f10, 0x0c112210 }, // cameraRender +0x000015b0
	{ 40, 0x0046db20, 0x0c11dfac }, // vaGameplayHook
	{ 40, 0x004951a4, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 40, 0x004b0b28, 0x0c138f20 }, // SetTextureArrow_Hook
	{ 40, 0x004b21f4, 0x0c138da4 }, // DM_RespawnUpdater
	{ 40, 0x004b22b0, 0x0c119cd8 }, // DM_PressXToRespawn
	{ 40, 0x004b614c, 0x03e00008 }, // pause menu callback
	{ 40, 0x004c9018, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 40, 0x004c901c, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 40, 0x004f4ac8, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f95d8, 0x0c13e224 }, // WhoHitMeHook
	{ 40, 0x004f96ec, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f9904, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f9c30, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004fa1ec, 0x0c151122 }, // Juggernaut vaOnPlayerKill_Func
	{ 40, 0x004fa1f4, 0x0c0fd826 }, // RespawnTimerFunc_Player
	{ 40, 0x004fa320, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 40, 0x004fa324, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 40, 0x004fa360, 0x0c13e996 }, // PlayerRespawnFunc +0x00000040
	{ 40, 0x004fa37c, 0x0c1466f4 }, // PlayerRespawnFunc +0x0000005c
	{ 40, 0x00507c70, 0x0c118708 }, // Healthbars_Hook
	{ 40, 0x00511cec, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 40, 0x00526d60, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00527458, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00527468, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x005274b4, 0x24020027 }, // PostHitInvinc
	{ 40, 0x005274b8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 40, 0x00527770, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x0052916c, 0x0c13e30c }, // PlayerInvincibleTimer_Hook
	{ 40, 0x0052c50c, 0x0c143536 }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00531e34, 0x0c150a0e }, // PlayerStateUpdate_Hook
	{ 40, 0x00532414, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 40, 0x00532a0c, 0x0c1255c2 }, // PatchDeadShooting_ShootingHook
	{ 40, 0x00533b14, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 40, 0x005361a8, 0x0c14d694 }, // PlayerPnetUpdates_Hook
	{ 40, 0x00543fe4, 0x0c15117e }, // vaGameTimerEndHookDmNoTeams
	{ 40, 0x00544510, 0x0c13ed12 }, // UpdateScoreboard +0x00000088
	{ 40, 0x005471b0, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 40, 0x005472a0, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 40, 0x005472a4, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 40, 0x005480fc, 0x0c152ac6 }, // MapScore_SelectBtn_Addr
	{ 40, 0x00549170, 0x0c152a2a }, // QuickSelectCheck_Hook
	{ 40, 0x0054b23c, 0x0c151944 }, // GadgetEventHook
	{ 40, 0x0054b2b8, 0x0c151122 }, // Juggernaut vaOnPlayerKill_Func
	// Hoven Gorge
	{ 41, 0x00240654, 0x0053b060 }, // remote bot leave callback
	{ 41, 0x00406cf0, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 41, 0x00406cfc, 0x0c117d60 }, // GetSniperShotDirectionHook
	{ 41, 0x00406d00, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 41, 0x00407e70, 0xa2400020 }, // SniperShotCreatedHook
	{ 41, 0x00408528, 0x0c113b2e }, // SniperShotCollLineFixHook
	{ 41, 0x0040852c, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 41, 0x00426778, 0x0c147104 }, // EnableTurretSync_Addr
	{ 41, 0x00431fc0, 0x0c123708 }, // vaFlagEventUiPopup_Hook
	{ 41, 0x00444448, 0x0c1341de }, // FpsCounter_Hooks
	{ 41, 0x004444a8, 0x0c115faa }, // FpsCounter_Hooks +0x00000060
	{ 41, 0x00444a2c, 0x0c1262dc }, // GameTimeUpdate_Hook
	{ 41, 0x004498ec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 41, 0x0044aa90, 0x0c1128f0 }, // cameraRender +0x000015b0
	{ 41, 0x0046f6d0, 0x0c11e69c }, // vaGameplayHook
	{ 41, 0x004972bc, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 41, 0x004b2c40, 0x0c139766 }, // SetTextureArrow_Hook
	{ 41, 0x004b430c, 0x0c1395ea }, // DM_RespawnUpdater
	{ 41, 0x004b43c8, 0x0c11a3c4 }, // DM_PressXToRespawn
	{ 41, 0x004b8264, 0x03e00008 }, // pause menu callback
	{ 41, 0x004cb130, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 41, 0x004cb134, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 41, 0x004f6be0, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004fb6f0, 0x0c13ea6a }, // WhoHitMeHook
	{ 41, 0x004fb804, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004fba1c, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004fbd48, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004fc304, 0x0c151994 }, // Juggernaut vaOnPlayerKill_Func
	{ 41, 0x004fc30c, 0x0c0fd3c8 }, // RespawnTimerFunc_Player
	{ 41, 0x004fc438, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 41, 0x004fc43c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 41, 0x004fc478, 0x0c13f1dc }, // PlayerRespawnFunc +0x00000040
	{ 41, 0x004fc494, 0x0c146f3a }, // PlayerRespawnFunc +0x0000005c
	{ 41, 0x00509d88, 0x0c118df4 }, // Healthbars_Hook
	{ 41, 0x00513e04, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 41, 0x00528e78, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00529570, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00529580, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x005295cc, 0x24020027 }, // PostHitInvinc
	{ 41, 0x005295d0, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 41, 0x00529888, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x0052b284, 0x0c13eb52 }, // PlayerInvincibleTimer_Hook
	{ 41, 0x0052e624, 0x0c143d7c }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00533f4c, 0x0c151280 }, // PlayerStateUpdate_Hook
	{ 41, 0x0053452c, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 41, 0x00534b24, 0x0c125e08 }, // PatchDeadShooting_ShootingHook
	{ 41, 0x00535c2c, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 41, 0x005382c0, 0x0c14deda }, // PlayerPnetUpdates_Hook
	{ 41, 0x005461ac, 0x0c1519f0 }, // vaGameTimerEndHookDmNoTeams
	{ 41, 0x005466d8, 0x0c13f558 }, // UpdateScoreboard +0x00000088
	{ 41, 0x00549378, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 41, 0x00549468, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 41, 0x0054946c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 41, 0x0054a2c4, 0x0c153338 }, // MapScore_SelectBtn_Addr
	{ 41, 0x0054b338, 0x0c15329c }, // QuickSelectCheck_Hook
	{ 41, 0x0054d404, 0x0c1521b6 }, // GadgetEventHook
	{ 41, 0x0054d480, 0x0c151994 }, // Juggernaut vaOnPlayerKill_Func
	// Outpost X12
	{ 42, 0x00240654, 0x00530938 }, // remote bot leave callback
	{ 42, 0x003febe8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 42, 0x003febf4, 0x0c1158e0 }, // GetSniperShotDirectionHook
	{ 42, 0x003febf8, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 42, 0x003ffd68, 0xa2400020 }, // SniperShotCreatedHook
	{ 42, 0x00400420, 0x0c1116ae }, // SniperShotCollLineFixHook
	{ 42, 0x00400424, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 42, 0x0041d6a8, 0x0c14473a }, // EnableTurretSync_Addr
	{ 42, 0x00428ef0, 0x0c121288 }, // vaFlagEventUiPopup_Hook
	{ 42, 0x0043b248, 0x0c131814 }, // FpsCounter_Hooks
	{ 42, 0x0043b2a8, 0x0c113b2a }, // FpsCounter_Hooks +0x00000060
	{ 42, 0x0043b82c, 0x0c123912 }, // GameTimeUpdate_Hook
	{ 42, 0x004406ec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 42, 0x00441890, 0x0c110470 }, // cameraRender +0x000015b0
	{ 42, 0x004664d0, 0x0c11c21c }, // vaGameplayHook
	{ 42, 0x0048cb94, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 42, 0x004a8518, 0x0c136d9c }, // SetTextureArrow_Hook
	{ 42, 0x004a9be4, 0x0c136c20 }, // DM_RespawnUpdater
	{ 42, 0x004a9ca0, 0x0c117f44 }, // DM_PressXToRespawn
	{ 42, 0x004adb3c, 0x03e00008 }, // pause menu callback
	{ 42, 0x004c0a08, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 42, 0x004c0a0c, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 42, 0x004ec4b8, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004f0fc8, 0x0c13c0a0 }, // WhoHitMeHook
	{ 42, 0x004f10dc, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004f12f4, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004f1620, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004f1bdc, 0x0c14efca }, // Juggernaut vaOnPlayerKill_Func
	{ 42, 0x004f1be4, 0x0c0fb386 }, // RespawnTimerFunc_Player
	{ 42, 0x004f1d10, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 42, 0x004f1d14, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 42, 0x004f1d50, 0x0c13c812 }, // PlayerRespawnFunc +0x00000040
	{ 42, 0x004f1d6c, 0x0c144570 }, // PlayerRespawnFunc +0x0000005c
	{ 42, 0x004ff660, 0x0c116974 }, // Healthbars_Hook
	{ 42, 0x005096dc, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 42, 0x0051e750, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051ee48, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051ee58, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051eea4, 0x24020027 }, // PostHitInvinc
	{ 42, 0x0051eea8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 42, 0x0051f160, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x00520b5c, 0x0c13c188 }, // PlayerInvincibleTimer_Hook
	{ 42, 0x00523efc, 0x0c1413b2 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x00529824, 0x0c14e8b6 }, // PlayerStateUpdate_Hook
	{ 42, 0x00529e04, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 42, 0x0052a3fc, 0x0c12343e }, // PatchDeadShooting_ShootingHook
	{ 42, 0x0052b504, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 42, 0x0052db98, 0x0c14b510 }, // PlayerPnetUpdates_Hook
	{ 42, 0x0053ba84, 0x0c14f026 }, // vaGameTimerEndHookDmNoTeams
	{ 42, 0x0053bfb0, 0x0c13cb8e }, // UpdateScoreboard +0x00000088
	{ 42, 0x0053ec50, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 42, 0x0053ed40, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 42, 0x0053ed44, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 42, 0x0053fb9c, 0x0c15096e }, // MapScore_SelectBtn_Addr
	{ 42, 0x00540c10, 0x0c1508d2 }, // QuickSelectCheck_Hook
	{ 42, 0x00542cdc, 0x0c14f7ec }, // GadgetEventHook
	{ 42, 0x00542d58, 0x0c14efca }, // Juggernaut vaOnPlayerKill_Func
	// Korgon Outpost
	{ 43, 0x00240654, 0x0052e0d0 }, // remote bot leave callback
	{ 43, 0x003fdfc8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 43, 0x003fdfd4, 0x0c114fc4 }, // GetSniperShotDirectionHook
	{ 43, 0x003fdfd8, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 43, 0x003ff148, 0xa2400020 }, // SniperShotCreatedHook
	{ 43, 0x003ff800, 0x0c110d9e }, // SniperShotCollLineFixHook
	{ 43, 0x003ff804, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 43, 0x0041ad18, 0x0c143d20 }, // EnableTurretSync_Addr
	{ 43, 0x00426a98, 0x0c1208e0 }, // vaFlagEventUiPopup_Hook
	{ 43, 0x00438e08, 0x0c130dfa }, // FpsCounter_Hooks
	{ 43, 0x00438e68, 0x0c11321a }, // FpsCounter_Hooks +0x00000060
	{ 43, 0x004393ec, 0x0c122ec6 }, // GameTimeUpdate_Hook
	{ 43, 0x0043e2ac, 0x03e00008 }, // cameraRender +0x0000040c
	{ 43, 0x0043f450, 0x0c10fb60 }, // cameraRender +0x000015b0
	{ 43, 0x00464060, 0x0c11b8fc }, // vaGameplayHook
	{ 43, 0x0048a264, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 43, 0x004a5cb0, 0x0c136382 }, // SetTextureArrow_Hook
	{ 43, 0x004a737c, 0x0c136206 }, // DM_RespawnUpdater
	{ 43, 0x004a7438, 0x0c117628 }, // DM_PressXToRespawn
	{ 43, 0x004ab2d4, 0x03e00008 }, // pause menu callback
	{ 43, 0x004be1a0, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 43, 0x004be1a4, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 43, 0x004e9c50, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ee760, 0x0c13b686 }, // WhoHitMeHook
	{ 43, 0x004ee874, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004eea8c, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004eedb8, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ef374, 0x0c14e584 }, // Juggernaut vaOnPlayerKill_Func
	{ 43, 0x004ef37c, 0x0c0fb336 }, // RespawnTimerFunc_Player
	{ 43, 0x004ef4a8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 43, 0x004ef4ac, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 43, 0x004ef4e8, 0x0c13bdf8 }, // PlayerRespawnFunc +0x00000040
	{ 43, 0x004ef504, 0x0c143b56 }, // PlayerRespawnFunc +0x0000005c
	{ 43, 0x004fcdf8, 0x0c116058 }, // Healthbars_Hook
	{ 43, 0x00506e74, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 43, 0x0051bee8, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0051c5e0, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0051c5f0, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0051c63c, 0x24020027 }, // PostHitInvinc
	{ 43, 0x0051c640, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 43, 0x0051c8f8, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0051e2f4, 0x0c13b76e }, // PlayerInvincibleTimer_Hook
	{ 43, 0x00521694, 0x0c140998 }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x00526fbc, 0x0c14de70 }, // PlayerStateUpdate_Hook
	{ 43, 0x0052759c, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 43, 0x00527b94, 0x0c1229f2 }, // PatchDeadShooting_ShootingHook
	{ 43, 0x00528c9c, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 43, 0x0052b330, 0x0c14aaf6 }, // PlayerPnetUpdates_Hook
	{ 43, 0x0053916c, 0x0c14e5e0 }, // vaGameTimerEndHookDmNoTeams
	{ 43, 0x00539698, 0x0c13c174 }, // UpdateScoreboard +0x00000088
	{ 43, 0x0053c338, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 43, 0x0053c428, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 43, 0x0053c42c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 43, 0x0053d284, 0x0c14ff28 }, // MapScore_SelectBtn_Addr
	{ 43, 0x0053e2f8, 0x0c14fe8c }, // QuickSelectCheck_Hook
	{ 43, 0x005403c4, 0x0c14eda6 }, // GadgetEventHook
	{ 43, 0x00540440, 0x0c14e584 }, // Juggernaut vaOnPlayerKill_Func
	// Metropolis
	{ 44, 0x00240654, 0x0052d420 }, // remote bot leave callback
	{ 44, 0x003fcb88, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 44, 0x003fcb94, 0x0c114c94 }, // GetSniperShotDirectionHook
	{ 44, 0x003fcb98, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 44, 0x003fdd08, 0xa2400020 }, // SniperShotCreatedHook
	{ 44, 0x003fe3c0, 0x0c110a6e }, // SniperShotCollLineFixHook
	{ 44, 0x003fe3c4, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 44, 0x0041a5a8, 0x0c1439f4 }, // EnableTurretSync_Addr
	{ 44, 0x00425df0, 0x0c1205e6 }, // vaFlagEventUiPopup_Hook
	{ 44, 0x00438148, 0x0c130ace }, // FpsCounter_Hooks
	{ 44, 0x004381a8, 0x0c112eea }, // FpsCounter_Hooks +0x00000060
	{ 44, 0x0043872c, 0x0c122bcc }, // GameTimeUpdate_Hook
	{ 44, 0x0043d5ec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 44, 0x0043e790, 0x0c10f830 }, // cameraRender +0x000015b0
	{ 44, 0x004633a0, 0x0c11b5cc }, // vaGameplayHook
	{ 44, 0x0048967c, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 44, 0x004a5000, 0x0c136056 }, // SetTextureArrow_Hook
	{ 44, 0x004a66cc, 0x0c135eda }, // DM_RespawnUpdater
	{ 44, 0x004a6788, 0x0c1172f8 }, // DM_PressXToRespawn
	{ 44, 0x004aa624, 0x03e00008 }, // pause menu callback
	{ 44, 0x004bd4f0, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 44, 0x004bd4f4, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 44, 0x004e8fa0, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004edab0, 0x0c13b35a }, // WhoHitMeHook
	{ 44, 0x004edbc4, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004edddc, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004ee108, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004ee6c4, 0x0c14e284 }, // Juggernaut vaOnPlayerKill_Func
	{ 44, 0x004ee6cc, 0x0c0fab6e }, // RespawnTimerFunc_Player
	{ 44, 0x004ee7f8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 44, 0x004ee7fc, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 44, 0x004ee838, 0x0c13bacc }, // PlayerRespawnFunc +0x00000040
	{ 44, 0x004ee854, 0x0c14382a }, // PlayerRespawnFunc +0x0000005c
	{ 44, 0x004fc148, 0x0c115d28 }, // Healthbars_Hook
	{ 44, 0x005061c4, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 44, 0x0051b238, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051b930, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051b940, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051b98c, 0x24020027 }, // PostHitInvinc
	{ 44, 0x0051b990, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 44, 0x0051bc48, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051d644, 0x0c13b442 }, // PlayerInvincibleTimer_Hook
	{ 44, 0x005209e4, 0x0c14066c }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0052630c, 0x0c14db70 }, // PlayerStateUpdate_Hook
	{ 44, 0x005268ec, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 44, 0x00526ee4, 0x0c1226f8 }, // PatchDeadShooting_ShootingHook
	{ 44, 0x00527fec, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 44, 0x0052a680, 0x0c14a7ca }, // PlayerPnetUpdates_Hook
	{ 44, 0x0053856c, 0x0c14e2e0 }, // vaGameTimerEndHookDmNoTeams
	{ 44, 0x00538a98, 0x0c13be48 }, // UpdateScoreboard +0x00000088
	{ 44, 0x0053b738, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 44, 0x0053b828, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 44, 0x0053b82c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 44, 0x0053c684, 0x0c14fc28 }, // MapScore_SelectBtn_Addr
	{ 44, 0x0053d6f8, 0x0c14fb8c }, // QuickSelectCheck_Hook
	{ 44, 0x0053f7c4, 0x0c14eaa6 }, // GadgetEventHook
	{ 44, 0x0053f840, 0x0c14e284 }, // Juggernaut vaOnPlayerKill_Func
	// Blackwater City
	{ 45, 0x00240654, 0x0052acb8 }, // remote bot leave callback
	{ 45, 0x003f9970, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 45, 0x003f997c, 0x0c1142a0 }, // GetSniperShotDirectionHook
	{ 45, 0x003f9980, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 45, 0x003faaf0, 0xa2400020 }, // SniperShotCreatedHook
	{ 45, 0x003fb1a8, 0x0c11006e }, // SniperShotCollLineFixHook
	{ 45, 0x003fb1ac, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 45, 0x00416410, 0x0c14301a }, // EnableTurretSync_Addr
	{ 45, 0x00421c58, 0x0c11fb70 }, // vaFlagEventUiPopup_Hook
	{ 45, 0x00435948, 0x0c1300f4 }, // FpsCounter_Hooks
	{ 45, 0x004359a8, 0x0c1124ea }, // FpsCounter_Hooks +0x00000060
	{ 45, 0x00435f2c, 0x0c1221f2 }, // GameTimeUpdate_Hook
	{ 45, 0x0043adec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 45, 0x0043bf90, 0x0c10ee30 }, // cameraRender +0x000015b0
	{ 45, 0x00460bd0, 0x0c11abdc }, // vaGameplayHook
	{ 45, 0x00486f14, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 45, 0x004a2898, 0x0c13567c }, // SetTextureArrow_Hook
	{ 45, 0x004a3f64, 0x0c135500 }, // DM_RespawnUpdater
	{ 45, 0x004a4020, 0x0c116904 }, // DM_PressXToRespawn
	{ 45, 0x004a7ebc, 0x03e00008 }, // pause menu callback
	{ 45, 0x004bad88, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 45, 0x004bad8c, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 45, 0x004e6838, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004eb348, 0x0c13a980 }, // WhoHitMeHook
	{ 45, 0x004eb45c, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004eb674, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004eb9a0, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004ebf5c, 0x0c14d87e }, // Juggernaut vaOnPlayerKill_Func
	{ 45, 0x004ebf64, 0x0c0f9ee8 }, // RespawnTimerFunc_Player
	{ 45, 0x004ec090, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 45, 0x004ec094, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 45, 0x004ec0d0, 0x0c13b0f2 }, // PlayerRespawnFunc +0x00000040
	{ 45, 0x004ec0ec, 0x0c142e50 }, // PlayerRespawnFunc +0x0000005c
	{ 45, 0x004f99e0, 0x0c115334 }, // Healthbars_Hook
	{ 45, 0x00503a5c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 45, 0x00518ad0, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005191c8, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005191d8, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x00519224, 0x24020027 }, // PostHitInvinc
	{ 45, 0x00519228, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 45, 0x005194e0, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x0051aedc, 0x0c13aa68 }, // PlayerInvincibleTimer_Hook
	{ 45, 0x0051e27c, 0x0c13fc92 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x00523ba4, 0x0c14d16a }, // PlayerStateUpdate_Hook
	{ 45, 0x00524184, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 45, 0x0052477c, 0x0c121d1e }, // PatchDeadShooting_ShootingHook
	{ 45, 0x00525884, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 45, 0x00527f18, 0x0c149df0 }, // PlayerPnetUpdates_Hook
	{ 45, 0x00535d54, 0x0c14d8da }, // vaGameTimerEndHookDmNoTeams
	{ 45, 0x00536280, 0x0c13b46e }, // UpdateScoreboard +0x00000088
	{ 45, 0x00538f20, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 45, 0x00539010, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 45, 0x00539014, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 45, 0x00539e6c, 0x0c14f222 }, // MapScore_SelectBtn_Addr
	{ 45, 0x0053aee0, 0x0c14f186 }, // QuickSelectCheck_Hook
	{ 45, 0x0053cfac, 0x0c14e0a0 }, // GadgetEventHook
	{ 45, 0x0053d028, 0x0c14d87e }, // Juggernaut vaOnPlayerKill_Func
	// Command Center
	{ 46, 0x00240654, 0x0052aa78 }, // remote bot leave callback
	{ 46, 0x00407d78, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 46, 0x00407d84, 0x0c1144de }, // GetSniperShotDirectionHook
	{ 46, 0x00407d88, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 46, 0x00408ef8, 0xa2400020 }, // SniperShotCreatedHook
	{ 46, 0x004095b0, 0x0c11038e }, // SniperShotCollLineFixHook
	{ 46, 0x004095b4, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 46, 0x004263e8, 0x0c11fc10 }, // vaFlagEventUiPopup_Hook
	{ 46, 0x004365c8, 0x0c1300f2 }, // FpsCounter_Hooks
	{ 46, 0x00436628, 0x0c11280a }, // FpsCounter_Hooks +0x00000060
	{ 46, 0x00436bac, 0x0c1221f0 }, // GameTimeUpdate_Hook
	{ 46, 0x0043ba6c, 0x03e00008 }, // cameraRender +0x0000040c
	{ 46, 0x0043cc10, 0x0c10f150 }, // cameraRender +0x000015b0
	{ 46, 0x004614c8, 0x0c11ae0c }, // vaGameplayHook
	{ 46, 0x00486f0c, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 46, 0x004a2890, 0x0c13566e }, // SetTextureArrow_Hook
	{ 46, 0x004a3f5c, 0x0c1354f2 }, // DM_RespawnUpdater
	{ 46, 0x004a4018, 0x0c116b42 }, // DM_PressXToRespawn
	{ 46, 0x004a7eb4, 0x03e00008 }, // pause menu callback
	{ 46, 0x004bad80, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 46, 0x004bad84, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 46, 0x004e6800, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004eb310, 0x0c13a972 }, // WhoHitMeHook
	{ 46, 0x004eb424, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004eb63c, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004eb968, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004ebf24, 0x0c14d694 }, // Juggernaut vaOnPlayerKill_Func
	{ 46, 0x004ebf2c, 0x0c0fdaa2 }, // RespawnTimerFunc_Player
	{ 46, 0x004ec058, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 46, 0x004ec05c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 46, 0x004ec098, 0x0c13b0e4 }, // PlayerRespawnFunc +0x00000040
	{ 46, 0x004ec0b4, 0x0c142dc0 }, // PlayerRespawnFunc +0x0000005c
	{ 46, 0x004f99a8, 0x0c115572 }, // Healthbars_Hook
	{ 46, 0x00503a24, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 46, 0x00518890, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00518f88, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00518f98, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00518fe4, 0x24020027 }, // PostHitInvinc
	{ 46, 0x00518fe8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 46, 0x005192a0, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x0051ac9c, 0x0c13aa5a }, // PlayerInvincibleTimer_Hook
	{ 46, 0x0051e03c, 0x0c13fc84 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00523964, 0x0c14cf80 }, // PlayerStateUpdate_Hook
	{ 46, 0x00523f44, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 46, 0x0052453c, 0x0c121d1c }, // PatchDeadShooting_ShootingHook
	{ 46, 0x00525644, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 46, 0x00527cd8, 0x0c149d60 }, // PlayerPnetUpdates_Hook
	{ 46, 0x005355ac, 0x0c14d6f0 }, // vaGameTimerEndHookDmNoTeams
	{ 46, 0x00535ad8, 0x0c13b460 }, // UpdateScoreboard +0x00000088
	{ 46, 0x00538778, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 46, 0x00538868, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 46, 0x0053886c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 46, 0x005396c4, 0x0c14f038 }, // MapScore_SelectBtn_Addr
	{ 46, 0x0053a738, 0x0c14ef9c }, // QuickSelectCheck_Hook
	{ 46, 0x0053c804, 0x0c14deb6 }, // GadgetEventHook
	{ 46, 0x0053c880, 0x0c14d694 }, // Juggernaut vaOnPlayerKill_Func
	// Blackwater Docks
	{ 47, 0x00240654, 0x0052d2f8 }, // remote bot leave callback
	{ 47, 0x00409cd8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 47, 0x00409ce4, 0x0c114efe }, // GetSniperShotDirectionHook
	{ 47, 0x00409ce8, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 47, 0x0040ae58, 0xa2400020 }, // SniperShotCreatedHook
	{ 47, 0x0040b510, 0x0c110dae }, // SniperShotCollLineFixHook
	{ 47, 0x0040b514, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 47, 0x00428c38, 0x0c120630 }, // vaFlagEventUiPopup_Hook
	{ 47, 0x00438e48, 0x0c130b12 }, // FpsCounter_Hooks
	{ 47, 0x00438ea8, 0x0c11322a }, // FpsCounter_Hooks +0x00000060
	{ 47, 0x0043942c, 0x0c122c10 }, // GameTimeUpdate_Hook
	{ 47, 0x0043e2ec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 47, 0x0043f490, 0x0c10fb70 }, // cameraRender +0x000015b0
	{ 47, 0x00463d48, 0x0c11b82c }, // vaGameplayHook
	{ 47, 0x0048978c, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 47, 0x004a5110, 0x0c13608e }, // SetTextureArrow_Hook
	{ 47, 0x004a67dc, 0x0c135f12 }, // DM_RespawnUpdater
	{ 47, 0x004a6898, 0x0c117562 }, // DM_PressXToRespawn
	{ 47, 0x004aa734, 0x03e00008 }, // pause menu callback
	{ 47, 0x004bd600, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 47, 0x004bd604, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 47, 0x004e9080, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004edb90, 0x0c13b392 }, // WhoHitMeHook
	{ 47, 0x004edca4, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004edebc, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004ee1e8, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004ee7a4, 0x0c14e0b4 }, // Juggernaut vaOnPlayerKill_Func
	{ 47, 0x004ee7ac, 0x0c0fe27a }, // RespawnTimerFunc_Player
	{ 47, 0x004ee8d8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 47, 0x004ee8dc, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 47, 0x004ee918, 0x0c13bb04 }, // PlayerRespawnFunc +0x00000040
	{ 47, 0x004ee934, 0x0c1437e0 }, // PlayerRespawnFunc +0x0000005c
	{ 47, 0x004fc228, 0x0c115f92 }, // Healthbars_Hook
	{ 47, 0x005062a4, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 47, 0x0051b110, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x0051b808, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x0051b818, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x0051b864, 0x24020027 }, // PostHitInvinc
	{ 47, 0x0051b868, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 47, 0x0051bb20, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x0051d51c, 0x0c13b47a }, // PlayerInvincibleTimer_Hook
	{ 47, 0x005208bc, 0x0c1406a4 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x005261e4, 0x0c14d9a0 }, // PlayerStateUpdate_Hook
	{ 47, 0x005267c4, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 47, 0x00526dbc, 0x0c12273c }, // PatchDeadShooting_ShootingHook
	{ 47, 0x00527ec4, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 47, 0x0052a558, 0x0c14a780 }, // PlayerPnetUpdates_Hook
	{ 47, 0x00537e2c, 0x0c14e110 }, // vaGameTimerEndHookDmNoTeams
	{ 47, 0x00538358, 0x0c13be80 }, // UpdateScoreboard +0x00000088
	{ 47, 0x0053aff8, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 47, 0x0053b0e8, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 47, 0x0053b0ec, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 47, 0x0053bf44, 0x0c14fa58 }, // MapScore_SelectBtn_Addr
	{ 47, 0x0053cfb8, 0x0c14f9bc }, // QuickSelectCheck_Hook
	{ 47, 0x0053f084, 0x0c14e8d6 }, // GadgetEventHook
	{ 47, 0x0053f100, 0x0c14e0b4 }, // Juggernaut vaOnPlayerKill_Func
	// Aquatos Sewers
	{ 48, 0x00240654, 0x0052c5f8 }, // remote bot leave callback
	{ 48, 0x004098e0, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 48, 0x004098ec, 0x0c114bbe }, // GetSniperShotDirectionHook
	{ 48, 0x004098f0, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 48, 0x0040aa60, 0xa2400020 }, // SniperShotCreatedHook
	{ 48, 0x0040b118, 0x0c110a6e }, // SniperShotCollLineFixHook
	{ 48, 0x0040b11c, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 48, 0x00427f50, 0x0c1202f0 }, // vaFlagEventUiPopup_Hook
	{ 48, 0x00438148, 0x0c1307d2 }, // FpsCounter_Hooks
	{ 48, 0x004381a8, 0x0c112eea }, // FpsCounter_Hooks +0x00000060
	{ 48, 0x0043872c, 0x0c1228d0 }, // GameTimeUpdate_Hook
	{ 48, 0x0043d5ec, 0x03e00008 }, // cameraRender +0x0000040c
	{ 48, 0x0043e790, 0x0c10f830 }, // cameraRender +0x000015b0
	{ 48, 0x00463048, 0x0c11b4ec }, // vaGameplayHook
	{ 48, 0x00488a8c, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 48, 0x004a4410, 0x0c135d4e }, // SetTextureArrow_Hook
	{ 48, 0x004a5adc, 0x0c135bd2 }, // DM_RespawnUpdater
	{ 48, 0x004a5b98, 0x0c117222 }, // DM_PressXToRespawn
	{ 48, 0x004a9a34, 0x03e00008 }, // pause menu callback
	{ 48, 0x004bc900, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 48, 0x004bc904, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 48, 0x004e8380, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004ece90, 0x0c13b052 }, // WhoHitMeHook
	{ 48, 0x004ecfa4, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004ed1bc, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004ed4e8, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004edaa4, 0x0c14dd74 }, // Juggernaut vaOnPlayerKill_Func
	{ 48, 0x004edbd8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 48, 0x004edbdc, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 48, 0x004edc18, 0x0c13b7c4 }, // PlayerRespawnFunc +0x00000040
	{ 48, 0x004edc34, 0x0c1434a0 }, // PlayerRespawnFunc +0x0000005c
	{ 48, 0x004fb528, 0x0c115c52 }, // Healthbars_Hook
	{ 48, 0x005055a4, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 48, 0x0051a410, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x0051ab08, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x0051ab18, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x0051ab64, 0x24020027 }, // PostHitInvinc
	{ 48, 0x0051ab68, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 48, 0x0051ae20, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x0051c81c, 0x0c13b13a }, // PlayerInvincibleTimer_Hook
	{ 48, 0x0051fbbc, 0x0c140364 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x005254e4, 0x0c14d660 }, // PlayerStateUpdate_Hook
	{ 48, 0x00525ac4, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 48, 0x005260bc, 0x0c1223fc }, // PatchDeadShooting_ShootingHook
	{ 48, 0x005271c4, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 48, 0x00529858, 0x0c14a440 }, // PlayerPnetUpdates_Hook
	{ 48, 0x0053712c, 0x0c14ddd0 }, // vaGameTimerEndHookDmNoTeams
	{ 48, 0x00537658, 0x0c13bb40 }, // UpdateScoreboard +0x00000088
	{ 48, 0x0053a2f8, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 48, 0x0053a3e8, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 48, 0x0053a3ec, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 48, 0x0053b244, 0x0c14f718 }, // MapScore_SelectBtn_Addr
	{ 48, 0x0053c2b8, 0x0c14f67c }, // QuickSelectCheck_Hook
	{ 48, 0x0053e384, 0x0c14e596 }, // GadgetEventHook
	{ 48, 0x0053e400, 0x0c14dd74 }, // Juggernaut vaOnPlayerKill_Func
	{ 48, 0x01795ffc, 0x0c0fe17c }, // RespawnTimerFunc_Player
	// Marcadia Palace
	{ 49, 0x00240654, 0x0052bf78 }, // remote bot leave callback
	{ 49, 0x00408958, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 49, 0x00408964, 0x0c114a1e }, // GetSniperShotDirectionHook
	{ 49, 0x00408968, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 49, 0x00409ad8, 0xa2400020 }, // SniperShotCreatedHook
	{ 49, 0x0040a190, 0x0c1108ce }, // SniperShotCollLineFixHook
	{ 49, 0x0040a194, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 49, 0x004278b8, 0x0c120150 }, // vaFlagEventUiPopup_Hook
	{ 49, 0x00437ac8, 0x0c130632 }, // FpsCounter_Hooks
	{ 49, 0x00437b28, 0x0c112d4a }, // FpsCounter_Hooks +0x00000060
	{ 49, 0x004380ac, 0x0c122730 }, // GameTimeUpdate_Hook
	{ 49, 0x0043cf6c, 0x03e00008 }, // cameraRender +0x0000040c
	{ 49, 0x0043e110, 0x0c10f690 }, // cameraRender +0x000015b0
	{ 49, 0x004629c8, 0x0c11b34c }, // vaGameplayHook
	{ 49, 0x0048840c, 0x0c06525c }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 49, 0x004a3d90, 0x0c135bae }, // SetTextureArrow_Hook
	{ 49, 0x004a545c, 0x0c135a32 }, // DM_RespawnUpdater
	{ 49, 0x004a5518, 0x0c117082 }, // DM_PressXToRespawn
	{ 49, 0x004a93b4, 0x03e00008 }, // pause menu callback
	{ 49, 0x004bc280, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 49, 0x004bc284, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 49, 0x004e7d00, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ec810, 0x0c13aeb2 }, // WhoHitMeHook
	{ 49, 0x004ec924, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ecb3c, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ece68, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ed424, 0x0c14dbd4 }, // Juggernaut vaOnPlayerKill_Func
	{ 49, 0x004ed42c, 0x0c0fdae2 }, // RespawnTimerFunc_Player
	{ 49, 0x004ed558, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 49, 0x004ed55c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 49, 0x004ed598, 0x0c13b624 }, // PlayerRespawnFunc +0x00000040
	{ 49, 0x004ed5b4, 0x0c143300 }, // PlayerRespawnFunc +0x0000005c
	{ 49, 0x004faea8, 0x0c115ab2 }, // Healthbars_Hook
	{ 49, 0x00504f24, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 49, 0x00519d90, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x0051a488, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x0051a498, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x0051a4e4, 0x24020027 }, // PostHitInvinc
	{ 49, 0x0051a4e8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 49, 0x0051a7a0, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x0051c19c, 0x0c13af9a }, // PlayerInvincibleTimer_Hook
	{ 49, 0x0051f53c, 0x0c1401c4 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x00524e64, 0x0c14d4c0 }, // PlayerStateUpdate_Hook
	{ 49, 0x00525444, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 49, 0x00525a3c, 0x0c12225c }, // PatchDeadShooting_ShootingHook
	{ 49, 0x00526b44, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 49, 0x005291d8, 0x0c14a2a0 }, // PlayerPnetUpdates_Hook
	{ 49, 0x00536aac, 0x0c14dc30 }, // vaGameTimerEndHookDmNoTeams
	{ 49, 0x00536fd8, 0x0c13b9a0 }, // UpdateScoreboard +0x00000088
	{ 49, 0x00539c78, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 49, 0x00539d68, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 49, 0x00539d6c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 49, 0x0053abc4, 0x0c14f578 }, // MapScore_SelectBtn_Addr
	{ 49, 0x0053bc38, 0x0c14f4dc }, // QuickSelectCheck_Hook
	{ 49, 0x0053dd04, 0x0c14e3f6 }, // GadgetEventHook
	{ 49, 0x0053dd80, 0x0c14dbd4 }, // Juggernaut vaOnPlayerKill_Func
#else
	// Bakisi Isles
	{ 40, 0x002407d4, 0x005366c8 }, // remote bot leave callback
	{ 40, 0x00406d20, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 40, 0x00406d2c, 0x0c117248 }, // GetSniperShotDirectionHook
	{ 40, 0x00406d30, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 40, 0x00407e90, 0xa2400020 }, // SniperShotCreatedHook
	{ 40, 0x00408540, 0x0c1130a4 }, // SniperShotCollLineFixHook
	{ 40, 0x00408544, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 40, 0x00424428, 0x0c145ebc }, // EnableTurretSync_Addr
	{ 40, 0x0042fae0, 0x0c1227be }, // vaFlagEventUiPopup_Hook
	{ 40, 0x00441c88, 0x0c132fc8 }, // FpsCounter_Hooks
	{ 40, 0x00441ce8, 0x0c1154ae }, // FpsCounter_Hooks +0x00000060
	{ 40, 0x0044226c, 0x0c125218 }, // GameTimeUpdate_Hook
	{ 40, 0x00446ec4, 0x03e00008 }, // cameraRender +0x0000040c
	{ 40, 0x00448068, 0x0c111e66 }, // cameraRender +0x000015b0
	{ 40, 0x0046bfb8, 0x0c11d844 }, // vaGameplayHook
	{ 40, 0x004931b4, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 40, 0x004ae5d8, 0x0c138570 }, // SetTextureArrow_Hook
	{ 40, 0x004afca4, 0x0c1383f4 }, // DM_RespawnUpdater
	{ 40, 0x004afd60, 0x0c119842 }, // DM_PressXToRespawn
	{ 40, 0x004b3c34, 0x03e00008 }, // pause menu callback
	{ 40, 0x004c68d8, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 40, 0x004c68dc, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 40, 0x004f2350, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f6e58, 0x0c13d844 }, // WhoHitMeHook
	{ 40, 0x004f6f6c, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f7184, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f74b0, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x004f7a6c, 0x0c1506de }, // Juggernaut vaOnPlayerKill_Func
	{ 40, 0x004f7a74, 0x0c0fd6ea }, // RespawnTimerFunc_Player
	{ 40, 0x004f7ba0, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 40, 0x004f7ba4, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 40, 0x004f7be0, 0x0c13dfb6 }, // PlayerRespawnFunc +0x00000040
	{ 40, 0x004f7bfc, 0x0c145cf2 }, // PlayerRespawnFunc +0x0000005c
	{ 40, 0x00505480, 0x0c11829c }, // Healthbars_Hook
	{ 40, 0x0050f4d4, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 40, 0x005244e0, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00524bd8, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00524be8, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x00524c34, 0x2402002f }, // PostHitInvinc
	{ 40, 0x00524c38, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 40, 0x00524ef0, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x005268ec, 0x0c13d92c }, // PlayerInvincibleTimer_Hook
	{ 40, 0x00529c8c, 0x0c142b3a }, // Juggernaut vaHurtPlayerFunc
	{ 40, 0x0052f5b4, 0x0c14ffca }, // PlayerStateUpdate_Hook
	{ 40, 0x0052fb94, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 40, 0x0053018c, 0x0c124dc6 }, // PatchDeadShooting_ShootingHook
	{ 40, 0x00531294, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 40, 0x00533928, 0x0c14cc74 }, // PlayerPnetUpdates_Hook
	{ 40, 0x005416d4, 0x0c15073a }, // vaGameTimerEndHookDmNoTeams
	{ 40, 0x00541c00, 0x0c13e332 }, // UpdateScoreboard +0x00000088
	{ 40, 0x005448a8, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 40, 0x00544998, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 40, 0x0054499c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 40, 0x005457f4, 0x0c15205c }, // MapScore_SelectBtn_Addr
	{ 40, 0x00546818, 0x0c151fc0 }, // QuickSelectCheck_Hook
	{ 40, 0x00548894, 0x0c150f00 }, // GadgetEventHook
	{ 40, 0x00548910, 0x0c1506de }, // Juggernaut vaOnPlayerKill_Func
	// Hoven Gorge
	{ 41, 0x002407d4, 0x00538720 }, // remote bot leave callback
	{ 41, 0x00406608, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 41, 0x00406614, 0x0c117904 }, // GetSniperShotDirectionHook
	{ 41, 0x00406618, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 41, 0x00407778, 0xa2400020 }, // SniperShotCreatedHook
	{ 41, 0x00407e28, 0x0c113754 }, // SniperShotCollLineFixHook
	{ 41, 0x00407e2c, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 41, 0x00425db8, 0x0c1466d2 }, // EnableTurretSync_Addr
	{ 41, 0x00431470, 0x0c122f06 }, // vaFlagEventUiPopup_Hook
	{ 41, 0x00443748, 0x0c1337de }, // FpsCounter_Hooks
	{ 41, 0x004437a8, 0x0c115b5e }, // FpsCounter_Hooks +0x00000060
	{ 41, 0x00443d2c, 0x0c125a2e }, // GameTimeUpdate_Hook
	{ 41, 0x00448984, 0x03e00008 }, // cameraRender +0x0000040c
	{ 41, 0x00449b28, 0x0c112516 }, // cameraRender +0x000015b0
	{ 41, 0x0046daa8, 0x0c11df04 }, // vaGameplayHook
	{ 41, 0x0049520c, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 41, 0x004b0630, 0x0c138d86 }, // SetTextureArrow_Hook
	{ 41, 0x004b1cfc, 0x0c138c0a }, // DM_RespawnUpdater
	{ 41, 0x004b1db8, 0x0c119efe }, // DM_PressXToRespawn
	{ 41, 0x004b5c8c, 0x03e00008 }, // pause menu callback
	{ 41, 0x004c8930, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 41, 0x004c8934, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 41, 0x004f43a8, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004f8eb0, 0x0c13e05a }, // WhoHitMeHook
	{ 41, 0x004f8fc4, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004f91dc, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004f9508, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x004f9ac4, 0x0c150f20 }, // Juggernaut vaOnPlayerKill_Func
	{ 41, 0x004f9acc, 0x0c0fd26c }, // RespawnTimerFunc_Player
	{ 41, 0x004f9bf8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 41, 0x004f9bfc, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 41, 0x004f9c38, 0x0c13e7cc }, // PlayerRespawnFunc +0x00000040
	{ 41, 0x004f9c54, 0x0c146508 }, // PlayerRespawnFunc +0x0000005c
	{ 41, 0x005074d8, 0x0c118958 }, // Healthbars_Hook
	{ 41, 0x0051152c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 41, 0x00526538, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00526c30, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00526c40, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00526c8c, 0x2402002f }, // PostHitInvinc
	{ 41, 0x00526c90, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 41, 0x00526f48, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x00528944, 0x0c13e142 }, // PlayerInvincibleTimer_Hook
	{ 41, 0x0052bce4, 0x0c143350 }, // Juggernaut vaHurtPlayerFunc
	{ 41, 0x0053160c, 0x0c15080c }, // PlayerStateUpdate_Hook
	{ 41, 0x00531bec, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 41, 0x005321e4, 0x0c1255dc }, // PatchDeadShooting_ShootingHook
	{ 41, 0x005332ec, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 41, 0x00535980, 0x0c14d48a }, // PlayerPnetUpdates_Hook
	{ 41, 0x005437dc, 0x0c150f7c }, // vaGameTimerEndHookDmNoTeams
	{ 41, 0x00543d08, 0x0c13eb48 }, // UpdateScoreboard +0x00000088
	{ 41, 0x005469b0, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 41, 0x00546aa0, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 41, 0x00546aa4, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 41, 0x005478fc, 0x0c15289e }, // MapScore_SelectBtn_Addr
	{ 41, 0x00548920, 0x0c152802 }, // QuickSelectCheck_Hook
	{ 41, 0x0054a99c, 0x0c151742 }, // GadgetEventHook
	{ 41, 0x0054aa18, 0x0c150f20 }, // Juggernaut vaOnPlayerKill_Func
	// Outpost X12
	{ 42, 0x002407d4, 0x0052e038 }, // remote bot leave callback
	{ 42, 0x003fe500, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 42, 0x003fe50c, 0x0c115494 }, // GetSniperShotDirectionHook
	{ 42, 0x003fe510, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 42, 0x003ff670, 0xa2400020 }, // SniperShotCreatedHook
	{ 42, 0x003ffd20, 0x0c1112e4 }, // SniperShotCollLineFixHook
	{ 42, 0x003ffd24, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 42, 0x0041cd00, 0x0c143d18 }, // EnableTurretSync_Addr
	{ 42, 0x004283b8, 0x0c120a96 }, // vaFlagEventUiPopup_Hook
	{ 42, 0x0043a588, 0x0c130e24 }, // FpsCounter_Hooks
	{ 42, 0x0043a5e8, 0x0c1136ee }, // FpsCounter_Hooks +0x00000060
	{ 42, 0x0043ab6c, 0x0c123074 }, // GameTimeUpdate_Hook
	{ 42, 0x0043f7c4, 0x03e00008 }, // cameraRender +0x0000040c
	{ 42, 0x00440968, 0x0c1100a6 }, // cameraRender +0x000015b0
	{ 42, 0x004648e8, 0x0c11ba94 }, // vaGameplayHook
	{ 42, 0x0048ab24, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 42, 0x004a5f48, 0x0c1363cc }, // SetTextureArrow_Hook
	{ 42, 0x004a7614, 0x0c136250 }, // DM_RespawnUpdater
	{ 42, 0x004a76d0, 0x0c117a8e }, // DM_PressXToRespawn
	{ 42, 0x004ab5a4, 0x03e00008 }, // pause menu callback
	{ 42, 0x004be248, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 42, 0x004be24c, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 42, 0x004e9cc0, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004ee7c8, 0x0c13b6a0 }, // WhoHitMeHook
	{ 42, 0x004ee8dc, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004eeaf4, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004eee20, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x004ef3dc, 0x0c14e566 }, // Juggernaut vaOnPlayerKill_Func
	{ 42, 0x004ef3e4, 0x0c0fb22a }, // RespawnTimerFunc_Player
	{ 42, 0x004ef510, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 42, 0x004ef514, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 42, 0x004ef550, 0x0c13be12 }, // PlayerRespawnFunc +0x00000040
	{ 42, 0x004ef56c, 0x0c143b4e }, // PlayerRespawnFunc +0x0000005c
	{ 42, 0x004fcdf0, 0x0c1164e8 }, // Healthbars_Hook
	{ 42, 0x00506e44, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 42, 0x0051be50, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051c548, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051c558, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051c5a4, 0x2402002f }, // PostHitInvinc
	{ 42, 0x0051c5a8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 42, 0x0051c860, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x0051e25c, 0x0c13b788 }, // PlayerInvincibleTimer_Hook
	{ 42, 0x005215fc, 0x0c140996 }, // Juggernaut vaHurtPlayerFunc
	{ 42, 0x00526f24, 0x0c14de52 }, // PlayerStateUpdate_Hook
	{ 42, 0x00527504, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 42, 0x00527afc, 0x0c122c22 }, // PatchDeadShooting_ShootingHook
	{ 42, 0x00528c04, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 42, 0x0052b298, 0x0c14aad0 }, // PlayerPnetUpdates_Hook
	{ 42, 0x005390f4, 0x0c14e5c2 }, // vaGameTimerEndHookDmNoTeams
	{ 42, 0x00539620, 0x0c13c18e }, // UpdateScoreboard +0x00000088
	{ 42, 0x0053c2c8, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 42, 0x0053c3b8, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 42, 0x0053c3bc, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 42, 0x0053d214, 0x0c14fee4 }, // MapScore_SelectBtn_Addr
	{ 42, 0x0053e238, 0x0c14fe48 }, // QuickSelectCheck_Hook
	{ 42, 0x005402b4, 0x0c14ed88 }, // GadgetEventHook
	{ 42, 0x00540330, 0x0c14e566 }, // Juggernaut vaOnPlayerKill_Func
	// Korgon Outpost
	{ 43, 0x002407d4, 0x0052b850 }, // remote bot leave callback
	{ 43, 0x003fd940, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 43, 0x003fd94c, 0x0c114b98 }, // GetSniperShotDirectionHook
	{ 43, 0x003fd950, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 43, 0x003feab0, 0xa2400020 }, // SniperShotCreatedHook
	{ 43, 0x003ff160, 0x0c1109f4 }, // SniperShotCollLineFixHook
	{ 43, 0x003ff164, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 43, 0x0041a418, 0x0c14331e }, // EnableTurretSync_Addr
	{ 43, 0x00426008, 0x0c12010e }, // vaFlagEventUiPopup_Hook
	{ 43, 0x004381c8, 0x0c13042a }, // FpsCounter_Hooks
	{ 43, 0x00438228, 0x0c112dfe }, // FpsCounter_Hooks +0x00000060
	{ 43, 0x004387ac, 0x0c122648 }, // GameTimeUpdate_Hook
	{ 43, 0x0043d404, 0x03e00008 }, // cameraRender +0x0000040c
	{ 43, 0x0043e5a8, 0x0c10f7b6 }, // cameraRender +0x000015b0
	{ 43, 0x004624f8, 0x0c11b194 }, // vaGameplayHook
	{ 43, 0x00488274, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 43, 0x004a3760, 0x0c1359d2 }, // SetTextureArrow_Hook
	{ 43, 0x004a4e2c, 0x0c135856 }, // DM_RespawnUpdater
	{ 43, 0x004a4ee8, 0x0c117192 }, // DM_PressXToRespawn
	{ 43, 0x004a8dbc, 0x03e00008 }, // pause menu callback
	{ 43, 0x004bba60, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 43, 0x004bba64, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 43, 0x004e74d8, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ebfe0, 0x0c13aca6 }, // WhoHitMeHook
	{ 43, 0x004ec0f4, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ec30c, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ec638, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x004ecbf4, 0x0c14db40 }, // Juggernaut vaOnPlayerKill_Func
	{ 43, 0x004ecbfc, 0x0c0fb1f2 }, // RespawnTimerFunc_Player
	{ 43, 0x004ecd28, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 43, 0x004ecd2c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 43, 0x004ecd68, 0x0c13b418 }, // PlayerRespawnFunc +0x00000040
	{ 43, 0x004ecd84, 0x0c143154 }, // PlayerRespawnFunc +0x0000005c
	{ 43, 0x004fa608, 0x0c115bec }, // Healthbars_Hook
	{ 43, 0x0050465c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 43, 0x00519668, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x00519d60, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x00519d70, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x00519dbc, 0x2402002f }, // PostHitInvinc
	{ 43, 0x00519dc0, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 43, 0x0051a078, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0051ba74, 0x0c13ad8e }, // PlayerInvincibleTimer_Hook
	{ 43, 0x0051ee14, 0x0c13ff9c }, // Juggernaut vaHurtPlayerFunc
	{ 43, 0x0052473c, 0x0c14d42c }, // PlayerStateUpdate_Hook
	{ 43, 0x00524d1c, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 43, 0x00525314, 0x0c1221f6 }, // PatchDeadShooting_ShootingHook
	{ 43, 0x0052641c, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 43, 0x00528ab0, 0x0c14a0d6 }, // PlayerPnetUpdates_Hook
	{ 43, 0x0053685c, 0x0c14db9c }, // vaGameTimerEndHookDmNoTeams
	{ 43, 0x00536d88, 0x0c13b794 }, // UpdateScoreboard +0x00000088
	{ 43, 0x00539a30, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 43, 0x00539b20, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 43, 0x00539b24, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 43, 0x0053a97c, 0x0c14f4be }, // MapScore_SelectBtn_Addr
	{ 43, 0x0053b9a0, 0x0c14f422 }, // QuickSelectCheck_Hook
	{ 43, 0x0053da1c, 0x0c14e362 }, // GadgetEventHook
	{ 43, 0x0053da98, 0x0c14db40 }, // Juggernaut vaOnPlayerKill_Func
	// Metropolis
	{ 44, 0x002407d4, 0x0052aba0 }, // remote bot leave callback
	{ 44, 0x003fc520, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 44, 0x003fc52c, 0x0c114868 }, // GetSniperShotDirectionHook
	{ 44, 0x003fc530, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 44, 0x003fd690, 0xa2400020 }, // SniperShotCreatedHook
	{ 44, 0x003fdd40, 0x0c1106c4 }, // SniperShotCollLineFixHook
	{ 44, 0x003fdd44, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 44, 0x00419cb0, 0x0c142ff2 }, // EnableTurretSync_Addr
	{ 44, 0x00425368, 0x0c11fe14 }, // vaFlagEventUiPopup_Hook
	{ 44, 0x00437508, 0x0c1300fe }, // FpsCounter_Hooks
	{ 44, 0x00437568, 0x0c112ace }, // FpsCounter_Hooks +0x00000060
	{ 44, 0x00437aec, 0x0c12234e }, // GameTimeUpdate_Hook
	{ 44, 0x0043c744, 0x03e00008 }, // cameraRender +0x0000040c
	{ 44, 0x0043d8e8, 0x0c10f486 }, // cameraRender +0x000015b0
	{ 44, 0x00461838, 0x0c11ae64 }, // vaGameplayHook
	{ 44, 0x0048768c, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 44, 0x004a2ab0, 0x0c1356a6 }, // SetTextureArrow_Hook
	{ 44, 0x004a417c, 0x0c13552a }, // DM_RespawnUpdater
	{ 44, 0x004a4238, 0x0c116e62 }, // DM_PressXToRespawn
	{ 44, 0x004a810c, 0x03e00008 }, // pause menu callback
	{ 44, 0x004badb0, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 44, 0x004badb4, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 44, 0x004e6828, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004eb330, 0x0c13a97a }, // WhoHitMeHook
	{ 44, 0x004eb444, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004eb65c, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004eb988, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x004ebf44, 0x0c14d840 }, // Juggernaut vaOnPlayerKill_Func
	{ 44, 0x004ebf4c, 0x0c0faa32 }, // RespawnTimerFunc_Player
	{ 44, 0x004ec078, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 44, 0x004ec07c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 44, 0x004ec0b8, 0x0c13b0ec }, // PlayerRespawnFunc +0x00000040
	{ 44, 0x004ec0d4, 0x0c142e28 }, // PlayerRespawnFunc +0x0000005c
	{ 44, 0x004f9958, 0x0c1158bc }, // Healthbars_Hook
	{ 44, 0x005039ac, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 44, 0x005189b8, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x005190b0, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x005190c0, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051910c, 0x2402002f }, // PostHitInvinc
	{ 44, 0x00519110, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 44, 0x005193c8, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x0051adc4, 0x0c13aa62 }, // PlayerInvincibleTimer_Hook
	{ 44, 0x0051e164, 0x0c13fc70 }, // Juggernaut vaHurtPlayerFunc
	{ 44, 0x00523a8c, 0x0c14d12c }, // PlayerStateUpdate_Hook
	{ 44, 0x0052406c, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 44, 0x00524664, 0x0c121efc }, // PatchDeadShooting_ShootingHook
	{ 44, 0x0052576c, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 44, 0x00527e00, 0x0c149daa }, // PlayerPnetUpdates_Hook
	{ 44, 0x00535c5c, 0x0c14d89c }, // vaGameTimerEndHookDmNoTeams
	{ 44, 0x00536188, 0x0c13b468 }, // UpdateScoreboard +0x00000088
	{ 44, 0x00538e30, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 44, 0x00538f20, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 44, 0x00538f24, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 44, 0x00539d7c, 0x0c14f1be }, // MapScore_SelectBtn_Addr
	{ 44, 0x0053ada0, 0x0c14f122 }, // QuickSelectCheck_Hook
	{ 44, 0x0053ce1c, 0x0c14e062 }, // GadgetEventHook
	{ 44, 0x0053ce98, 0x0c14d840 }, // Juggernaut vaOnPlayerKill_Func
	// Blackwater City
	{ 45, 0x002407d4, 0x005283b8 }, // remote bot leave callback
	{ 45, 0x003f92a8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 45, 0x003f92b4, 0x0c113e54 }, // GetSniperShotDirectionHook
	{ 45, 0x003f92b8, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 45, 0x003fa418, 0xa2400020 }, // SniperShotCreatedHook
	{ 45, 0x003faac8, 0x0c10fca4 }, // SniperShotCollLineFixHook
	{ 45, 0x003faacc, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 45, 0x00415ad0, 0x0c1425f8 }, // EnableTurretSync_Addr
	{ 45, 0x00421188, 0x0c11f37e }, // vaFlagEventUiPopup_Hook
	{ 45, 0x00434c88, 0x0c12f704 }, // FpsCounter_Hooks
	{ 45, 0x00434ce8, 0x0c1120ae }, // FpsCounter_Hooks +0x00000060
	{ 45, 0x0043526c, 0x0c121954 }, // GameTimeUpdate_Hook
	{ 45, 0x00439ec4, 0x03e00008 }, // cameraRender +0x0000040c
	{ 45, 0x0043b068, 0x0c10ea66 }, // cameraRender +0x000015b0
	{ 45, 0x0045efe8, 0x0c11a454 }, // vaGameplayHook
	{ 45, 0x00484ea4, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 45, 0x004a02c8, 0x0c134cac }, // SetTextureArrow_Hook
	{ 45, 0x004a1994, 0x0c134b30 }, // DM_RespawnUpdater
	{ 45, 0x004a1a50, 0x0c11644e }, // DM_PressXToRespawn
	{ 45, 0x004a5924, 0x03e00008 }, // pause menu callback
	{ 45, 0x004b85c8, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 45, 0x004b85cc, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 45, 0x004e4040, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004e8b48, 0x0c139f80 }, // WhoHitMeHook
	{ 45, 0x004e8c5c, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004e8e74, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004e91a0, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x004e975c, 0x0c14ce1a }, // Juggernaut vaOnPlayerKill_Func
	{ 45, 0x004e9764, 0x0c0f9d94 }, // RespawnTimerFunc_Player
	{ 45, 0x004e9890, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 45, 0x004e9894, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 45, 0x004e98d0, 0x0c13a6f2 }, // PlayerRespawnFunc +0x00000040
	{ 45, 0x004e98ec, 0x0c14242e }, // PlayerRespawnFunc +0x0000005c
	{ 45, 0x004f7170, 0x0c114ea8 }, // Healthbars_Hook
	{ 45, 0x005011c4, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 45, 0x005161d0, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005168c8, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005168d8, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x00516924, 0x2402002f }, // PostHitInvinc
	{ 45, 0x00516928, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 45, 0x00516be0, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005185dc, 0x0c13a068 }, // PlayerInvincibleTimer_Hook
	{ 45, 0x0051b97c, 0x0c13f276 }, // Juggernaut vaHurtPlayerFunc
	{ 45, 0x005212a4, 0x0c14c706 }, // PlayerStateUpdate_Hook
	{ 45, 0x00521884, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 45, 0x00521e7c, 0x0c121502 }, // PatchDeadShooting_ShootingHook
	{ 45, 0x00522f84, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 45, 0x00525618, 0x0c1493b0 }, // PlayerPnetUpdates_Hook
	{ 45, 0x005333c4, 0x0c14ce76 }, // vaGameTimerEndHookDmNoTeams
	{ 45, 0x005338f0, 0x0c13aa6e }, // UpdateScoreboard +0x00000088
	{ 45, 0x00536598, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 45, 0x00536688, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 45, 0x0053668c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 45, 0x005374e4, 0x0c14e798 }, // MapScore_SelectBtn_Addr
	{ 45, 0x00538508, 0x0c14e6fc }, // QuickSelectCheck_Hook
	{ 45, 0x0053a584, 0x0c14d63c }, // GadgetEventHook
	{ 45, 0x0053a600, 0x0c14ce1a }, // Juggernaut vaOnPlayerKill_Func
	// Command Center
	{ 46, 0x002407d4, 0x00528338 }, // remote bot leave callback
	{ 46, 0x004076f8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 46, 0x00407704, 0x0c114102 }, // GetSniperShotDirectionHook
	{ 46, 0x00407708, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 46, 0x00408868, 0xa2400020 }, // SniperShotCreatedHook
	{ 46, 0x00408f18, 0x0c110034 }, // SniperShotCollLineFixHook
	{ 46, 0x00408f1c, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 46, 0x00425a38, 0x0c11f48e }, // vaFlagEventUiPopup_Hook
	{ 46, 0x00435ac8, 0x0c12f772 }, // FpsCounter_Hooks
	{ 46, 0x00435b28, 0x0c11243e }, // FpsCounter_Hooks +0x00000060
	{ 46, 0x004360ac, 0x0c1219c2 }, // GameTimeUpdate_Hook
	{ 46, 0x0043ad04, 0x03e00008 }, // cameraRender +0x0000040c
	{ 46, 0x0043bea8, 0x0c10edf6 }, // cameraRender +0x000015b0
	{ 46, 0x0045faa0, 0x0c11a6f4 }, // vaGameplayHook
	{ 46, 0x0048505c, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 46, 0x004a0480, 0x0c134d0e }, // SetTextureArrow_Hook
	{ 46, 0x004a1b4c, 0x0c134b92 }, // DM_RespawnUpdater
	{ 46, 0x004a1c08, 0x0c1166fc }, // DM_PressXToRespawn
	{ 46, 0x004a5adc, 0x03e00008 }, // pause menu callback
	{ 46, 0x004b8780, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 46, 0x004b8784, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 46, 0x004e41c8, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004e8cd0, 0x0c139fe2 }, // WhoHitMeHook
	{ 46, 0x004e8de4, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004e8ffc, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004e9328, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x004e98e4, 0x0c14cca6 }, // Juggernaut vaOnPlayerKill_Func
	{ 46, 0x004e98ec, 0x0c0fd960 }, // RespawnTimerFunc_Player
	{ 46, 0x004e9a18, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 46, 0x004e9a1c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 46, 0x004e9a58, 0x0c13a754 }, // PlayerRespawnFunc +0x00000040
	{ 46, 0x004e9a74, 0x0c14240e }, // PlayerRespawnFunc +0x0000005c
	{ 46, 0x004f72f8, 0x0c115156 }, // Healthbars_Hook
	{ 46, 0x0050134c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 46, 0x00516150, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00516848, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00516858, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x005168a4, 0x2402002f }, // PostHitInvinc
	{ 46, 0x005168a8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 46, 0x00516b60, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x0051855c, 0x0c13a0ca }, // PlayerInvincibleTimer_Hook
	{ 46, 0x0051b8fc, 0x0c13f2d8 }, // Juggernaut vaHurtPlayerFunc
	{ 46, 0x00521224, 0x0c14c592 }, // PlayerStateUpdate_Hook
	{ 46, 0x00521804, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 46, 0x00521dfc, 0x0c121570 }, // PatchDeadShooting_ShootingHook
	{ 46, 0x00522f04, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 46, 0x00525598, 0x0c149390 }, // PlayerPnetUpdates_Hook
	{ 46, 0x00532df4, 0x0c14cd02 }, // vaGameTimerEndHookDmNoTeams
	{ 46, 0x00533320, 0x0c13aad0 }, // UpdateScoreboard +0x00000088
	{ 46, 0x00535fc8, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 46, 0x005360b8, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 46, 0x005360bc, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 46, 0x00536f14, 0x0c14e624 }, // MapScore_SelectBtn_Addr
	{ 46, 0x00537f38, 0x0c14e588 }, // QuickSelectCheck_Hook
	{ 46, 0x00539fb4, 0x0c14d4c8 }, // GadgetEventHook
	{ 46, 0x0053a030, 0x0c14cca6 }, // Juggernaut vaOnPlayerKill_Func
	// Blackwater Docks
	{ 47, 0x002407d4, 0x0052ab78 }, // remote bot leave callback
	{ 47, 0x00409658, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 47, 0x00409664, 0x0c114b12 }, // GetSniperShotDirectionHook
	{ 47, 0x00409668, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 47, 0x0040a7c8, 0xa2400020 }, // SniperShotCreatedHook
	{ 47, 0x0040ae78, 0x0c110a44 }, // SniperShotCollLineFixHook
	{ 47, 0x0040ae7c, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 47, 0x00428270, 0x0c11fe9e }, // vaFlagEventUiPopup_Hook
	{ 47, 0x00438308, 0x0c130182 }, // FpsCounter_Hooks
	{ 47, 0x00438368, 0x0c112e4e }, // FpsCounter_Hooks +0x00000060
	{ 47, 0x004388ec, 0x0c1223d2 }, // GameTimeUpdate_Hook
	{ 47, 0x0043d544, 0x03e00008 }, // cameraRender +0x0000040c
	{ 47, 0x0043e6e8, 0x0c10f806 }, // cameraRender +0x000015b0
	{ 47, 0x004622e0, 0x0c11b104 }, // vaGameplayHook
	{ 47, 0x0048789c, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 47, 0x004a2cc0, 0x0c13571e }, // SetTextureArrow_Hook
	{ 47, 0x004a438c, 0x0c1355a2 }, // DM_RespawnUpdater
	{ 47, 0x004a4448, 0x0c11710c }, // DM_PressXToRespawn
	{ 47, 0x004a831c, 0x03e00008 }, // pause menu callback
	{ 47, 0x004bafc0, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 47, 0x004bafc4, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 47, 0x004e6a08, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004eb510, 0x0c13a9f2 }, // WhoHitMeHook
	{ 47, 0x004eb624, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004eb83c, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004ebb68, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x004ec124, 0x0c14d6b6 }, // Juggernaut vaOnPlayerKill_Func
	{ 47, 0x004ec12c, 0x0c0fe138 }, // RespawnTimerFunc_Player
	{ 47, 0x004ec258, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 47, 0x004ec25c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 47, 0x004ec298, 0x0c13b164 }, // PlayerRespawnFunc +0x00000040
	{ 47, 0x004ec2b4, 0x0c142e1e }, // PlayerRespawnFunc +0x0000005c
	{ 47, 0x004f9b38, 0x0c115b66 }, // Healthbars_Hook
	{ 47, 0x00503b8c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 47, 0x00518990, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x00519088, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x00519098, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x005190e4, 0x2402002f }, // PostHitInvinc
	{ 47, 0x005190e8, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 47, 0x005193a0, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x0051ad9c, 0x0c13aada }, // PlayerInvincibleTimer_Hook
	{ 47, 0x0051e13c, 0x0c13fce8 }, // Juggernaut vaHurtPlayerFunc
	{ 47, 0x00523a64, 0x0c14cfa2 }, // PlayerStateUpdate_Hook
	{ 47, 0x00524044, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 47, 0x0052463c, 0x0c121f80 }, // PatchDeadShooting_ShootingHook
	{ 47, 0x00525744, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 47, 0x00527dd8, 0x0c149da0 }, // PlayerPnetUpdates_Hook
	{ 47, 0x00535634, 0x0c14d712 }, // vaGameTimerEndHookDmNoTeams
	{ 47, 0x00535b60, 0x0c13b4e0 }, // UpdateScoreboard +0x00000088
	{ 47, 0x00538808, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 47, 0x005388f8, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 47, 0x005388fc, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 47, 0x00539754, 0x0c14f034 }, // MapScore_SelectBtn_Addr
	{ 47, 0x0053a778, 0x0c14ef98 }, // QuickSelectCheck_Hook
	{ 47, 0x0053c7f4, 0x0c14ded8 }, // GadgetEventHook
	{ 47, 0x0053c870, 0x0c14d6b6 }, // Juggernaut vaOnPlayerKill_Func
	// Aquatos Sewers
	{ 48, 0x002407d4, 0x00529eb8 }, // remote bot leave callback
	{ 48, 0x00409260, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 48, 0x0040926c, 0x0c1147e2 }, // GetSniperShotDirectionHook
	{ 48, 0x00409270, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 48, 0x0040a3d0, 0xa2400020 }, // SniperShotCreatedHook
	{ 48, 0x0040aa80, 0x0c110714 }, // SniperShotCollLineFixHook
	{ 48, 0x0040aa84, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 48, 0x004275a0, 0x0c11fb6e }, // vaFlagEventUiPopup_Hook
	{ 48, 0x00437648, 0x0c12fe52 }, // FpsCounter_Hooks
	{ 48, 0x004376a8, 0x0c112b1e }, // FpsCounter_Hooks +0x00000060
	{ 48, 0x00437c2c, 0x0c1220a2 }, // GameTimeUpdate_Hook
	{ 48, 0x0043c884, 0x03e00008 }, // cameraRender +0x0000040c
	{ 48, 0x0043da28, 0x0c10f4d6 }, // cameraRender +0x000015b0
	{ 48, 0x00461620, 0x0c11add4 }, // vaGameplayHook
	{ 48, 0x00486bdc, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 48, 0x004a2000, 0x0c1353ee }, // SetTextureArrow_Hook
	{ 48, 0x004a36cc, 0x0c135272 }, // DM_RespawnUpdater
	{ 48, 0x004a3788, 0x0c116ddc }, // DM_PressXToRespawn
	{ 48, 0x004a765c, 0x03e00008 }, // pause menu callback
	{ 48, 0x004ba300, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 48, 0x004ba304, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 48, 0x004e5d48, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004ea850, 0x0c13a6c2 }, // WhoHitMeHook
	{ 48, 0x004ea964, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004eab7c, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004eaea8, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x004eb464, 0x0c14d386 }, // Juggernaut vaOnPlayerKill_Func
	{ 48, 0x004eb46c, 0x0c0fe03a }, // RespawnTimerFunc_Player
	{ 48, 0x004eb598, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 48, 0x004eb59c, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 48, 0x004eb5d8, 0x0c13ae34 }, // PlayerRespawnFunc +0x00000040
	{ 48, 0x004eb5f4, 0x0c142aee }, // PlayerRespawnFunc +0x0000005c
	{ 48, 0x004f8e78, 0x0c115836 }, // Healthbars_Hook
	{ 48, 0x00502ecc, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 48, 0x00517cd0, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x005183c8, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x005183d8, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x00518424, 0x2402002f }, // PostHitInvinc
	{ 48, 0x00518428, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 48, 0x005186e0, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x0051a0dc, 0x0c13a7aa }, // PlayerInvincibleTimer_Hook
	{ 48, 0x0051d47c, 0x0c13f9b8 }, // Juggernaut vaHurtPlayerFunc
	{ 48, 0x00522da4, 0x0c14cc72 }, // PlayerStateUpdate_Hook
	{ 48, 0x00523384, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 48, 0x0052397c, 0x0c121c50 }, // PatchDeadShooting_ShootingHook
	{ 48, 0x00524a84, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 48, 0x00527118, 0x0c149a70 }, // PlayerPnetUpdates_Hook
	{ 48, 0x00534974, 0x0c14d3e2 }, // vaGameTimerEndHookDmNoTeams
	{ 48, 0x00534ea0, 0x0c13b1b0 }, // UpdateScoreboard +0x00000088
	{ 48, 0x00537b48, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 48, 0x00537c38, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 48, 0x00537c3c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 48, 0x00538a94, 0x0c14ed04 }, // MapScore_SelectBtn_Addr
	{ 48, 0x00539ab8, 0x0c14ec68 }, // QuickSelectCheck_Hook
	{ 48, 0x0053bb34, 0x0c14dba8 }, // GadgetEventHook
	{ 48, 0x0053bbb0, 0x0c14d386 }, // Juggernaut vaOnPlayerKill_Func
	// Marcadia Palace
	{ 49, 0x002407d4, 0x005297f8 }, // remote bot leave callback
	{ 49, 0x004082d8, 0x44816000 }, // GetSniperShotDirectionHook -0x0000000c
	{ 49, 0x004082e4, 0x0c114632 }, // GetSniperShotDirectionHook
	{ 49, 0x004082e8, 0x460c0302 }, // GetSniperShotDirectionHook +0x00000004
	{ 49, 0x00409448, 0xa2400020 }, // SniperShotCreatedHook
	{ 49, 0x00409af8, 0x0c110564 }, // SniperShotCollLineFixHook
	{ 49, 0x00409afc, 0x0000302d }, // SniperShotCollLineFixHook +0x00000004
	{ 49, 0x00426ef0, 0x0c11f9be }, // vaFlagEventUiPopup_Hook
	{ 49, 0x00436f88, 0x0c12fca2 }, // FpsCounter_Hooks
	{ 49, 0x00436fe8, 0x0c11296e }, // FpsCounter_Hooks +0x00000060
	{ 49, 0x0043756c, 0x0c121ef2 }, // GameTimeUpdate_Hook
	{ 49, 0x0043c1c4, 0x03e00008 }, // cameraRender +0x0000040c
	{ 49, 0x0043d368, 0x0c10f326 }, // cameraRender +0x000015b0
	{ 49, 0x00460f60, 0x0c11ac24 }, // vaGameplayHook
	{ 49, 0x0048651c, 0x0c065298 }, // LOAD_LEVEL_RADAR_MAP_HOOK
	{ 49, 0x004a1940, 0x0c13523e }, // SetTextureArrow_Hook
	{ 49, 0x004a300c, 0x0c1350c2 }, // DM_RespawnUpdater
	{ 49, 0x004a30c8, 0x0c116c2c }, // DM_PressXToRespawn
	{ 49, 0x004a6f9c, 0x03e00008 }, // pause menu callback
	{ 49, 0x004b9c40, 0x02c3b020 }, // LevelOfDetail_Hook
	{ 49, 0x004b9c44, 0x000d6842 }, // LevelOfDetail_Hook +0x00000004
	{ 49, 0x004e5688, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ea190, 0x0c13a512 }, // WhoHitMeHook
	{ 49, 0x004ea2a4, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ea4bc, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004ea7e8, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x004eada4, 0x0c14d1d6 }, // Juggernaut vaOnPlayerKill_Func
	{ 49, 0x004eadac, 0x0c0fd9a0 }, // RespawnTimerFunc_Player
	{ 49, 0x004eaed8, 0x27bdffb0 }, // PlayerRespawnFunc
	{ 49, 0x004eaedc, 0xffb20040 }, // PlayerRespawnFunc +0x00000004
	{ 49, 0x004eaf18, 0x0c13ac84 }, // PlayerRespawnFunc +0x00000040
	{ 49, 0x004eaf34, 0x0c14293e }, // PlayerRespawnFunc +0x0000005c
	{ 49, 0x004f87b8, 0x0c115686 }, // Healthbars_Hook
	{ 49, 0x0050280c, 0x504001c5 }, // ForceGadgetMobyCreation_Addr
	{ 49, 0x00517610, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x00517d08, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x00517d18, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x00517d64, 0x2402002f }, // PostHitInvinc
	{ 49, 0x00517d68, 0x01252818 }, // PostHitInvinc +0x00000004
	{ 49, 0x00518020, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x00519a1c, 0x0c13a5fa }, // PlayerInvincibleTimer_Hook
	{ 49, 0x0051cdbc, 0x0c13f808 }, // Juggernaut vaHurtPlayerFunc
	{ 49, 0x005226e4, 0x0c14cac2 }, // PlayerStateUpdate_Hook
	{ 49, 0x00522cc4, 0xac430048 }, // TNW_PlayerData_GadgetIdUpdate
	{ 49, 0x005232bc, 0x0c121aa0 }, // PatchDeadShooting_ShootingHook
	{ 49, 0x005243c4, 0xad22004c }, // TNW_PlayerData_TimeUpdate
	{ 49, 0x00526a58, 0x0c1498c0 }, // PlayerPnetUpdates_Hook
	{ 49, 0x005342b4, 0x0c14d232 }, // vaGameTimerEndHookDmNoTeams
	{ 49, 0x005347e0, 0x0c13b000 }, // UpdateScoreboard +0x00000088
	{ 49, 0x00537488, 0x03e00008 }, // GiveWeaponFunc +0x00000538
	{ 49, 0x00537578, 0x3c020001 }, // GiveWeaponUpgradeFunc
	{ 49, 0x0053757c, 0x90830001 }, // GiveWeaponUpgradeFunc +0x00000004
	{ 49, 0x005383d4, 0x0c14eb54 }, // MapScore_SelectBtn_Addr
	{ 49, 0x005393f8, 0x0c14eab8 }, // QuickSelectCheck_Hook
	{ 49, 0x0053b474, 0x0c14d9f8 }, // GadgetEventHook
	{ 49, 0x0053b4f0, 0x0c14d1d6 }, // Juggernaut vaOnPlayerKill_Func
#endif

	// patch (PAL and NTSC are same)
	{ -1, 0x00153248, 0x03e00008 }, // GET_MEDIUS_APP_HANDLER_HOOK
	{ -1, 0x0013cae0, 0x0c0474e0 }, // ScePadRead_memcpy Hook
};

const int clears[][2] = {
	{ 0x000d0000, 0x00020000 }, // usb modules
	{ 0x000f0000, 0x00010000 }, // game mode
	{ 0x000cf000, 0x00000800 }, // module definitions
	{ 0x000cffd0, 0x00000020 }, // patch hash
	{ 0x000cffc0, 0x00000010 }, // patch pointers
};

int hasClearedMemory = 0;
int bytesReceived = 0;
int totalBytes = 0;

/*
 * NAME : onServerDownloadDataRequest
 * DESCRIPTION : Copy patch chunks and publish their code before acknowledging completion.
 * NOTES :
 * ARGS : Native connection and download packet.
 * RETURN : Number of consumed packet bytes.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
int onServerDownloadDataRequest(void * connection, void * data)
{
	ServerDownloadDataRequest_t* request = (ServerDownloadDataRequest_t*)data;

	// copy bytes to target
	totalBytes = request->TotalSize;
	bytesReceived += request->DataSize;
	memcpy((void*)request->TargetAddress, request->Data, request->DataSize);
	DPRINTF("DOWNLOAD: {%d} %d/%d, writing %d to %08X\n", request->Id, bytesReceived, request->TotalSize, request->DataSize, request->TargetAddress);

	// Publish downloaded instructions before the server activates the patch.
	if (bytesReceived >= totalBytes)
	{
		FlushCache(0);
		FlushCache(2);
	}

	// respond
	if (connection && (!request->Chunk || bytesReceived >= request->TotalSize))
	{
		ClientDownloadDataResponse_t response;
		response.Id = request->Id;
		response.BytesReceived = bytesReceived;
		response.Stop = 0;
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_DOWNLOAD_DATA_RESPONSE, sizeof(ClientDownloadDataResponse_t), &response);
	}

	return sizeof(ServerDownloadDataRequest_t) - sizeof(request->Data) + request->DataSize;
}

/*
 * NAME : onDownloadMessage
 * DESCRIPTION : Receive only patch download packets while the patch is unloaded.
 * NOTES : Avoid a 256-entry callback table inside the 4 KiB unpatch region.
 * ARGS : Native connection, packet context, and message payload.
 * RETURN : Number of consumed packet bytes.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
int onDownloadMessage(void* connection, u64 a1, u64 a2, u8* data)
{
	if (data[0] == CUSTOM_MSG_ID_SERVER_DOWNLOAD_DATA_REQUEST)
		return 4 + onServerDownloadDataRequest(connection, data + 4);
	return 4;
}

/*
 * NAME : getDownloadMessageHandler
 * DESCRIPTION : Route custom Medius packets to the temporary download receiver.
 * NOTES : Preserve native lookup behavior for other message types.
 * ARGS : Native handler lookup arguments.
 * RETURN : Native lookup result.
 * AUTHOR : Troy "Metroynome" Pruitt
 */
int getDownloadMessageHandler(u64 a0, u64 a1, u32* callback, u64 messageClass, u64 messageId)
{
	if (messageClass == NET_CUSTOM_MESSAGE_CLASS && messageId == NET_CUSTOM_MESSAGE_ID)
	{
		*callback = (u32)&onDownloadMessage;
		return 0;
	}
	return a0 == 0;
}

/*
 * NAME :		onOnlineMenu
 * DESCRIPTION :
 * 			Called every ui update.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
void onOnlineMenu(void)
{
	// call normal draw routine
#ifdef UYA_PAL
    ((void (*)(void))0x0067c9c0)();
#else
	((void (*)(void))0x00679f08)();
#endif

	// only show on main menu
	if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
	{
		uiProgressDialog("Downloading patch...", bytesReceived, totalBytes);
  }
}

/*
 * NAME : restoreGameMobies
 * DESCRIPTION : Detach moby callbacks before their patch or game-mode code is cleared.
 * NOTES : Restore the native game controller and remove game-mode-owned mobies.
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
void restoreGameMobies(void)
{
	static const u32 update[] = {
#if UYA_PAL
		0x003db0e8,
		0x003dad08,
		0x003d2c08,
		0x003d3020,
		0x003d0ba8,
		0x003d2908,
		0x003dec18,
		0x003e1098,
		0x003e0780,
		0x003ded18
#else
		0x003daec0,
		0x003daa60,
		0x003d2960,
		0x003d2dd8,
		0x003d0980,
		0x003d2660,
		0x003de9b0,
		0x003e0e30,
		0x003e0518,
		0x003deab0
#endif
	};
	Moby* moby = mobyListGetStart();
	Moby* end = mobyListGetEnd();
	int level = GAME_MAP_ID - 40;
	if (!moby || !end || level < 0 || level >= 10)
		return;

	for (; moby < end; ++moby)
	{
		u32 callback = (u32)moby->pUpdate;
		if (callback < 0x000d0000 || callback >= 0x00100000)
			continue;
		if (moby->oClass == 0x106a)
			moby->pUpdate = (void*)update[level];
		else
		{
			moby->pUpdate = NULL;
			mobyDestroy(moby);
		}
	}
}

/*
 * NAME :		main
 * DESCRIPTION :
 * 			Entrypoint.
 * NOTES :
 * ARGS : 
 * RETURN :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */
int main (void)
{

	int i;
	int initialize = !hasClearedMemory;
	const int patchesSize =  sizeof(patches) / (3 * sizeof(int));
	const int clearsSize =  sizeof(clears) / (2 * sizeof(int));

	// state
	// 0 = menus
	// 1 = in game
	// 2 = loading scene
	int state = isInGame() ? 1 : (isInMenus() ? 0 : 2);

	// Restore all hooks before any patch code or callbacks are cleared.
	if (!hasClearedMemory)
	{
		hasClearedMemory = 1;
		for (i = 0; i < patchesSize; ++i)
		{
			int context = patches[i][0];
			if (context < 0 || context == state || (state == 1 && context == GAME_MAP_ID))
				*(u32*)patches[i][1] = (u32)patches[i][2];
		}

		if (state == 1)
			restoreGameMobies();

		// Restore the resident network-player callbacks used while joining.
#if UYA_PAL
		int type = *(int*)0x0024063c;
		((int (*)(int, void*, void*, void*))0x0016eeb0)
			(type, (void*)0x00199a60, (void*)0x00199b00, (void*)0x00199b78);
#else
		int type = *(int*)0x002407bc;
		((int (*)(int, void*, void*, void*))0x0016eeb0)
			(type, (void*)0x00199ba0, (void*)0x00199c40, (void*)0x00199cb8);
#endif
		*(u32*)NET_GLOBAL_CALLBACKS_ADDRESS = 0;
		padEnableInput();
		for (i = 0; i < clearsSize; ++i)
			memset((void*)clears[i][0], 0, clears[i][1]);

	}

	// just clear if selfDestruct is true
	// if (selfDestruct) {
	// 	return;
	// }

	// 
	*(u32*)0x00153248 = 0x08000000 | ((u32)&getDownloadMessageHandler >> 2);
#if UYA_PAL
	*(u32*)0x00240608 = (u32)&onDownloadMessage;
#else
	*(u32*)0x00240788 = (u32)&onDownloadMessage;
#endif

	if (state == 0 && netGetLobbyServerConnection() && uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
	{
		// Hook menu loop
		#ifdef UYA_PAL
		*(u32*)0x005760e4 = 0x0c000000 | ((u32)(&onOnlineMenu) / 4);
		*(u32*)0x0057611c = 0x0c000000 | ((u32)(&onOnlineMenu) / 4);
		#else
		*(u32*)0x005753a4 = 0x0c000000 | ((u32)(&onOnlineMenu) / 4);
		*(u32*)0x005753dc = 0x0c000000 | ((u32)(&onOnlineMenu) / 4);
		#endif

		// disable pad on online main menu
		// if (uiGetActiveMenu(UI_MENU_ONLINE_LOBBY, 0) != 0)
		padDisableInput();
	}

	if (state == 1)
		uiProgressDialog("Downloading patch...", bytesReceived, totalBytes);

	if (!netGetLobbyServerConnection())
		padEnableInput();

	if (initialize)
	{
		FlushCache(0);
		FlushCache(2);
	}

	return 0;
}
