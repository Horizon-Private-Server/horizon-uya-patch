#include "graphics.h"
#include "game.h"
#include "interop.h"

#if UYA_PAL

VariableAddress_t vaDrawTextFunc = {
	.Lobby = 0x00592238,
	.Bakisi = 0x0045f4a8,
	.Hoven = 0x00461058,
	.OutpostX12 = 0x00457e58,
    .KorgonOutpost = 0x004559e8,
	.Metropolis = 0x00454d28,
	.BlackwaterCity = 0x00452558,
	.CommandCenter = 0x00452e50,
    .BlackwaterDocks = 0x004556d0,
    .AquatosSewers = 0x004549d0,
    .MarcadiaPalace = 0x00454350,
};

VariableAddress_t vaGetTextWidthFunc = {
	.Lobby = 0x00591f98,
	.Bakisi = 0x0045F208,
	.Hoven = 0x00460DB8,
	.OutpostX12 = 0x00457BB8,
    .KorgonOutpost = 0x00455748,
	.Metropolis = 0x00454A88,
	.BlackwaterCity = 0x004522B8,
	.CommandCenter = 0x00452BB0,
    .BlackwaterDocks = 0x00455430,
    .AquatosSewers = 0x00454730,
    .MarcadiaPalace = 0x004540B0,
};

VariableAddress_t vaDrawBoxFunc = {
	.Lobby = 0x00617790,
	.Bakisi = 0x004e96b8,
	.Hoven = 0x004eb828,
	.OutpostX12 = 0x004e10a8,
    .KorgonOutpost = 0x004de840,
	.Metropolis = 0x004ddb90,
	.BlackwaterCity = 0x004db428,
	.CommandCenter = 0x004db3f0,
    .BlackwaterDocks = 0x004ddc70,
    .AquatosSewers = 0x004dcf70,
    .MarcadiaPalace = 0x004dc8f0,
};


#define IS_PROGRESSIVE_SCAN					(*(int*)0x002413a0)
#else

// drawTextFunc -> widthFunc Offset: -0x2a0
VariableAddress_t vaDrawTextFunc = {
	.Lobby = 0x00590F90,
	.Bakisi = 0x0045e300,
	.Hoven = 0x0045FDF0,
	.OutpostX12 = 0x00456c30,
    .KorgonOutpost = 0x00454840,
	.Metropolis = 0x00456c30,
	.BlackwaterCity = 0x00451330,
	.CommandCenter = 0x00451de8,
    .BlackwaterDocks = 0x00454628,
    .AquatosSewers = 0x00453968,
    .MarcadiaPalace = 0x004532a8,
};

VariableAddress_t vaGetTextWidthFunc = {
	.Lobby = 0x00590cf0,
	.Bakisi = 0x0045E060,
	.Hoven = 0x0045FB50,
	.OutpostX12 = 0x00456990,
    .KorgonOutpost = 0x004545A0,
	.Metropolis = 0x00456990,
	.BlackwaterCity = 0x00451090,
	.CommandCenter = 0x00451B48,
    .BlackwaterDocks = 0x00454388,
    .AquatosSewers = 0x004536C8,
    .MarcadiaPalace = 0x00453008,
};

VariableAddress_t vaDrawBoxFunc = {
	.Lobby = 0x00615078,
	.Bakisi = 0x004e6ff8,
	.Hoven = 0x004e9050,
	.OutpostX12 = 0x004de968,
    .KorgonOutpost = 0x004dc180,
	.Metropolis = 0x004dc180,
	.BlackwaterCity = 0x004d8ce8,
	.CommandCenter = 0x004d8e70,
    .BlackwaterDocks = 0x004db6b0,
    .AquatosSewers = 0x004da9f0,
    .MarcadiaPalace = 0x004da330,
};


#define IS_PROGRESSIVE_SCAN					(*(int*)0x00241520)
#endif

int internal_drawFunc(u32,const char*,long,u64,u64,u64,float,float,float,float,float,float);
void internal_drawBox(void *, void *);

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
int gfxScreenSpaceText(float x, float y, float scaleX, float scaleY, u32 color, const char * string, int length, int alignment)
{
    // draw
    internal_drawFunc(color, string, length, alignment, 0, 0x80000000, x, y, scaleX, scaleY, 0, 0);
    return x + gfxGetFontWidth(string, length, scaleX);
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

    internal_drawBox(buffer, arg2);
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

    internal_drawBox(buffer, arg2);
}

int gfxGetIsProgressiveScan(void)
{
    return IS_PROGRESSIVE_SCAN;
}

void gfxSetIsProgressiveScan(int on)
{
    IS_PROGRESSIVE_SCAN = on;
}
