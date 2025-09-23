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
#define GLOBAL_CHAT_COOLDOWN_TICKS  (60 * 3)  // 3 second cooldown for global messages

// Static variable to store current config
static PatchConfig_t* currentConfig = NULL;

// Global chat cooldown tracking
static int lastGlobalChatTime = 0;

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
  GameSettings* gs = gameGetSettings();
  Player** players = playerGetAll();
  if (!gs) return;

  int yOff = 100;

  char buf[64];
  int i;
  for (i = 0; i < QuickChatCount; ++i) {

    int pid = QuickChats[i].FromPlayerId;
    int chatid = QuickChats[i].ChatId;

    // draw full or truncated name
    int nameWidth = gfxGetFontWidth(gs->PlayerNames[pid], -1, 0.8);
    if (nameWidth > NAME_MAX_WIDTH)
      snprintf(buf, sizeof(buf), "%.6s. %s", gs->PlayerNames[pid], CHAT_MESSAGES[chatid]);
    else
      snprintf(buf, sizeof(buf), "%s %s", gs->PlayerNames[pid], CHAT_MESSAGES[chatid]);

    // draw team *
    int width = gfxScreenSpaceText(490, yOff + (i * 14), 0.8, 0.8, 0x80FFFFFF, buf, -1, 2, FONT_BOLD);
    if (chatid > 3) {
      int team = gs->PlayerTeams[pid];
      Player* player = players[pid];
      if (player) team = player->mpTeam;
      // Use team colors - adapted for UYA
      u32 teamColor = 0x80FFFFFF;
      if (team == 0) teamColor = 0x80FF0000; // Red
      else if (team == 1) teamColor = 0x800000FF; // Blue
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
void quickChatRun(PatchConfig_t* config)
{
  static int ticksMenuOpen;
  static int quickChatMenu;
  static int init = 0;

  // Store current config for use by receive handler
  currentConfig = config;

  if (!isInGame()) {
    if (init) {
      quickChatMenu = 0;
      ticksMenuOpen = 0;
      memset(QuickChats, 0, sizeof(QuickChats));
      QuickChatCount = 0;
      lastGlobalChatTime = 0; // Reset cooldown when leaving game
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
    lastGlobalChatTime = 0; // Reset cooldown for new game
    init = 1; // Mark as initialized
  }
  if (!gameConfig.grQuickChat) return;
  if (isConfigMenuActive) return;
  
  //
  netInstallCustomMsgHandler(CUSTOM_MSG_PLAYER_QUICK_CHAT, &quickChatOnReceiveRemoteQuickChat);

  // draw quick chats
  if (QuickChatShowTicks) {
    quickChatDraw();
  }

  int menuId = quickChatMenu;
  Player* player = playerGetFromSlot(0);
  if (!player || !player->pMoby) return;

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
        // Global messages (first 4 only: desync, stuck, lag, gg) - check global setting and cooldown
        if (!config->quickChatGlobal) {
          int currentTime = gameGetTime();
          if (currentTime - lastGlobalChatTime >= GLOBAL_CHAT_COOLDOWN_TICKS) {
            // Show locally AND send to network
            quickChatPush(player->mpIndex, chatId);
            
            // Send to network
            void* connection = netGetDmeServerConnection();
            if (connection) {
              struct QuickChatMsg msg;
              msg.FromPlayerId = player->mpIndex;
              msg.ChatId = chatId;
              netBroadcastCustomAppMessage(connection, CUSTOM_MSG_PLAYER_QUICK_CHAT, sizeof(msg), &msg);
            }
            
            lastGlobalChatTime = currentTime;
          }
          // If on cooldown, completely ignore - no local display, no network send
        }
      } else {
        // Team messages (Status & Commands) - check team setting (no cooldown)
        if (!config->quickChatTeam) {
          quickChatBroadcast(player->mpIndex, chatId);
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

