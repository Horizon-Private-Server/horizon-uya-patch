#include <libuya/stdio.h>
#include <libuya/net.h>
#include "rpc.h"
#include "messageid.h"
#include "config.h"
#include <libuya/string.h>
#include <libuya/ui.h>
#include <libuya/graphics.h>
#include <libuya/pad.h>
#include <libuya/gamesettings.h>
#include <libuya/game.h>
#include <libuya/interop.h>
#include "config.h"

#include <sifcmd.h>
#include <iopheap.h>
#include <iopcontrol.h>
#include <sifrpc.h>
#include <loadfile.h>

#define NEWLIB_PORT_AWARE
#include <io_common.h>

#define MAP_FRAG_PAYLOAD_MAX_SIZE               (1024)
#define LOAD_MODULES_STATE                      (*(u8*)0x000CFFF0)
#define LOAD_MODULES_RESULT                     (*(u8*)0x000CFFF1)
#define HAS_LOADED_MODULES                      (LOAD_MODULES_STATE == 100)

#define USB_FS_ID                               (*(u8*)0x000CFFF4)
#define USB_SRV_ID                              (*(u8*)0x000CFFF8)

#if UYA_PAL

#define CDVD_LOAD_ASYNC_FUNC					(0x00194970)
#define LEVEL_CDVD_LOAD_ASYNC_FUNC				(0x005c2a40)
#define CDVD_IS_LOADING_FUNC					(0x00194a38)
#define READ_WAD_GETSECTORS_FUNC				(0x00194ed8)

#define LOAD_LEVEL_MAP_ID						(*(int*)0x002412dc)
#define LOAD_LEVEL_PART_ID						(*(int*)0x00240288)
#define LOAD_LEVEL_READ_WAD_HOOK				((u32*)0x005a41c8)
#define LOAD_LEVEL_READ_WAD_HOOK_VALUE			(0x0c170a90)
#define LOAD_LEVEL_CD_SYNC_HOOK					((u32*)0x005a4070)
#define LOAD_LEVEL_READ_LEVEL_TOC_HOOK			((u32*)0x00194f8c)
#define LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK	((u32*)0x006787a8)
#define LOAD_LEVEL_TRANSITION_MAPNAME			(0x00352D20)

#define LEVEL_EXIT_FUNCTION_HOOK     ((u32*)0x00193340)
#define LEVEL_EXIT_FUNCTION_FUNC     ((u32*)0x00192F68)

VariableAddress_t LOAD_LEVEL_RADAR_MAP_HOOK = {
	.Lobby = 0,
	.Bakisi = 0x004951A4,
	.Hoven = 0x004972BC,
	.OutpostX12 = 0x0048CB94,
	.KorgonOutpost = 0x0048A264,
	.Metropolis = 0x0048967C,
	.BlackwaterCity = 0x00486F14,
	.CommandCenter = 0x00486F0C,
	.BlackwaterDocks = 0x0048978C,
	.AquatosSewers = 0x00488A8C,
	.MarcadiaPalace = 0x0048840C,
};

// paths for level specific files
char * fWad = "uya/%s.pal.wad";
char * fWorld = "uya/%s.pal.world";
char * fBg = "uya/%s.pal.bg";
char * fMap = "uya/%s.pal.map";
char * fVersion = "uya/%s.pal.version";
char * fGlobalVersion = "uya/version";

#else

#define CDVD_LOAD_ASYNC_FUNC					(0x00194A60)
#define LEVEL_CDVD_LOAD_ASYNC_FUNC				(0x005c09f8)
#define CDVD_IS_LOADING_FUNC					(0x00194b28)
#define READ_WAD_GETSECTORS_FUNC				(0x00194fc8)

