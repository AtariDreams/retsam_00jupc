//==============================================================================
/**
 * @file	we_207.s
 * @brief	いばる			207
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_IBARU
	
// =============================================================================
//
//
//	■いばる			207
//
//
// =============================================================================
#define IBARU_RES_NO			(0)
#define IBARU_RES_NUM		(1)
#define IBARU_ACT_NUM		(1)
WEST_IBARU:
	
	///<初期化処理　読み込み処理
	CATS_RES_INIT		IBARU_RES_NO, IBARU_ACT_NUM, IBARU_RES_NUM, IBARU_RES_NUM, IBARU_RES_NUM, IBARU_RES_NUM, 0, 0
	CATS_CAHR_RES_LOAD	IBARU_RES_NO, EFFECT_207_NCGR_BIN
	CATS_PLTT_RES_LOAD	IBARU_RES_NO, EFFECT_207_NCLR, 1
	CATS_CELL_RES_LOAD	IBARU_RES_NO, EFFECT_207_NCER_BIN
	CATS_CELL_ANM_RES_LOAD	IBARU_RES_NO, EFFECT_207_NANR_BIN
	
	///<動作
	FUNC_CALL		WEST_SP_WE_207_MAIN, 0,0,0,0,0,0,0,0,0

	//SE_L			SEQ_SE_DP_W207
	SE_L			SEQ_SE_DP_W207D
	WAIT			30
	WAIT_FLAG
	
	CATS_ACT_ADD		IBARU_RES_NO, WEST_CSP_WE_207_SUB, EFFECT_207_NCGR_BIN, EFFECT_207_NCLR, EFFECT_207_NCER_BIN, EFFECT_207_NANR_BIN, 0, 0, 0,0,
	POKE_FADE_EZ	WE_PAL_RED, WE_TOOL_E1, 1

	//SE_R			SEQ_SE_DP_BASI
	SE_R			SEQ_SE_DP_W207B
	WAIT	8
	//SE_R			SEQ_SE_DP_BASI
	SE_R			SEQ_SE_DP_W207B

	WAIT_FLAG

	///<破棄処理
	CATS_RES_FREE		IBARU_RES_NO
	
	SEQEND
