//==============================================================================
/**
 * @file	we_225.s
 * @brief	りゅうのいぶき			225
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_RYUUNOIBUKI
	
// =============================================================================
//
//
//	■りゅうのいぶき			225
//
//
// =============================================================================
WEST_RYUUNOIBUKI:
	
	LOAD_PARTICLE_DROP	0,W_225_SPA

	SE_REPEAT_C		SEQ_SE_DP_400,3,6
	
	SIDE_JP			0,WEST_225_SIDE_MINE, WEST_225_SIDE_MINE
	
	SEQEND

WEST_225_SIDE_MINE:

	ADD_PARTICLE 	0,W_225_225_RYUIBUKI_FIRE1A, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_225, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE	
	ADD_PARTICLE 	0,W_225_225_RYUIBUKI_FIRE1B, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_225, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE	
	
	SE_R			SEQ_SE_DP_W172
	WAIT			15
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 5,  WE_TOOL_E1, 0, 2, WE_PAL_RED, 12
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 2, 0, 1, 8,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	WAIT_FLAG

	SEQEND

CONTEST:

	ADD_PARTICLE 	0,W_225_225_RYUIBUKI_FIRE1A, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_RSP, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE	
	ADD_PARTICLE 	0,W_225_225_RYUIBUKI_FIRE1B, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_RSP, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE	
	
	KEY_WAIT
			
	SE_R			SEQ_SE_DP_W172
	WAIT			15
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 5,  WE_TOOL_E1, 0, 2, WE_PAL_RED, 12
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 2, 0, 1, 8,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	WAIT_FLAG

	SEQEND