//==============================================================================
/**
 * @file	we_001.s
 * @brief	はたく
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_363
	
// =============================================================================
//
//
//	■しぜんのめぐみ
//
//
// =============================================================================
WEST_363:

	LOAD_PARTICLE_DROP	0,W_363_SPA
	ADD_PARTICLE 	0,W_363_363_DAICHI_BALL,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_363_363_DAICHI_BALL2,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_363_363_DAICHI_RING,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_363_363_DAICHI_RING2,	EMTFUNC_ATTACK_POS

	SE_L			SEQ_SE_DP_W363
	SE_WAITPLAY_L	SEQ_SE_DP_REAPOKE,40

	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	SEQEND

