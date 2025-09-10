#include <tamtypes.h>
#include <string.h>

#include <libuya/player.h>
#include <libuya/math3d.h>
#include <libuya/moby.h>
#include <libuya/string.h>
#include <libuya/gamesettings.h>
#include <libuya/stdio.h>
#include <libuya/game.h>
#include <libuya/stdlib.h>
#include <libuya/utils.h>
#include "messageid.h"
#include "config.h"
#include <libuya/net.h>
#include "interop/playersync.h"

#define PLAYER_SYNC_DATAS_PTR     (*(PlayerSyncPlayerData_t**)0x000CFFB0)
#define CMD_BUFFER_SIZE           (8)

// TODO players don't exit from vehicles for remote players, vehicles move really weird, vehicles completely broken

typedef struct PlayerSyncStateUpdatePacked
{
  float Position[3];
  float Rotation[3];
  int GameTime;
  int GroundMobyUID;
  short CameraDistance;
  short CameraYaw;
  short CameraPitch;
  short NoInput;
  float Health;
  u8 PadBits0;
  u8 PadBits1;
  u8 MoveX;
  u8 MoveY;
  u8 GadgetId;
//  char GadgetLevel;
  int State;
  char StateId;
  struct {
    char PlayerIdx : 5;
    char Flags : 3;
  };
  u8 CmdId;
} PlayerSyncStateUpdatePacked_t;

typedef struct PlayerSyncStateUpdateUnpacked
{
  VECTOR Position;
  VECTOR Rotation;
  Moby* GroundMoby;
  int GameTime;
  float CameraDistance;
  float CameraHeight;
  float CameraYaw;
  float CameraPitch;
  float Health;
  short NoInput;
  u16 PadBits;  // left 8 bits == padbits1 right 8 bits == padbits0
  u8 MoveX;  // left stick intensity Y axis
  u8 MoveY;  // left stick intensity X axis
  u8 GadgetId;
  //char GadgetLevel; // WONT DO uya doesn't care
  int State;
  char StateId;
  char PlayerIdx;
  char Valid;
  u8 CmdId;
} PlayerSyncStateUpdateUnpacked_t;

typedef struct PlayerSyncPlayerData
{
  VECTOR LastReceivedPosition;
  VECTOR LastLocalPosition;
  VECTOR LastLocalRotation;
  int LastNetTime;
  int LastState;
  int LastStateTime;
  int TicksSinceLastUpdate;
  int SendRateTicker;
  char LastStateId;
  char Pad[32];
  u8 CurrentStateUpdateCmdId;
  u8 CurrentSubStateId; // used to track where we're at in between ticks
  u8 StateUpdateCmdId;
  PlayerSyncStateUpdateUnpacked_t StateUpdates[CMD_BUFFER_SIZE];
} PlayerSyncPlayerData_t;


// config reference to check options such as if the playersync config is enabled
extern PatchGameConfig_t gameConfig;

//--------------------------------------------------------------------------
int playerSyncCmdDelta(int fromCmdId, int toCmdId)
{
  int delta = toCmdId - fromCmdId;
  // if (delta < -(CMD_BUFFER_SIZE/2))
  //   delta += CMD_BUFFER_SIZE;
  // else if (delta > (CMD_BUFFER_SIZE/2))
  //   delta -= CMD_BUFFER_SIZE;
  if (delta < -128)
    delta += 256;
  else if (delta > 128)
    delta -= 256;

  return delta;
}

//--------------------------------------------------------------------------
int playerSyncGetCmdId(int cmdId)
{
  return (cmdId + 256) % 256;
}

//--------------------------------------------------------------------------
int playerSyncCmdGetBufIndex(int cmdId)
{
  return (cmdId + CMD_BUFFER_SIZE) % CMD_BUFFER_SIZE;
}

//--------------------------------------------------------------------------
int playerSyncGetSendRate(void)
{

  // in larger lobbies we want to reduce network bandwidth by reducing send rate
  GameSettings* gs = gameGetSettings();
  if (gs && gs->PlayerCount > 8) return 4;
  if (gs && gs->PlayerCount > 6) return 3;
  if (gs && gs->PlayerCount > 4) return 1;

  return 0;
}

//--------------------------------------------------------------------------
int playerSyncShouldImmediatelySendStateChange(int fromState, int toState)
{
  if (toState == PLAYER_STATE_GET_HIT) return 1;

  return 0;
}

//--------------------------------------------------------------------------
float playerSyncLerpAngleIfDelta(float from, float to, float lerpAmount, float minAngleBetween)
{
  float dt = clampAngle(from - to);
  if (dt < minAngleBetween) return from;

  return lerpfAngle(from, to, lerpAmount);
}

