//==============================================================================
/**
 * @file	we_174.s
 * @brief	のろい			174
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_NOROI
	
// =============================================================================
//
//
//	■のろい
//
//
// =============================================================================
WEST_NOROI:

	LOAD_PARTICLE_DROP	0,W_174_SPA

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG
	
	SE_REPEAT_L		SEQ_SE_DP_032,12,3
	ADD_PARTICLE 	0,W_174_174_NOROI_KUGI1, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_174_174_NOROI_KUGI2, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_174_174_NOROI_KUGI3, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_174_174_NOROI_BALL,	 EMTFUNC_ATTACK_POS
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_M_X, 0, SHAKE_M_WAIT, SHAKE_M_NUM*3,  WE_TOOL_M1 | WE_TOOL_SSP,

	WAIT_FLAG

	WAIT_PARTICLE
	EXIT_PARTICLE	0,


	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG

	SEQEND

