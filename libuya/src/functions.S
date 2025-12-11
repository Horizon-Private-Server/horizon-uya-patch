#if UYA_PAL
	#define GAME_ACTIVE                         (0x00241a50)
	#define SCENE_LOADED                        (0x00245848)
	#define GAME_MAP_ID                         (0x001f83a8)
#else
	#define GAME_ACTIVE                         (0x00241BD0)
	#define SCENE_LOADED                        (0x002459C8)
	#define GAME_MAP_ID                         (0x001F8528)
#endif

// fixed address function
#define FUNCTION(name, address) \
	.set	push;		        \
	.set	noreorder;	        \
	.section	.text.##name,"ax",@progbits; \
	.align 2;		            \
	.globl	name;		        \
	.type	name,@function;	    \
	.ent	name,0;		        \
name:	j address;              \
	nop;			            \
	.end	name;		        \
	.size	name,.-name;	    \
	.set	pop;

// variable address function
#define VARIABLE_FUNCTION(name, variable_address_struct) \
	.set	push;		        				\
	.set	noreorder;	        		\
	.section	.text.##name,"ax",@progbits; \
	.align 2;		            			\
	.globl	name;		        			\
	.type	name,@function;	    		\
	.ent	name,0;		        			\
	.extern variable_address_struct; \
name:														\
		addiu $sp, $sp, -0x40;			\
		sq $ra, 0x00($sp);					\
		sq $a0, 0x10($sp);					\
		sq $a1, 0x20($sp);					\
		sq $s0, 0x30($sp);					\
		la $a0, variable_address_struct; \
		la $a1, ##name##_return;				\
		j __LocalGetAddress;        \
		nop;			            			\
##name##_return:										\
		beq $zero, $v0, ##name##_exit;	\
		nop;												\
		lq $a1, 0x20($sp);					\
		jalr $v0;										\
		lq $a0, 0x10($sp);					\
##name##_exit:											\
		lq $ra, 0x00($sp);					\
		lq $s0, 0x30($sp);					\
		jr $ra;											\
		addiu $sp, $sp, 0x40;				\
	.end	name;		        				\
	.size	name,.-name;	   			 	\
	.set	pop;

// 
	.set	push;
	.set	noreorder;
	.section	.text.##__LocalGetAddress,"ax",@progbits;
	.align 2;
	.globl	__LocalGetAddress;
	.type	__LocalGetAddress,@function;
	.ent	__LocalGetAddress,0;
__LocalGetAddress:
	// if scene is loading, then we have no address
	li $v1, 1;
	lw $v0, SCENE_LOADED;
	bne $v0, $v1, __LocalGetAddress_exit;
	li $v0, 0;

	// if in menus then return lobby address
	lw $v0, GAME_MAP_ID;
  li $v1, 39;
	beq $v0, $v1, __LocalGetAddress_exit;
	lw $v0, 0($a0)

	// if not in game then no address
	lw $v0, GAME_ACTIVE;
	beq $v0, $zero, __LocalGetAddress_exit;
	nop;

	// otherwise in game
	// return address by map
	lw $v0, GAME_MAP_ID;
	
	// map local map ids (50+) to real map ids (40+)
	addiu $v1, $v0, -10;
	slti $s0, $v0, 50;
	movz $v0, $v1, $s0;

	// return 0 if map is not between 40 and 50
	addiu $v1, $v0, -40
	sltiu $v0, $v1, 10
	beqzl $v0, __LocalGetAddress_exit;
	move $v0, $zero;

	// load offset into VariableAddress struct by map id
	sll $v1, $v1, 2; // multiply map id [0,9] by 4
	addiu $v0, $v1, 4; // offset by 4 to skip lobby address

	// load address from struct
	addu $a0, $v0;
	lw $v0, 0($a0);

	// return to calling function
__LocalGetAddress_exit:
	jr $a1;
	nop;
	.end	__LocalGetAddress;
	.size	__LocalGetAddress,.-__LocalGetAddress;
	.set	pop;

// 
	.set	push;
	.set	noreorder;
	.section	.text.##GetAddress,"ax",@progbits;
	.align 2;
	.globl	GetAddress;
	.type	GetAddress,@function;
	.ent	GetAddress,0;
