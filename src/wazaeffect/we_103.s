//==============================================================================
/**
 * @file	we_103.s
 * @brief	いやなおと			103
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_IYANAOTO
	
// =============================================================================
//
//
//	■いやなおと			103
//
//
// =============================================================================
WEST_IYANAOTO:
	
	LOAD_PARTICLE_DROP	0,W_103_SPA
	ADD_PARTICLE 	0,W_103_RING_RADIATE01, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_103_RING_RADIATE02, EMTFUNC_ATTACK_POS

	SE_L			SEQ_SE_DP_020
	
	WAIT			15

	LOOP_LABEL		1
		FUNC_CALL		WEST_SP_WE_T05, 3,  8,  +8, WE_TOOL_E1 | WE_TOOL_SSP
		WAIT		8
		FUNC_CALL		WEST_SP_WE_T05, 3, 16, -16, WE_TOOL_E1 | WE_TOOL_SSP
		WAIT		16
		FUNC_CALL		WEST_SP_WE_T05, 3,  8,  +8, WE_TOOL_E1 | WE_TOOL_SSP
		WAIT		8
	LOOP

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