/*
This was more trouble than its worth, maybe DL handles pad different or dan knows something I don't
//--------------------------------------------------------------------------
int playerSyncHandlePlayerPadHook(Player* player)
{
  int padIdx = 0;
  
  // enable pad
  if (!player->isLocal && PLAYER_SYNC_DATAS_PTR) {

    // get player sync data
    PlayerSyncPlayerData_t* data = &PLAYER_SYNC_DATAS_PTR[player->fps.vars.camSettingsIndex];
    // process input
    // IN DL DECOMPILED WITH SYMBOLS V6: 00491050 - UpdatePad
    // ((void (*)(struct PAD*))GetAddress(&vaUpdatePadAddr))(player->pPad); // 0x00494460 kisi

    // update pad
    // IN DL DECOMPILED WITH SYMBOLS V6: 004907c0 - ProcessPadInput
    // ((void (*)(struct PAD*, void*, int))GetAddress(&vaProcessPadInputAddr))(player->pPad, data->Pad, 0x14); // 0x493a68 kisi
  }
  // IN DL DECOMPILED WITH SYMBOLS V6: 00512608 - GadgetTransitions, maybe UpdateGadgetEvents from vtable in uya?
  int result = ((int (*)(Player*))0x0052c920)(player); // call parent function GadgetTransitions

  return result;
}
*/

//--------------------------------------------------------------------------
void playerSyncHandlePostPlayerState(Player* player)
{
  if (!player || player->isLocal || !player->pMoby || !PLAYER_SYNC_DATAS_PTR) return;
  
  PlayerSyncPlayerData_t* data = &PLAYER_SYNC_DATAS_PTR[player->fps.vars.camSettingsIndex];
  data->TicksSinceLastUpdate += 1;
}

//--------------------------------------------------------------------------
void playerRotationLerp(VECTOR out, VECTOR a, VECTOR b, float t)
{
  out[0] = lerpfAngle(a[0], b[0], t);
  out[1] = lerpfAngle(a[1], b[1], t);
  out[2] = lerpfAngle(a[2], b[2], t);
}

//--------------------------------------------------------------------------
void playerStateUpdateLerp(PlayerSyncStateUpdateUnpacked_t* out, PlayerSyncStateUpdateUnpacked_t* a, PlayerSyncStateUpdateUnpacked_t* b, float t)
{
  int isHalfway = t >= 0.5;
  if (t <= 0) {
    memcpy(out, a, sizeof(PlayerSyncStateUpdateUnpacked_t));
    return;
  }

  if (t >= 1) {
    memcpy(out, b, sizeof(PlayerSyncStateUpdateUnpacked_t));
    return;
  }

  // interpolate states
  vector_lerp(out->Position, a->Position, b->Position, t);
  playerRotationLerp(out->Rotation, a->Rotation, b->Rotation, t);
  out->CameraDistance = lerpf(a->CameraDistance, b->CameraDistance, t);
  out->CameraHeight = lerpf(a->CameraHeight, b->CameraHeight, t);
  out->CameraPitch = lerpfAngle(a->CameraPitch, b->CameraPitch, t);
  out->CameraYaw = lerpfAngle(a->CameraYaw, b->CameraYaw, t);
  out->NoInput = (short)lerpf(a->NoInput, b->NoInput, t);
  out->MoveX = (u8)lerpf(a->MoveX, b->MoveX, t);
  out->MoveY = (u8)lerpf(a->MoveY, b->MoveY, t);
  out->GameTime = (int)lerpf(a->GameTime, b->GameTime, t);
  out->GroundMoby = isHalfway ? b->GroundMoby : a->GroundMoby;
  out->Health = isHalfway ? b->Health : a->Health;
  out->PadBits = isHalfway ? b->PadBits : a->PadBits;
  out->GadgetId = isHalfway ? b->GadgetId : a->GadgetId;
  //out->GadgetLevel = isHalfway ? b->GadgetLevel : a->GadgetLevel;
  out->State = isHalfway ? b->State : a->State;
  out->StateId = isHalfway ? b->StateId : a->StateId;
  out->PlayerIdx = isHalfway ? b->PlayerIdx : a->PlayerIdx;
  out->Valid = isHalfway ? b->Valid : a->Valid;
  out->CmdId = isHalfway ? b->CmdId : a->CmdId;

  // if we've changed ground moby
  // we want to respect the change by forcing the position to b->Position
  // otherwise the world space a != local space b
  // we could also translate both a,b positions into world, then lerp, then convert to out->GroundMoby local too
  if (b->GroundMoby != a->GroundMoby) {
    out->GroundMoby = b->GroundMoby;
    vector_copy(out->Position, b->Position);
  }
}

