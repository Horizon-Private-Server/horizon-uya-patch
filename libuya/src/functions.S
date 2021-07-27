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

#else
FUNCTION(printf, 0x0011D5D8)
FUNCTION(sprintf, 0x0011D920)
FUNCTION(memset, 0x0011A518)
FUNCTION(memcpy, 0x0011A370)
FUNCTION(strncpy, 0x0011AFC8)
#endif

#--------------------------------------------------------
#------------------------- MATH -------------------------
#--------------------------------------------------------
# FUNCTION(cosf, 0x00135878)
FUNCTION(asinf, 0x00136278)
FUNCTION(sqrtf, 0x00136C90)
FUNCTION(powf, 0x00136788)
FUNCTION(fabsf, 0x00135960)
FUNCTION(fptodp, 0x00131918)
FUNCTION(floorf, 0x001359A8)

#--------------------------------------------------------
#------------------------- SHA1 -------------------------
#--------------------------------------------------------
FUNCTION(sha1, 0x01EB30C8)
