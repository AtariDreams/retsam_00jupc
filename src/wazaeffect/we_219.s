//==============================================================================
/**
 * @file	we_219.s
 * @brief	しんぴのまもり			219
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_SINPINOMAMORI
	
// =============================================================================
//
//
//	■しんぴのまもり			219
//
//
// =============================================================================
WEST_SINPINOMAMORI:
	
	LOAD_PARTICLE_DROP	0,W_219_SPA
		
	SE_REPEAT_L		SEQ_SE_DP_W208,4,4

	ADD_PARTICLE 	0,W_219_219_SINPMAMORI_RING, EMTFUNC_AT_SIDE
	ADD_PARTICLE 	0,W_219_219_SINPMAMORI_KIRA, EMTFUNC_AT_SIDE

	WAIT			15

	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 5, WE_TOOL_M1, 0, 1, WE_PAL_WHITE, 12
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 5, WE_TOOL_M2, 0, 1, WE_PAL_WHITE, 12
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
