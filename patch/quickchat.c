#include <tamtypes.h>
#include <libuya/game.h>
#include <libuya/gamesettings.h>
#include <libuya/stdio.h>
#include <libuya/string.h>
#include <libuya/team.h>
#include <libuya/ui.h>
#include <libuya/player.h>
#include <libuya/pad.h>
#include <libuya/time.h>
#include <libuya/graphics.h>
#include <libuya/net.h>
#include <libuya/utils.h>
#include "messageid.h"
#include "config.h"
#include "include/config.h"

#define QUICK_CHAT_MSG_ID_START     (50)
#define QUICK_CHAT_MAX_MSGS         (5)
#define QUICK_CHAT_SHOW_TICKS       (60 * 5)
#define MENU_OPEN_TICKS             (60 * 3)
#define NAME_MAX_WIDTH              (60)

// Static variable to store current config
static PatchConfig_t* currentConfig = NULL;

// Global caches for performance optimization
static GameSettings* cachedGameSettings = NULL;
static Player** cachedAllPlayers = NULL;
static Player* cachedLocalPlayer = NULL;

struct QuickChatMsg
{
  char FromPlayerId;
  char ChatId;
};

enum QuickChatIds
{
  // POST GAME
  QUICK_CHAT_GG,
  QUICK_CHAT_FF,
  QUICK_CHAT_REMATCH,
  QUICK_CHAT_ONE_MORE_GAME,
  // REACTIONS
  QUICK_CHAT_WHAT_A_SAVE,
  QUICK_CHAT_WOW,
  QUICK_CHAT_CLOSE_ONE,
  QUICK_CHAT_NICE_SHOT,
  // STATUS
  QUICK_CHAT_PUSHING,
  QUICK_CHAT_DEFENDING,
  QUICK_CHAT_IN_POSITION,
  QUICK_CHAT_NEED_HEALTH,
  // COMMANDS
  QUICK_CHAT_GET_THE_FLAG,
  QUICK_CHAT_PUSH_UP,
  QUICK_CHAT_STAY_BACK,
  QUICK_CHAT_HELP_ME,

  // COUNT
  QUICK_CHAT_COUNT
};

extern PatchGameConfig_t gameConfig;
extern int isConfigMenuActive;

char* CHAT_GROUPS[4] = {
  "Post Game",
  "Status",
  "Enemy",
  "Commands"
};

char* CHAT_SHORTNAMES[QUICK_CHAT_COUNT] = {
  "Desync",
  "Stuck",
  "Lag",
  "GG",
  "In position",
  "V2",
  "Health",
  "Defend",
  "Sneak",
  "V2",
  "Low health",
  "Flag",
  "Middle",
  "Push",
  "Help",
  "Defend"
};

char* CHAT_MESSAGES[QUICK_CHAT_COUNT] = {
  "Desync",
  "Im stuck",
  "Lag",
  "GG",
  "In position!",
  "I have V2!",
  "I need health!",
  "Defending!",
  "Enemy sneak!",
  "Enemy has V2!",
  "Enemy low health!",
  "Enemy has flag!",
  "Go middle!",
  "Push up!",
  "Help me!",
  "Defend base!"
};

int CHAT_GRID_X_OFF[4] = {
  0,     // Triangle (top)
  26,    // Circle (right) 
  0,     // Cross (bottom)
  -26    // Square (left)
};

int CHAT_GRID_Y_OFF[4] = {
  -26,   // Triangle (top)
  0,     // Circle (right)
  26,    // Cross (bottom) 
  0      // Square (left)
};

int PAD_INDICES[4] = {
  PAD_UP,
  PAD_RIGHT,
  PAD_DOWN,
  PAD_LEFT
};

int QuickChatCount = 0;
int QuickChatShowTicks = 0;
struct QuickChatMsg QuickChats[QUICK_CHAT_MAX_MSGS];

// Forward declare static cache reset function
void quickChatResetCaches(void);

