//==============================================================================
/**
 * @file	we_046.s
 * @brief	ほえる			46
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HOERU
	
// =============================================================================
//
//
//	■ほえる			46
//
//
// =============================================================================
WEST_HOERU:
	
	LOAD_PARTICLE_DROP	0,W_046_SPA
	
	ADD_PARTICLE 	0,W_046_046_VOICE_BEAM, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_LSP, OPERATOR_AXIS_AT, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	VOICE_PLAY		PV_W_HOERU1, WAZA_SE_PAN_AT, PV_VOL_DEFAULT
	VOICE_WAIT_STOP 0
	VOICE_PLAY		PV_W_HOERU2, WAZA_SE_PAN_AT, PV_VOL_MAX

	ADD_PARTICLE 	0,W_046_046_RING_BIG,	EMTFUNC_ATTACK_POS
	
	FUNC_CALL	WEST_SP_WE_SSP_POKE_SCALE_UPDOWN, 8, (WE_TOOL_M1 | WE_TOOL_SSP), 100, 120, 100, 120, 100, 1, (5 << 16) | 5
	
	WAIT		10
	
	///< 飛ばし
	FUNC_CALL	WEST_SP_WE_DISP_OUT, 2, WE_TOOL_E1, DISP_OUT_FRAME
	WAIT_FLAG
	
	///< 消し
	FUNC_CALL	WEST_SP_WE_SSP_POKE_VANISH, 2, WE_TOOL_E1, 1, 0
	
	///< 元の位置
	FUNC_CALL	WEST_SP_WE_DISP_DEF, 1, WE_TOOL_E1
	
	WAIT_FLAG

	VOICE_WAIT_STOP 0
	WAIT			2			///< 鳴き声TCB削除待ち

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
