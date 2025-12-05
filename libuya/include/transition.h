#ifndef _LIBUYA_TRANSITION_H_
#define _LIBUYA_TRANSITION_H_

#ifdef UYA_PAL
#define TRANSITION_MULTIPLAYER ((TransitionData_Multiplayer_t *)0x00352ce0)
#else
#define TRANSITION_MULTIPLAYER ((TransitionData_Multiplayer_t *)0x00352de0)
#endif

typedef struct t_TransitionData_Lobby { // 0x18
	/* 0x00 */ int m_State;
	/* 0x04 */ int m_Timer;
	/* 0x08 */ int m_Direction;
	/* 0x0c */ float m_PropBubblesTheta;
	/* 0x10 */ int m_nMapToRender;
	/* 0x14 */ float m_fLocalFadeFactor;
} TransitionData_Lobby_t;

typedef struct t_TransitionData_Multiplayer { // 0xa80
	/* 0x000 */ char m_bDefaultArray[11];
	/* 0x00c */ int m_nMapIDUsed;
	/* 0x010 */ int m_Timer;
	/* 0x014 */ int m_State;
	/* 0x020 */ float m_vCamPos[4];
	/* 0x030 */ float m_vCamRot[4];
	/* 0x040 */ char m_sMapNames[10][32];
	/* 0x180 */ char m_sGameTypeString[32];
	/* 0x1a0 */ char m_sTimed[32];
	/* 0x1c0 */ char m_sNumberOfCTFS[32];
	/* 0x1e0 */ char m_sPlayerNames[32];
	/* 0x200 */ char m_sVehicles[32];
	/* 0x220 */ char m_sNodes[32];
	/* 0x240 */ char m_sBaseDefenses[32];
	/* 0x260 */ char m_sUnlimitedAmmo[32];
	/* 0x280 */ char m_sChargeBootsAlways[32];
	/* 0x2a0 */ char m_sSpawnWithWeapons[32];
	/* 0x2c0 */ char m_sWeaponsString[32];
	/* 0x2e0 */ char m_sSubTitles[11][32];
	/* 0x440 */ char m_sValues[11][32];
	/* 0x5a0 */ char m_sTipToDisplay[256];
	/* 0x6a0 */ char m_sMapName[32];
	/* 0x6c0 */ char m_sGameName[32];
	/* 0x6e0 */ char m_sWeaponsLbl[32];
	/* 0x700 */ char m_sVehiclesLbl[32];
	/* 0x720 */ char m_sGameLbls[7][32];
	/* 0x800 */ char m_sGameValues[7][32];
	/* 0x8e0 */ int m_numGameSettings;
	/* 0x8e4 */ char m_sGameTypeLbls[6][32];
	/* 0x9a4 */ char m_sGameTypeValues[6][32];
	/* 0xa64 */ int m_numGameTypes;
	/* 0xa68 */ char m_displayBecauseValid;
	/* 0xa69 */ char m_displayBecauseNotLobby;
	/* 0xa6c */ int m_nTipToShow;
	/* 0xa70 */ int m_nTimeInMinutes;
	/* 0xa74 */ char m_bWeaponEnabledSniper;
	/* 0xa75 */ char m_bWeaponEnabledMachineGunN60;
	/* 0xa76 */ char m_bWeaponEnabledShotGunBlitz;
	/* 0xa77 */ char m_bWeaponEnabledRocketLauncher;
	/* 0xa78 */ char m_bWeaponEnabledBombLauncherGravity;
	/* 0xa79 */ char m_bWeaponEnabledMineGlove;
	/* 0xa7a */ char m_bWeaponEnabledSheepinator;
	/* 0xa7b */ char m_bWeaponEnabledLavaGun;
} TransitionData_Multiplayer_t;

TransitionData_Multiplayer_t * transGetData(void);

#endif // _LIBUYA_TRANSITION_H_