//--------------------------------------------------------------------------
char * quickChatGetMsgString(int fragMsgId)
{
  if (fragMsgId < QUICK_CHAT_MSG_ID_START) return NULL;

  switch (fragMsgId - QUICK_CHAT_MSG_ID_START)
  {
    case QUICK_CHAT_GG: return "Desync";
    case QUICK_CHAT_FF: return "Im stuck";
    case QUICK_CHAT_REMATCH: return "Lag";
    case QUICK_CHAT_ONE_MORE_GAME: return "GG";
    case QUICK_CHAT_CLOSE_ONE: return "In position!";
    case QUICK_CHAT_WOW: return "I have V2!";
    case QUICK_CHAT_WHAT_A_SAVE: return "I need health!";
    case QUICK_CHAT_NICE_SHOT: return "Defending!";
    case QUICK_CHAT_PUSHING: return "Enemy sneak!";
    case QUICK_CHAT_DEFENDING: return "Enemy has V2!";
    case QUICK_CHAT_NEED_HEALTH: return "Enemy low health!";
    case QUICK_CHAT_IN_POSITION: return "Enemy has flag!";
    case QUICK_CHAT_GET_THE_FLAG: return "Go middle!";
    case QUICK_CHAT_HELP_ME: return "Push up!";
    case QUICK_CHAT_PUSH_UP: return "Help me!";
    case QUICK_CHAT_STAY_BACK: return "Defend base!";
  }
  return NULL;
}


//--------------------------------------------------------------------------
void quickChatDraw(void)
{
  // Use global caches
  if (!cachedGameSettings) cachedGameSettings = gameGetSettings();
  if (!cachedAllPlayers) cachedAllPlayers = playerGetAll();
  
  if (!cachedGameSettings) return;

  int yOff = 100;
  char buf[64];
  int i;
  
  for (i = 0; i < QuickChatCount; ++i) {
    int pid = QuickChats[i].FromPlayerId;
    int chatid = QuickChats[i].ChatId;

    // Simple name truncation without expensive gfxGetFontWidth call
    if (strlen(cachedGameSettings->PlayerNames[pid]) > 8)
      snprintf(buf, sizeof(buf), "%.6s. %s", cachedGameSettings->PlayerNames[pid], CHAT_MESSAGES[chatid]);
    else
      snprintf(buf, sizeof(buf), "%s %s", cachedGameSettings->PlayerNames[pid], CHAT_MESSAGES[chatid]);

    // draw text
    int width = gfxScreenSpaceText(490, yOff + (i * 14), 0.8, 0.8, 0x80FFFFFF, buf, -1, 2, FONT_BOLD);
    
    // draw team asterisk for team messages only
    if (chatid > 3) {
      int team = cachedGameSettings->PlayerTeams[pid];
      if (cachedAllPlayers && cachedAllPlayers[pid]) {
        team = cachedAllPlayers[pid]->mpTeam;
      }
      // Use team colors
      u32 teamColor = (team == 0) ? 0x80FF0000 : 0x800000FF; // Red : Blue
      gfxScreenSpaceText(490 + (490 - width) - 4, yOff + (i * 14), 1, 1, teamColor, "*", -1, 2, FONT_BOLD);
    }
  }
}

//--------------------------------------------------------------------------
void quickChatPush(int sourcePlayerId, int chatId)
{
  QuickChatShowTicks = QUICK_CHAT_SHOW_TICKS;

  if (QuickChatCount == QUICK_CHAT_MAX_MSGS) {

    // move down
    memmove(QuickChats, &QuickChats[1], sizeof(struct QuickChatMsg) * (QUICK_CHAT_MAX_MSGS - 1));

    QuickChats[QuickChatCount-1].FromPlayerId = sourcePlayerId;
    QuickChats[QuickChatCount-1].ChatId = chatId;
  } else {
    QuickChats[QuickChatCount].FromPlayerId = sourcePlayerId;
    QuickChats[QuickChatCount].ChatId = chatId;
    QuickChatCount++;
  }
}

//--------------------------------------------------------------------------
int quickChatOnReceiveRemoteQuickChat(void* connection, void* data)
{
  if (!isInGame()) return sizeof(struct QuickChatMsg);

  struct QuickChatMsg msg;
  memcpy(&msg, data, sizeof(msg));

  // Check personal settings first (using inverted logic: 0 = enabled, 1 = disabled)
  if (currentConfig) {
    if (msg.ChatId < 4) {
      // Global messages (first 4 only: desync, stuck, lag, gg) - check global setting
      if (currentConfig->quickChatGlobal) return sizeof(struct QuickChatMsg);
    } else {
      // Team messages (everything else) - check team setting
      if (currentConfig->quickChatTeam) return sizeof(struct QuickChatMsg);
      
      // For team messages, also check if sender is on same team
      Player** players = playerGetAll();
      Player* fromPlayer = players[msg.FromPlayerId];
      if (!fromPlayer) return sizeof(struct QuickChatMsg);

      Player* localPlayer = playerGetFromSlot(0);
      if (!localPlayer || !localPlayer->pMoby) return sizeof(struct QuickChatMsg);
      if (localPlayer->mpTeam != fromPlayer->mpTeam) return sizeof(struct QuickChatMsg);
    }
  }

  quickChatPush(msg.FromPlayerId, msg.ChatId);
  return sizeof(struct QuickChatMsg);
}

