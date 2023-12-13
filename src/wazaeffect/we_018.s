//==============================================================================
/**
 * @file	we_018.s
 * @brief	ふきとばし			18
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HUKITOBASI
	
// =============================================================================
//
//
//	■ふきとばし			18
//
//
// =============================================================================
WEST_HUKITOBASI:
	
	LOAD_PARTICLE_DROP	0,W_018_SPA
	
	ADD_PARTICLE 	0,W_018_DUST_BEAM, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_LSP, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	SE_FLOW_LR		SEQ_SE_DP_KAZE
	
	WAIT			10
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 1, 0, 1, 10,  WE_TOOL_E1 | WE_TOOL_SSP
	WAIT_FLAG

	///< 飛ばし
	//SE_R		SEQ_SE_DP_W081
	SE_R		SEQ_SE_DP_W059B
	//SE_R		SEQ_SE_DP_W239
	FUNC_CALL	WEST_SP_WE_DISP_OUT, 2, WE_TOOL_E1, DISP_OUT_FRAME
	WAIT_FLAG
	
	///< 消し
	FUNC_CALL	WEST_SP_WE_SSP_POKE_VANISH, 2, WE_TOOL_E1, 1
	
	///< 元の位置
	FUNC_CALL	WEST_SP_WE_DISP_DEF, 1, WE_TOOL_E1
	WAIT_FLAG

	//SE_STOP		SEQ_SE_DP_W239

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
