//==============================================================================
/**
 * @file	we_313.s
 * @brief	うそなき			313
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_USONAKI
	
// =============================================================================
//
//
//	■うそなき			313
//
//
// =============================================================================
WEST_USONAKI:

	LOAD_PARTICLE_DROP	0,W_313_SPA
	ADD_PARTICLE 	0,W_313_313_BUB_BURN1, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_313_313_BUB_BURN2, EMTFUNC_ATTACK_POS
	
	FUNC_CALL		WEST_SP_WE_175,	1, 0
	SE_REPEAT_L		SEQ_SE_DP_W039,8,4
	
	WAIT_FLAG
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