//--------------------------------------------------------------------------
void playerSyncHandlePlayerState(Player* player)
{
  MATRIX m, mInv;
  VECTOR dt;
  int i;
  int rate = playerSyncGetSendRate();
  if (!player || player->isLocal || !player->pMoby || !PLAYER_SYNC_DATAS_PTR) return;

  PlayerSyncPlayerData_t* data = &PLAYER_SYNC_DATAS_PTR[player->fps.vars.camSettingsIndex];

  // move forward subtick
  data->CurrentSubStateId++;
  if (data->CurrentSubStateId > rate && data->StateUpdateCmdId != data->CurrentStateUpdateCmdId) {
    //DPRINTF("Marking stateUpdates[%d] invalid because currentSubStateId %d is greater than rate and StateUpdateCmdId %d != CurrentStateUpdateCmdId %d\n",
    //playerSyncCmdGetBufIndex(data->CurrentStateUpdateCmdId), data->CurrentSubStateId, data->StateUpdateCmdId, data->CurrentStateUpdateCmdId);
    data->StateUpdates[playerSyncCmdGetBufIndex(data->CurrentStateUpdateCmdId)].Valid = 0; // mark invalid/used
    data->CurrentStateUpdateCmdId = playerSyncGetCmdId(data->CurrentStateUpdateCmdId + 1);
    data->CurrentSubStateId = 0;
  }

  // we're running behind ()
  int stateIdDelta = playerSyncCmdDelta(data->CurrentStateUpdateCmdId, data->StateUpdateCmdId);
  if (stateIdDelta > 1) {
    //DPRINTF("%d running behind %d, %d=>%d\n", gameGetTime(), stateIdDelta, data->CurrentStateUpdateCmdId, data->StateUpdateCmdId);
    data->CurrentSubStateId = 0;

    int targetId = playerSyncGetCmdId(data->StateUpdateCmdId - 1);
    while (data->CurrentStateUpdateCmdId != targetId) {
      data->StateUpdates[playerSyncCmdGetBufIndex(data->CurrentStateUpdateCmdId)].Valid = 0; // mark invalid/used
      data->CurrentStateUpdateCmdId = playerSyncGetCmdId(data->CurrentStateUpdateCmdId + 1);
    }
  } else if (stateIdDelta == 0 && data->CurrentSubStateId > (rate/2)) {
    // DPRINTF("%d running ahead %d\n", gameGetTime(), data->CurrentSubStateId);
    data->CurrentSubStateId = (int)maxf(0, data->CurrentSubStateId - 1);
  }
  
  PlayerSyncStateUpdateUnpacked_t stateInterpolated;
  PlayerSyncStateUpdateUnpacked_t* stateCurrent = &data->StateUpdates[playerSyncCmdGetBufIndex(data->CurrentStateUpdateCmdId)];
  PlayerSyncStateUpdateUnpacked_t* stateNext = &data->StateUpdates[playerSyncCmdGetBufIndex(data->CurrentStateUpdateCmdId+1)];
  
  if (!stateCurrent->Valid) return;
  
  Moby* playerMoby = player->pMoby;
  PlayerVTable* vtable = playerGetVTable(player);
  float tSub = data->CurrentSubStateId / (float)(rate + 1); // calculate lerp t based off of where we are between ticks i.e tick 2 when sync rate is 4 = 2/5 = .40
  float tPos = 0.15;
  float tRot = 0.15;
  float tCam = 0.5;
  int padIdx = 0;

  if (!stateNext->Valid) {
    tSub = 0; 
  }
  //printf("%d %d/%f\n", data->CurrentStateUpdateCmdId, data->CurrentSubStateId, tSub);

  // interpolate states
  playerStateUpdateLerp(&stateInterpolated, stateCurrent, stateNext, tSub);

  // set no input
  //if (data->TicksSinceLastUpdate == 0) {
  if (data->CurrentSubStateId == 0) {
    player->timers.noInput = stateInterpolated.NoInput;
  }
      
  // resurrecting 
  // TODO player lerps towards spawn location if they died close to spawn
  if (playerStateIsDead(player->RemoteHero.remoteState) && player->pNetPlayer && player->pNetPlayer->warpMessage.isResurrecting) {
    playerRespawn(player);
    player->pNetPlayer->warpMessage.isResurrecting = 0;
  }

  // compute absolute position
  VECTOR stateCurrentPosition;
  vector_copy(stateCurrentPosition, stateInterpolated.Position);
  if (stateInterpolated.GroundMoby) {
    vector_add(stateCurrentPosition, stateCurrentPosition, stateInterpolated.GroundMoby->position);
  }


  // snap position if lerp distance is too much (i.e using teleport pads)
  float snapRadius = (stateInterpolated.GroundMoby != player->ground.pMoby) ? 24 : 49;
  vector_subtract(dt, stateCurrentPosition, player->playerPosition);
  if (vector_sqrmag(dt) > snapRadius) {
    VECTOR dif;
    vector_subtract(dif, stateCurrentPosition, player->playerPosition);
    vector_copy(player->playerPosition, stateCurrentPosition);
    vector_add(player->fps.cameraPos, player->fps.cameraPos, dif);
    //vector_copy(stateInterpolated.Position, data->LastReceivedPosition); //commented out in dl prod
    DPRINTF("tp player %d (dist %d) snap radius %d\n", player->fps.vars.camSettingsIndex, (int)(1000*vector_length(dt)), (int)(1000*snapRadius));
  }

  // lerp position if distance is greater than threshold
  else if (vector_sqrmag(dt) > (0.01*0.01)) {
    VECTOR dif;
    vector_subtract(dif, stateCurrentPosition, player->playerPosition);
    vector_scale(dif, dif, tPos);
    vector_add(player->playerPosition, player->playerPosition, dif);
    vector_add(player->fps.cameraPos, player->fps.cameraPos, dif);
  }

  vector_copy(playerMoby->position, player->playerPosition);
  vector_copy(player->RemoteHero.receivedSyncPos, player->playerPosition);
  vector_copy(player->RemoteHero.posAtSyncFrame, player->playerPosition);

  // lerp rotation
  player->playerRotation[0] = lerpfAngle(player->playerRotation[0], stateInterpolated.Rotation[0], tRot);
  player->playerRotation[1] = lerpfAngle(player->playerRotation[1], stateInterpolated.Rotation[1], tRot);
  player->playerRotation[2] = lerpfAngle(player->playerRotation[2], stateInterpolated.Rotation[2], tRot);
  vector_copy(player->RemoteHero.receivedSyncRot, player->playerRotation);

  // lerp camera rotation
  player->camRot[0] = 0;
  player->camRot[1] = lerpfAngle(player->camRot[1], stateInterpolated.CameraPitch, tCam);
  player->camRot[2] = lerpfAngle(player->camRot[2], stateInterpolated.CameraYaw, tCam);

  // lerp camera position
  vector_write(player->fps.vars.positionOffset, 0);
  vector_write(player->fps.vars.rotationOffset, 0);
  player->fps.vars.positionOffset[0] = -stateInterpolated.CameraDistance;
  vector_copy(&player->fps.vars.cameraMatrix[12], player->fps.cameraPos);
  vector_copy(player->camPos, player->fps.cameraPos);

  // lerp camera rotations
  player->fps.vars.cameraZ.rotation = player->camRot[2];
  player->fps.vars.cameraY.rotation = player->camRot[1];

  // compute matrix
  matrix_unit(m);
  matrix_rotate_y(m, m, -player->camRot[1]);
  matrix_rotate_z(m, m, player->camRot[2]);
  vector_copy(player->fps.vars.facing_dir, &m[0]);
  vector_copy(player->fps.cameraDir, player->fps.vars.facing_dir);

  // copy to player camera
  if (player->camera) {
    VECTOR off;
    matrix_unit(m);
    matrix_rotate_y(m, m, player->camRot[1]);
    matrix_rotate_z(m, m, player->camRot[2]);
    vector_apply(off, player->fps.vars.positionOffset, m);
    vector_add(player->camera->pos, player->fps.cameraPos, off);
    vector_copy(player->camera->rot, player->camRot);
  }

  // compute inv matrix
  matrix_unit(mInv);
  matrix_rotate_y(mInv, mInv, clampAngle(-player->camRot[1] + MATH_PI));
  matrix_rotate_z(mInv, mInv, clampAngle(player->camRot[2] + MATH_PI));
  memcpy(player->camUMtx, mInv, sizeof(VECTOR)*3);

  float moveX = (stateInterpolated.MoveX - 127) / 128.0;
  float moveY = (stateInterpolated.MoveY - 127) / 128.0;
  float mag = minf(1, sqrtf((moveX*moveX) + (moveY*moveY)));
  float ang = atan2f(moveY, moveX);

  player->stickStrength = mag; // stickStrength
  player->stickRawAngle = ang; // stickRawAngle
  player->analogStickStrength = mag; // analog stick strength
  player->pPad->analog[2] = moveX;
  player->pPad->analog[3] = moveY;

  struct tNW_Player* netPlayer = player->pNetPlayer;
  if (netPlayer) {
    netPlayer->padMessageElems[padIdx].msg.pad_data[2] = stateInterpolated.PadBits & 0xFF;
    netPlayer->padMessageElems[padIdx].msg.pad_data[3] = stateInterpolated.PadBits >> 8 | 0xc0; // 0x40 | 0x80; // don't let pad handle x or square, process as a state instead
    netPlayer->padMessageElems[padIdx].msg.pad_data[4] = 0x7F;
    netPlayer->padMessageElems[padIdx].msg.pad_data[5] = 0x7F;
    netPlayer->padMessageElems[padIdx].msg.pad_data[6] = stateInterpolated.MoveX;
    netPlayer->padMessageElems[padIdx].msg.pad_data[7] = stateInterpolated.MoveY;
    netPlayer->padMessageElems[padIdx].inUse = 1;
    netPlayer->activePadFrame = 0;
    netPlayer->pActivePadMsg = &netPlayer->padMessageElems[padIdx];
  }

  // set health
  playerSetHealth(player, stateInterpolated.Health);

  // set remote received state
  int playerState = player->RemoteHero.remoteState;
  player->RemoteHero.remotePad.ipad[8] = playerState;
  player->RemoteHero.receivedState = stateInterpolated.State;

  // update state
  if (stateInterpolated.StateId != data->LastStateId ) { //&& data->delayedState == 0) {
    int skip = 0;
    //DPRINTF("%d => %d\n", data->LastState, stateInterpolated.State);

    // from
    switch (playerState)
    {
      case PLAYER_STATE_VEHICLE: 
      case PLAYER_STATE_TURRET_DRIVER:
      {
        if (data->LastState != playerState) break;

        // leave vehicle
        Vehicle* vehicle = player->vehicle;
        if (stateInterpolated.State != PLAYER_STATE_VEHICLE && vehicle) {

          // driver leave
          if (vehicle->pDriver == player) {
            vehicle->pDriver = 0;
            vehicle->flags |= 2;
          }
          
          // passenger leave
          if (vehicle->pPassenger == player) {
            vehicle->pPassenger = 0;
          }

          vehicle->justExited = player->fps.vars.camSettingsIndex + 1;
          player->vehicleState = 0;
          player->vehicle = NULL;
        }
        break;
      }
      case PLAYER_STATE_SWING:
      {
        // if we're swinging, don't force out of it
        skip = 1;
        break;
      }
      case PLAYER_STATE_GET_HIT:
      {
        // if we're getting hit, don't force out of it
        skip = 1;
        break;
      }
    }

    // to
    switch (stateInterpolated.State)
    {
      // pad does NOT handle square well, just let initbody handle wrench shit TODO - this works 80% of the time, look at 0x00504f88, understand the behavior of hero->unk1a1a[5]
      case PLAYER_STATE_JUMP_ATTACK:
      case PLAYER_STATE_COMBO_ATTACK:
      case PLAYER_STATE_THROW_ATTACK:
      {
        if (player->gadget.weapon.id != 10) {
          player->gadget.weapon.state = 3; 
          ((void (*)(Player*))GetAddress(&vaProcessGadgetState_Func))(player); // processgadgetstate, deletes current gadget moby, always succeeds
          player->weaponHeldId = 10;
          ((void (*)(Player*))GetAddress(&vaProcessGadget_Func))(player); // processgadget creates wrench moby, equips in gadget.weapon slot // sometimes places old weapon in slot...
          // ((void (*)(Player*))0x0050f488)(player); // parent function of function above
        }
        break;
      }
      case PLAYER_STATE_GET_HIT:
      {
        // let game handle flinchings
        skip = 1;
        break;
      }
      case PLAYER_STATE_SWING:
      {
        // force R1 when on swingshot
        // let game handle the rest
        //data->Pad[3] &= ~0x08;
        // player->pNetPlayer->padMessageElems[padIdx].msg.pad_data[3] &= ~0x08;
        skip = 1;
        break;
      }
    }

    if ((!skip) && stateInterpolated.State != playerState) {
      DPRINTF("player %d new state %d (from %d)\n", player->fps.vars.camSettingsIndex, stateInterpolated.State, playerState);

      //if (player->subState > 0) {
        //DPRINTF("substate fix %d=>0\n", player->subState);
        //player->subState = 0;
      //}

      int force = playerStateIsDead(playerState) && !playerStateIsDead(stateInterpolated.State);
      DPRINTF("force is %d. Player %08x will be updated to state %d.\n", force, player, stateInterpolated.State);
      vtable->UpdateState(player, stateInterpolated.State, 1, force, 1);
      data->LastStateId = stateInterpolated.StateId;
      data->LastState = stateInterpolated.State;
    } else {
      data->LastStateTime = player->timers.state;
    }
  }

  // handles case where player fires while still chargebooting
  // causing them to stop on remote client's screen
  // if they do fire and stop, the client should tell the remote clients
  // the remote clients should assume they didn't stop
  // this fixes wrench lag

  if (player->timers.state < 0x3D && playerState == PLAYER_STATE_CHARGE) {
    // *(char*)((u32)player + 0x265a) = 0; DL's line (set firing to 0)
    player->firing = 0;
  }






  if (player->pNetPlayer && player->pNetPlayer->pNetPlayerData) {
    player->pNetPlayer->pNetPlayerData->handGadget = (int)stateInterpolated.GadgetId;
    player->pNetPlayer->pNetPlayerData->lastKeepAlive = data->LastNetTime;
    player->pNetPlayer->pNetPlayerData->timeStamp = data->LastNetTime;
    player->pNetPlayer->pNetPlayerData->hitPoints = stateInterpolated.Health;
    vector_copy(player->pNetPlayer->pNetPlayerData->position, player->playerPosition);
  }

  vector_copy(data->LastLocalPosition, player->playerPosition);
  vector_copy(data->LastLocalRotation, player->playerRotation);
}




