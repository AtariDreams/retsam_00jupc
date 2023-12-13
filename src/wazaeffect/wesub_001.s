//==============================================================================
/**
 * @file	wesub_001.s
 * @brief	ねむる
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_StatusNemuru
	
// =============================================================================
//
//
//	■ねむる
//
//
// =============================================================================
WEST_StatusNemuru:

	LOAD_PARTICLE_DROP	0, SPA_STATUS_EFF
	
	//SE_REPEAT_L		SEQ_SE_DP_W173,20,3
	SE_REPEAT_L		SEQ_SE_DP_W173B,20,1

	ADD_PARTICLE 	0, STATUS_EFF_TURN_ZZZ, EMTFUNC_ATTACK_POS
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	SEQEND
