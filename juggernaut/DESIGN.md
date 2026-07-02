# Juggernaut v2 — Host-Authoritative Crown Pickup

Design for replacing the current kill/stat-based crown tracking with a physical
**crown pickup** object. This document is the build spec; no code has been
written against it yet.

## Why change
The shipped v1 derives the juggernaut from the synced DM kill/death stats
(`updateJuggyFromStats`). That works, but ties the crown to kill attribution and
handles non-kill deaths (suicide, fall/lava, disconnect) awkwardly. Moving to a
world pickup makes the crown a visible, contestable object and removes kill
attribution from the design entirely.

## Principle
The crown is a **world object**. Holding it = juggy. The host only ever watches
two things:
1. Did the current juggy die? (`playerIsDead(players[JuggyIndex])`)
2. Did a living player touch the active pickup? (radius test)

No kill tracking anywhere. The buff layer is untouched — `processPlayer` still
just reads `JuggyIndex`; we only replace *how that value is decided*.

## Authority: host-owned, KOTH-style
- The host is the single source of truth for crown state and pickup position.
- The host broadcasts a **heartbeat** (~500ms), not only on transitions:
  `{ seq, juggyIdx (-1 = none), pickupActive, pickupPos[3] }`.
- Clients apply any snapshot with a newer `seq`, set `JuggyIndex`, and render a
  **local cosmetic sprite** at `pickupPos`. Clients never decide the crown.
- **Host migration:** because the full state rides the heartbeat, a reassigned
  host (`gameAmIHost()` flips when UYA reassigns `GAME_HOST_ID`) resumes from its
  last cached snapshot and keeps broadcasting. Same model as KOTH — no handshake.

## Host state machine
```
MATCH START -> host picks a random, validated player spawn -> PICKUP_ACTIVE

PICKUP_ACTIVE (crown in world):
  host checks every living player vs pickupPos (radius test)
    on overlap        -> crown that player: JuggyIndex=X, pickupActive=0 -> CROWNED
    on stale (~20s)   -> relocate to a new random validated spawn

CROWNED(X):
  host watches playerIsDead(players[X])
    on juggy death    -> record deathPos, arm 1s timer -> (after 1s) PICKUP_ACTIVE @ deathPos
    on juggy disconnect -> PICKUP_ACTIVE @ last known pos (fallback: random spawn)
```

## Contest behavior
- On juggy death the crown drops at the **dead juggy's location** after a **1s arm
  delay**. The delay is the contest window — nearby players (including whoever got
  the kill) converge and race for it. No auto-regrab: there's a beat before it is
  live and it sits on neutral ground.
- Knob held in reserve if testing shows it still favors the killer: a short
  "last holder can't instantly regrab" lockout. Not in v1.

## Client side (cosmetic only)
Lift the presentation from `patch/scavengerhunt.c`:
- `mobySpawn` + `pUpdate` for the visual object.
- `gfxDrawBillboardQuad` + particles (`gfxSpawnParticle`) for the sprite.
- `soundPlayByOClass` on spawn/grab.
- The grab **decision is host-side**, so unlike scav hunt there is no
  `GAME_MAX_LOCALS` self-detection — the sprite is purely a render of the
  host-broadcast `pickupPos`/`pickupActive`.

## Spawn placement (random player spawn, validated)
API in `libuya/spawnpoint.h`:
- `spawnPointGetCount()` — total cuboids.
- `spawnPointGet(index)` -> `Cuboid*`; `Cuboid.pos` (VECTOR @ 0x30) is the spawn
  position. NOTE: no bounds/sanity checking inside — caller owns it.
- `spawnPointIsPlayer(index)` — true for player-spawn cuboids (checks
  `gameData->DeathMatchGameData->resurrectionPts[]`).

Selection procedure (host, at match start and on stale relocate):
1. Collect indices `i` in `[0, spawnPointGetCount())` where `spawnPointIsPlayer(i)`.
2. Pick one at random; read `->pos`.
3. Validate with a KOTH-style sanity check (reject NaN + out-of-range):
   ```c
   // from koth/game.c
   static int floatIsSane(float v)  { return v == v && v > -10000.0f && v < 10000.0f; }
   static int vectorIsSane(VECTOR v){ return floatIsSane(v[0]) && floatIsSane(v[1]) && floatIsSane(v[2]); }
   ```
4. On failure, re-roll (bounded retries); final fallback = a living player's
   position.
5. Broadcast the validated `pos`; trust it thereafter (validate once at selection,
   not per tick — KOTH's lean-path lesson).

Vanilla map spawns pass trivially; the validation exists to catch malformed
**custom-map** cuboids.

## The clean seam (what to add / remove)
- **Remove:** `updateJuggyFromStats()`, `LastKills[]`/`LastDeaths[]`, and the
  `SCORE_JUGGY_ONLY` erase (revisit scoring separately once the crown is solid).
- **Add:**
  - `juggyHostTick()` — the state machine above, host-only.
  - `juggyBroadcastState()` / `juggyOnReceiveState()` — heartbeat send + handler
    installed via `netInstallCustomMsgHandler`.
  - Pickup sprite spawn/despawn + `pUpdate` render.
  - New `CUSTOM_MSG_ID_JUGGERNAUT_STATE`.
- **Unchanged:** everything in `processPlayer` — 2x health, V2 weapons, and all
  `isLocal`-gated buffs. `JuggyIndex` simply gets its value from the broadcast
  instead of the stat poll.

## Defaults (revisit if needed)
- Heartbeat interval: ~500ms.
- Pickup radius: reuse scav hunt's `HBOLT_PICKUP_RADIUS` feel.
- Stale relocate timeout: ~20s.
- Death-drop arm delay: 1s.

## Open items to nail at build time
- Confirm `CUSTOM_MSG_ID_JUGGERNAUT_STATE` id allocation against the existing
  custom-message enum.
- Confirm the message send path (`netGetDmeServerConnection()` +
  `netBroadcastCustomAppMessage`, mirroring KOTH's hill sync).
- Decide the crown sprite oclass/asset (reuse an existing pickup oclass vs a
  dedicated one).
