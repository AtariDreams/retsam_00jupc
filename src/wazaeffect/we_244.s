//==============================================================================
/**
 * @file	we_244.s
 * @brief	じこあんじ			244
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_ZIKOANZI
	
// =============================================================================
//
//
//	■じこあんじ			244
//
//
// =============================================================================
WEST_ZIKOANZI:
	LOAD_PARTICLE_DROP	0,W_244_SPA
	
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG
	
	SE_L			SEQ_SE_DP_W060B

	ADD_PARTICLE 	0,W_244_244_SPIRAL, EMTFUNC_ATTACK_POS

	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG	

	SEQEND
