//==============================================================================
/**
 * @file	we_067.s
 * @brief	けたぐり			67
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KETAGURI
	
// =============================================================================
//
//
//	■けたぐり			67
//
//
// =============================================================================
WEST_KETAGURI:

	LOAD_PARTICLE_DROP	0,W_067_SPA
	
	SE_R			SEQ_SE_DP_007

	ADD_PARTICLE 	0,W_067_067_KETAGURI_FOOT, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_067_067_KETAGURI_BURN, EMTFUNC_DEFENCE_POS

	WAIT 2
	SE_R			SEQ_SE_DP_030
	
	ADD_PARTICLE 	0,W_067_067_KETAGURI_HIT, EMTFUNC_DEFENCE_POS
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_M_X, 0, SHAKE_M_WAIT, SHAKE_M_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