#define LOAD_LEVEL_MAP_ID						(*(int*)0x0024145C)
#define LOAD_LEVEL_PART_ID						(*(int*)0x00240408)
#define LOAD_LEVEL_READ_WAD_HOOK				((u32*)0x005a2560)
#define LOAD_LEVEL_READ_WAD_HOOK_VALUE			(0x0C17027E)
#define LOAD_LEVEL_CD_SYNC_HOOK					((u32*)0x005a2408)
#define LOAD_LEVEL_READ_LEVEL_TOC_HOOK			((u32*)0x0019507c)
#define LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK	((u32*)0x00675dc0)
#define LOAD_LEVEL_TRANSITION_MAPNAME			(0x00352e20)

#define LEVEL_EXIT_FUNCTION_HOOK     ((u32*)0x00193430)
#define LEVEL_EXIT_FUNCTION_FUNC     ((u32*)0x00193058)

VariableAddress_t LOAD_LEVEL_RADAR_MAP_HOOK = {
	.Lobby = 0,
	.Bakisi = 0x004931B4,
	.Hoven = 0x0049520C,
	.OutpostX12 = 0x0048AB24,
	.KorgonOutpost = 0x00488274,
	.Metropolis = 0x0048768C,
	.BlackwaterCity = 0x00484EA4,
	.CommandCenter = 0x0048505C,
	.BlackwaterDocks = 0x0048789C,
	.AquatosSewers = 0x00486BDC,
	.MarcadiaPalace = 0x0048651C,
};

// paths for level specific files
char * fWad = "uya/%s.wad";
char * fWorld = "uya/%s.world";
char * fBg = "uya/%s.bg";
char * fMap = "uya/%s.map";
char * fVersion = "uya/%s.version";
char * fGlobalVersion = "uya/version";

#endif

void grLoadStart();

void hook(void);
void loadModules(void);

int readLevelVersion(char * name, int * version);
int readGlobalVersion(int * version);

void * usbFsModuleStart = (void*)0x000D0000;
int usbFsModuleSize = 0;
void * usbSrvModuleStart = (void*)0x000DD000;
int usbSrvModuleSize = 0;

// patch config
extern PatchConfig_t config;

// game config
extern PatchGameConfig_t gameConfig;

// game mode overrides
//extern struct MenuElem_ListData dataCustomModes;

// map overrides
//extern struct MenuElem_ListData dataCustomMaps;

extern u32 colorBlack;
extern u32 colorBg;
extern u32 colorRed;
extern u32 colorContentBg;
extern u32 colorText;

extern char mapOverrideResponse;

enum MenuActionId
{
	ACTION_ERROR_LOADING_MODULES = -1,

	ACTION_MODULES_NOT_INSTALLED = 0,
	ACTION_DOWNLOADING_MODULES = 1,
	ACTION_MODULES_INSTALLED = 2,
	ACTION_NEW_MAPS_UPDATE = 3,

	ACTION_NONE = 100
};

// memory card fd
int initialized = 0;
int actionState = ACTION_MODULES_NOT_INSTALLED;
int rpcInit = 0;

// 
char membuffer[256];


typedef struct MapOverrideMessage
{
	u8 MapId;
	char MapName[32];
	char MapFileName[128];
} MapOverrideMessage;

typedef struct MapOverrideResponseMessage
{
	int Version;
} MapOverrideResponseMessage;

typedef struct MapClientRequestModulesMessage
{
	u32 Module1Start;
	u32 Module2Start;
} MapClientRequestModulesMessage;

typedef struct MapServerSentModulesMessage
{
	int Version;
	int Module1Size;
	int Module2Size;
} MapServerSentModulesMessage;

struct MapLoaderState
{
    u8 Enabled;
    u8 MapId;
	u8 CheckState;
    char MapName[32];
    char MapFileName[128];
    int LoadingFileSize;
    int LoadingFd;
} State;