GetAddress:
	// if scene is loading, then we have no address
	li $v1, 1;
	lw $v0, SCENE_LOADED;
	bne $v0, $v1, GetAddress_exit;
	li $v0, 0;

	// if in menus then return lobby address
	lw $v0, GAME_MAP_ID;
  li $v1, 39;
	beq $v0, $v1, GetAddress_exit;
	lw $v0, 0($a0)

	// if not in game then no address
	lw $v0, GAME_ACTIVE;
	beq $v0, $zero, GetAddress_exit;
	nop;

	// otherwise in game
	// return address by map
	lw $v0, GAME_MAP_ID;
	
	// map local map ids (50+) to real map ids (40+)
	addiu $v1, $v0, -10;
	slti $t0, $v0, 50;
	movz $v0, $v1, $t0;

	// return 0 if map is not between 40 and 50
	addiu $v1, $v0, -40
	sltiu $v0, $v1, 10
	beqzl $v0, GetAddress_exit;
	move $v0, $zero;

	// load offset into VariableAddress struct by map id
	sll $v1, $v1, 2; // multiply map id [0,9] by 4
	addiu $v0, $v1, 4; // offset by 4 to skip lobby address

	// load address from struct
	addu $a0, $v0;
	lw $v0, 0($a0);

	// return to calling function
GetAddress_exit:
	jr $ra;
	nop;
	.end	GetAddress;
	.size	GetAddress,.-GetAddress;
	.set	pop;

// 
	.set	push;
	.set	noreorder;
	.section	.text.##GetAddress,"ax",@progbits;
	.align 2;
	.globl	GetAddressImmediate;
	.type	GetAddressImmediate,@function;
	.ent	GetAddressImmediate,0;
GetAddressImmediate:
	// if scene is loading, then we have no address
	//li $v1, 1;
	//lw $v0, SCENE_LOADED;
	//bne $v0, $v1, GetAddressImmediate_exit;
	//li $v0, 0;

	// if in menus then return lobby address
	lw $v0, GAME_MAP_ID;
  li $v1, 39;
	beq $v0, $v1, GetAddressImmediate_exit;
	lw $v0, 0($a0)

	// if not in game then no address
	//lw $v0, GAME_ACTIVE;
	//beq $v0, $zero, GetAddressImmediate_exit;
	//nop;

	// otherwise in game
	// return address by map
	lw $v0, GAME_MAP_ID;
	
	// map local map ids (50+) to real map ids (40+)
	addiu $v1, $v0, -10;
	slti $t0, $v0, 50;
	movz $v0, $v1, $t0;

	// return 0 if map is not between 40 and 50
	addiu $v1, $v0, -40
	sltiu $v0, $v1, 10
	beqzl $v0, GetAddressImmediate_exit;
	move $v0, $zero;

	// load offset into VariableAddress struct by map id
	sll $v1, $v1, 2; // multiply map id [0,9] by 4
	addiu $v0, $v1, 4; // offset by 4 to skip lobby address

	// load address from struct
	addu $a0, $v0;
	lw $v0, 0($a0);

	// return to calling function
GetAddressImmediate_exit:
	jr $ra;
	nop;
	.end	GetAddressImmediate;
	.size	GetAddressImmediate,.-GetAddressImmediate;
	.set	pop;


#--------------------------------------------------------
#------------------- STANDARD LIBRARY -------------------
#--------------------------------------------------------
#
# NTSC and PAL address' are the same.
#
FUNCTION(kprintf, 0x00128bf0)
FUNCTION(printf, 0x00128c70)
FUNCTION(sce_snprintf, 0x00128ba8)
FUNCTION(sprintf, 0x00120048)
FUNCTION(snprintf, 0x0011ffa8)
FUNCTION(memset, 0x0011d534)
FUNCTION(memcpy, 0x0011d380)
FUNCTION(memmove, 0x0011d430)
FUNCTION(strncpy, 0x00120d28)
FUNCTION(strcat, 0x001202f4)
FUNCTION(strncat, 0x001209c0)
FUNCTION(strcmp, 0x001205b0)
FUNCTION(strncmp, 0x00120b70)
#FUNCTION(strlen, 0x00120884)
FUNCTION(malloc, 0x0011c7b8)
FUNCTION(free, 0x0011c810)

