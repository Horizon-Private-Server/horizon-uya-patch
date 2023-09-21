#include <libuya/gameplay.h>

// User Settings
void disableCameraShake(void);

// General Game Rules
void disableWeaponPacks(void);
void spawnWeaponPackOnDeath(void);
void v2_Setting(int setting, int FirstPass);
int disableHealthBoxes(void);
void AutoRespawn(void);
int setGattlingTurretHealth(int value);
int deleteSiegeNodeTurrets(void);
void vampireLogic(float healRate);
void setRespawnTimer(void);
void onGameplayLoad_disableDrones(GameplayHeaderDef_t * gameplay);
int keepBaseHealthPadActive(void);
void noPostHitInvinc(void);
void onGameplayLoad_removeWeaponCrates(GameplayHeaderDef_t * gameplay);
void onGameplayLoad_removeAmmoPickups(GameplayHeaderDef_t * gameplay);

// Party Game Rules
void survivor(void);
void chargebootForever(void);

// Other
void disableRespawning(void);
