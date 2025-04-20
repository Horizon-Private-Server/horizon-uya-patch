#include <libuya/gameplay.h>

// User Settings
void disableCameraShake(void);

// General Game Rules
void disableWeaponPacks(void);
void spawnWeaponPackOnDeath(void);
void v2_Setting(int setting, int FirstPass);
void AutoRespawn(void);
int setGatlingTurretHealth(int value);
int deleteSiegeNodeTurrets(void);
void deleteNodeTurretsUpdate(void);
void setRespawnTimer_Player(void);
void onGameplayLoad_disableMoby(GameplayHeaderDef_t * gameplay, int mobyId, int ypos);
int keepBaseHealthPadActive(void);
void noPostHitInvinc(void);
void onGameplayLoad_disableWeaponCrates(GameplayHeaderDef_t * gameplay);
void onGameplayLoad_disableAmmoPickups(GameplayHeaderDef_t * gameplay);
void healthbars(void);
void radarBlips(void);
void onGameplayLoad_miscRespawnTimers(GameplayHeaderDef_t * gameplay);
void respawnInvincTimer(void);
void onGameplayLoad_disableHealthContainer(GameplayHeaderDef_t * gameplay);
void onGameplayLoad_destructableBridges(GameplayHeaderDef_t * gameplay);
void destructableBridges(void);
void onGameplayLoad_disableDrones(GameplayHeaderDef_t * gameplay);

// Party Game Rules
void survivor(void);
void chargebootForever(void);
void modifyWeaponTweakers(void);

// Other
void disableRespawning(void);

// variable addresses
VariableAddress_t vaUpdateWeaponKill;

typedef struct Tweaker_GravityBomb {
	float maxThrowDist;
	float minThrowDist;
	float gravity;
	float maxThrowSpeed;
} Tweaker_GravityBomb_t;
