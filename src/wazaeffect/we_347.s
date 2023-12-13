//==============================================================================
/**
 * @file	we_347.s
 * @brief	めいそう			347
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_MEISOU
	
// =============================================================================
//
//
//	■めいそう			347
//
//
// =============================================================================
WEST_MEISOU:
	
	LOAD_PARTICLE_DROP	0,W_347_SPA

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK
	WAIT_FLAG

	SE_REPEAT_L		SEQ_SE_DP_W048,2,4

	ADD_PARTICLE 	0,W_347_347_BALL_BURN, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_347_347_RING_BURN, EMTFUNC_ATTACK_POS

	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK
	WAIT_FLAG
	
	SEQEND
