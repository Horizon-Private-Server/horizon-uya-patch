
  .section .text.functions
	.globl	_lodScale;
_lodScale: 
  .align 4
  .float 1.0
  .float 0.0
  .float 0.0
  .float 0.0


	.globl	_correctTieLod;
  .globl _correctTieLod_Jump;
_correctTieLod:
  add	        $s6,   $s6,   $v1;
  la          $s2, _lodScale;
  lqc2        $vf2,  0($s2);
  vmulx.xyz   vf16, vf16, vf2x;
  _correctTieLod_Jump:
  j 0x004c68e0;

/*
 * NAME : onGameplayLoadHook
 * DESCRIPTION : Capture the caller's gameplay pointer before entering C.
 * NOTES :
 * ARGS :
 * RETURN :
 * AUTHOR : Troy "Metroynome" Pruitt
 */
	.section .text.functions
	.set push
	.set noreorder
	.align 2
	.globl onGameplayLoadHook
	.ent onGameplayLoadHook
onGameplayLoadHook:
	j onGameplayLoad
	move $a2, $s6
	.end onGameplayLoadHook
	.set pop
