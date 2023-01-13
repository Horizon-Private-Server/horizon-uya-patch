#if UYA_PAL
	#define GAME_ACTIVE                         (0x00241a50)
	#define MENU_ACTIVE                         (0x001c3b30)
	#define SCENE_LOADED                        (0x00245848)
	#define GAME_MAP_ID                         (0x001f83a8)
#else
	#define GAME_ACTIVE                         (0x00241BD0)
	#define MENU_ACTIVE                         (0x001C3CB0)
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
	lw $v0, MENU_ACTIVE;
	bne $v0, $zero, __LocalGetAddress_exit;
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
	lw $v0, MENU_ACTIVE;
	bne $v0, $zero, GetAddress_exit;
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


#--------------------------------------------------------
#------------------- STANDARD LIBRARY -------------------
#--------------------------------------------------------
#
# NTSC and PAL address' are the same.
#
FUNCTION(printf, 0x00128BF0)
FUNCTION(sprintf, 0x00120048)
FUNCTION(memset, 0x0011d534)
FUNCTION(memcpy, 0x0011d380)
FUNCTION(strncpy, 0x00120d28)

#--------------------------------------------------------
#------------------------- MATH -------------------------
#--------------------------------------------------------
# Currently not ported.
#ifdef UYA_PAL

#else
# FUNCTION(cosf, 0x00136aa8)
FUNCTION(asinf, 0x00136278) # not ported
FUNCTION(sqrtf, 0x00136C90) # not ported
FUNCTION(powf, 0x00136788) # not ported
FUNCTION(fabsf, 0x00135960) # not ported
FUNCTION(fptodp, 0x00131918) # not ported
FUNCTION(floorf, 0x001359A8) # not ported
#endif

#--------------------------------------------------------
#----------------------- GRAPHICS -----------------------
#--------------------------------------------------------
VARIABLE_FUNCTION(internal_drawFunc, vaDrawTextFunc)
VARIABLE_FUNCTION(gfxGetFontWidth, vaGetTextWidthFunc)
VARIABLE_FUNCTION(internal_drawBox, vaDrawBoxFunc)

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
# NTSC and PAL address' are the same.
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

#--------------------------------------------------------
#------------------------ COLOR -------------------------
#--------------------------------------------------------
# FUNCTION(internal_colorLerp_inGame, 0x004d75e0)

# Not ported to PAL yet
FUNCTION(internal_colorLerp_inLobby, 0x00590438) # Not Ported (For in game)

#--------------------------------------------------------
#-------------------------- UI --------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
	FUNCTION(internal_uiDialog, 0x006872E8)
#else
	FUNCTION(internal_uiDialog, 0x006847C8)
#endif
FUNCTION(uiShowPopup, 0x00540170) # Not Ported (For in game)
FUNCTION(uiShowHelpPopup, 0x00540140) # Not Ported (For in game)
VARIABLE_FUNCTION(uiMsgString, vaUiMsgStringFunc)

#--------------------------------------------------------
#-------------------------- NET -------------------------
#--------------------------------------------------------
FUNCTION(internal_netSendMessage, 0x0014ac48)
FUNCTION(internal_netSendAppMessage, 0x0014ad08)

#--------------------------------------------------------
#------------------------- HELP -------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
	FUNCTION(internal_helpGetString_inLobby, 0)
	FUNCTION(internal_helpGetString_inGame, 0)
#else
	FUNCTION(internal_helpGetString_inLobby, 0x00599618)
	FUNCTION(internal_helpGetString_inGame, 0)
#endif
