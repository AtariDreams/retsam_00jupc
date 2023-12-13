//==============================================================================
/**
 * @file	we_137.s
 * @brief	へびにらみ			137
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HEBINIRAMI
	
// =============================================================================
//
//
//	■へびにらみ			137
//
//
// =============================================================================
#define FACE_RES_NO			(0)
#define FACE_RES_NUM		(1)
#define FACE_ACT_NUM		(1)
WEST_HEBINIRAMI:

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG

	///<初期化処理　読み込み処理
	CATS_RES_INIT		FACE_RES_NO, FACE_ACT_NUM, FACE_RES_NUM, FACE_RES_NUM, FACE_RES_NUM, FACE_RES_NUM, 0, 0
	CATS_CAHR_RES_LOAD	FACE_RES_NO, EFFECT_137_NCGR_BIN
	CATS_PLTT_RES_LOAD	FACE_RES_NO, EFFECT_137_NCLR, 1
	CATS_CELL_RES_LOAD	FACE_RES_NO, EFFECT_137_NCER_BIN
	CATS_CELL_ANM_RES_LOAD	FACE_RES_NO, EFFECT_137_NANR_BIN
	
	///<動作
	CATS_ACT_ADD		FACE_RES_NO, WEST_CSP_WE_184, EFFECT_137_NCGR_BIN, EFFECT_137_NCLR, EFFECT_137_NCER_BIN, EFFECT_137_NANR_BIN, 0, 0, 0,0,

	WAIT 8
	SE_C			SEQ_SE_DP_W043D
	
	WAIT_FLAG

	///<破棄処理
	CATS_RES_FREE		FACE_RES_NO

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
	
	SEQEND
