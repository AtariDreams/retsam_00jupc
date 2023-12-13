//==============================================================================
/**
 * @file	we_163.s
 * @brief	きりさく			163
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KIRISAKU
	
// =============================================================================
//
//
//	■きりさく			163
//
//
// =============================================================================
WEST_KIRISAKU:
	
	LOAD_PARTICLE_DROP	0,W_163_SPA
	ADD_PARTICLE 	0,W_163_163_KIRISAKU_BALL, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_163_163_KIRISAKU_LINE, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_163_163_KIRISAKU_BALL, EMTFUNC_DEFENCE_POS

	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 4, 0, 1, 3,  WE_TOOL_E1 | WE_TOOL_SSP,
	
	SE_R			SEQ_SE_DP_W013
	WAIT_FLAG


	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
