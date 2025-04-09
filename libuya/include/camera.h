/***************************************************
 * FILENAME :		camera.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_CAMERA_H_
#define _LIBUYA_CAMERA_H_

#include "moby.h"
#include "math3d.h"

struct CameraFov { // 0x30
	/*   0 */ float gain;
	/*   4 */ float speed;
	/*   8 */ float ideal;
	/*   c */ float actual;
	/*  10 */ float damp;
	/*  14 */ float limit;
	/*  18 */ char changeType;
	/*  19 */ char state;
	/*  1a */ short int timer;
	/*  1c */ float timerInv;
	/*  20 */ float init;
	/*  24 */ float pad[3];
};

struct CameraControlActivation {
	/*   0 */ int activationType;
	/*   4 */ float blendSpeed;
	/*   8 */ char priority;
	/*   9 */ char activate;
	/*   a */ short int deactivate;
	/*   c */ short int repCam;
	/*   e */ short int orgCam;
};

struct PolarSm {
	/*   0 */ float azimuth;
	/*   4 */ float elevation;
	/*   8 */ float radius;
};

struct Polar {
	/*   0 */ float azimuth;
	/*   4 */ float elevation;
	/*   8 */ float radius;
	/*   c */ float rotY;
	/*  10 */ float rotZ;
};

struct UpdateCam {
	/*   0 */ VECTOR mtx[3];
	/*  30 */ VECTOR pos;
	/*  40 */ VECTOR rot;
	/*  50 */ struct Polar pol;
	/*  64 */ float lPos[3];
	/*  70 */ void* control;
	/*  74 */ struct CameraControlActivation activation;
	/*  84 */ short int importCameraIdx;
	/*  86 */ short int type;
	/*  88 */ char subType;
	/*  89 */ char bumped;
	/*  8a */ short int bumpOff;
	/*  8c */ short int funcIdx;
	/*  8e */ short int active;
	/*  90 */ float fov;
	/*  94 */ int gameCamIdx;
	/*  98 */ float prevExternalMoveZ;
	/*  9c */ int pad[2];
};

struct CameraStatics {
	/*   0 */ float heroNoJump[3];
	/*   c */ float heroLastZ;
	/*  10 */ float heroLastZSpeed;
	/*  20 */ VECTOR heroUp;
	/*  30 */ VECTOR heroUpActual;
	/*  40 */ VECTOR heroUpLast;
	/*  50 */ VECTOR heroUpSpeed;
	/*  60 */ VECTOR heroLastPos;
	/*  70 */ VECTOR heroMoveVec;
	/*  80 */ VECTOR heroMoveVec2D;
	/*  90 */ VECTOR heroMoveVecUp;
	/*  a0 */ float heroSpeed;
	/*  a4 */ float heroSpeed2D;
	/*  a8 */ float heroSpeedUp;
	/*  ac */ float heroLastRotZ[5];
	/*  c0 */ int hotspot;
	/*  c4 */ Moby* pCamColl;
	/*  c8 */ float fadeSpeed;
	/*  cc */ float fadeIdeal;
	/*  d0 */ int fadeTimer;
	/*  d4 */ float flashInSpeed;
	/*  d8 */ float flashOutSpeed;
	/*  dc */ float flashIdeal;
	/*  e0 */ int flashTimer;
	/*  e4 */ Moby* boss;
	/*  e8 */ int bossTimer;
};

struct CamBlenderPosQuat {
	/*   0 */ float quatInterpFac;
	/*   4 */ float quatInterpAdd;
	/*   8 */ float reqQuatInterpAdd;
	/*   c */ float reqQuatInterpInit;
	/*  10 */ float posInterpFac;
	/*  14 */ float posInterpAdd;
	/*  18 */ float reqPosInterpAdd;
	/*  1c */ float reqPosInterpInit;
	/*  20 */ VECTOR orgQuat;
	/*  30 */ VECTOR orgPos;
	/*  40 */ VECTOR pos;
	/*  50 */ VECTOR q;
};

struct CamBlenderPolarQuat {
	/*   0 */ struct PolarSm orgPol;
	/*   c */ int interpFrames;
	/*  10 */ float interpMaxInv;
	/*  14 */ int reqInterpFrames;
	/*  20 */ VECTOR fwd;
	/*  30 */ VECTOR up;
	/*  40 */ VECTOR orgQuat;
	/*  50 */ VECTOR pos;
	/*  60 */ VECTOR q;
};

struct CamBlenderData {
	/*   0 */ struct CamBlenderPosQuat posData;
	/*  60 */ struct CamBlenderPolarQuat polarData;
};

