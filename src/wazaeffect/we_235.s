//==============================================================================
/**
 * @file	we_235.s
 * @brief	こうごうせい			235
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KOUGOUSEI
	
// =============================================================================
//
//
//	■こうごうせい			235
//
//
// =============================================================================
WEST_KOUGOUSEI:
	
	LOAD_PARTICLE_DROP	0,W_235_SPA
	ADD_PARTICLE 	0,W_235_CENTER_LIGHT01,  EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_235_LIGHT_RADIATE01, EMTFUNC_ATTACK_POS

	SE_L			SEQ_SE_DP_W025
	
	POKE_FADE_EZ	WE_PAL_YELLOW,	WE_TOOL_M1, 1
	WAIT_FLAG
	POKE_FADE_EZ	WE_PAL_WHITE,	WE_TOOL_M1, 1

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	WAIT_FLAG
	
	SEQEND
