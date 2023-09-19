#include <libuya/gameplay.h>

// User Settings
void disableCameraShake(void);

// General Game Rules
void disableWeaponPacks(void);
void spawnWeaponPackOnDeath(void);
void v2_Setting(int setting);
int disableHealthBoxes(void);
void AutoRespawn(void);
int setGattlingTurretHealth(int value);
void vampireLogic(float healRate);
void setRespawnTimer(void);
void onGameplayLoad_disableDrones(GameplayHeaderDef_t * gameplay);
int keepBaseHealthPadActive(void);
void noPostHitInvinc(void);

// Party Game Rules
void survivor(void);
void chargebootForever(void);

// Other
void disableRespawning(void);
