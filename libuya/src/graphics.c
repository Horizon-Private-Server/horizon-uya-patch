#include "graphics.h"
#include "game.h"

#if UYA_PAL

#define IS_PROGRESSIVE_SCAN					(*(int*)0)
#else

#define IS_PROGRESSIVE_SCAN					(*(int*)0x00241520)
#endif

int internal_drawFunc_inLobby(u32,const char*,long,u64,u64,u64,float,float,float,float,float,float);
int internal_widthFunc_inLobby(const char*,long,float);
void internal_drawBox_inLobby(void *, void *);

//--------------------------------------------------------
int gfxWorldSpaceToScreenSpace(VECTOR position, int * x, int * y)
{
    int output = 0;

    asm __volatile__ (
        "addiu      $sp, $sp, -0x40     \n"
        "sq         $ra, 0x00($sp)      \n"
        "sq         $s0, 0x10($sp)      \n"
        "sq         $s1, 0x20($sp)      \n"
        "swc1       $f20, 0x30($sp)     \n"

        "move       $s0, %0             \n"
        "move       $s1, %1             \n"
        "li.s       $f20, 0.0625        \n"
        
        "lq		    $a0, 0x00(%2)	    \n"
        "jal		0x004BFCA8	        \n"
        "lui		$v1, 0x001D	        \n"
        "mtc1		$v0, $f01   	    \n"
        "addiu		$a0, $v1, 0x3F40    \n"
        "lwc1		$f02, 0x10($a0)	    \n"
        "cvt.s.w	$f02, $f02	        \n"
        "prot3w		$v1, $v0	        \n"
        "mtc1		$v1, $f00	        \n"
        "lwc1		$f03, 0x14($a0)	    \n"
        "cvt.s.w	$f03, $f03	        \n"
        "lw		    $v0, 0x18($a0)	    \n"
        "sub.s		$f01, $f01, $f02    \n"
        "addiu		$v0, $v0, 0x40	    \n"
        "sub.s		$f00, $f00, $f03    \n"
        "mul.s		$f01, $f01, $f20    \n"
        "mul.s		$f00, $f00, $f20    \n"
        "cvt.w.s	$f02, $f01  	    \n"
        "mfc1       $t0, $f02           \n"
        "sw		    $t0, 0x00($s0)	    \n"
        "cvt.w.s	$f01, $f00	        \n"
        "mfc1       $t1, $f01           \n"
        "sw		    $t1, 0x00($s1)	    \n"

        "lw         $v0, 0x18($a0)      \n"
        "addiu      $v0, $v0, 0x40      \n"
        "slt        $v1, $v0, $t0       \n"
        "bne        $v1, $0, exit       \n"
        "slti       $v1, $t0, -0x40     \n"
        "bne        $v1, $0, exit       \n"
        "lw         $v0, 0x1C($a0)      \n"
        "addiu      $v0, $v0, 0x40      \n"
        "slt        $v1, $v0, $t1       \n"
        "bne        $v1, $0, exit       \n"
        "slti       $v1, $t1, -0x40     \n"
        "bne        $v1, $0, exit       \n"
        "li         %3, 1              \n"

        "exit:                          \n"
        "lq         $ra, 0x00($sp)      \n"
        "lq         $s0, 0x10($sp)      \n"
        "lq         $s1, 0x20($sp)      \n"
        "lwc1       $f20, 0x30($sp)     \n"
        "addiu      $sp, $sp, 0x40      \n"
        : : "r" (x), "r" (y), "r" (position), "r" (output)
    );

    return output;
}

//--------------------------------------------------------
int gfxGetFontWidth(const char * string, int length, float scale)
{
    return internal_widthFunc_inLobby(string, length, scale);
}

//--------------------------------------------------------
int gfxScreenSpaceText(float x, float y, float scaleX, float scaleY, u32 color, const char * string, int length, int alignment)
{
    // draw
    internal_drawFunc_inLobby(color, string, length, alignment, 0, 0x80000000, x, y, scaleX, scaleY, 0, 0);
    return x + internal_widthFunc_inLobby(string, length, scaleX);
}

//--------------------------------------------------------
void gfxScreenSpaceQuad(RECT * rect, u32 colorTL, u32 colorTR, u32 colorBL, u32 colorBR)
{
    // crop (xOff, yOff, width, height)
    float arg2[4] = {
        0, 0, 1, 1
    };
    u32 buffer[25];

    float centerX = (rect->TopLeft[0] + rect->BottomRight[0]) * 0.5;
    float centerY = (rect->TopLeft[1] + rect->BottomRight[1]) * 0.5;
    float width = rect->BottomRight[0] - rect->TopLeft[0];
    float height = rect->BottomRight[1] - rect->TopLeft[1];

    buffer[0] = 1;
    buffer[1] = 0;
    buffer[2] = 0x00245E88;
    buffer[3] = 0x00009E0D;
    buffer[4] = *(u32*)(&centerX);
    buffer[5] = *(u32*)(&centerY);
    buffer[6] = *(u32*)(&width);
    buffer[7] = *(u32*)(&height);
    buffer[8] = 0;
    buffer[9] = 0;
    buffer[10] = 0x40000000;
    buffer[11] = 0;
    buffer[12] = 0;
    buffer[13] = 0;
    buffer[14] = colorTL;
    buffer[15] = colorTR;
    buffer[16] = colorBL;
    buffer[17] = colorBR;
    buffer[18] = 0;
    buffer[19] = 10;
    buffer[20] = 0; //0x002B5960;
    buffer[21] = 0;
    buffer[22] = 0;
    buffer[23] = 0;
    buffer[24] = 0;

    internal_drawBox_inLobby(buffer, arg2);
}

//--------------------------------------------------------
void gfxScreenSpaceBox(float x, float y, float w, float h, u32 color)
{
    // crop (xOff, yOff, width, height)
    float arg2[4] = {
        0, 0, 1, 1
    };
    u32 buffer[25];

    float centerX = x + w*0.5;
    float centerY = y + h*0.5;

    buffer[0] = 1;
    buffer[1] = 0;
    buffer[2] = 0x00245E88;
    buffer[3] = 0x00009E0D;
    buffer[4] = *(u32*)(&centerX);
    buffer[5] = *(u32*)(&centerY);
    buffer[6] = *(u32*)(&w);
    buffer[7] = *(u32*)(&h);
    buffer[8] = 0;
    buffer[9] = 0;
    buffer[10] = 0x40000000;
    buffer[11] = 0;
    buffer[12] = 0;
    buffer[13] = 0;
    buffer[14] = color;
    buffer[15] = color;
    buffer[16] = color;
    buffer[17] = color;
    buffer[18] = 0;
    buffer[19] = 10;
    buffer[20] = 0; //0x002B5960;
    buffer[21] = 0;
    buffer[22] = 0;
    buffer[23] = 0;
    buffer[24] = 0;

    internal_drawBox_inLobby(buffer, arg2);
}

int gfxGetIsProgressiveScan(void)
{
    return IS_PROGRESSIVE_SCAN;
}

void gfxSetIsProgressiveScan(int on)
{
    IS_PROGRESSIVE_SCAN = on;
}
