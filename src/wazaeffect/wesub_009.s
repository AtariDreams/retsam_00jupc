//==============================================================================
/**
 * @file	wesub_006.s
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

	.global		WEST_StatusKonran
	
// =============================================================================
//
//
//	■こんらん
//
//
// =============================================================================
WEST_ItemTR:

	LOAD_PARTICLE_DROP	0, SPA_ITEM_EFF
	
	SE_L			SEQ_SE_DP_BT_001

	ADD_PARTICLE 	0, ITEM_EFF_ITEM_TRANER, EMTFUNC_ATTACK_POS
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	SEQEND
