/***************************************************
 * FILENAME :		camera.h
 * DESCRIPTION :
 * AUTHOR :			Daniel "Dnawrkshp" Gerendasy
 */

#ifndef _LIBUYA_CAMERA_H_
#define _LIBUYA_CAMERA_H_

#include "moby.h"
#include "math3d.h"

struct Path { // 0x10
	/* 0x0 */ int nNodes;
	/* 0x4 */ int bPathNormalized;
	/* 0x8 */ char pad[8];
	/* 0x10 */ vec4 nodes[0];
};

struct PolarSm { // 0xc
	/* 0x0 */ float azimuth;
	/* 0x4 */ float elevation;
	/* 0x8 */ float radius;
};

struct CameraFov { // 0x30
	/* 0x00 */ float gain;
	/* 0x04 */ float speed;
	/* 0x08 */ float ideal;
	/* 0x0c */ float actual;
	/* 0x10 */ float damp;
	/* 0x14 */ float limit;
	/* 0x18 */ char changeType;
	/* 0x19 */ char state;
	/* 0x1a */ short int timer;
	/* 0x1c */ float timerInv;
	/* 0x20 */ float init;
	/* 0x24 */ float pad[3];
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

struct UpdateCam { // 0xb0
	/* 0x00 */ mtx3 mtx;
	/* 0x30 */ vec4 pos;
	/* 0x40 */ vec4f rot;
	/* 0x50 */ polar pol;
	/* 0x64 */ vec3 lPos;
	/* 0x70 */ void *control;
	/* 0x74 */ CameraControlActivation activation;
	/* 0x84 */ short int importCameraIdx;
	/* 0x86 */ short int type;
	/* 0x88 */ char subType;
	/* 0x89 */ char bumped;
	/* 0x8a */ short int bumpOff;
	/* 0x8c */ short int funcIdx;
	/* 0x8e */ short int active;
	/* 0x90 */ float fov;
	/* 0x94 */ int gameCamIdx;
	/* 0x98 */ float prevExternalMoveZ;
	/* 0x9c */ int pad[2];
};

struct CameraStatics { // 0xf0
	/* 0x00 */ vec3 heroNoJump;
	/* 0x0c */ float heroLastZ;
	/* 0x10 */ float heroLastZSpeed;
	/* 0x20 */ vec4 heroUp;
	/* 0x30 */ vec4 heroUpActual;
	/* 0x40 */ vec4 heroUpLast;
	/* 0x50 */ vec4 heroUpSpeed;
	/* 0x60 */ vec4 heroLastPos;
	/* 0x70 */ vec4 heroMoveVec;
	/* 0x80 */ vec4 heroMoveVec2D;
	/* 0x90 */ vec4 heroMoveVecUp;
	/* 0xa0 */ float heroSpeed;
	/* 0xa4 */ float heroSpeed2D;
	/* 0xa8 */ float heroSpeedUp;
	/* 0xac */ float heroLastRotZ[5];
	/* 0xc0 */ int hotspot;
	/* 0xc4 */ moby *pCamColl;
	/* 0xc8 */ float fadeSpeed;
	/* 0xcc */ float fadeIdeal;
	/* 0xd0 */ int fadeTimer;
	/* 0xd4 */ float flashInSpeed;
	/* 0xd8 */ float flashOutSpeed;
	/* 0xdc */ float flashIdeal;
	/* 0xe0 */ int flashTimer;
	/* 0xe4 */ moby *boss;
	/* 0xe8 */ int bossTimer;
};

struct CamBlenderPosQuat { // 0x60
	/* 0x00 */ float quatInterpFac;
	/* 0x04 */ float quatInterpAdd;
	/* 0x08 */ float reqQuatInterpAdd;
	/* 0x0c */ float reqQuatInterpInit;
	/* 0x10 */ float posInterpFac;
	/* 0x14 */ float posInterpAdd;
	/* 0x18 */ float reqPosInterpAdd;
	/* 0x1c */ float reqPosInterpInit;
	/* 0x20 */ quat orgQuat;
	/* 0x30 */ vec4 orgPos;
	/* 0x40 */ vec4 pos;
	/* 0x50 */ quat q;
};

struct CamBlenderPolarQuat { // 0x70
	/* 0x00 */ PolarSm orgPol;
	/* 0x0c */ int interpFrames;
	/* 0x10 */ float interpMaxInv;
	/* 0x14 */ int reqInterpFrames;
	/* 0x20 */ vec4 fwd;
	/* 0x30 */ vec4 up;
	/* 0x40 */ VECTOR orgQuat;
	/* 0x50 */ vec4 pos;
	/* 0x60 */ VECTOR q;
};

struct CamBlenderData { // 0xd0
	/* 0x00 */ CamBlenderPosQuat posData;
	/* 0x60 */ CamBlenderPolarQuat polarData;
};

struct CamBlender { // 0xe0
	/* 0x00 */ short int state;
	/* 0x02 */ char type;
	/* 0x03 */ char reqType;
	/* 0x10 */ CamBlenderData blendData;
};

struct CameraWidget { // 0x20
	/* 0x00 */ UpdateCam *pCam;
	/* 0x04 */ moby *semaphore;
	/* 0x08 */ float closest;
	/* 0x0c */ float interp;
	/* 0x10 */ void *preFunc;
	/* 0x14 */ int pad_14[3];
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

struct CameraHeroData { // 0x150
	/* 0x000 */ mtx4 mtx;
	/* 0x040 */ vec4 pos;
	/* 0x050 */ vec4f rot;
	/* 0x060 */ vec4 cg;
	/* 0x070 */ vec4 moveActualFromExternal;
	/* 0x080 */ vec4 groundGravity;
	/* 0x090 */ vec4 jumpGravity;
	/* 0x0a0 */ vec4 sphereCenter;
	/* 0x0b0 */ mtx3 fpsCamMtx;
	/* 0x0e0 */ vec4 fpsCamPos;
	/* 0x0f0 */ Moby *pMoby;
	/* 0x0f4 */ Moby *vehicleMoby;
	/* 0x0f8 */ Moby *groundMoby;
	/* 0x0fc */ int desiredCam;
	/* 0x100 */ int state;
	/* 0x104 */ int stateType;
	/* 0x108 */ int previousState;
	/* 0x10c */ float moveSpeed;
	/* 0x110 */ float moveSpeed2D;
	/* 0x114 */ float groundDist;
	/* 0x118 */ float groundWaterHeight;
	/* 0x11c */ int groundOnGood;
	/* 0x120 */ short int groundOffAny;
	/* 0x122 */ short int groundOffGood;
	/* 0x124 */ short int groundMagnetic;
	/* 0x126 */ short int lockOnStrafing;
	/* 0x128 */ short int jumpFramesToLand;
	/* 0x12a */ char jumpDescend;
	/* 0x12b */ char critterMode;
	/* 0x12c */ char multiplayer;
	/* 0x12d */ char fpsActive;
	/* 0x12e */ char hotSpotLava;
	/* 0x12f */ char hotSpotDeathSand;
	/* 0x130 */ char hotSpotQuickSand;
	/* 0x131 */ char hotSpotIceWater;
	/* 0x132 */ char hotSpotWater;
	/* 0x133 */ char aiFollowingMe;
	/* 0x134 */ PAD *pPad;
	/* 0x138 */ Moby *pSwingTargetMoby;
	/* 0x13c */ float swingForwardAng;
	/* 0x140 */ float swingIdealRadius;
	/* 0x144 */ int timersLedgeCamAdj;
	/* 0x148 */ float ledgeWallAngZ;
	/* 0x14c */ int EOPtime;
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
/* 0x000 */ VECTOR pos;
/* 0x010 */ VECTOR rot;
/* 0x020 */ struct CameraShake shake;
/* 0x030 */ struct CameraShake shakeFwd;
/* 0x040 */ struct CameraShake shakeTilt;
/* 0x050 */ struct UpdateCam* pCurrentUpdCam;
/* 0x054 */ struct UpdateCam* pLastUpdCam;
/* 0x060 */ struct CameraStatics camStatics;
/* 0x150 */ struct CameraHeroData camHeroData;
/* 0x2a0 */ struct CamBlender blender;
/* 0x380 */ mtx3 uMtx;
/* 0x3b0 */ mtx4 bsMtx;
/* 0x3f0 */ struct CameraWidget widget;
/* 0x410 */ struct CameraExternal external;
/* 0x420 */ struct CameraFov fov;
/* 0x450 */ int CamUnderWater;
/* 0x454 */ int camTimer;
/* 0x458 */ int disableBlendTimer;
/* 0x45c */ int pad;
} GameCamera;

Camera_t *cameraGetCamera(void);
GameCamera* cameraGetGameCamera(int cameraIndex);
void cameraSetFov(float fov, float gain, float damp, float limit, int cameraIndex, int frames, int type);

#endif // _LIBUYA_CAMERA_H_
