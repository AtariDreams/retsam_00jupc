//==============================================================================
/**
 * @file	we_165.s
 * @brief	わるあがき			165
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_WARUAGAKI
	
// =============================================================================
//
//
//	■わるあがき			165
//
//
// =============================================================================
WEST_WARUAGAKI:
	
	LOAD_PARTICLE_DROP	0,W_165_SPA
	
	FUNC_CALL		WEST_SP_WE_175,	1, 0

	SE_REPEAT_L		SEQ_SE_DP_001,2,4

	WAIT			15
	
	ADD_PARTICLE 	0,W_165_165_WARUAGAKI_HIT, EMTFUNC_DEFENCE_POS

	SE_R			SEQ_SE_DP_W025B
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_S_X, 0, SHAKE_S_WAIT, SHAKE_S_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
