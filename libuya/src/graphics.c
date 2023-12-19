#include "graphics.h"
#include "game.h"
#include "interop.h"

#if UYA_PAL
#define IS_PROGRESSIVE_SCAN					(*(int*)0x002413a0)
#define IS_WIDESCREEN                       (*(char*)0x001a58bd)
#else
#define IS_PROGRESSIVE_SCAN					(*(int*)0x00241520)
#define IS_WIDESCREEN                       (*(char*)0x001a5a3d)
#endif

#define SCREEN_VISIBOMB_EFFECT              ((ScreenVBEffect*)0x00242624)
#define SCREEN_INSERT_EFFECT                ((ScreenInsertEffect*)0x002426A0)
#define SHELL_CIRCLE_COS                    (0x00242920) // part of struct FXUtilsInterface
#define SHELL_CIRCLE_SIN                    (0x00242950) // part of struct FXUtilsInterface

#define VIEW_CONTEXT                        ((ViewContext*)GetAddress(&vaViewContext))

int internal_drawFunc(float, float, float, float, float, float, u32, const char*, u64, u64, int, u32);
void internal_drawBox(void *, void *);

// drawTextFunc -> widthFunc Offset: -0x2a0
VariableAddress_t vaFontPrintFunc = {
#if UYA_PAL
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
#else
	.Lobby = 0x00590F90,
	.Bakisi = 0x0045e300,
	.Hoven = 0x0045FDF0,
	.OutpostX12 = 0x00456c30,
    .KorgonOutpost = 0x00454840,
	.Metropolis = 0x00453b80,
	.BlackwaterCity = 0x00451330,
	.CommandCenter = 0x00451de8,
    .BlackwaterDocks = 0x00454628,
    .AquatosSewers = 0x00453968,
    .MarcadiaPalace = 0x004532a8,
#endif
};

VariableAddress_t vaFontPrintCenterFunc = {
#if UYA_PAL
	.Lobby = 0x005949b0,
	.Bakisi = 0x00461c20,
	.Hoven = 0x004637D0,
	.OutpostX12 = 0x0045A5D0,
    .KorgonOutpost = 0x00458160,
	.Metropolis = 0x004574A0,
	.BlackwaterCity = 0x00454CD0,
	.CommandCenter = 0x004555C8,
    .BlackwaterDocks = 0x00457E48,
    .AquatosSewers = 0x00457148,
    .MarcadiaPalace = 0x00456AC8,
#else
	.Lobby = 0x00593700,
	.Bakisi = 0x00460A70,
	.Hoven = 0x00462560,
	.OutpostX12 = 0x004593A0,
    .KorgonOutpost = 0x00456FB0,
	.Metropolis = 0x004562F0,
	.BlackwaterCity = 0x00453AA0,
	.CommandCenter = 0x00454558,
    .BlackwaterDocks = 0x00456D98,
    .AquatosSewers = 0x004560D8,
    .MarcadiaPalace = 0x00455A18,
#endif
};

VariableAddress_t vaGetTextWidthFunc = {
#if UYA_PAL
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
#else
	.Lobby = 0x00590cf0,
	.Bakisi = 0x0045E060,
	.Hoven = 0x0045FB50,
	.OutpostX12 = 0x00456990,
    .KorgonOutpost = 0x004545A0,
	.Metropolis = 0x004538e0,
	.BlackwaterCity = 0x00451090,
	.CommandCenter = 0x00451B48,
    .BlackwaterDocks = 0x00454388,
    .AquatosSewers = 0x004536C8,
    .MarcadiaPalace = 0x00453008,
#endif
};

VariableAddress_t vaDrawBoxFunc = {
#if UYA_PAL
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
#else
	.Lobby = 0x00615078,
	.Bakisi = 0x004e6ff8,
	.Hoven = 0x004e9050,
	.OutpostX12 = 0x004de968,
    .KorgonOutpost = 0x004dc180,
	.Metropolis = 0x004db4d0,
	.BlackwaterCity = 0x004d8ce8,
	.CommandCenter = 0x004d8e70,
    .BlackwaterDocks = 0x004db6b0,
    .AquatosSewers = 0x004da9f0,
    .MarcadiaPalace = 0x004da330,
#endif
};

