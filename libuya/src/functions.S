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
FUNCTION(internal_drawFunc_inLobby, 0x00592238) #untested in libuya
FUNCTION(internal_widthFunc_inLobby, 0x005946B0) #untested in libuya
FUNCTION(internal_drawBox_inLobby, 0x00619308) #untested in libuya
#else
# all InGame function address' change per map :(
FUNCTION(internal_drawFunc_inLobby, 0x00590F90) #untested in libuya
FUNCTION(internal_widthFunc_inLobby, 0x00593400) #untested in libuya
FUNCTION(internal_drawBox_inLobby, 0x00616BF0) #untested in libuya
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
#-------------------------- UI --------------------------
#--------------------------------------------------------
#ifdef UYA_PAL
FUNCTION(internal_uiDialog, 0x006872E8)
#else
FUNCTION(internal_uiDialog, 0x006847C8)
#endif
FUNCTION(uiShowPopup, 0x00540170) # Not Ported
FUNCTION(uiShowHelpPopup, 0x00540140) # Not Ported
FUNCTION(uiMsgString,  0x004E4180) # Not Ported