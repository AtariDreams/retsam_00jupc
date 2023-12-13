//==============================================================================
/**
 * @file	we_001.s
 * @brief	はたく
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_374
	
// =============================================================================
//
//
//	■はたく
//
//
// =============================================================================
WEST_374:

	LOAD_PARTICLE_DROP	0,W_374_SPA

	SE_FLOW_LR		SEQ_SE_DP_W374
	
	ADD_PARTICLE 	0,W_374_374_NAGE_BALL, EMTFUNC_ATTACK_POS
	FUNC_CALL		WEST_SP_EMIT_PARABOLIC, 6, 0, 0, 0, 0, 14, 64,
	
	WAIT			13
	SE_R			SEQ_SE_DP_186
	
	ADD_PARTICLE 	0,W_374_374_NAGE_HIT1, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_374_374_NAGE_HIT2, EMTFUNC_DEFENCE_POS
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_M_X, 0, SHAKE_M_WAIT, SHAKE_M_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
		
	SEQEND

