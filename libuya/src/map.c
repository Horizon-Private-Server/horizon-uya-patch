#include "map.h"
#include "help.h"

//--------------------------------------------------------
int mapMaskHasMask(int value, int maskValue)
{
    return (value & maskValue) == maskValue;
}

//--------------------------------------------------------
int mapIdToMask(int mapId)
{
    switch (mapId)
    {
        case MAP_ID_BAKISI: return MAP_MASK_BAKISI;
        case MAP_ID_HOVEN: return MAP_MASK_HOVEN;
        case MAP_ID_OUTPOST_X12: return MAP_MASK_OUTPOST_X12;
        case MAP_ID_KORGON: return MAP_MASK_KORGON;
        case MAP_ID_METROPOLIS: return MAP_MASK_METROPOLIS;
        case MAP_ID_BLACKWATER_CITY: return MAP_MASK_BLACKWATER_CITY;
        case MAP_ID_COMMAND_CENTER: return MAP_MASK_COMMAND_CENTER;
        case MAP_ID_BLACKWATER_DOCKS: return MAP_MASK_BLACKWATER_DOCKS;
        case MAP_ID_AQUATOS_SEWERS: return MAP_MASK_AQUATOS_SEWERS;
        case MAP_ID_MARCADIA: return MAP_MASK_MARCADIA;
        default: return 0;
    }
}

//--------------------------------------------------------
int mapMaskToId(int mapMask)
{
    switch (mapMask)
    {
        case MAP_MASK_BAKISI: return MAP_ID_BAKISI;
        case MAP_MASK_HOVEN: return MAP_ID_HOVEN;
        case MAP_MASK_OUTPOST_X12: return MAP_ID_OUTPOST_X12;
        case MAP_MASK_KORGON: return MAP_ID_KORGON;
        case MAP_MASK_METROPOLIS: return MAP_ID_METROPOLIS;
        case MAP_MASK_BLACKWATER_CITY: return MAP_ID_BLACKWATER_CITY;
        case MAP_MASK_COMMAND_CENTER: return MAP_ID_COMMAND_CENTER;
        case MAP_MASK_BLACKWATER_DOCKS: return MAP_ID_BLACKWATER_DOCKS;
        case MAP_MASK_AQUATOS_SEWERS: return MAP_ID_AQUATOS_SEWERS;
        case MAP_MASK_MARCADIA: return MAP_ID_MARCADIA;
        default: return 0;
    }
}

//--------------------------------------------------------
char * mapGetName(enum MAP_IDS mapId)
{
  switch (mapId)
  {
      case MAP_ID_BAKISI: return uiMsgString(HELP_STRING_ID_BAKISI_ISLES);
      case MAP_ID_HOVEN: return uiMsgString(HELP_STRING_ID_HOVEN_GORGE);
      case MAP_ID_OUTPOST_X12: return uiMsgString(HELP_STRING_ID_OUTPOST_X12);
      case MAP_ID_KORGON: return uiMsgString(HELP_STRING_ID_KORGON_OUTPOST);
      case MAP_ID_METROPOLIS: return uiMsgString(HELP_STRING_ID_METROPOLIS);
      case MAP_ID_BLACKWATER_CITY: return uiMsgString(HELP_STRING_ID_BLACKWATER_CITY);
      case MAP_ID_COMMAND_CENTER: return uiMsgString(HELP_STRING_ID_COMMAND_CENTER);
      case MAP_ID_BLACKWATER_DOCKS: return uiMsgString(HELP_STRING_ID_BLACKWATER_DOCKS);
      case MAP_ID_AQUATOS_SEWERS: return uiMsgString(HELP_STRING_ID_AQUATOS_SEWERS);
      case MAP_ID_MARCADIA: return uiMsgString(HELP_STRING_ID_MARCADIA_PALACE);
      default: return uiMsgString(-1);
  }
}