#--------------------------------------------------------
#------------------------- MATH -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(cosf, vaFastCosf)
VARIABLE_FUNCTION(sinf, vaFastSinf)
VARIABLE_FUNCTION(atan2f, vaFastArcTan)
# NTSC and PAL addresses are the same for the following.
FUNCTION(powf, 0x00136d28)
FUNCTION(asinf, 0x001348b8)
#FUNCTION(sqrtf, 0x0011a808)
#FUNCTION(fabsf, 0x0011a550)
FUNCTION(fptodp, 0x001321c8)
FUNCTION(__divdi3, 0x00130760)
#FUNCTION(dptoli, 0x00131d70)
#FUNCTION(litodp, 0x00131cb8)
FUNCTION(floorf, 0x00136ba8)

#--------------------------------------------------------
#------------------------ MATH 3D -----------------------
#--------------------------------------------------------
FUNCTION(matrix_unit, 0x0013d5c0)
FUNCTION(matrix_rotate_x, 0x0013d708)
FUNCTION(matrix_rotate_y, 0x0013d7b0)
FUNCTION(matrix_rotate_z, 0x0013d660)

#--------------------------------------------------------
#----------------------- GRAPHICS -----------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(internal_drawFunc, vaFontPrintFunc)
VARIABLE_FUNCTION(gfxScreenSpaceTextCenter, vaFontPrintCenterFunc);
VARIABLE_FUNCTION(gfxScreenSpaceTextWindow, vaFontPrintWindowFunc);
VARIABLE_FUNCTION(gfxGetFontWidth, vaGetTextWidthFunc)
VARIABLE_FUNCTION(internal_drawBox, vaDrawBoxFunc)
VARIABLE_FUNCTION(gfxSetupGifPaging, vaSetupGifPaging)
VARIABLE_FUNCTION(gfxDoGifPaging, vaDoGifPaging)
VARIABLE_FUNCTION(gfxGetFrameTex, vaGetFrameTex)
VARIABLE_FUNCTION(gfxGetEffectTex, vaGetEffectTex)
VARIABLE_FUNCTION(gfxDrawSprite, vaDrawSprite)
VARIABLE_FUNCTION(gfxSetupEffectTex, vaSetupEffectTex)
VARIABLE_FUNCTION(gfxDrawEffect, vaDrawEffect)
VARIABLE_FUNCTION(gfxRegisterDrawFunction, vaRegisterDrawFunction)
VARIABLE_FUNCTION(gfxDrawBillboardQuad, vaDrawBillboardQuad)
VARIABLE_FUNCTION(gfxDrawQuad, vaFastDrawQuadReal)
VARIABLE_FUNCTION(gfxGetPreLoadedImageBufferSource, vaGetPreLoadedImageBufferSource)
VARIABLE_FUNCTION(gfxSetScissor, vaSetScissor)
VARIABLE_FUNCTION(gfxLoadPalToGs, vaLoadPalToGs)
VARIABLE_FUNCTION(gfxLoadTexToGs, vaLoadTexToGs)
VARIABLE_FUNCTION(gfxConstructEffectTex, vaConstructEffectTex)
VARIABLE_FUNCTION(internal_SpawnPart_059, vaSpawnPart_059)
VARIABLE_FUNCTION(gfxDestroyParticle, vaDeletePart)
VARIABLE_FUNCTION(internal_WorldSpaceToScreenSpace, vaWorldSpaceToScreenSpace)
VARIABLE_FUNCTION(gfxDrawScreenOverlay, vaDrawScreenOverlay)
VARIABLE_FUNCTION(gfxDrawStrip, vaFastDrawStrip);
VARIABLE_FUNCTION(gfxDrawStripInit, vaDrawStripInit)
VARIABLE_FUNCTION(gfxAddRegister, vaVU0_addGSRegister)

#--------------------------------------------------------
#------------------------- GAME -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(gameEnd, vaGameEndFunc)

#--------------------------------------------------------
#------------------------- SHA1 -------------------------
#--------------------------------------------------------
#
# NTSC and PAL address' are the same.
#
FUNCTION(sha1, 0x0015de28)

