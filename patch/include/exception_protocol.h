#ifndef EXCEPTION_PROTOCOL_H
#define EXCEPTION_PROTOCOL_H

#define EXCEPTION_DISPLAY_MAGIC (0x45584332)
#define EXCEPTION_DISPLAY_VERSION (1)

/* EE ABI: 32-bit pointers, normal alignment. */
typedef struct exceptionContext {
	/* 0x00 */ u32 progressive;
	/* 0x04 */ char region[8];
	/* 0x0c */ char map[32];
} exceptionContext_t;

typedef struct exceptionPayload {
	/* 0x00 */ u32 entry[2];
	/* 0x08 */ u32 magic;
	/* 0x0c */ u32 version;
	/* 0x10 */ volatile exceptionContext_t* context;
	/* 0x14 */ void (*handler)(void);
	/* 0x18 */ u32 memoryEnd;
	/* 0x1c */ u32 reserved;
} exceptionPayload_t;

#endif
