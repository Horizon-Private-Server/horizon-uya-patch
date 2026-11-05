# libuya

## Compact address tables

The resolver accepts original full `VariableAddress_t` tables, fixed-base `variableAddressCompact_t` tables, and selectable-base `variableAddressCompactBase_t` tables. Full-table ABI is unchanged. Fixed compact tables occupy 28 bytes; custom-base tables occupy 28 bytes on the EE.

```c
/* Illustrative values; not verified game addresses. */
variableAddressCompact_t vaFixed = {
	.Lobby = 0,
	.Bakisi = VA_COMPACT_BASE(0x00400000),
	.Hoven = VA_COMPACT_OFFSET(0x00400000, 0x00400100),
};

variableAddressCompactBase_t vaCustom = {
	.Lobby = 0,
	.Base = VA_COMPACT_CUSTOM_BASE(0x00400100),
	.Bakisi = VA_COMPACT_OFFSET(0x00400100, 0x00400000),
	.Hoven = VA_COMPACT_OFFSET(0x00400100, 0x00400100),
};
```

The first stored word carries the format marker: 0x80000000 for fixed-base compact, 0xc0000000 for custom-base compact. The resolver removes both marker bits before using the base. Use low EE addresses (or REL offsets), without marker bits, as OFFSET arguments. Nonzero map entries must differ from the chosen base by a multiple of four within +/-32767 words. Encoded zero is unavailable; 0x8000 represents the same address as the base. A custom base need not match any map entry, and omitted maps remain unavailable.

`Lobby` is a plain 32-bit address at byte 4 in both compact layouts. You may write its initializer first; designated initializer order does not control struct layout. The fixed layout stores `Bakisi` first internally; the custom layout stores `Base` first.

The unpublished menu/lobby-tagged compact layout has been replaced. Update old compact initializers and rebuild their consumers together with this resolver; old compact binaries are not supported by the revised menu-bearing layouts. Full tables remain compatible. librac3's existing fixed Veldin layout retains its encoding. No live PCSX2 verification was performed for this change.
