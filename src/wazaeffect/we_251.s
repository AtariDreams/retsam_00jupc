//==============================================================================
/**
 * @file	we_251.s
 * @brief	ふくろだたき			251
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HUKURODATAKI
	
// =============================================================================
//
//
//	■ふくろだたき			251
//
//
// =============================================================================
WEST_HUKURODATAKI:
	
	LOAD_PARTICLE_DROP	0,W_251_SPA
	ADD_PARTICLE 	0,W_251_251_2HIT_PUNCH, EMTFUNC_DEFENCE_POS
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_S_X, 0, SHAKE_S_WAIT, SHAKE_S_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,

	SE_R			SEQ_SE_DP_W233B

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
