//==============================================================================
/**
 * @file	we_015.s
 * @brief	いあいぎり			15
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_IAIGIRI
	
// =============================================================================
//
//
//	■いあいぎり			15
//
//
// =============================================================================
WEST_IAIGIRI:
	
	LOAD_PARTICLE_DROP	0,W_015_SPA
	ADD_PARTICLE 	0,W_015_SLASH01, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_015_B_BURN_YEL, EMTFUNC_DEFENCE_POS

	SE_R			SEQ_SE_DP_W015
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 3, 0, 1, 2,  WE_TOOL_E1 | WE_TOOL_SSP

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