VariableAddress_t vaSetupGifPaging = {
#if UYA_PAL
	.Lobby = 0x00589538,
	.Bakisi = 0x00456450,
	.Hoven = 0x00457fd0,
	.OutpostX12 = 0x0044edd0,
    .KorgonOutpost = 0x0044c990,
	.Metropolis = 0x0044bcd0,
	.BlackwaterCity = 0x004494d0,
	.CommandCenter = 0x0044a150,
    .BlackwaterDocks = 0x0044c9d0,
    .AquatosSewers = 0x0044bcd0,
    .MarcadiaPalace = 0x0044b650,
#else
	.Lobby = 0x005883c8,
	.Bakisi = 0x004553e0,
	.Hoven = 0x00456ea0,
	.OutpostX12 = 0x0044dce0,
    .KorgonOutpost = 0x0044b920,
	.Metropolis = 0x0044ac60,
	.BlackwaterCity = 0x004483e0,
	.CommandCenter = 0x00449220,
    .BlackwaterDocks = 0x0044bc60,
    .AquatosSewers = 0x0044ada0,
    .MarcadiaPalace = 0x0044a6e0,
#endif
};

VariableAddress_t vaDoGifPaging = {
#if UYA_PAL
	.Lobby = 0x00589668,
	.Bakisi = 0x00456580,
	.Hoven = 0x00458100,
	.OutpostX12 = 0x0044ef00,
    .KorgonOutpost = 0x0044cac0,
	.Metropolis = 0x0044be00,
	.BlackwaterCity = 0x00449600,
	.CommandCenter = 0x0044a280,
    .BlackwaterDocks = 0x0044cb00,
    .AquatosSewers = 0x0044be00,
    .MarcadiaPalace = 0x0044b780,
#else
	.Lobby = 0x005884f8,
	.Bakisi = 0x00455510,
	.Hoven = 0x00456fd0,
	.OutpostX12 = 0x0044de10,
    .KorgonOutpost = 0x0044ba50,
	.Metropolis = 0x0044ad90,
	.BlackwaterCity = 0x00448510,
	.CommandCenter = 0x00449350,
    .BlackwaterDocks = 0x0044bb90,
    .AquatosSewers = 0x0044aed0,
    .MarcadiaPalace = 0x0044a810,
#endif
};

VariableAddress_t vaGetFrameTex = {
#if UYA_PAL
	.Lobby = 0x0059c910,
	.Bakisi = 0x00469300,
	.Hoven = 0x0046aeb0,
	.OutpostX12 = 0x00461cb0,
    .KorgonOutpost = 0x0045f840,
	.Metropolis = 0x0045eb84,
	.BlackwaterCity = 0x0045c3b0,
	.CommandCenter = 0x0045cca8,
    .BlackwaterDocks = 0x0045f528,
    .AquatosSewers = 0x0045e828,
    .MarcadiaPalace = 0x0045e1a8,
#else
	.Lobby = 0x0059b580,
	.Bakisi = 0x00468070,
	.Hoven = 0x00469b60,
	.OutpostX12 = 0x004609a0,
    .KorgonOutpost = 0x0045e5b0,
	.Metropolis = 0x0045d8f0,
	.BlackwaterCity = 0x0045b0a0,
	.CommandCenter = 0x0045bb58,
    .BlackwaterDocks = 0x0045e398,
    .AquatosSewers = 0x0045d6d8,
    .MarcadiaPalace = 0x0045d018,
#endif
};

VariableAddress_t vaGetEffectTex = {
#if UYA_PAL
	.Lobby = 0x00589890,
	.Bakisi = 0x004567a8,
	.Hoven = 0x00458328,
	.OutpostX12 = 0x0044f128,
    .KorgonOutpost = 0x0044cce8,
	.Metropolis = 0x0044c028,
	.BlackwaterCity = 0x00449828,
	.CommandCenter = 0x0044a4a8,
    .BlackwaterDocks = 0x0044cd28,
    .AquatosSewers = 0x0044c028,
    .MarcadiaPalace = 0x0044b9a8,
#else
	.Lobby = 0x00588720,
	.Bakisi = 0x00455738,
	.Hoven = 0x004571f8,
	.OutpostX12 = 0x0044e038,
    .KorgonOutpost = 0x0044bc78,
	.Metropolis = 0x0044afb8,
	.BlackwaterCity = 0x00448738,
	.CommandCenter = 0x00449578,
    .BlackwaterDocks = 0x0044bdb8,
    .AquatosSewers = 0x0044b0f8,
    .MarcadiaPalace = 0x0044aa38,
#endif
};

