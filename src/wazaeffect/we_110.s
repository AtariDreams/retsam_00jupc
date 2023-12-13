//==============================================================================
/**
 * @file	we_110.s
 * @brief	からにこもる			110
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KARANIKOMORU
	
// =============================================================================
//
//
//	■からにこもる			110
//
//
// =============================================================================
WEST_KARANIKOMORU:
	
	LOAD_PARTICLE_DROP	0,W_110_SPA
	ADD_PARTICLE 	0,W_110_110_KARA_GIZA1,  EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_110_110_KARA_GIZA2,	 EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_110_110_KARA_BUBBLE, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_110_110_KARA_BALL,	 EMTFUNC_ATTACK_POS

	SE_L			SEQ_SE_DP_W029
	WAIT			17
	SE_REPEAT_L		SEQ_SE_DP_W145C,3,5

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
