//==============================================================================
/**
 * @file	we_150.s
 * @brief	はねる			150
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HANERU
	
// =============================================================================
//
//
//	■はねる			150
//
//
// =============================================================================
WEST_HANERU:
	
	LOAD_PARTICLE_DROP	0,W_150_SPA
	
	FUNC_CALL		WEST_SP_WE_150, 0,0,0,0,0,0,0,0,0
	
	LOOP_LABEL		3
	
		ADD_PARTICLE 	0,W_150_150_HANERU_BUB1, EMTFUNC_ATTACK_POS
		ADD_PARTICLE 	0,W_150_150_HANERU_BUB2, EMTFUNC_ATTACK_POS
	
		SE_L			SEQ_SE_DP_023
		WAIT			20
	
	LOOP
	
	WAIT_FLAG
	
	SEQEND
