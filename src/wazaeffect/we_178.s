//==============================================================================
/**
 * @file	we_178.s
 * @brief	わたほうし			178
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_WATAHOUSI
	
// =============================================================================
//
//
//	■わたほうし			178
//
//
// =============================================================================
WEST_WATAHOUSI:
	
	LOAD_PARTICLE_DROP	0,W_178_SPA
	ADD_PARTICLE 	0,W_178_178_WATAHOU_KONA, EMTFUNC_DEFENCE_POS
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_E1, 0, 3, WE_PAL_WHITE, 10, 10

	//SE_REPEAT_R	SEQ_SE_DP_W077,3,6
	SE_REPEAT_R		SEQ_SE_DP_W199,3,8

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