#--------------------------------------------------------
#------------------------- SIF --------------------------
#--------------------------------------------------------
#
# NTSC and PAL addresses are the same.
#
FUNCTION(SifInitRpc, 0x00129490)
FUNCTION(SifExitRpc, 0x00129630)
FUNCTION(SifRpcGetOtherData, 0x00129970)
FUNCTION(SifBindRpc, 0x00129c38)
FUNCTION(SifCallRpc, 0x00129e18)
FUNCTION(SifCheckStatRpc, 0x0012a018)
FUNCTION(SifInitIopHeap, 0x0012b8b8)
FUNCTION(SifAllocIopHeap, 0x0012b940)
FUNCTION(SifFreeIopHeap, 0x0012baa8)
FUNCTION(SifIopReset, 0x0012c570)
FUNCTION(SifIopSync, 0x0012c6f0)
FUNCTION(_SifLoadModuleBuffer, 0x0012c2c0)
FUNCTION(SifWriteBackDCache, 0x001293e0)
FUNCTION(SifSetDma, 0x00126620)
FUNCTION(SifDmaStat, 0x00126600)
FUNCTION(FlushCache, 0x001264e0)
FUNCTION(InvalidDCache, 0x00126b28)

#--------------------------------------------------------
#------------------------ COLOR -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(colorLerp, vaColorLerpFunc)

#--------------------------------------------------------
#-------------------------- UI --------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
	FUNCTION(internal_uiDialog, 0x006872E8)
	FUNCTION(internal_uiInputDialog, 0x00687450)
	FUNCTION(internal_uiAdvancedOptionsDialog, 0x00687508)
	FUNCTION(internal_uiInGameDetailsDialog, 0x00687640)
	FUNCTION(internal_uiPrivateMessageDialog, 0x006876A0)
	FUNCTION(internal_uiWeaponSelectDialog, 0x00687750)
	FUNCTION(internal_uiCitySelectDialog, 0x006877B0)
	FUNCTION(internal_uiSelectDialog, 0x00687800)
	FUNCTION(internal_uiSelectDialog_Simple, 0x006878A0)
	FUNCTION(internal_uiChangeTeamSkinDialog, 0x006878F0) 
	FUNCTION(internal_uiInvitePlayersDialog, 0x006873f0)
#else
	FUNCTION(internal_uiDialog, 0x006847C8)
	FUNCTION(internal_uiInputDialog, 0x00684930)
	FUNCTION(internal_uiAdvancedOptionsDialog, 0x006849e8)
	FUNCTION(internal_uiInGameDetailsDialog, 0x00684b20)
	FUNCTION(internal_uiPrivateMessageDialog, 0x00684b80)
	FUNCTION(internal_uiWeaponSelectDialog, 0x00684c30)
	FUNCTION(internal_uiCitySelectDialog, 0x00684c90)
	FUNCTION(internal_uiSelectDialog, 0x00684ce0)
	FUNCTION(internal_uiSelectDialog_Simple, 0x00684d80)
	FUNCTION(internal_uiChangeTeamSkinDialog, 0x00684dd0)
	FUNCTION(internal_uiInvitePlayersDialog, 0x006848d0)
#endif
VARIABLE_FUNCTION(internal_uiShowPopup, vaUIShowPopup)
FUNCTION(uiShowHelpPopup, 0x00540140) # Not Ported (For in game)
VARIABLE_FUNCTION(uiMsgString, vaUiMsgStringFunc)
VARIABLE_FUNCTION(gfxSetFont, vaFontSet)

#--------------------------------------------------------
#-------------------------- NET -------------------------
#--------------------------------------------------------
FUNCTION(internal_netSendMessage, 0x0014ac48)
FUNCTION(internal_netSendAppMessage, 0x0014ad08)
FUNCTION(netInterfaceControl, 0x00148b40)
FUNCTION(netSetHeartbeatInterval, 0x0014bf08)
#ifdef UYA_PAL
FUNCTION(internal_netUpdatetNWGameSettings, 0x006d4ed8)
#else
FUNCTION(internal_netUpdatetNWGameSettings, 0x006d2750)
#endif

#--------------------------------------------------------
#------------------------- WAD --------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
FUNCTION(internal_wadGetSectors, 0x00194ed8)
#else
FUNCTION(internal_wadGetSectors, 0x00194fc8)
#endif

