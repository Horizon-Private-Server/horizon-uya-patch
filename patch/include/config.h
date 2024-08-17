#ifndef __PATCH_CONFIG_H__
#define __PATCH_CONFIG_H__

#define MAX_CUSTOM_MAP_DEFINITIONS              (64)

enum ActionType
{
  ACTIONTYPE_DRAW,
  ACTIONTYPE_GETHEIGHT,
  ACTIONTYPE_SELECT,
  ACTIONTYPE_SELECT_SECONDARY,
  ACTIONTYPE_INCREMENT,
  ACTIONTYPE_DECREMENT,
  ACTIONTYPE_VALIDATE
};

enum ElementState
{
  ELEMENT_HIDDEN = 0,
  ELEMENT_VISIBLE = (1 << 0),
  ELEMENT_EDITABLE = (1 << 1),
  ELEMENT_SELECTABLE = (1 << 2),
};

enum LabelType
{
  LABELTYPE_HEADER,
  LABELTYPE_LABEL
};

struct MenuElem;
struct TabElem;
struct MenuElem_ListData;

typedef void (*ActionHandler)(struct TabElem* tab, struct MenuElem* element, int actionType, void * actionArg);
typedef void (*ButtonSelectHandler)(struct TabElem* tab, struct MenuElem* element);
typedef void (*MenuElementStateHandler)(struct TabElem* tab, struct MenuElem* element, int * state);
typedef int (*MenuElementListStateHandler)(struct MenuElem_ListData* listData, char* value);
typedef int (*MenuElementRangeStateHandler)(struct MenuElem_RangeData* listData, char* value);
typedef void (*TabStateHandler)(struct TabElem* tab, int * state);

typedef struct MenuElem
{
  char name[48];
  ActionHandler handler;
  MenuElementStateHandler stateHandler;
  void * userdata;
  char * help;
} MenuElem_t;

typedef struct MenuElem_ListData
{
  char * value;
  MenuElementListStateHandler stateHandler;
  int count;
  char * items[];
} MenuElem_ListData_t;

typedef struct MenuElem_RangeData
{
  char * value;
  MenuElementRangeStateHandler stateHandler;
  char minValue;
  char maxValue;
  char stepValue;
} MenuElem_RangeData_t;

typedef struct TabElem
{
  char name[32];
  TabStateHandler stateHandler;
  MenuElem_t * elements;
  int elementsCount;
  int selectedMenuItem;
  int menuOffset;
} TabElem_t;

typedef struct CustomMapVersionFileDef
{
  int Version;
  int BaseMapId;
  int ForcedCustomModeId;
  char padding[4];
  char Name[32];
} CustomMapVersionFileDef_t;

typedef void (*SndCompleteProc)(int loc, int user_data);

enum MapLoaderLoaded
{
  MAPLOADED_NONE = 0,
  MAPLOADED_LEVEL = 1,
  MAPLOADED_GAMEPLAY = 2,
  MAPLOADED_SOUND = 4,
  MAPLOADED_SOUND_SENT = 8,
};

struct MapLoaderState
{
    u8 Enabled;
    u8 MapId;
	  u8 CheckState;
    char MapName[32];
    char MapFileName[128];
    int LoadingFileSize;
    int LoadingFd;
    int Loaded;
    void * LevelBuffer;
    void * GameplayBuffer;
    void * SoundBuffer;
    SndCompleteProc SoundLoadCb;
    u64 SoundLoadUserData;
};

typedef struct VoteToEndState
{
  int TimeoutTime;
  int Count;
  char Votes[GAME_MAX_PLAYERS];
} VoteToEndState_t;

extern struct MapLoaderState MapLoaderState;
extern CustomMapDef_t CustomMapDefs[MAX_CUSTOM_MAP_DEFINITIONS];
extern int CustomMapDefCount;
extern int SelectedCustomMapId;

#endif // __PATCH_CONFIG_H__
