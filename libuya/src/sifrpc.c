#include <sifrpc.h>
#include <sifcmd.h>
#include <iopheap.h>
#include <iopcontrol.h>
#include <loadfile.h>
#include <kernel.h>

int _SifLoadModuleBuffer(void *ptr, int arg_len, const char *args, int *modres);

/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (C)2001, Gustavo Scotti (gustavo@scotti.com)
# (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/
int SifExecModuleBuffer(void *ptr, u32 size, u32 arg_len, const char *args, int *mod_res)
{
	SifDmaTransfer_t dmat;
	void *iop_addr;
	int res;
	unsigned int qid;

	/* Round the size up to the nearest 16 bytes. */
	size = (size + 15) & -16;

	if (!(iop_addr = SifAllocIopHeap(size)))
		return -400;

	dmat.src = ptr;
	dmat.dest = iop_addr;
	dmat.size = size;
	dmat.attr = 0;
	SifWriteBackDCache(ptr, size);
	qid = SifSetDma(&dmat, 1);

	if (!qid)
	    return -1; // should have a better error here...

	while(SifDmaStat(qid) >= 0);

	res = _SifLoadModuleBuffer(iop_addr, arg_len, args, mod_res);
	SifFreeIopHeap(iop_addr);

	return res;
}

