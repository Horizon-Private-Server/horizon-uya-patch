#include <libuya/gameplay.h>

// User Settings
void disableCameraShake(void);

// General Game Rules
void disableWeaponPacks(void);
void spawnWeaponPackOnDeath(void);
void v2_Setting(int setting, int FirstPass);
void AutoRespawn(void);
int setGatlingTurretHealth(int value);
void setRespawnTimer_Player(void);
void onGameplayLoad_disableMoby(GameplayHeaderDef_t * gameplay, int mobyId, int ypos);
void onGameplayLoad_hideTeamNodeTurrets(GameplayHeaderDef_t * gameplay, bool disableAll);
void noPostHitInvinc(void);
void onGameplayLoad_disableWeaponCrates(GameplayHeaderDef_t * gameplay);
void onGameplayLoad_disableAmmoPickups(GameplayHeaderDef_t * gameplay);
void radarBlips(void);
void onGameplayLoad_miscRespawnTimers(GameplayHeaderDef_t * gameplay);
void patchHealthPickups(void);
void healthBoxTick(void);
void onGameplayLoad_disableHealthContainer(GameplayHeaderDef_t * gameplay);
void patchDroneSpawning(void);
void runCheckAllNodes(void);
void runSelectNodeTimer(void);
void patchSiegeTimeUp(void);

// Party Game Rules
void survivor(void);

// Other
void disableRespawning(void);

// variable addresses
variableAddressCompact_t vaUpdateWeaponKill;
