/***************************************************
 * FILENAME :		main.c
 * DESCRIPTION :
 * NOTES :
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

int queryTotalFreeMemSize(void)
{
	return ((int (*)(void))0x0012BAC8)();
}

int sifExecLoadModuleBufferWrapper(void * ee_src, int size, void* iop_dest, int arg_len, char* args)
{
	// if ((u32)ee_src == 0x010BEE80) { // usb_driver
	// 	ee_src = 0x010D58C0;
	// 	size = 0x3005;
	// 	iop_dest = 0x4E300;
	// 	arg_len = 0x0;
	// 	args = 0;
	// } else if ((u32)ee_src == 0x010D58C0) { // inetctrl
	// 	ee_src = 0x010C7740;
	// 	size = 0xC1B5;
	// 	iop_dest = 0x4E300;
	// 	arg_len = 0;
	// 	args = 0;
	// } else if ((u32)ee_src == 0x010C7740) { // lgAud
	// 	ee_src = 0x010BEE80;
	// 	size = 0x88B1;
	// 	iop_dest = 0x4E300;
	// 	arg_len = 0x2B;
	// 	args = "hub=1";
	// }

	char * modName = (char*)((u32)ee_src + 0x8E);
	int free1 = queryTotalFreeMemSize();
	DPRINTF("loading %s (%d bytes)...", modName, size);

	int res = ((int (*)(void*, int, void*, int, char*))0x00198240)(ee_src, size, iop_dest, arg_len, args);
	int free2 = queryTotalFreeMemSize();
	DPRINTF("%d (iop mem %d => %d)\n", res, free1, free2);

	return res;
}

void hook(void * dest, void * functionPtr)
{
	*(u32*)dest = 0x0C000000 | ((u32)functionPtr >> 2);
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
	hook((void*)0x001983c4, &sifExecLoadModuleBufferWrapper); // NET_configuration 
	hook((void*)0x001983e4, &sifExecLoadModuleBufferWrapper); // eznetcnf 
	hook((void*)0x00198404, &sifExecLoadModuleBufferWrapper); // INET_service 
	hook((void*)0x00198428, &sifExecLoadModuleBufferWrapper); // INET_control 
	hook((void*)0x00198448, &sifExecLoadModuleBufferWrapper); // INET_PPP 
	hook((void*)0x00198468, &sifExecLoadModuleBufferWrapper); // INET_PPPoE_driver 
	hook((void*)0x00198488, &sifExecLoadModuleBufferWrapper); // IOP_MSIF_rpc_interface 
	hook((void*)0x001984ac, &sifExecLoadModuleBufferWrapper); // Libnet 
	hook((void*)0x001984d8, &sifExecLoadModuleBufferWrapper); // INET_SMAP_driver 
	hook((void*)0x001984fc, &sifExecLoadModuleBufferWrapper); // USB_driver 
	hook((void*)0x0019851c, &sifExecLoadModuleBufferWrapper); // lgAud 
	hook((void*)0x00198540, &sifExecLoadModuleBufferWrapper); // Lgkbm 
	hook((void*)0x00198580, &sifExecLoadModuleBufferWrapper); // eznetctl_inet 

	
	//*(u32*)0x0019851c = 0; // disable loading usb_driver
	//*(u32*)0x0019b90c = 0; // disable lgAudInit
	return 0;
}
