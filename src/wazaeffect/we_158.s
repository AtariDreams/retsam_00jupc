//==============================================================================
/**
 * @file	we_158.s
 * @brief	ひっさつまえば			158
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HISSATUMAEBA
	
// =============================================================================
//
//
//	■ひっさつまえば			158
//
//
// =============================================================================
WEST_HISSATUMAEBA:
	
	LOAD_PARTICLE_DROP	0,W_158_SPA
	ADD_PARTICLE 	0,W_158_158_HIMAEBA_LINE1,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_158_158_HIMAEBA_LINE2,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_158_158_HIMAEBA_HIT1,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_158_158_HIMAEBA_HIT2,	EMTFUNC_DEFENCE_POS
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 2, 2, 1, 2,  WE_TOOL_E1 | WE_TOOL_SSP,

	SE_R			SEQ_SE_DP_W044
	SE_WAITPLAY_R	SEQ_SE_DP_W043,5

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
