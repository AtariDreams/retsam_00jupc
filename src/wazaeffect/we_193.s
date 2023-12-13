//==============================================================================
/**
 * @file	we_193.s
 * @brief	みやぶる			193
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_MIYABURU
	
// =============================================================================
//
//
//	■みやぶる			193
//
//
// =============================================================================
#define MIYABURU_RES_NO			(0)
#define MIYABURU_RES_NUM		(1)
#define MIYABURU_ACT_NUM		(1)
WEST_MIYABURU:
	
	///<初期化処理　読み込み処理
	CATS_RES_INIT		MIYABURU_RES_NO, MIYABURU_ACT_NUM, MIYABURU_RES_NUM, MIYABURU_RES_NUM, MIYABURU_RES_NUM, MIYABURU_RES_NUM, 0, 0
	CATS_CAHR_RES_LOAD	MIYABURU_RES_NO, EFFECT_193_NCGR_BIN
	CATS_PLTT_RES_LOAD	MIYABURU_RES_NO, EFFECT_193_NCLR, 1
	CATS_CELL_RES_LOAD	MIYABURU_RES_NO, EFFECT_193_NCER_BIN
	CATS_CELL_ANM_RES_LOAD	MIYABURU_RES_NO, EFFECT_193_NANR_BIN
	
	///<動作
	CATS_ACT_ADD		MIYABURU_RES_NO, WEST_CSP_WE_193, EFFECT_193_NCGR_BIN, EFFECT_193_NCLR, EFFECT_193_NCER_BIN, EFFECT_193_NANR_BIN, 0, 0, 0,0,


	//LOOP_LABEL	5
	LOOP_LABEL	6
		WAIT 2
		SE_R			SEQ_SE_DP_100
		WAIT 10
	LOOP

	WAIT		2
	//SE_R				SEQ_SE_DP_154
	SE_R				SEQ_SE_DP_W043
	
	
	WAIT_FLAG

	///<破棄処理
	CATS_RES_FREE		MIYABURU_RES_NO
	
	SEQEND