//--------------------------------------------------------------------------

int playerSyncOnReceivePlayerState(void* connection, void* data)
{
  if (!isInGame() || !PLAYER_SYNC_DATAS_PTR) return sizeof(PlayerSyncStateUpdatePacked_t);
  if (!gameConfig.grNewPlayerSync) return sizeof(PlayerSyncStateUpdatePacked_t);
 
  PlayerSyncStateUpdatePacked_t msg;
  PlayerSyncStateUpdateUnpacked_t unpacked;
  memcpy(&msg, data, sizeof(msg));

  // unpack
  memcpy(unpacked.Position, msg.Position, sizeof(float) * 3);
  memcpy(unpacked.Rotation, msg.Rotation, sizeof(float) * 3);
  unpacked.GameTime = msg.GameTime;
  unpacked.GroundMoby = NULL;
  unpacked.CameraDistance = msg.CameraDistance / 1024.0;
  unpacked.CameraYaw = msg.CameraYaw / 10240.0;
  unpacked.CameraPitch = msg.CameraPitch / 10240.0;
  unpacked.NoInput = msg.NoInput;
  unpacked.Health = msg.Health;
  unpacked.MoveX = msg.MoveX;
  unpacked.MoveY = msg.MoveY;
  unpacked.PadBits = (msg.PadBits1 << 8) | (msg.PadBits0);
  unpacked.GadgetId = msg.GadgetId;
  unpacked.State = msg.State;
  unpacked.StateId = msg.StateId;
  unpacked.PlayerIdx = msg.PlayerIdx;
  unpacked.CmdId = msg.CmdId;
  unpacked.Valid = 1;


  if (msg.GroundMobyUID != -1 && (msg.Flags & 1)) {
    Guber* groundGuber = guberGetObjectByUID(msg.GroundMobyUID);
    if (groundGuber) {
      unpacked.GroundMoby = groundGuber->Vtable->GetMoby(groundGuber); // 53ee60, or vtable+0x10
    }
  } else if (msg.GroundMobyUID != -1 && (msg.Flags & 2)) {
    unpacked.GroundMoby = mobyFindByUID(msg.GroundMobyUID);
  }

  // target sync player data
  PlayerSyncPlayerData_t* pSyncData = &PLAYER_SYNC_DATAS_PTR[msg.PlayerIdx];

  // move into buffer
  int cmdDt = playerSyncCmdDelta(pSyncData->StateUpdateCmdId, unpacked.CmdId);
  //DPRINTF("%d => %d (%d) %08X\n", data->StateUpdateCmdId, unpacked.CmdId, cmdDt, (u32)&data->StateUpdates[msg.CmdId]);
  if (cmdDt > 0) {

    int bufIdx = playerSyncCmdGetBufIndex(unpacked.CmdId);
    memcpy(&pSyncData->StateUpdates[bufIdx], &unpacked, sizeof(unpacked));

    // create sub items
    int startBufIdx = playerSyncCmdGetBufIndex(pSyncData->StateUpdateCmdId);
    int nextId = playerSyncGetCmdId(pSyncData->StateUpdateCmdId + 1);
    while (nextId != unpacked.CmdId) {
      int nextBufIdx = playerSyncCmdGetBufIndex(nextId);
      if (!pSyncData->StateUpdates[nextBufIdx].Valid) {
        float t = playerSyncCmdDelta(pSyncData->StateUpdateCmdId, nextId) / (float)cmdDt;
        playerStateUpdateLerp(&pSyncData->StateUpdates[nextBufIdx], &pSyncData->StateUpdates[startBufIdx], &pSyncData->StateUpdates[bufIdx], t);
      }
      nextId = playerSyncGetCmdId(nextId + 1);
    }

    pSyncData->LastNetTime = unpacked.GameTime;
    pSyncData->StateUpdateCmdId = unpacked.CmdId;
    pSyncData->TicksSinceLastUpdate = 0;
    vector_copy(pSyncData->LastReceivedPosition, unpacked.Position);
  }

  //DPRINTF("recv player sync state %d time:%d dt:%d\n", msg.PlayerIdx, msg.GameTime, msg.GameTime - gameGetTime());
  return sizeof(PlayerSyncStateUpdatePacked_t);

}

