//==============================================================================
/**
 * @file	wesub_005.s
 * @brief	まひ
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_StatusMahi
	
// =============================================================================
//
//
//	■まひ
//
//
// =============================================================================
WEST_StatusMahi:

	LOAD_PARTICLE_DROP	0,W_084_SPA
	
	SE_L			SEQ_SE_DP_W085C

	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_M1, 0, 1, WE_PAL_BLACK, 15, 0
	
	ADD_PARTICLE 	0,W_084_084_THUNDER_SPARK01,	EMTFUNC_ATTACK_POS

	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	SEQEND

