//==============================================================================
/**
 * @file	we_159.s
 * @brief	かくばる			159
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KAKUBARU
	
// =============================================================================
//
//
//	■かくばる			159
//
//
// =============================================================================
WEST_KAKUBARU:
	
	LOAD_PARTICLE_DROP	0,W_159_SPA
	ADD_PARTICLE 	0,W_159_159_KAKUBAR_KAKU1, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_159_159_KAKUBAR_KAKU2, EMTFUNC_ATTACK_POS

	//SE_L			SEQ_SE_DP_W112
	SE_REPEAT_L		SEQ_SE_DP_W112,15,2

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
