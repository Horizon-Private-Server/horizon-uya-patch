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


#--------------------------------------------------------
#------------------- STANDARD LIBRARY -------------------
#--------------------------------------------------------
#
# NTSC and PAL address' are the same.
#
FUNCTION(printf, 0x0011e718)
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
#ifdef UYA_PAL
	# all InGame function address' change per map :(
	FUNCTION(internal_drawFunc_inLobby, 0x00592238)
	FUNCTION(internal_widthFunc_inLobby, 0x005946B0) # address is not correct for PAL
	FUNCTION(internal_drawBox_inLobby, 0x00619308) # address is not correct for PAL
#else
	# all InGame function address' change per map :(
	FUNCTION(internal_drawFunc_inLobby, 0x00590F90)
	FUNCTION(internal_widthFunc_inLobby, 0x00590cf0)
	FUNCTION(internal_drawBox_inLobby, 0x00615078)
#endif
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
FUNCTION(uiMsgString,  0x004E4180) # Not Ported (For in game)

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
