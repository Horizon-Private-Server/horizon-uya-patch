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
#include <libuya/utils.h>
#include <libuya/interop.h>
#include "include/config.h"
#include "module.h"

#include <sifcmd.h>
#include <iopheap.h>
#include <iopcontrol.h>
#include <sifrpc.h>
#include <loadfile.h>

#define NEWLIB_PORT_AWARE
#include <io_common.h>

#define MASS_PATH_PREFIX      "mass:"
#define HOST_PATH_PREFIX      "host:"

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

#define SOUND_FLUSH_SOUND_COMMANDS_FUNC (0x00193590)
#define LOAD_SOUND_LOADBUSY ((int*)0x00240BC8)
#define LOAD_SOUND_LOCALLOADERROR ((int*)0x00240B88)
#define LOAD_SOUND_LOADRETURNVALUE ((int*)0x00240140)
#define LOAD_SOUND_LOADPARAMS ((int*)0x00240180)
#define LOAD_SOUND_RPC_CLIENTDATA (0x00240100)
#define LOAD_LEVEL_SOUND_BANK_HOOK        ((u32*)0x005c1354)
#define LOAD_LEVEL_SOUND_INIT_FUNC        ((u32*)0x005f16f0)
#define LOAD_LEVEL_SOUND_LOAD_BANK_FUNC   ((u32*)0x005c1310)
#define LOAD_LEVEL_SOUND_BY_LOC_FUNC      ((u32*)0x00193888)
#define LOAD_LEVEL_SOUND_NOP_ADDR         ((u32)0x005a41f4)

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
char * fWad = "%suya/%s.pal.wad";
char * fWorld = "%suya/%s.pal.world";
char * fSound = "%suya/%s.pal.sound";
char * fBg = "%suya/%s.pal.bg";
char * fMap = "%suya/%s.pal.map";
char * fVersion = "%suya/%s.version";

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

#define SOUND_FLUSH_SOUND_COMMANDS_FUNC (0x00193680)
#define LOAD_SOUND_LOADBUSY ((int*)0x00240D48)
#define LOAD_SOUND_LOCALLOADERROR ((int*)0x00240D08)
#define LOAD_SOUND_LOADRETURNVALUE ((int*)0x002402c0)
#define LOAD_SOUND_LOADPARAMS ((int*)0x00240300)
#define LOAD_SOUND_RPC_CLIENTDATA (0x00240280)
#define LOAD_LEVEL_SOUND_BANK_HOOK        ((u32*)0x005bf36c)
#define LOAD_LEVEL_SOUND_INIT_FUNC        ((u32*)0x005eeff0)
#define LOAD_LEVEL_SOUND_LOAD_BANK_FUNC   ((u32*)0x005bf328)
#define LOAD_LEVEL_SOUND_BY_LOC_FUNC      ((u32*)0x00193978)
#define LOAD_LEVEL_SOUND_NOP_ADDR         ((u32)0x005a258c)

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
char * fWad = "%suya/%s.wad";
char * fWorld = "%suya/%s.world";
char * fSound = "%suya/%s.sound";
char * fBg = "%suya/%s.bg";
char * fMap = "%suya/%s.map";
char * fVersion = "%suya/%s.version";

#endif

#if DSCRPRINT
void pushScrPrintLine(char* str);
void clearScrPrintLine(void);
#endif

void grLoadStart();

void hook(void);
void loadModules(void);

int readLevelVersion(char * name, int * version);

void * usbFsModuleStart = (void*)0x000D0000;
int usbFsModuleSize = 0;
void * usbSrvModuleStart = (void*)0x000DD000;
int usbSrvModuleSize = 0;

// patch config
extern PatchConfig_t config;

// game config
extern PatchGameConfig_t gameConfig;

// custom map defs
CustomMapDef_t *CustomMapDefs = NULL;
int CustomMapDefCount = 0;

extern MenuElem_ListData_t dataCustomMaps;

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
extern char expectedMapVersion;

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
int useHost = 0;


typedef struct MapOverrideMessage
{
  CustomMapDef_t CustomMap;
} MapOverrideMessage;

