#include <tamtypes.h>
#include "interop.h"
#include "time.h"

variableAddressCompact_t vaFastDecTimeInt = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x00590138),
	.Bakisi = 0x0045d3a8,
	.Hoven = VA_COMPACT_OFFSET(0x0045d3a8, 0x0045ef28),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045d3a8, 0x00455d28),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045d3a8, 0x004538e8),
	.Metropolis = VA_COMPACT_OFFSET(0x0045d3a8, 0x00452c28),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045d3a8, 0x00450428),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045d3a8, 0x00450d50),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045d3a8, 0x004535d0),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045d3a8, 0x004528d0),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045d3a8, 0x00452250),
#else
	.Lobby = VA_COMPACT_LOBBY(0x0058ef88),
	.Bakisi = 0x0045c2f8,
	.Hoven = VA_COMPACT_OFFSET(0x0045c2f8, 0x0045ddb8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045c2f8, 0x00454bf8),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045c2f8, 0x00452838),
	.Metropolis = VA_COMPACT_OFFSET(0x0045c2f8, 0x00451b78),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045c2f8, 0x0044f2f8),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045c2f8, 0x0044fde0),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045c2f8, 0x00452620),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045c2f8, 0x00451960),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045c2f8, 0x004512a0),
#endif
};

variableAddressCompact_t vaFastDecTimeShort = {
#if UYA_PAL
	.Lobby = VA_COMPACT_LOBBY(0x00590168),
	.Bakisi = 0x0045d3d8,
	.Hoven = VA_COMPACT_OFFSET(0x0045d3d8, 0x0045ef58),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045d3d8, 0x00455d58),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045d3d8, 0x00453918),
	.Metropolis = VA_COMPACT_OFFSET(0x0045d3d8, 0x00452c58),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045d3d8, 0x00450458),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045d3d8, 0x00450d80),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045d3d8, 0x00453600),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045d3d8, 0x00452900),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045d3d8, 0x00452280),
#else
	.Lobby = VA_COMPACT_LOBBY(0x0058efb8),
	.Bakisi = 0x0045c328,
	.Hoven = VA_COMPACT_OFFSET(0x0045c328, 0x0045dde8),
	.OutpostX12 = VA_COMPACT_OFFSET(0x0045c328, 0x00454c28),
	.KorgonOutpost = VA_COMPACT_OFFSET(0x0045c328, 0x00452868),
	.Metropolis = VA_COMPACT_OFFSET(0x0045c328, 0x00451ba8),
	.BlackwaterCity = VA_COMPACT_OFFSET(0x0045c328, 0x0044f328),
	.CommandCenter = VA_COMPACT_OFFSET(0x0045c328, 0x0044fe10),
	.BlackwaterDocks = VA_COMPACT_OFFSET(0x0045c328, 0x00452650),
	.AquatosSewers = VA_COMPACT_OFFSET(0x0045c328, 0x00451990),
	.MarcadiaPalace = VA_COMPACT_OFFSET(0x0045c328, 0x004512d0),
#endif
};
