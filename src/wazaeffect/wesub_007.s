//==============================================================================
/**
 * @file	wesub_007.s
 * @brief	こんらん
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_MeroMero
	
// =============================================================================
//
//
//	■メロメロ
//
//
// =============================================================================
WEST_MeroMero:

	LOAD_PARTICLE_DROP	0, SPA_STATUS_EFF
	
	SE_REPEAT_L		SEQ_SE_DP_W204,8,4

	ADD_PARTICLE 	0, STATUS_EFF_TURN_HART, EMTFUNC_ATTACK_POS

	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	SEQEND