//--------------------------------------------------------------------------
void quickChatBroadcast(int playerId, int chatId)
{
  if (!isInGame()) return;

  struct QuickChatMsg msg;
  msg.FromPlayerId = playerId;
  msg.ChatId = chatId;

  // local
  quickChatPush(msg.FromPlayerId, msg.ChatId);

  // broadcast
  void* connection = netGetDmeServerConnection();
  if (connection) {
    netBroadcastCustomAppMessage(connection, CUSTOM_MSG_PLAYER_QUICK_CHAT, sizeof(msg), &msg);
  }
}

//--------------------------------------------------------------------------
void quickChatResetCaches(void)
{
  // Reset all caches when game state changes
  cachedGameSettings = NULL;
  cachedAllPlayers = NULL;
  cachedLocalPlayer = NULL;
}

//--------------------------------------------------------------------------
void quickChatRun(PatchConfig_t* config)
{
  static int ticksMenuOpen;
  static int quickChatMenu;
  static int init = 0;
  static int msgHandlerInstalled = 0;

  // Store current config for use by receive handler
  currentConfig = config;

  if (!isInGame()) {
    if (init) {
      quickChatMenu = 0;
      ticksMenuOpen = 0;
      memset(QuickChats, 0, sizeof(QuickChats));
      QuickChatCount = 0;
      msgHandlerInstalled = 0; // Reset handler flag when leaving game
      quickChatResetCaches(); // Clear all caches when leaving game
      init = 0;
    }
    return;
  }
  
  // Clear chat history when entering a new game
  if (!init) {
    quickChatMenu = 0;
    ticksMenuOpen = 0;
    memset(QuickChats, 0, sizeof(QuickChats));
    QuickChatCount = 0;
    msgHandlerInstalled = 0; // Reset handler flag for new game
    quickChatResetCaches(); // Clear all caches for new game
    init = 1; // Mark as initialized
  }
  if (!gameConfig.grQuickChat) return;
  if (isConfigMenuActive) return;
  
  // Only install message handler once per game session
  if (!msgHandlerInstalled) {
    netInstallCustomMsgHandler(CUSTOM_MSG_PLAYER_QUICK_CHAT, &quickChatOnReceiveRemoteQuickChat);
    msgHandlerInstalled = 1;
  }

  // draw quick chats
  if (QuickChatShowTicks) {
    quickChatDraw();
  }

  int menuId = quickChatMenu;
  
  // Use global cache for local player
  if (!cachedLocalPlayer) {
    cachedLocalPlayer = playerGetFromSlot(0);
    if (!cachedLocalPlayer || !cachedLocalPlayer->pMoby) return;
  }

  // draw
  if (menuId) {
    // print group
    char * group = CHAT_GROUPS[menuId-1];
    gfxScreenSpaceText(60, 300, 1, 1, 0x80FFFFFF, group, -1, 4, FONT_BOLD);

    // print options
    int i;
    for (i = 0; i < 4; ++i) {
      char * name = CHAT_SHORTNAMES[(menuId-1)*4 + i];
      gfxScreenSpaceText(60 + CHAT_GRID_X_OFF[i], 352 + CHAT_GRID_Y_OFF[i], 0.7, 0.75, 0x80202020, name, -1, 4, FONT_BOLD);
    }
  }

  // input
  int pad = -1;
  int i;
  for (i = 0; i < 4; ++i) {
    if (padGetButtonDown(0, PAD_INDICES[i]) > 0) {
      pad = i;
      break;
    }
  }

  if (pad >= 0) {

    // send chat or open menu
    if (menuId) {
      int chatId = (menuId - 1)*4 + pad;
      
      // Check personal settings before sending (using inverted logic: 0 = enabled, 1 = disabled)
      if (chatId < 4) {
        // Global messages (first 4 only: desync, stuck, lag, gg) - check global setting
        if (!config->quickChatGlobal) {
          quickChatBroadcast(cachedLocalPlayer->mpIndex, chatId);
        }
      } else {
        // Team messages (Status & Commands) - check team setting
        if (!config->quickChatTeam) {
          quickChatBroadcast(cachedLocalPlayer->mpIndex, chatId);
        }
      }
      
      quickChatMenu = 0;
    } else {
      quickChatMenu = pad + 1;
      ticksMenuOpen = MENU_OPEN_TICKS;
    }
  }
  
  // ticks
  if (ticksMenuOpen) {
    ticksMenuOpen--;
    if (!ticksMenuOpen) quickChatMenu = 0;
  }

  if (QuickChatShowTicks) --QuickChatShowTicks;
}