//------------------------------------------------------------------------------
int onSetMapOverride(void * connection, void * data)
{
	MapOverrideMessage *payload = (MapOverrideMessage*)data;

	if (payload->MapId == 0)
	{
		DPRINTF("recv empty map\n");
		State.Enabled = 0;
		State.CheckState = 0;
		mapOverrideResponse = 1;
	}
	else
	{
		// check for version
		int version = -1;
		if (LOAD_MODULES_STATE != 100)
			version = -1;
		else if (!readLevelVersion(payload->MapFileName, &version))
			version = -2;

		// print
		DPRINTF("MapId:%d MapName:%s MapFileName:%s Version:%d\n", payload->MapId, payload->MapName, payload->MapFileName, version);

		// send response
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_SET_MAP_OVERRIDE_RESPONSE, 4, &version);

		// enable
		if (version >= 0)
		{
			State.Enabled = 1;
			State.CheckState = 0;
			mapOverrideResponse = version;
			State.MapId = payload->MapId;
			State.LoadingFd = -1;
			State.LoadingFileSize = -1;
			strncpy(State.MapName, payload->MapName, 32);
			strncpy(State.MapFileName, payload->MapFileName, 128);
		}
		else
		{
			State.Enabled = 0;
			mapOverrideResponse = version;
		}
	}

	return sizeof(MapOverrideMessage);
}

//------------------------------------------------------------------------------
int onServerSentMapIrxModules(void * connection, void * data)
{
	DPRINTF("server sent map irx modules\n");

	MapServerSentModulesMessage * msg = (MapServerSentModulesMessage*)data;

	// initiate loading
	if (LOAD_MODULES_STATE == 0)
		LOAD_MODULES_STATE = 7;

	// 
	usbFsModuleSize = msg->Module1Size;
	usbSrvModuleSize = msg->Module2Size;

	// 
	loadModules();

	//
	int init = rpcInit = rpcUSBInit();

	DPRINTF("rpcUSBInit: %d, %08X:%d, %08X:%d\n", init, (u32)usbFsModuleStart, usbFsModuleSize, (u32)usbSrvModuleStart, usbSrvModuleSize);
	
	//
	if (init < 0)
	{
		actionState = ACTION_ERROR_LOADING_MODULES;
	}
	else
	{
		int remoteVersion = msg->Version;
		int localVersion = 0;
		if (!readGlobalVersion(&localVersion) || localVersion != remoteVersion)
		{
			// Indicate new version
			actionState = ACTION_NEW_MAPS_UPDATE;
		}
		else
		{
			// Indicate maps installed
			actionState = ACTION_MODULES_INSTALLED;
		}
		
		DPRINTF("local maps version %d || remote maps version %d\n", localVersion, remoteVersion);

		// if in game, ask server to resend map override to use
		if (gameGetSettings())
			netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_REQUEST_MAP_OVERRIDE, 0, NULL);
	}

	return sizeof(MapServerSentModulesMessage);
}

//------------------------------------------------------------------------------
void loadModules(void)
{
	int usbFsRes = 0, usbServRes = 0;
	if (LOAD_MODULES_STATE < 7)
		return;

	if (LOAD_MODULES_STATE != 100)
	{
		//
		SifInitRpc(0);

		// Load modules
		//int usbd_id = SifExecModuleBuffer((void*)0x000AA000, 34993, 0, NULL, &mod_res);
		USB_FS_ID = SifExecModuleBuffer(usbFsModuleStart, usbFsModuleSize, 0, NULL, &usbFsRes);
		USB_SRV_ID = SifExecModuleBuffer(usbSrvModuleStart, usbSrvModuleSize, 0, NULL, &usbServRes);

		//DPRINTF("Loading USBD: %d\n", usbd_id);
		DPRINTF("Loading MASS: %d %d\n", USB_FS_ID, usbFsRes);
		DPRINTF("Loading USBSERV: %d %d\n", USB_SRV_ID, usbServRes);
	}

	LOAD_MODULES_STATE = 100;
}

