//==============================================================================
/**
 * @file	we_156.s
 * @brief	ねむる			156
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_NEMURU
	
// =============================================================================
//
//
//	■ねむる			156
//
//
// =============================================================================
WEST_NEMURU:
	
	LOAD_PARTICLE_DROP	0,W_156_SPA
	ADD_PARTICLE 	0,W_156_S_ZZZ01, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_156_S_ZZZ02, EMTFUNC_ATTACK_POS

	//SE_REPEAT_L		SEQ_SE_DP_W173,20,3
	//SE_REPEAT_L		SEQ_SE_DP_W173B,30,2
	//SE_REPEAT_L		SEQ_SE_DP_W173C,20,3
	SE_REPEAT_L		SEQ_SE_DP_W173B,20,1

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