//--------------------------------------------------------------------------
void playerSyncBroadcastPlayerState(Player* player)
{
  VECTOR dt;
  PlayerSyncStateUpdatePacked_t msg;
  void * connection = netGetDmeServerConnection();
  if (!connection || !player || !player->isLocal || !PLAYER_SYNC_DATAS_PTR) return;

  PlayerSyncPlayerData_t* data = &PLAYER_SYNC_DATAS_PTR[player->fps.vars.camSettingsIndex];
  int rate = playerSyncGetSendRate();
  int ticker = data->SendRateTicker--;
  int playerState = player->camera->camHeroData.state;

  // stall until ticker is <= 0
  // or if there's a state change that must be sent right away
  if (ticker > 0) {
    if (data->LastState == playerState || !playerSyncShouldImmediatelySendStateChange(data->LastState, playerState))
    return;
  }

  data->SendRateTicker = rate;


  // set cur frame
  player->LocalHero.curPadMsgFrame = 0;

  // detect state change

  if (data->LastState != playerState || player->timers.state < data->LastStateTime) {
    data->LastState = playerState;
    data->LastStateId = (data->LastStateId + 1) % 256;
  }
  data->LastStateTime = player->timers.state;

  // compute camera yaw and pitch
  float yaw = player->camera->rot[2];
  float pitch = player->camera->rot[1];

  // compute camera distance
  vector_subtract(dt, player->camera->pos, player->fps.cameraPos);
  float dist = vector_length(dt);
  memcpy(msg.Position, player->playerPosition, sizeof(float) * 3);
  memcpy(msg.Rotation, player->playerRotation, sizeof(float) * 3);
  msg.GameTime = data->LastNetTime = gameGetTime();
  msg.GroundMobyUID = -1;
  msg.PlayerIdx = player->fps.vars.camSettingsIndex;
  msg.CameraDistance = (short)(dist * 1024.0);
  msg.CameraPitch = (short)(pitch * 10240.0);
  msg.CameraYaw = (short)(yaw * 10240.0);
  msg.NoInput = player->timers.noInput;
  msg.Health = player->pNetPlayer->pNetPlayerData->hitPoints;
  msg.MoveX = ((struct PAD*)player->pPad)->rdata[6];
  msg.MoveY = ((struct PAD*)player->pPad)->rdata[7];
  msg.PadBits0 = ((struct PAD*)player->pPad)->rdata[2];
  msg.PadBits1 = ((struct PAD*)player->pPad)->rdata[3]; // << 8
  msg.GadgetId = (u8)player->gadget.weapon.id;
  //msg.GadgetLevel = -1;
  msg.State = playerState; // playerGetState(player);
  msg.StateId = data->LastStateId;
  msg.CmdId = data->StateUpdateCmdId = playerSyncGetCmdId(data->StateUpdateCmdId + 1);

  // check if we're on a ground moby
  // if so, sync relative position
  Moby* groundMoby = player->ground.pMoby;
  if (groundMoby) {
    //DPRINTF("Moby found with UID %d and oClass %d\n", (u32))
    Guber* groundMobyGuber = guberGetObjectByMoby(groundMoby);
    if (groundMobyGuber) {
      msg.GroundMobyUID = groundMobyGuber->Id.UID;
      msg.Flags = 1;
    } else if (groundMoby->UID > 0) {
      msg.GroundMobyUID = groundMoby->UID;
      msg.Flags = 2;
    }

    if (msg.GroundMobyUID != -1) {
      VECTOR relativePosition;
      vector_subtract(relativePosition, player->playerPosition, groundMoby->position);
      memcpy(msg.Position, relativePosition, sizeof(float) * 3);
    }
  }

  // WONTDO - uya doesn't seem to care, wont do
  // sync gadget level
  /*
  if (msg.GadgetId >= 0 && msg.GadgetId < 32)
    msg.GadgetLevel = player->GadgetBox->Gadgets[msg.GadgetId].Level;
  */ 
  netBroadcastCustomAppMessage(connection, CUSTOM_MSG_PLAYER_SYNC_STATE_UPDATE, sizeof(msg), &msg);
}

