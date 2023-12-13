//==============================================================================
/**
 * @file	we_021.s
 * @brief	たたきつける			21
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_TATAKITUKERU
	
// =============================================================================
//
//
//	■たたきつける			21
//
//
// =============================================================================
WEST_TATAKITUKERU:
	
	LOAD_PARTICLE_DROP	0,W_021_SPA
	ADD_PARTICLE 	0,W_021_021_HIT_MARK, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_021_B_BURN_RED2, EMTFUNC_DEFENCE_POS
	
	FUNC_CALL		WEST_SP_WE_T10,	4, 2, +14, +4, WE_TOOL_M1 | WE_TOOL_SSP
	WAIT			1
	FUNC_CALL		WEST_SP_WE_T10,	4, 2, -14, -4, WE_TOOL_E1 | WE_TOOL_SSP
	
	WAIT_FLAG

	FUNC_CALL		WEST_SP_WE_T10,	4, 2, -14, -4, WE_TOOL_M1 | WE_TOOL_SSP
	FUNC_CALL		WEST_SP_WE_T10,	4, 4, +14, +4, WE_TOOL_E1 | WE_TOOL_SSP

	SE_R			SEQ_SE_DP_W025B

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