struct CamBlender {
	/*   0 */ short int state;
	/*   2 */ char type;
	/*   3 */ char reqType;
	/*  10 */ struct CamBlenderData blendData;
};

struct CameraWidget {
	/*   0 */ struct UpdateCam* pCam;
	/*   4 */ Moby* semaphore;
	/*   8 */ float closest;
	/*   c */ float interp;
	/*  10 */ void* preFunc;
	/*  14 */ int pad[3];
};

struct CameraShake {
	/*   0 */ float strength;
	/*   4 */ float adjust;
	/*   8 */ int time;
	/*   c */ int div;
};

struct CameraExternal {
	/*   0 */ VECTOR move;
};

struct CameraHeroData {
	/*   0 */ MATRIX mtx;
	/*  40 */ VECTOR pos;
	/*  50 */ VECTOR rot;
	/*  60 */ VECTOR cg;
	/*  70 */ VECTOR moveActualFromExternal;
	/*  80 */ VECTOR groundGravity;
	/*  90 */ VECTOR jumpGravity;
	/*  a0 */ VECTOR sphereCenter;
	/*  b0 */ VECTOR fpsCamMtx[3];
	/*  e0 */ VECTOR fpsCamPos;
	/*  f0 */ Moby * pMoby;
	/*  f4 */ Moby * vehicleMoby;
	/*  f8 */ Moby * groundMoby;
	/*  fc */ int desiredCam;
	/* 100 */ int state;
	/* 104 */ int stateType;
	/* 108 */ int previousState;
	/* 10c */ float moveSpeed;
	/* 110 */ float moveSpeed2D;
	/* 114 */ float groundDist;
	/* 118 */ float groundWaterHeight;
	/* 11c */ int groundOnGood;
	/* 120 */ short int groundOffAny;
	/* 122 */ short int groundOffGood;
	/* 124 */ short int groundMagnetic;
	/* 126 */ short int lockOnStrafing;
	/* 128 */ short int jumpFramesToLand;
	/* 12a */ char jumpDescend;
	/* 12b */ char critterMode;
	/* 12c */ char multiplayer;
	/* 12d */ char fpsActive;
	/* 12e */ char hotSpotLava;
	/* 12f */ char hotSpotDeathSand;
	/* 130 */ char hotSpotQuickSand;
	/* 131 */ char hotSpotIceWater;
	/* 132 */ char hotSpotWater;
	/* 133 */ char aiFollowingMe;
	/* 134 */ void* pPad;
	/* 138 */ struct Moby * pSwingTargetMoby;
	/* 13c */ float swingForwardAng;
	/* 140 */ float swingIdealRadius;
	/* 144 */ int timersLedgeCamAdj;
	/* 148 */ float ledgeWallAngZ;
	/* 14c */ int EOPtime;
	/* 150 */ void* pPath;
	/* 154 */ int pad[3];
};

typedef struct CAMERA { // 0x1d0
/* 0x000 */ mtx4 rMtx;
/* 0x040 */ mtx4 fMtx;
/* 0x080 */ mtx4 nfMtx;
/* 0x0c0 */ mtx4 hMtx;
/* 0x100 */ mtx4 hsMtx;
/* 0x140 */ vec4 pos;
/* 0x150 */ vec4f rot;
/* 0x160 */ mtx3 uMtx;
/* 0x190 */ mtx4 uvMtx;
} Camera_t;

typedef struct GameCamera {
/*   0 */ VECTOR pos;
/*  10 */ VECTOR rot;
/*  20 */ struct CameraShake shake;
/*  30 */ struct CameraShake shakeFwd;
/*  40 */ struct CameraShake shakeTilt;
/*  50 */ struct UpdateCam* pCurrentUpdCam;
/*  54 */ struct UpdateCam* pLastUpdCam;
/*  60 */ struct CameraStatics camStatics;
/* 150 */ struct CameraHeroData camHeroData;
/* 2b0 */ struct CamBlender blender;
/* 390 */ VECTOR uMtx[3];
/* 3c0 */ MATRIX bsMtx;
/* 400 */ struct CameraWidget widget;
/* 420 */ // struct CameraExternal external;
/*
	UYA's Camera Struct is different from DLs.
	Unsure if I have it correct
	Commented CameraExternal
	Added `int pad[3]` to  CameraWidget
*/
/* 420 */ struct CameraFov fov;
/* 450 */ int CamUnderWater;
/* 454 */ int camTimer;
/* 458 */ int disableBlendTimer;
} GameCamera;

Camera_t *cameraGetCamera(void);
GameCamera* cameraGetGameCamera(int cameraIndex);

#endif // _LIBUYA_CAMERA_H_
