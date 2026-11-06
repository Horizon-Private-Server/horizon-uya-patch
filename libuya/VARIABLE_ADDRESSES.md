# Compact variable addresses

`GetAddress`, `GetAddressImmediate`, and the assembly variable-function wrappers
accept both the original 44-byte `VariableAddress_t` and the 28-byte
`variableAddressCompact_t`. Rebuild libuya and all consumers together: old
resolvers cannot decode compact tables. No server protocol change is needed.

Fixed compact tables keep absolute Lobby and Bakisi addresses. Bakisi uses
`VA_COMPACT_BASE(address)` to mark the first stored word; Lobby is a plain
32-bit address at byte 4. Each remaining map uses
`VA_COMPACT_OFFSET(bakisiAddress, mapAddress)`.

For an explicit base, use `variableAddressCompactBase_t` with
`.Base = VA_COMPACT_CUSTOM_BASE(address)` and OFFSET entries for every map,
including Bakisi. Both compact layouts occupy 28 bytes on the EE. The resolver
supports both alongside the original 44-byte full table. See README.md for examples.

Offsets are signed four-byte displacements biased by `0x8000`. Encoded zero means
unavailable; `0x8000` means the same address as the base. Omitted fields remain
unavailable. The encoding is for low EE addresses, not kernel or uncached aliases.

Keep source addresses absolute so reviewers can compare them with disassembly.
The offset macro rejects non-word displacements and values outside
`-131068..131068` bytes at compile time. Keep a full table when either region
cannot use that encoding, when Bakisi is unavailable, or when the definition
needs further investigation. Do not truncate an address to force it to fit.

Keep all declarations of a converted table in sync with its definition. Consumers
must resolve tables through the API; compact map members are encoded values,
not absolute addresses. The fixed layout includes two bytes of tail padding; the explicit-base layout uses those bytes for the additional map offset.

The immediate resolver still works during loading. The other resolvers retain
their scene/game-active checks, local-map ID remapping, and invalid-map behavior.
The full format remains available for existing modules and deferred tables.

Historical initial experiment (before explicit-base resolver support) on `bugs/size-variableaddress`:

| Region | Baseline patch BIN | Compact patch BIN | Saved |
| --- | ---: | ---: | ---: |
| PAL (10683) | 112404 | 110740 | 1664 bytes |
| NTSC-U (10684) | 112276 | 110612 | 1664 bytes |

119 compact tables are linked into each patch: 1904 bytes of table storage saved,
partly offset by resolver code and section alignment. The full allocated patch
footprint, including BSS, also shrinks by 1664 bytes. Free space below `0x000f0000`
is 16728 bytes for PAL and 16856 bytes for NTSC-U.

Validation compared all 1309 converted addresses per region with the baseline ELF.
A focused interpreter of the compiled resolver instructions also matched baseline
results for 39312 cases per region, including full-format tables, loading states,
local maps, and invalid maps. The standard Makefile modules build in both regions.
This is build and instruction-level validation; PCSX2 gameplay, lobby/map
transitions, and hardware behavior still require testing.

The deferred `join.c` table has also been converted: its old and compact forms
measure 44 and 28 bytes in isolated EE object files for both regions. Only the
table initializers were compiled for this measurement, not `join.c`.
`JOIN_IN_PROGRESS` remains disabled, so this extra 16-byte table reduction is
not included in the current patch savings above.

## Current revision

The revised resolver supports fixed BASE and explicit CUSTOM_BASE markers in the
first word. The old unpublished Lobby-tagged compact encoding is not compatible;
rebuild compact consumers and libuya together. Full tables remain compatible.

Current patch binaries measure 110868 bytes (PAL) and 110740 bytes (NTSC-U),
128 bytes above the initial experiment due to the additional resolver support.
Both regions built successfully. The user reported successful NTSC-U online
patch testing; PAL remains untested in-game. Disabled join.c was separately
compile-checked for both regions and remains excluded from the patch build.
