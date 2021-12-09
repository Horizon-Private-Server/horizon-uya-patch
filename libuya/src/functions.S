#define FUNCTION(name, address) \
    .set	push;		        \
	.set	noreorder;	        \
	.section	.text.##name,"ax",@progbits; \
	.align 2;		            \
	.globl	name;		        \
	.type	name,@function;	    \
	.ent	name,0;		        \
name:	j address;              \
	nop;			            \
	.end	name;		        \
	.size	name,.-name;	    \
	.set	pop;


#--------------------------------------------------------
#------------------- STANDARD LIBRARY -------------------
#--------------------------------------------------------

#ifdef UYA_PAL

FUNCTION(printf, 0x0011e718)
FUNCTION(sprintf, 0x00120048)
FUNCTION(memset, 0x0011d534)
FUNCTION(memcpy, 0x0011d380)
FUNCTION(strncpy, 0x00120d28)

#else
# NTSC Functions
FUNCTION(printf, 0x0011e718)
FUNCTION(sprintf, 0x00120048)
FUNCTION(memset, 0x0011d534)
FUNCTION(memcpy, 0x0011d380)
FUNCTION(strncpy, 0x00120d28)

#endif

#--------------------------------------------------------
#------------------------- MATH -------------------------
#--------------------------------------------------------
#ifdef UYA_PAL

#else
# FUNCTION(cosf, 0x00136aa8)
FUNCTION(asinf, 0x00136278) # not ported
FUNCTION(sqrtf, 0x00136C90) # not ported
FUNCTION(powf, 0x00136788) # not ported
FUNCTION(fabsf, 0x00135960) # not ported
FUNCTION(fptodp, 0x00131918) # not ported
FUNCTION(floorf, 0x001359A8) # not ported
#endif

#--------------------------------------------------------
#------------------------- SHA1 -------------------------
#--------------------------------------------------------
#ifdef UYA_PAL

#else
FUNCTION(sha1, 0x01EB30C8) # not ported
#endif
