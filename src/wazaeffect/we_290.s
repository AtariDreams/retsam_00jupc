//==============================================================================
/**
 * @file	we_290.s
 * @brief	ひみつのちから			290
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HIMITUNOTIKARA
	
// =============================================================================
//
//
//	■ひみつのちから			290
//
//
// =============================================================================
WEST_HIMITUNOTIKARA:
	
	LOAD_PARTICLE_DROP	0,W_HIT_01_SPA
	ADD_PARTICLE 	0,W_HIT_01_B_BURN_RED, EMTFUNC_DEFENCE_POS

	SE_R			SEQ_SE_DP_BASI

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