#--------------------------------------------------------
#------------------------ PLAYER ------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(internal_SetPlayerState, vaSetPlayerStateFunc)
VARIABLE_FUNCTION(internal_HurtPlayer, vaHurtPlayerFunc)
VARIABLE_FUNCTION(playerRespawn, vaPlayerRespawnFunc)
VARIABLE_FUNCTION(internal_playerWarp, vaPlayerSetPosRotFunc)
VARIABLE_FUNCTION(internal_playerGiveWeaponUpgrade, vaGiveWeaponUpgradeFunc)
VARIABLE_FUNCTION(internal_playerGiveWeapon, vaGiveWeaponFunc)
VARIABLE_FUNCTION(internal_playerEquipWeapon, vaEquipWeaponFunc)
VARIABLE_FUNCTION(playerStripWeapons, vaWeaponStripMeFunc)
VARIABLE_FUNCTION(internal_GiveMeRandomWeapons, vaGiveMeRandomWeaponsFunc)
VARIABLE_FUNCTION(playerGetFromSlot, vaGetMissionDef)
VARIABLE_FUNCTION(playerGiveShield, vaGiveShieldFunc)

#--------------------------------------------------------
#-------------------------- MOBY ------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(mobySpawn, vaMobyCreateFunc)
VARIABLE_FUNCTION(mobyDestroy, vaMobyDeleteFunc)
VARIABLE_FUNCTION(mobyGetDamage, vaMobyGetDamageFunc)
VARIABLE_FUNCTION(mobyGetDamager, vaGetDamager)
VARIABLE_FUNCTION(mobyGetFunctions, vaGetMobyFunctions)

#--------------------------------------------------------
#----------------------- SPAWNPOINT ---------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(spawnPointGetRandom, vaGetRandomSpawnPointFunc)

#--------------------------------------------------------
#------------------------ SOUND -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(soundHeroPlay, vaSoundHeroPlayFunc)
VARIABLE_FUNCTION(soundMobyPlay, vaSoundMobyPlayFunc)
VARIABLE_FUNCTION(soundPlayByOClass, vaSoundPlayByOClassFunc)
VARIABLE_FUNCTION(soundPlay, vaSoundPlayFunc)

#--------------------------------------------------------
#------------------------ MUSIC -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(musicPlayTrack, vaPlayTrackFunc)
VARIABLE_FUNCTION(musicStopTrack, vaStopTrackFunc)
VARIABLE_FUNCTION(musicTransitionTrack, vaTransitionTrackFunc)

#--------------------------------------------------------
#------------------------ GUBER -------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(guberGetObjectByUID, vaGuberGetObjectByUID)
VARIABLE_FUNCTION(guberGetObjectByMoby, vaGuberGetObjectByMoby)
VARIABLE_FUNCTION(guberEventCreate, vaGuberEventCreate)
VARIABLE_FUNCTION(guberGetUID, vaGuberGetUID)

#--------------------------------------------------------
#------------------------- TIME -------------------------
#--------------------------------------------------------
FUNCTION(timerGetSystemTime, 0x0012dcb0)
VARIABLE_FUNCTION(timeDecTimer, vaFastDecTimeInt)
VARIABLE_FUNCTION(timeDecTimerShort, vaFastDecTimeShort)

#--------------------------------------------------------
#---------------------- COLLISION -----------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(CollLine_Fix, vaCollLineFix)
VARIABLE_FUNCTION(CollHotspot, vaCollHotspot)

#--------------------------------------------------------
#------------------------ FLAG --------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(flagIsBeingPickedUp, vaFlagIsBeingPickedUp_Func)
VARIABLE_FUNCTION(flagPickup, vaFlagPickup_Func)
VARIABLE_FUNCTION(flagIsReturning, vaFlagIsReturning_Func)
VARIABLE_FUNCTION(flagReturnToBase, vaFlagReturnToBase_Func)
VARIABLE_FUNCTION(flagIsAtBase, vaFlagIsAtBase_Func)
VARIABLE_FUNCTION(flagUpdate, vaFlagUpdate_Func)

#--------------------------------------------------------
#------------------------ MASTER ------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(masterGet, vaGetMaster)
VARIABLE_FUNCTION(masterCreate, vaCreateMaster)
VARIABLE_FUNCTION(masterDelete, vaDeleteMaster)

#--------------------------------------------------------
#------------------------ RANDOM ------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
FUNCTION(randSeed, 0x0019d510)
#else
FUNCTION(randSeed, 0x0019d650)
#endif

#--------------------------------------------------------
#------------------------ WEAPON ------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(weaponIsEnabled, vaGB_IsWeaponEnabled)

#--------------------------------------------------------
#------------------------ CAMERA ------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(cameraSetFov, vaCameraSetFOV)

#--------------------------------------------------------
#------------------------ PAD ---------------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(padRingBitsOn, vaPadRingBitsOn)