//==============================================================================
/**
 * @file	we_175.s
 * @brief	じたばた			175
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_ZITABATA
	
// =============================================================================
//
//
//	■じたばた			175
//
//
// =============================================================================
WEST_ZITABATA:

	LOAD_PARTICLE_DROP	0,W_175_SPA
	//SE_REPEAT_L		SEQ_SE_DP_050,2,6
	SE_REPEAT_L		SEQ_SE_DP_050,3,6

	FUNC_CALL		WEST_SP_WE_175,	1, 0
	ADD_PARTICLE 	0,W_175_175_JITABATA_BALL,	EMTFUNC_ATTACK_POS
	
	WAIT_FLAG
	
	ADD_PARTICLE 	0,W_175_175_JITABATA_HIT,	EMTFUNC_DEFENCE_POS
	
	SE_R			SEQ_SE_DP_W025B

	FUNC_CALL		WEST_SP_SHAKE,	5, 1, 2, 0, 0, 5
	
	WAIT_FLAG

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
