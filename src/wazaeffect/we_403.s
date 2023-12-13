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

	.global		WEST_403
	
// =============================================================================
//
//
//	■はたく
//
//
// =============================================================================
WEST_403:
	LOAD_PARTICLE_DROP	0,W_403_SPA
	
	SE_R			SEQ_SE_DP_BRADE

	ADD_PARTICLE 	0,W_403_403_LINE_SLASH, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	ADD_PARTICLE 	0,W_403_403_LINE_BURN, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	WAIT			5
	SE_R			SEQ_SE_DP_208

	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_S_X, 0, SHAKE_S_WAIT, SHAKE_S_NUM,  WE_TOOL_E1 | WE_TOOL_SSP,
	POKE_FADE_EZ	WE_PAL_WHITE, WE_TOOL_E1, 1	

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND

