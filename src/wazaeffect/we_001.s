//==============================================================================
/**
 * @file	we_001.s
 * @brief	はたく			1
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HATAKU
	
// =============================================================================
//
//
//	■はたく			1
//
//
// =============================================================================
WEST_HATAKU:

	LOAD_PARTICLE_DROP	0,W_001_SPA	
	ADD_PARTICLE 	0,W_001_001_HIT_MARK,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_001_001_BALL_BURN,	EMTFUNC_DEFENCE_POS

	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 1, 0, 1, 2,  WE_TOOL_E1 | WE_TOOL_SSP,

	SE_R			SEQ_SE_DP_030

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