typedef struct MapOverrideResponseMessage
{
  char Filename[64];
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

struct MapLoaderState MapLoaderState;

//------------------------------------------------------------------------------
char * getMapPathPrefix(void)
{
  if (useHost) return HOST_PATH_PREFIX;
  return MASS_PATH_PREFIX;
}

//------------------------------------------------------------------------------
int onSetMapOverride(void * connection, void * data)
{
	MapOverrideMessage *payload = (MapOverrideMessage*)data;
	MapOverrideResponseMessage msg;

	// reset
	patchStateContainer.CustomMapId = 0;

	if (payload->CustomMap.BaseMapId == 0) {
		DPRINTF("recv empty map\n");
		MapLoaderState.Enabled = 0;
		MapLoaderState.CheckState = 0;
		MapLoaderState.MapFileName[0] = 0;
		MapLoaderState.MapName[0] = 0;
		expectedMapVersion = -1;
		mapOverrideResponse = gameAmIHost() ? 9001 : -3;
	} else {
		// check for version
		int version = -1;
		if (LOAD_MODULES_STATE != 100)
			version = -1;

		int i = 0;
		for (i = 0; i < CustomMapDefCount; ++i) {
    		if (strcmp(CustomMapDefs[i].Filename, payload->CustomMap.Filename) == 0) {
				patchStateContainer.CustomMapId = i + 1;
				version = CustomMapDefs[i].Version;
				break;
			}
		}

		if (!patchStateContainer.CustomMapId)
			version = -2;

		// Store the host's expected version for comparison
		expectedMapVersion = payload->CustomMap.Version;

		DPRINTF("MapId:%d MapName:%s MapFileName:%s Version:%d\n", payload->CustomMap.BaseMapId, payload->CustomMap.Name, payload->CustomMap.Filename, version);
		// send response
		msg.Version = version;
		strncpy(msg.Filename, payload->CustomMap.Filename, sizeof(msg.Filename));
		netSendCustomAppMessage(connection, NET_LOBBY_CLIENT_INDEX, CUSTOM_MSG_ID_SET_MAP_OVERRIDE_RESPONSE, sizeof(msg), &msg);

		strncpy(MapLoaderState.MapName, payload->CustomMap.Name, sizeof(MapLoaderState.MapName));
		strncpy(MapLoaderState.MapFileName, payload->CustomMap.Filename, sizeof(MapLoaderState.MapFileName));
		
		// enable
		if (version >= 0) {
			MapLoaderState.Enabled = 1;
			MapLoaderState.CheckState = 0;
			mapOverrideResponse = version;
			MapLoaderState.MapId = payload->CustomMap.BaseMapId;
			MapLoaderState.LoadingFd = -1;
			MapLoaderState.LoadingFileSize = -1;
		} else {
			MapLoaderState.Enabled = 0;
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
    actionState = ACTION_MODULES_INSTALLED;

    // refresh map list
    refreshCustomMapList();
		
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

//--------------------------------------------------------------
int readLevelVersion(char * name, int * version)
{
	int r;

	// Generate version filename
	sprintf(membuffer, fVersion, getMapPathPrefix(), name);

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
	sprintf(membuffer, fMap, getMapPathPrefix(), MapLoaderState.MapFileName);

	// read
	return readFile(membuffer, (void*)buf, size) > 0;
}

//--------------------------------------------------------------
int readLevelBgUsb(u8 * buf, int size)
{
	// Ensure a wad isn't already open
	if (MapLoaderState.LoadingFd >= 0)
	{
		DPRINTF("readLevelBgUsb() called but a file is already open (%d)!", MapLoaderState.LoadingFd);
		return 0;
	}

	// Generate toc filename
	sprintf(membuffer, fBg, getMapPathPrefix(), MapLoaderState.MapFileName);

	// read
	return readFile(membuffer, (void*)buf, size) > 0;
}

//--------------------------------------------------------------
int getSizeUsb(char * filename)
{
	// Generate wad filename
	sprintf(membuffer, filename, getMapPathPrefix(), MapLoaderState.MapFileName);

	// get file length
	MapLoaderState.LoadingFileSize = readFileLength(membuffer);

	// Check the file has a valid size
	if (MapLoaderState.LoadingFileSize <= 0)
	{
		DPRINTF("error seeking file (%s): %d\n", membuffer, MapLoaderState.LoadingFileSize);
		return 0;
	}

	return MapLoaderState.LoadingFileSize;
}

//--------------------------------------------------------------
int openUsb(char * filename)
{
	// Ensure a wad isn't already open
	if (MapLoaderState.LoadingFd >= 0)
	{
		DPRINTF("openUsb() called but a file is already open (%d)!", MapLoaderState.LoadingFd);
		return 0;
	}

	// Generate wad filename
	sprintf(membuffer, filename, getMapPathPrefix(), MapLoaderState.MapFileName);

	// open wad file
	rpcUSBopen(membuffer, FIO_O_RDONLY);
	rpcUSBSync(0, NULL, &MapLoaderState.LoadingFd);
	
	// Ensure wad successfully opened
	if (MapLoaderState.LoadingFd < 0)
	{
		DPRINTF("error opening file (%s): %d\n", membuffer, MapLoaderState.LoadingFd);
		return 0;									
	}

	// Get length of file
	rpcUSBseek(MapLoaderState.LoadingFd, 0, SEEK_END);
	rpcUSBSync(0, NULL, &MapLoaderState.LoadingFileSize);

	// Check the file has a valid size
	if (MapLoaderState.LoadingFileSize <= 0)
	{
		DPRINTF("error seeking file (%s): %d\n", membuffer, MapLoaderState.LoadingFileSize);
		rpcUSBclose(MapLoaderState.LoadingFd);
		rpcUSBSync(0, NULL, NULL);
		MapLoaderState.LoadingFd = -1;
		MapLoaderState.Enabled = 0;
		return 0;
	}

	// Go back to start of file
	// The read will be called later
	rpcUSBseek(MapLoaderState.LoadingFd, 0, SEEK_SET);
	rpcUSBSync(0, NULL, NULL);

	DPRINTF("%s is %d byte long.\n", membuffer, MapLoaderState.LoadingFileSize);
	return MapLoaderState.LoadingFileSize;
}

//--------------------------------------------------------------
int readUsb(u8 * buf)
{
	// Ensure the wad is open
	if (MapLoaderState.LoadingFd < 0 || MapLoaderState.LoadingFileSize <= 0)
	{
		DPRINTF("error opening file: %d\n", MapLoaderState.LoadingFd);
		return 0;									
	}

	// Try to read from usb
	if (rpcUSBread(MapLoaderState.LoadingFd, buf, MapLoaderState.LoadingFileSize) != 0)
	{
		DPRINTF("error reading from file.\n");
		rpcUSBclose(MapLoaderState.LoadingFd);
		rpcUSBSync(0, NULL, NULL);
		MapLoaderState.LoadingFd = -1;
		return 0;
	}
				
	return 1;
}

//------------------------------------------------------------------------------
void checkForHostFs(void)
{
  char dirpath[64];
  int fd;
  
  useHost = 1;
  snprintf(dirpath, sizeof(dirpath), "%suya", getMapPathPrefix());
  
  // try to open directory on host:
	rpcUSBdopen(dirpath);
	rpcUSBSync(0, NULL, &fd);

	// Ensure the dir was opened successfully
	if (fd < 0)
	{
    useHost = 0;
		return;
	}
	
  // close
  rpcUSBdclose(fd);
	rpcUSBSync(0, NULL, NULL);
}

//------------------------------------------------------------------------------
void refreshCustomMapList(void)
{
  int fd, r, i;
  const char* versionExt = ".version";
  char dirpath[64];
  char fullpath[256];
  char filename[256];
  char buf[64];
  char filenameWithoutExtension[64];
  int versionExtLen = strlen(versionExt);
  iox_dirent_t dirent;
  io_dirent_t* iomanDirent = (io_dirent_t*)&dirent;
  
  // reset
  dataCustomMaps.count = 1;
  CustomMapDefCount = 0;
  memset(CustomMapDefs, 0, sizeof(CustomMapDef_t) * MAX_CUSTOM_MAP_DEFINITIONS);

  // need usb modules
  if (!HAS_LOADED_MODULES) return;

#if DSCRPRINT
  clearScrPrintLine();
#endif

  // check if host fs exists
  checkForHostFs();

  //
  snprintf(dirpath, sizeof(dirpath), "%suya", getMapPathPrefix());
  DPRINTF("dir path %s\n", dirpath);

	// Open
	rpcUSBdopen(dirpath);
	rpcUSBSync(0, NULL, &fd);

	// Ensure the dir was opened successfully
	if (fd < 0)
	{
		DPRINTF("error opening dir (%s): %d\n", dirpath, fd);
		return;
	}
	
  DPRINTF("opening dir (%s): returned %d\n", dirpath, fd);

  // read
  do 
  {
    // read next entry
    // stop if we've reached the end
    if (rpcUSBdread(fd, &dirent) != 0) break;
    rpcUSBSync(0, NULL, &r);
    if (r <= 0) break;

    // extract filename
    // for some reason there's a mixup between if we're using ioman or iomanX
    // PS2s use iomanX but the emu HLE hostfs thinks we're using ioman
    if (useHost) strncpy(filename, iomanDirent->name, sizeof(filename));
    else strncpy(filename, dirent.name, sizeof(filename));

    // we want to parse the .version files
    // check if filename ends with ".version"
    int len = strlen(filename);
    if (strncmp(&filename[len-versionExtLen], versionExt, versionExtLen) != 0) continue;

    #if DSCRPRINT
    snprintf(buf, sizeof(buf), "y %s", filename);
    pushScrPrintLine(buf);
    #endif

    DPRINTF("found version %s\n", filename);

    // parse version file
    CustomMapVersionFileDef_t versionFileDef;
    snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, filename);
    int read = readFile(fullpath, &versionFileDef, sizeof(CustomMapVersionFileDef_t));

    // ensure version file is valid
    if (read != sizeof(CustomMapVersionFileDef_t))
    {
      DPRINTF("%s (%d) does not match expected file size %d. Skipping.\n", filename, read, sizeof(CustomMapVersionFileDef_t));
      continue;
    }
    
    // compute filename without extension
    strncpy(filenameWithoutExtension, filename, sizeof(filenameWithoutExtension));
    len = strlen(filenameWithoutExtension);
    filenameWithoutExtension[len - versionExtLen] = 0;

    // ensure version file has matching .world OR .wad
    snprintf(fullpath, sizeof(fullpath), fWad, getMapPathPrefix(), filenameWithoutExtension);
    int fWadLen = readFileLength(fullpath);
    snprintf(fullpath, sizeof(fullpath), fWorld, getMapPathPrefix(), filenameWithoutExtension);
    int fWorldLen = readFileLength(fullpath);
    if (fWadLen <= 0 && fWorldLen <= 0) continue;

    DPRINTF("(%d) \"%s\" f:\"%s\" v:%d bmap:%d mode:%d\n", CustomMapDefCount, versionFileDef.Name, filenameWithoutExtension, versionFileDef.Version, versionFileDef.BaseMapId, versionFileDef.ForcedCustomModeId);

	// set CustomMapDef location to "Tips" UI.
	if (CustomMapDefs == NULL) {
		CustomMapDefs = (CustomMapDef_t *)uiGetMenu(UI_MENU_TIPS)->pFirstChild;
	}

    // bring to custom map defs
    CustomMapDefs[CustomMapDefCount].Version = versionFileDef.Version;
    CustomMapDefs[CustomMapDefCount].BaseMapId = versionFileDef.BaseMapId;
    CustomMapDefs[CustomMapDefCount].ForcedCustomModeId = versionFileDef.ForcedCustomModeId;
    strncpy(CustomMapDefs[CustomMapDefCount].Filename, filenameWithoutExtension, sizeof(CustomMapDefs[CustomMapDefCount].Filename));
    strncpy(CustomMapDefs[CustomMapDefCount].Name, versionFileDef.Name, sizeof(CustomMapDefs[CustomMapDefCount].Name));
    CustomMapDefCount++;

    // reached max maps
    if (CustomMapDefCount >= MAX_CUSTOM_MAP_DEFINITIONS) break;
    
  } while (1);

  // close
  rpcUSBdclose(fd);
	rpcUSBSync(0, NULL, NULL);
  
    // sort names alphabetically
	CustomMapDef_t temp[MAX_CUSTOM_MAP_DEFINITIONS];
	int k = 0, j;
    for(k; k < CustomMapDefCount; ++k) {
        for(j = 0; j < CustomMapDefCount; ++j) {
            if(strcmp(CustomMapDefs[k].Name, CustomMapDefs[j].Name) < 0) {
                temp[k] = CustomMapDefs[k];
                CustomMapDefs[k] = CustomMapDefs[j];
                CustomMapDefs[j] = temp[k];
            }
        }
    }
  // populate config
  for (i = 0; i < CustomMapDefCount; ++i) {
    dataCustomMaps.items[i+1] = (char*)CustomMapDefs[i].Name;
    dataCustomMaps.count += 1;
  }

  // clamp
  if (patchStateContainer.CustomMapId >= dataCustomMaps.count)
    patchStateContainer.CustomMapId = dataCustomMaps.count - 1;
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
void soundFlushSoundCommands(void)
{
	((void (*)(void))SOUND_FLUSH_SOUND_COMMANDS_FUNC)();
}

//------------------------------------------------------------------------------
int soundLoadBankFromEE(void* buf)
{
  int r;

  *LOAD_SOUND_LOCALLOADERROR = 0;
  if (*LOAD_SOUND_LOADBUSY == 0) {

    LOAD_SOUND_LOADRETURNVALUE[0] = -1;
    LOAD_SOUND_LOADPARAMS[0] = buf;

    // load from
    InvalidDCache(LOAD_SOUND_LOADRETURNVALUE, LOAD_SOUND_LOADRETURNVALUE + 1);
    while ((r = SifCheckStatRpc(LOAD_SOUND_RPC_CLIENTDATA)) != 0) {
      soundFlushSoundCommands();
      FlushCache(0);
    }

    // call RPC
    *LOAD_SOUND_LOADBUSY = 1;
    r = SifCallRpc(LOAD_SOUND_RPC_CLIENTDATA, 0x57, 1, LOAD_SOUND_LOADPARAMS, 4, LOAD_SOUND_LOADRETURNVALUE, 4, 0, 0);
    if (r < 0) {
      // RPC call failed
      *LOAD_SOUND_LOCALLOADERROR = 0x106;
      return 0;
    } else {

      // wait for load to finish
      int loadReturnValue = *LOAD_SOUND_LOADRETURNVALUE;
      while (loadReturnValue == -1) {
        FlushCache(0);
        loadReturnValue = *LOAD_SOUND_LOADRETURNVALUE;
      }

      return loadReturnValue;
    }
  } else {
    // busy (load already in progress), fail
    return 0;
  }
}

//------------------------------------------------------------------------------
u64 hookedLevelExit(void)
{
  u64 r = ((u64 (*)(void))LEVEL_EXIT_FUNCTION_FUNC)();

	// We try and hook here to just to make sure that after tha game loads
	// We can still load our custom minimap
  hook();

	// Change default "Waiting for Players.." time (Default: 0x1518 == 90s)
	int playerWaitTime = 45; // 45s
#if UYA_PAL
	POKE_U32(0x00241a4c, playerWaitTime * 50);
#else
	POKE_U32(0x00241bcc, playerWaitTime * 60);
#endif

  grLoadStart();

  return r;
}

//------------------------------------------------------------------------------
int beginLoadingLevelWad(int loadGameplay)
{
	char * filename = NULL;
  void * dest = NULL;

  switch (loadGameplay)
  {
    case 0: // loading level wad
    {
      filename = fWad;
      dest = MapLoaderState.LevelBuffer;
      break;
    }
    case 1: // loading gameplay
    {
      filename = fWorld;
      dest = MapLoaderState.GameplayBuffer;
      break;
    }
  }

  if (filename != NULL)
  {
    DPRINTF("loading %s\n", filename);
    int fSize = openUsb(filename);
    if (fSize > 0)
    {
      if (readUsb(dest) > 0) {
        MapLoaderState.Loaded |= loadGameplay ? MAPLOADED_GAMEPLAY : MAPLOADED_LEVEL;
        return 1;
      }
    }
  }

  return 0;
}

//------------------------------------------------------------------------------
int beginLoadingSoundWad(void* dest)
{
  int fSize = openUsb(fSound);
  if (fSize > 0) {
    MapLoaderState.SoundBuffer = dest;

    DPRINTF("level: %08X\nsound: %08X\n", MapLoaderState.LevelBuffer, MapLoaderState.SoundBuffer);

    // read sound bank in background
    // let hookedCheck handle when sound is finished loading
    DPRINTF("begin sound bank read\n");
    if (readUsb(MapLoaderState.SoundBuffer) > 0) {
      MapLoaderState.Loaded |= MAPLOADED_SOUND;
      return 1;
    }
  }

  return 0;
}

//------------------------------------------------------------------------------
void hookedLoad(void * dest, u32 sectorStart, u32 sectorSize)
{
  if (LOAD_LEVEL_PART_ID == 0) {
    MapLoaderState.LevelBuffer = dest;
    MapLoaderState.GameplayBuffer = 0;
    MapLoaderState.SoundBuffer = 0;
    MapLoaderState.Loaded = 0;
  } else {
    MapLoaderState.GameplayBuffer = dest;
  }

	// Check if loading MP map
	if (MapLoaderState.Enabled && HAS_LOADED_MODULES)
	{
    // load sound wad first
    // Generate sound filename
    if ((MapLoaderState.Loaded & MAPLOADED_SOUND) == 0) {
      sprintf(membuffer, fSound, getMapPathPrefix(), MapLoaderState.MapFileName);
      int filelen = readFileLength(membuffer);
      if (filelen > 0)
        if (beginLoadingSoundWad(dest)) return;
    }

    // if we've reached here, then the sound wad doesn't exist, so load the level wad
    if (beginLoadingLevelWad(LOAD_LEVEL_PART_ID)) return;
	}

	// Default to cdvd load if the usb load failed
	((void (*)(void*, u32, u32))LEVEL_CDVD_LOAD_ASYNC_FUNC)(dest, sectorStart, sectorSize);
}

//------------------------------------------------------------------------------
u32 hookedCheck(int a0)
{
	int r, cmd;

	// If the wad is not open then we're loading from cdvd
	if (MapLoaderState.LoadingFd < 0 || !MapLoaderState.Enabled)
		return cdvdSync(a0);

	// Otherwise check to see if we've finished loading the data from USB
	if (rpcUSBSyncNB(0, &cmd, &r) == 1)
	{
		// If the command is USBREAD close and return
		if (cmd == 0x04)
		{
			// DPRINTF("finished reading %d bytes from USB\n", r);
			// rpcUSBclose(MapLoaderState.LoadingFd);
			// rpcUSBSync(0, NULL, NULL);
			// MapLoaderState.LoadingFd = -1;
			// return cdvdSync(a0);

      DPRINTF("finished reading %d bytes from USB\n", r);
			rpcUSBclose(MapLoaderState.LoadingFd);
			rpcUSBSync(0, NULL, NULL);
			MapLoaderState.LoadingFd = -1;

      if ((MapLoaderState.Loaded & MAPLOADED_SOUND_SENT) == 0 && (MapLoaderState.Loaded & MAPLOADED_SOUND)) {
        // finish loading sound wad
        ((void (*)())LOAD_LEVEL_SOUND_INIT_FUNC)();       // sound_LevelInit()
        ((void (*)())LOAD_LEVEL_SOUND_LOAD_BANK_FUNC)();  // music_LoadCoreBank()
        POKE_U32(LOAD_LEVEL_SOUND_NOP_ADDR, 0);           // nop sound_LevelInit()
        POKE_U32(LOAD_LEVEL_SOUND_NOP_ADDR + 8, 0);       // nop music_LoadCoreBank()
      }

      if ((MapLoaderState.Loaded & MAPLOADED_LEVEL) == 0 && MapLoaderState.LevelBuffer) {
        // load level wad
        if (beginLoadingLevelWad(0)) return;
      } else if ((MapLoaderState.Loaded & MAPLOADED_GAMEPLAY) == 0 && MapLoaderState.GameplayBuffer) {
        // load gameplay wad
        if (beginLoadingLevelWad(1)) return;
      }

      // finished loading level +/ sound wads
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
	if (MapLoaderState.Enabled && HAS_LOADED_MODULES && readLevelBgUsb(a1, a3 * 0x800) > 0)
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
		MapLoaderState.Enabled = 0;
		return;
	}

	// Check if loading MP map
	if (MapLoaderState.Enabled && HAS_LOADED_MODULES)
	{
		// Disable if map doesn't match
		if (levelId != MapLoaderState.MapId && (levelId - 10) != MapLoaderState.MapId)
		{
			MapLoaderState.Enabled = 0;
			DPRINTF("loading map id doesn't match custom map's base id. %d\n", levelId);
			return;
		}

		int fWadSize = getSizeUsb(fWad);
		int fWorldSize = getSizeUsb(fWorld);

    if (fWadSize > 0)
    {
			((int*)dest)[5] = (fWadSize / 0x800) + ((fWadSize % 0x800) == 0 ? 0 : 1);
			DPRINTF("MAPLOADER: Get Table || fWadSize: %d\n", fWadSize, fWorldSize);
    }

    if (fWorldSize > 0)
    {
			((int*)dest)[9] = (fWorldSize / 0x800) + ((fWorldSize % 0x800) == 0 ? 0 : 1);
			DPRINTF("MAPLOADER: Get Table || fWorldSize: %d\n", fWadSize, fWorldSize);
    }

    // if we can't find a wad or world file
    // then disable loading the custom map
    if (fWadSize <= 0 && fWorldSize <= 0)
    {
			MapLoaderState.Enabled = 0;
			DPRINTF("Error reading level/world wad from usb (%d, %d)\n", fWadSize, fWorldSize);
    }
	}
}

//------------------------------------------------------------------------------
void hookedSoundCoreBankLoad(int loc, int offset, SndCompleteProc cb, u64 user_data)
{
	// Check if loading MP map
	if (MapLoaderState.Enabled && HAS_LOADED_MODULES && (MapLoaderState.LevelBuffer > 0 || MapLoaderState.GameplayBuffer > 0) && MapLoaderState.SoundBuffer > 0)
	{
    MapLoaderState.SoundLoadCb = cb;
    MapLoaderState.SoundLoadUserData = user_data;

    *(u64*)0x00240d58 = user_data;
    *(u32*)0x00240D50 = cb;
  
    // sound wad finished, pass to Cb
    if (cb && (MapLoaderState.Loaded & MAPLOADED_SOUND_SENT) == 0 && (MapLoaderState.Loaded & MAPLOADED_SOUND)) {

      int r = soundLoadBankFromEE(MapLoaderState.SoundBuffer);
      DPRINTF("load sound bank from EE %08X to IOP %08X\n", MapLoaderState.SoundBuffer, r);
      if (MapLoaderState.SoundLoadCb) {
        MapLoaderState.SoundLoadCb(r, MapLoaderState.SoundLoadUserData);
        MapLoaderState.SoundLoadCb = NULL; // reset
      }
      MapLoaderState.Loaded |= MAPLOADED_SOUND_SENT;
    }

    // we're loading the sound wad from USB
    return;
	}

  // snd_BankLoadByLoc_CB
	((void (*)(int, int, SndCompleteProc, u64))LOAD_LEVEL_SOUND_BY_LOC_FUNC)(loc, offset, cb, user_data);
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
	if (MapLoaderState.Enabled) {
		int mapIdx = (LOAD_LEVEL_MAP_ID - 40) % 10;
		if (mapIdx >= 0) {
			strncpy(LOAD_LEVEL_TRANSITION_MAPNAME + (mapIdx * 0x20), MapLoaderState.MapName, 32);
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
	if (MapLoaderState.Enabled && HAS_LOADED_MODULES && sectorSize == 0x21)
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

		actionState = ACTION_NONE;
		LOAD_MODULES_RESULT = 1;
	}
	else if (actionState == ACTION_NEW_MAPS_UPDATE)
	{
		// enable input
		padEnableInput();
		
		uiShowOkDialog("Custom Maps", "New updates are available. Please download them at https://rac-horizon.com");
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
    *LOAD_LEVEL_SOUND_BANK_HOOK = 0x0C000000 | ((u32)(&hookedSoundCoreBankLoad) / 4);
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

	// 
	if (!initialized)
	{
		initialized = 1;

		// set map loader defaults
		MapLoaderState.Enabled = 0;
		MapLoaderState.CheckState = 0;

		// install on login
		if (LOAD_MODULES_RESULT == 0)
		{
			initialized = 2;
		}
	}

	// force map id to current map override if in staging
	if (MapLoaderState.Enabled == 1) {
		if (isInMenus()) {
			GameSettings * settings = gameGetSettings();
			if (settings && settings->GameLoadStartTime > 0) {
				settings->GameLevel = MapLoaderState.MapId;
				gameConfig.isCustomMap = 1;
			}
		} else {
			// if not in menus, null CustomMapDef pointer and map number.
			if (CustomMapDefs != NULL) {
				CustomMapDefs = NULL;
				CustomMapDefCount = 0;
			}
		}
	}
}