VariableAddress_t vaDrawSprite = {
#if UYA_PAL
	.Lobby = 0x0058b9f8,
	.Bakisi = 0x00458910,
	.Hoven = 0x0045a490,
	.OutpostX12 = 0x00451290,
    .KorgonOutpost = 0x0044ee50,
	.Metropolis = 0x0044e190,
	.BlackwaterCity = 0x0044b990,
	.CommandCenter = 0x0044c610,
    .BlackwaterDocks = 0x0044ee90,
    .AquatosSewers = 0x0044e190,
    .MarcadiaPalace = 0x0044db10,
#else
	.Lobby = 0x0058a828,
	.Bakisi = 0x00457840,
	.Hoven = 0x00459300,
	.OutpostX12 = 0x00450140,
    .KorgonOutpost = 0x0044dd80,
	.Metropolis = 0x0044d0c0,
	.BlackwaterCity = 0x0044a840,
	.CommandCenter = 0x0044b680,
    .BlackwaterDocks = 0x0044dec0,
    .AquatosSewers = 0x0044d200,
    .MarcadiaPalace = 0x0044cb40,
#endif
};

VariableAddress_t vaSticky_FX = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00457240,
    .Hoven = 0x00458dc0,
    .OutpostX12 = 0x0044fbc0,
    .KorgonOutpost = 0x0044d780,
    .Metropolis = 0x0044cac0,
    .BlackwaterCity = 0x0044a2c0,
    .CommandCenter = 0x0044af40,
    .BlackwaterDocks = 0x0044d7c0,
    .AquatosSewers = 0x0044cac0,
    .MarcadiaPalace = 0x0044c440,
#else
    .Lobby = 0,
    .Bakisi = 0x00456108,
    .Hoven = 0x00457c90,
    .OutpostX12 = 0x0044ead0,
    .KorgonOutpost = 0x0044c710,
    .Metropolis = 0x0044ba50,
    .BlackwaterCity = 0x004491d0,
    .CommandCenter = 0x0044a010,
    .BlackwaterDocks = 0x0044c850,
    .AquatosSewers = 0x0044bac8,
    .MarcadiaPalace = 0x0044b4d0,
#endif
};

VariableAddress_t vaDrawBillboardQuad = {
#if UYA_PAL

    .Lobby = 0,
    .Hoven = 0x0045dab0,
    .OutpostX12 = 0x004548b0,
    .KorgonOutpost = 0x00452470,
    .Metropolis = 0x004517b0,
    .BlackwaterCity = 0x0044efb0,
    .CommandCenter = 0x0044fad8,
    .BlackwaterDocks = 0x00452358,
    .AquatosSewers = 0x00451658,
    .MarcadiaPalace = 0x00450fd8,
#else
    .Lobby = 0,
    .Bakisi = 0x0045ae68,
    .Hoven = 0x0045c928,
    .OutpostX12 = 0x00453768,
    .KorgonOutpost = 0x004513a8,
    .Metropolis = 0x004506e8,
    .BlackwaterCity = 0x0044de68,
    .CommandCenter = 0x0044eb50,
    .BlackwaterDocks = 0x00451390,
    .AquatosSewers = 0x004506d0,
    .MarcadiaPalace = 0x00450010,
#endif
};

VariableAddress_t vaViewContext = {
#if UYA_PAL
    .Lobby = 0,
    .Bakisi = 0x00259900,
    .Hoven = 0x00259a40,
    .OutpostX12 = 0x00259940,
    .KorgonOutpost = 0x002597c0,
    .Metropolis = 0x00259800,
    .BlackwaterCity = 0x002597c0,
    .CommandCenter = 0x00259380,
    .BlackwaterDocks = 0x002594c0,
    .AquatosSewers = 0x00259500,
    .MarcadiaPalace = 0x00259480,
#else
    .Lobby = 0,
    .Bakisi = 0x00259A80,
    .Hoven = 0x00259bc0,
    .OutpostX12 = 0x00259ac0,
    .KorgonOutpost = 0x00259940,
    .Metropolis = 0x00259980,
    .BlackwaterCity = 0x00259940,
    .CommandCenter = 0x00259500,
    .BlackwaterDocks = 0x00259640,
    .AquatosSewers = 0x00259680,
    .MarcadiaPalace = 0x00259600,
#endif
};