//--------------------------------------------------------------------------
int playerSyncDisablePlayerStateUpdates(void)
{
  return 0;
}

int playerSyncEnableTurretSync(void)
{
  return 1;
}

//--------------------------------------------------------------------------
int playerSyncDisablePlayerPnetUpdates(void)
{
  return 0;
}

//--------------------------------------------------------------------------
void playerSyncPostTick(void)
{
  int i;
  if (!isInGame()) return;
  if (!gameConfig.grNewPlayerSync) return;

  // player updates
  Player** players = playerGetAll();
  for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    playerSyncHandlePostPlayerState(players[i]);
  }
}

//--------------------------------------------------------------------------
void playerSyncTick(void)
{

  static int delay = 50;
  static int initialized = 0;
  int i;

  // net
  netInstallCustomMsgHandler(CUSTOM_MSG_PLAYER_SYNC_STATE_UPDATE, &playerSyncOnReceivePlayerState);

  if (!isInGame()) {
    delay = 50;
    initialized = 0;
    return;
  }

  if (!gameConfig.grNewPlayerSync) return;

  
  // reset buffer
  if (!initialized && PLAYER_SYNC_DATAS_PTR) {
    memset(PLAYER_SYNC_DATAS_PTR, 0, sizeof(PlayerSyncPlayerData_t) * GAME_MAX_PLAYERS);
    DPRINTF("freed the player sync datas ptr\n");
  }

  // allocate buffer
  if (PLAYER_SYNC_DATAS_PTR == 0) {
    DPRINTF("allocated memory for the player sync datas ptr!\n");
    DPRINTF("The address of the player sync datas pts is: %p and is of size %d", PLAYER_SYNC_DATAS_PTR, sizeof(PlayerSyncPlayerData_t) * GAME_MAX_PLAYERS);
    PLAYER_SYNC_DATAS_PTR = malloc(sizeof(PlayerSyncPlayerData_t) * GAME_MAX_PLAYERS);
    initialized = 0;
  }

  // not enough memory to allocate
  // fail and disable
  if (PLAYER_SYNC_DATAS_PTR == 0) {
    gameConfig.grNewPlayerSync = 0;
    return;
  }


  // hooks
  if (!initialized) {
    DPRINTF("hooked main player sync addresses\n");
    HOOK_JAL(GetAddress(&vaPlayerStateUpdate_Hook), &playerSyncDisablePlayerStateUpdates); // replaces function in big ass function at 0060e260 (FUN_0060e260)
    // HOOK_JAL(GetAddress(&vaHandlePlayerPad_Hook), &playerSyncHandlePlayerPadHook); //GadgetTransitions: symbolsv6: 0053b7f8
    HOOK_JAL(GetAddress(&vaPlayerPnetUpdates_Hook), &playerSyncDisablePlayerPnetUpdates); // disable function that keeps on overriding camRot, camumtx, camPos
    int EnableTurretSyncAddr = GetAddress(&vaEnableTurretSync_Addr);
    if (EnableTurretSyncAddr != 0){
      HOOK_JAL(EnableTurretSyncAddr, &playerSyncEnableTurretSync);
    }

    // POKE_U32(0x005F7BDC, 0x24020001); // Hero::IsPlayerLinkHealthy - makes it so it always returns true - i.eline 99 in uya playerupdate_vtable

    // disable tnw_PlayerData update gadgetid
    POKE_U32(GetAddress(&vaTNW_PlayerData_GadgetIdUpdate), 0);

    // disable tnw_PlayerData time update
    POKE_U32(GetAddress(&vaTNW_PlayerData_TimeUpdate), 0);

    // force gadget moby to be created regardless of gsframe
    POKE_U32(GetAddress(&vaForceGadgetMobyCreation_Addr),0);

  }

  // init
  initialized = 1;

  // player updates
  Player** players = playerGetAll();
  for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    playerSyncHandlePlayerState(players[i]);
  }

  // player updates
  for (i = 0; i < GAME_MAX_PLAYERS; ++i) {
    playerSyncBroadcastPlayerState(players[i]);
  }

}
