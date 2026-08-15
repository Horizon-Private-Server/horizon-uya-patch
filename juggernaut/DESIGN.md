# Juggernaut

Juggernaut is a crown-pickup mode layered onto DM, CTF, and Siege. The crown
holder is the juggy: their base weapons are upgraded to V2 and they receive a
damage cushion equivalent to one extra health bar.

## State and authority

The host owns the crown state. It chooses a validated player-spawn position,
detects pickups, drops the crown after a juggy death, and relocates stale
pickups. It broadcasts a heartbeat containing:

`{ sequence, juggy index, pickup active, pickup position }`

Clients accept newer snapshots, update `JuggyIndex`, and render the crown
locally. A migrated host continues from the latest cached state.

The juggy's client owns the damage cushion and broadcasts shield visibility.
The local owner uses `shieldTrigger`, while other clients use
`playerGiveShield` on their remote representation. The shield is only an
indicator; the custom cushion remains the source of extra durability.

Shield packets carry the crown-state sequence, and periodic resends repair
packet reordering. Pending local shield requests are cancelled on crown loss or
death.

Shield is suppressd in vehicle or turret. Eestroyed by engine while occupied
a create attempt never latches and the per-frame poll would replay the equip sound 
on remote clients. Its not needed here anyway so just return on exit. 

## Crown flow

```text
match start -> delayed pickup at a validated player spawn
pickup      -> living player overlaps it -> that player becomes juggy
juggy dies  -> short arm delay -> pickup at death position
disconnect  -> pickup at last known position, or a new spawn
stale       -> relocate to a new validated player spawn
```

Pickup positions reject NaN and out-of-range coordinates. If no valid spawn is
available, the host retries and eventually falls back to a living player's
position.

## Implementation notes

- `processPlayer` reads `JuggyIndex`; crown ownership is separate from buffs.
- V2 upgrades are applied only to valid weapon IDs decoded from populated
  inventory slots; absent weapons are never passed to the engine upgrade path.
- The health buff uses a guarded damage hook instead of writing obfuscated
  health/max-health fields, which share state with player status.
- Damage hooks scan all matching call sites because PvP damage does not use one
  universal site. Hook installation and removal are map-reload safe.
- Crown visuals are client-local. The held crown is an orb over the juggy; an
  unheld crown also draws a beam and a minimap marker.
- Only the juggy receives frag credit when the juggy-only scoring option is on.

## Tuning

- Heartbeat: 500 ms
- Initial pickup delay: 10 s
- Stale relocation: 20 s
- Death-drop arm delay: 3 s
- Pickup radius: 3 world units
