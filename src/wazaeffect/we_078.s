//==============================================================================
/**
 * @file	we_078.s
 * @brief	しびれごな			78
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_SIBIREGONA
	
// =============================================================================
//
//
//	■しびれごな			78
//
//
// =============================================================================
WEST_SIBIREGONA:
	
	LOAD_PARTICLE_DROP	0,W_078_SPA
	ADD_PARTICLE 	0,W_078_KONA_YEL, EMTFUNC_DEFENCE_POS

	SE_REPEAT_R		SEQ_SE_DP_W077B,4,6
	
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_E1, 0, 2, WE_PAL_YELLOW, 10,10
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
