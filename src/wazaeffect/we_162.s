//==============================================================================
/**
 * @file	we_162.s
 * @brief	いかりのまえば			162
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_IKARINOMAEBA
	
// =============================================================================
//
//
//	■いかりのまえば			162
//
//
// =============================================================================
WEST_IKARINOMAEBA:
	
	LOAD_PARTICLE_DROP	0,W_162_SPA
	
	ADD_PARTICLE 	0,W_162_162_MAEBA_BALL1,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_162_162_MAEBA_HIT,		EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_162_162_MAEBA_LINE,		EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_162_162_MAEBA_LINE2,	EMTFUNC_DEFENCE_POS

	SE_R			SEQ_SE_DP_W044
	SE_WAITPLAY_R	SEQ_SE_DP_185,5

	WAIT			5
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_M_X, 0, SHAKE_M_WAIT, SHAKE_M_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