//------------------------------------------------------------------------------
int readFileLength(char * path)
{
	int fd, fSize;

	// Open
	rpcUSBopen(path, FIO_O_RDONLY);
	rpcUSBSync(0, NULL, &fd);

	// Ensure the file was opened successfully
	if (fd < 0)
	{
		DPRINTF("error opening file (%s): %d\n", path, fd);
		return -1;
	}
	
	// Get length of file
	rpcUSBseek(fd, 0, SEEK_END);
	rpcUSBSync(0, NULL, &fSize);

	// Close file
	rpcUSBclose(fd);
	rpcUSBSync(0, NULL, NULL);

	return fSize;
}

//------------------------------------------------------------------------------
int readFile(char * path, void * buffer, int length)
{
	int r, fd, fSize;

	// Open
	rpcUSBopen(path, FIO_O_RDONLY);
	rpcUSBSync(0, NULL, &fd);

	// Ensure the file was opened successfully
	if (fd < 0)
	{
		DPRINTF("error opening file (%s): %d\n", path, fd);
		return -1;
	}
	
	// Get length of file
	rpcUSBseek(fd, 0, SEEK_END);
	rpcUSBSync(0, NULL, &fSize);

	// limit read length to size of file
	if (fSize < length)
		length = fSize;

	// Go back to start of file
	rpcUSBseek(fd, 0, SEEK_SET);
	rpcUSBSync(0, NULL, NULL);

	// Read map
	if (rpcUSBread(fd, (void*)buffer, length) != 0)
	{
		DPRINTF("error reading from file.\n");
		rpcUSBclose(fd);
		rpcUSBSync(0, NULL, NULL);
		return -1;
	}
	rpcUSBSync(0, NULL, &r);

	// Close file
	rpcUSBclose(fd);
	rpcUSBSync(0, NULL, NULL);

	return r;
}

