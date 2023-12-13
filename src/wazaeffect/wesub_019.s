//==============================================================================
/**
 * @file	we_240.s
 * @brief	あまごい			240
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_AMAGOI
	
// =============================================================================
//
//
//	■あまごい			240
//
//
// =============================================================================
WEST_AMAGOI:
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,	
	
	LOAD_PARTICLE_DROP	0,W_240_SPA
	ADD_PARTICLE 	0,W_240_240_RAIN_SHOWER, EMTFUNC_DUMMY

	SE_C			SEQ_SE_DP_W240

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
	SEQEND