VariableAddress_t vaGetPreLoadedImageBufferSource = {
#if UYA_PAL
    .Lobby = 0x005e7d20,
    .Bakisi = 0x004ba3c0,
    .Hoven = 0x004bc4d8,
    .OutpostX12 = 0x004b1db0,
    .KorgonOutpost = 0x004af548,
    .Metropolis = 0x004ae898,
    .BlackwaterCity = 0x004ac130,
    .CommandCenter = 0x004ac128,
    .BlackwaterDocks = 0x004ae9a8,
    .AquatosSewers = 0x004adca8,
    .MarcadiaPalace = 0x004ad628,
#else
    .Lobby = 0x005e57b0,
    .Bakisi = 0x004b7ea8,
    .Hoven = 0x004b9f00,
    .OutpostX12 = 0x004af818,
    .KorgonOutpost = 0x004ad030,
    .Metropolis = 0x004ac380,
    .BlackwaterCity = 0x004a9b98,
    .CommandCenter = 0x004a9d50,
    .BlackwaterDocks = 0x004ac590,
    .AquatosSewers = 0x004ab8d0,
    .MarcadiaPalace = 0x004ab210,
#endif
};

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
    /*
        Aligntment: 0: Right
                    1: Left
                    2: Center
                    3: ????
                    4: None
    */
    // float x, float y, float scaleX, float scaleY, float shadowX, float shadowY, u32 color, const char* string, u64 length, u64 alignment, int bold, u32 shadowColor
    internal_drawFunc(x, y, scaleX, scaleY, 0, 0, color, string, length, alignment, 1, 0x80000000);
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
#if UYA_PAL
    buffer[2] = 0x00245D08;
#else
    buffer[2] = 0x00245E88;
#endif
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
#if UYA_PAL
    buffer[2] = 0x00245D08;
#else
    buffer[2] = 0x00245E88;
#endif
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

void gfxPixelSpaceBox(float x, float y, float w, float h, u32 color)
{
    x /= SCREEN_WIDTH;
    y /= SCREEN_HEIGHT;
    w /= SCREEN_WIDTH;
    h /= SCREEN_HEIGHT;
    
    gfxScreenSpaceBox(x, y, w, h, color);
}

int gfxGetIsProgressiveScan(void)
{
    return IS_PROGRESSIVE_SCAN;
}

void gfxSetIsProgressiveScan(int on)
{
    IS_PROGRESSIVE_SCAN = on;
}

void gfxOcclusion(int OnOff)
{
    VariableAddress_t vaOcclusionAddr = {
#if UYA_PAL
        .Lobby = 0,
        .Bakisi = 0x00248094,
        .Hoven = 0x00248294,
        .OutpostX12 = 0x00248194,
        .KorgonOutpost = 0x00248014,
        .Metropolis = 0x00248094,
        .BlackwaterCity = 0x00248014,
        .CommandCenter = 0x00247c14,
        .BlackwaterDocks = 0x00247d14,
        .AquatosSewers = 0x00247d14,
        .MarcadiaPalace = 0x00247d14,
#else
        .Lobby = 0,
        .Bakisi = 0x00248214,
        .Hoven = 0x00248414,
        .OutpostX12 = 0x00248314,
        .KorgonOutpost = 0x00248194,
        .Metropolis = 0x00248214,
        .BlackwaterCity = 0x00248194,
        .CommandCenter = 0x00247d94,
        .BlackwaterDocks = 0x00247e94,
        .AquatosSewers = 0x00247e94,
        .MarcadiaPalace = 0x00247e94,
#endif
    };
    // int OnOff = (OnOff == 1) ? 2 : OnOff;
    *(u32*)GetAddress(&vaOcclusionAddr) = OnOff;
}

ScreenVBEffect* gfxScreenVBEffect(void)
{
    return SCREEN_VISIBOMB_EFFECT;
}
ScreenInsertEffect* gfxScreenInsertEffect(void)
{
    return SCREEN_INSERT_EFFECT;
}
ViewContext* gfxViewContext(void)
{
    return VIEW_CONTEXT;
}