//------------------------------------------------------------------------------
int readGlobalVersion(int * version)
{
	int r;

	r = readFile(fGlobalVersion, (void*)version, 4);
	if (r != 4)
	{
		DPRINTF("error reading file (%s)\n", fGlobalVersion);
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------
int readLevelVersion(char * name, int * version)
{
	int r;

	// Generate version filename
	sprintf(membuffer, fVersion, name);

	// read
	r = readFile(membuffer, (void*)version, 4);
	if (r != 4)
	{
		DPRINTF("error reading file (%s)\n", membuffer);
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------
int readLevelMapUsb(u8 * buf, int size)
{
	// Generate toc filename
	sprintf(membuffer, fMap, State.MapFileName);

	// read
	return readFile(membuffer, (void*)buf, size) > 0;
}

//--------------------------------------------------------------
int readLevelBgUsb(u8 * buf, int size)
{
	// Ensure a wad isn't already open
	if (State.LoadingFd >= 0)
	{
		DPRINTF("readLevelBgUsb() called but a file is already open (%d)!", State.LoadingFd);
		return 0;
	}

	// Generate toc filename
	sprintf(membuffer, fBg, State.MapFileName);

	// read
	return readFile(membuffer, (void*)buf, size) > 0;
}

//--------------------------------------------------------------
int getSizeUsb(char * filename)
{
	// Generate wad filename
	sprintf(membuffer, filename, State.MapFileName);

	// get file length
	State.LoadingFileSize = readFileLength(membuffer);

	// Check the file has a valid size
	if (State.LoadingFileSize <= 0)
	{
		DPRINTF("error seeking file (%s): %d\n", membuffer, State.LoadingFileSize);
		return 0;
	}

	return State.LoadingFileSize;
}

//--------------------------------------------------------------
int openUsb(char * filename)
{
	// Ensure a wad isn't already open
	if (State.LoadingFd >= 0)
	{
		DPRINTF("openUsb() called but a file is already open (%d)!", State.LoadingFd);
		return 0;
	}

	// Generate wad filename
	sprintf(membuffer, filename, State.MapFileName);

	// open wad file
	rpcUSBopen(membuffer, FIO_O_RDONLY);
	rpcUSBSync(0, NULL, &State.LoadingFd);
	
	// Ensure wad successfully opened
	if (State.LoadingFd < 0)
	{
		DPRINTF("error opening file (%s): %d\n", membuffer, State.LoadingFd);
		return 0;									
	}

	// Get length of file
	rpcUSBseek(State.LoadingFd, 0, SEEK_END);
	rpcUSBSync(0, NULL, &State.LoadingFileSize);

	// Check the file has a valid size
	if (State.LoadingFileSize <= 0)
	{
		DPRINTF("error seeking file (%s): %d\n", membuffer, State.LoadingFileSize);
		rpcUSBclose(State.LoadingFd);
		rpcUSBSync(0, NULL, NULL);
		State.LoadingFd = -1;
		State.Enabled = 0;
		return 0;
	}

	// Go back to start of file
	// The read will be called later
	rpcUSBseek(State.LoadingFd, 0, SEEK_SET);
	rpcUSBSync(0, NULL, NULL);

	DPRINTF("%s is %d byte long.\n", membuffer, State.LoadingFileSize);
	return State.LoadingFileSize;
}

//--------------------------------------------------------------
int readUsb(u8 * buf)
{
	// Ensure the wad is open
	if (State.LoadingFd < 0 || State.LoadingFileSize <= 0)
	{
		DPRINTF("error opening file: %d\n", State.LoadingFd);
		return 0;									
	}

	// Try to read from usb
	if (rpcUSBread(State.LoadingFd, buf, State.LoadingFileSize) != 0)
	{
		DPRINTF("error reading from file.\n");
		rpcUSBclose(State.LoadingFd);
		rpcUSBSync(0, NULL, NULL);
		State.LoadingFd = -1;
		return 0;
	}
				
	return 1;
}

//------------------------------------------------------------------------------
int cdvdLoad(u32 sectorStart, u32 sectorSize, void * dest, void * a3)
{
	int (*_cdvdLoad)(u32, u32, void*, void*) = (int (*)(u32, u32, void*, void*))CDVD_LOAD_ASYNC_FUNC;
	return _cdvdLoad(sectorStart, sectorSize, dest, a3);
}

//------------------------------------------------------------------------------
int cdvdSync(int a0)
{
	int (*_cdvdSync)(int) = (int (*)(int))CDVD_IS_LOADING_FUNC;
	return _cdvdSync(a0);
}

//------------------------------------------------------------------------------
void cdvdReadWadSectors(u32 startSector, u32 sectorCount, void * dest)
{
	// load table
	((void (*)(u32,u32,void*))READ_WAD_GETSECTORS_FUNC)(startSector, sectorCount, dest);
}

//------------------------------------------------------------------------------
u64 hookedLevelExit(void)
{
  u64 r = ((u64 (*)(void))LEVEL_EXIT_FUNCTION_FUNC)();

	// We try and hook here to just to make sure that after tha game loads
	// We can still load our custom minimap
  hook();

  grLoadStart();

  return r;
}

//------------------------------------------------------------------------------
void hookedLoad(void * dest, u32 sectorStart, u32 sectorSize)
{
	char * filename = NULL;

	// Check if loading MP map
	if (State.Enabled && HAS_LOADED_MODULES)
	{
		switch (LOAD_LEVEL_PART_ID)
		{
			case 0: // loading level wad
			{
				filename = fWad;
				break;
			}
			case 1: // loading gameplay
			{
				filename = fWorld;
				break;
			}
		}

		if (filename != NULL)
		{
			DPRINTF("loading %s\n", filename);
			int fSize = openUsb(filename);
			if (fSize > 0)
			{
				if (readUsb(dest) > 0)
					return;
			}
		}
	}

	// Default to cdvd load if the usb load failed
	((void (*)(void*, u32, u32))LEVEL_CDVD_LOAD_ASYNC_FUNC)(dest, sectorStart, sectorSize);
}

//------------------------------------------------------------------------------
u32 hookedCheck(int a0)
{
	int r, cmd;

	// If the wad is not open then we're loading from cdvd
	if (State.LoadingFd < 0 || !State.Enabled)
		return cdvdSync(a0);

	// Otherwise check to see if we've finished loading the data from USB
	if (rpcUSBSyncNB(0, &cmd, &r) == 1)
	{
		// If the command is USBREAD close and return
		if (cmd == 0x04)
		{
			DPRINTF("finished reading %d bytes from USB\n", r);
			rpcUSBclose(State.LoadingFd);
			rpcUSBSync(0, NULL, NULL);
			State.LoadingFd = -1;
			return cdvdSync(a0);
		}
	}

	// Set bg color to red
	*((vu32*)0x120000e0) = 0x1010B4;

	// Not sure if this is necessary but it doesn't hurt to call the game's native load check
	cdvdSync(a0);

	// Indicate that we haven't finished loading
	return 1;
}

//------------------------------------------------------------------------------
void hookedLoadingScreen(u64 a0, void * a1, u64 a2, u64 a3, u64 t0, u64 t1, u64 t2)
{
	if (State.Enabled && HAS_LOADED_MODULES && readLevelBgUsb(a1, a3 * 0x800) > 0)
	{

	}
	else
	{
		//((void (*)(u64, void *, u64, u64, u64, u64, u64))CDVD_LOAD_FUNC)(a0, a1, a2, a3, t0, t1, t2);
	}
}

//------------------------------------------------------------------------------
void hookedGetTable(u32 startSector, u32 sectorCount, u8 * dest)
{
	int levelId = LOAD_LEVEL_MAP_ID;

	// load table
	cdvdReadWadSectors(startSector, sectorCount, dest);
	
	// Disable when loading menu
	if (levelId < 40)
	{
		DPRINTF("level id is not mp map %d.", levelId);
		State.Enabled = 0;
		return;
	}

	// Check if loading MP map
	if (State.Enabled && HAS_LOADED_MODULES)
	{
		// Disable if map doesn't match
		if (levelId != State.MapId && (levelId - 10) != State.MapId)
		{
			State.Enabled = 0;
			DPRINTF("loading map id doesn't match custom map's base id. %d\n", levelId);
			return;
		}

		int fWadSize = getSizeUsb(fWad);
		int fWorldSize = getSizeUsb(fWorld);
		if (fWadSize > 0 && fWorldSize)
		{
			((int*)dest)[5] = (fWadSize / 0x800) + ((fWadSize % 0x800) == 0 ? 0 : 1);
			((int*)dest)[9] = (fWorldSize / 0x800) + ((fWorldSize % 0x800) == 0 ? 0 : 1);
			DPRINTF("MAPLOADER: Get Table || fWadSize: %d, fWorldSize: %d\n", fWadSize, fWorldSize);
		}
		else
		{
			State.Enabled = 0;
			DPRINTF("Error reading level/world wad from usb (%d, %d)\n", fWadSize, fWorldSize);
		}
	}
}

//------------------------------------------------------------------------------
void hookedGetAudio(u64 a0, void * dest, u32 startSector, u32 sectorCount, u64 t0, u64 t1, u64 t2)
{
	//((void (*)(u64, void*,u32,u32,u64,u64,u64))CDVD_LOAD_FUNC)(a0, dest, startSector, sectorCount, t0, t1, t2);
}

//------------------------------------------------------------------------------
u64 hookedLoadCdvd(u64 a0, u64 a1, u64 a2, u64 a3, u64 t0, u64 t1, u64 t2)
{
	u64 result = ((u64 (*)(u64,u64,u64,u64,u64,u64,u64))CDVD_LOAD_ASYNC_FUNC)(a0, a1, a2, a3, t0, t1, t2);

	// We try and hook here to just to make sure that after tha game loads
	// We can still load our custom minimap
	hook();

	return result;
}

//------------------------------------------------------------------------------
int hookedLoadScreenMapNameString(void)
{
	if (State.Enabled) {
		int mapIdx = (LOAD_LEVEL_MAP_ID - 40) % 10;
		if (mapIdx >= 0) {
			strncpy(LOAD_LEVEL_TRANSITION_MAPNAME + (mapIdx * 0x20), State.MapName, 32);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------
char* hookedLoadScreenModeNameString(char * dest, char * src)
{
	int i = 0;

	// if we're loading a custom map
	// and that map has an exclusive gamemode
	// save map name as gamemode
	// for (i = 0; i < dataCustomMapsWithExclusiveGameModeCount; ++i)
	// {
	// 	if (gameConfig.customMapId == dataCustomMapsWithExclusiveGameMode[i])
	// 	{
	// 		strncpy(dest, dataCustomMaps.items[(int)gameConfig.customMapId], 32);
	// 		return dest;
	// 	}
	// }

	// if custom mode is set
	// if (gameConfig.customModeId > 0)
	// 	strncpy(dest, dataCustomModes.items[(int)gameConfig.customModeId], 32);
	// else
	// 	strncpy(dest, src, 32);
	
	return dest;
}

//------------------------------------------------------------------------------
int hookedGetMap(u32 sectorStart, u32 sectorSize, void * dest, void * a3)
{

	// Check if loading MP map
	if (State.Enabled && HAS_LOADED_MODULES && sectorSize == 0x21)
	{
		// We hardcode the size because that's the max that deadlocked can hold
		if (readLevelMapUsb(dest, 0x21 * 0x800))
		  return 0;
	}

  return cdvdLoad(sectorStart, sectorSize, dest, a3);
}

//------------------------------------------------------------------------------
int mapsGetInstallationResult(void)
{
	return LOAD_MODULES_RESULT;
}

//------------------------------------------------------------------------------
int mapsDownloadingModules(void)
{
	return actionState == ACTION_DOWNLOADING_MODULES;
}

//------------------------------------------------------------------------------
int mapsPromptEnableCustomMaps(void)
{
	MapClientRequestModulesMessage request = { 0, 0 };
	if (uiShowYesNoDialog("Enable Custom Maps", "Are you sure?") == 1)
	{
		// request irx modules from server
		request.Module1Start = (u32)usbFsModuleStart;
		request.Module2Start = (u32)usbSrvModuleStart;
		netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_REQUEST_MAP_IRX_MODULES, sizeof(MapClientRequestModulesMessage), &request);
		actionState = ACTION_DOWNLOADING_MODULES;
		return 1;
	}

	return 0;
}

//------------------------------------------------------------------------------
void onMapLoaderOnlineMenu(void)
{
	u32 bgColorDownload = 0x70000000;

	if (actionState == ACTION_DOWNLOADING_MODULES)
	{
		// disable input
		padDisableInput();

		// render background
		//gfxScreenSpaceBox(0.2, 0.35, 0.6, 0.3, bgColorDownload);

		// flash color
		//u32 downloadColor = 0x80808080;
		//int gameTime = ((gameGetTime()/100) % 15);
		//if (gameTime > 7)
		//	gameTime = 15 - gameTime;
		//downloadColor += 0x101010 * gameTime;

		// render text
		//gfxScreenSpaceText(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 1, 1, downloadColor, "Downloading modules, please wait...", -1, 4);
	}
	else if (actionState == ACTION_MODULES_INSTALLED)
	{
		// enable input
		padEnableInput();

		uiShowOkDialog("Custom Maps", "Custom maps have been enabled.");

		actionState = ACTION_NONE;
		LOAD_MODULES_RESULT = 1;
	}
	else if (actionState == ACTION_NEW_MAPS_UPDATE)
	{
		// enable input
		padEnableInput();
		
		uiShowOkDialog("Custom Maps", "New updates are available. Please download them at https://rac-horizon.com/maps");
		actionState = ACTION_MODULES_INSTALLED;
		LOAD_MODULES_RESULT = 2;
	}
	else if (actionState == ACTION_ERROR_LOADING_MODULES)
	{
		// enable input
		padEnableInput();
		
		uiShowOkDialog("Custom Maps", "There was an error loading the custom map modules.");
		actionState = ACTION_NONE;
		LOAD_MODULES_RESULT = -1;
	}
	else if (initialized == 2)
	{
		// request irx modules from server
		MapClientRequestModulesMessage request = { 0, 0 };
		request.Module1Start = (u32)usbFsModuleStart;
		request.Module2Start = (u32)usbSrvModuleStart;
		netSendCustomAppMessage(netGetLobbyServerConnection(), NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_CLIENT_REQUEST_MAP_IRX_MODULES, sizeof(MapClientRequestModulesMessage), &request);
		actionState = ACTION_DOWNLOADING_MODULES;
		initialized = 1;
	}

	return;
}

//------------------------------------------------------------------------------
void hook(void)
{
	// Install hooks
	if (*LOAD_LEVEL_READ_WAD_HOOK == LOAD_LEVEL_READ_WAD_HOOK_VALUE)
	{
		*LOAD_LEVEL_READ_LEVEL_TOC_HOOK = 0x0C000000 | ((u32)(&hookedGetTable) / 4);
		*LOAD_LEVEL_CD_SYNC_HOOK = 0x0C000000 | ((u32)(&hookedCheck) / 4);
		*LOAD_LEVEL_READ_WAD_HOOK = 0x0C000000 | ((u32)(&hookedLoad) / 4);
		*LEVEL_EXIT_FUNCTION_HOOK = 0x0C000000 | ((u32)(&hookedLevelExit) / 4);
	}

	if (*LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK == 0x03E00008)
	{
		*LOAD_LEVEL_TRANSITION_MENU_LOAD_HOOK = 0x08000000 | ((u32)(&hookedLoadScreenMapNameString) / 4);
	}

  // hook radar
  // this needs to be hook when the level loads
  u32 radarMapHookAddr = GetAddressImmediate(&LOAD_LEVEL_RADAR_MAP_HOOK);
  if (radarMapHookAddr) {

    u32 hookReplaceIfValueIs = 0x0C000000 | (CDVD_LOAD_ASYNC_FUNC >> 2);
    if (*(u32*)radarMapHookAddr == hookReplaceIfValueIs) {
      *(u32*)radarMapHookAddr = 0x0C000000 | ((u32)(&hookedGetMap) / 4);
    }
  }
}

//------------------------------------------------------------------------------
void runMapLoader(void)
{
	// 
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_SET_MAP_OVERRIDE, &onSetMapOverride);
	netInstallCustomMsgHandler(CUSTOM_MSG_ID_SERVER_SENT_MAP_IRX_MODULES, &onServerSentMapIrxModules);

	// hook irx module loading 
	hook();

	/*if (padGetButtonDown(0, PAD_L1 | PAD_R1) > 0) {
		MapOverrideMessage data = {
			.MapId = 40,
			.MapFileName = "maraxus",
			.MapName = "Test"
		};
		onSetMapOverride(netGetLobbyServerConnection(), &data);
	}*/

	// 
	if (!initialized)
	{
		initialized = 1;

		// set map loader defaults
		State.Enabled = 0;
		State.CheckState = 0;

		// install on login
		if (config.enableAutoMaps && LOAD_MODULES_RESULT == 0)
		{
			initialized = 2;
		}
	}

	// force map id to current map override if in staging
	if (State.Enabled == 1 && isInMenus())
	{
		GameSettings * settings = gameGetSettings();
		if (settings && settings->GameLoadStartTime > 0)
		{
			settings->GameLevel = State.MapId;
		}
	}
}
