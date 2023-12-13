//==============================================================================
/**
 * @file	we_132.s
 * @brief	からみつく			132
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_KARAMITUKU
	
// =============================================================================
//
//
//	■からみつく			132
//
//
// =============================================================================
#define KARAMI_RES_NO		(0)
#define KARAMI_RES_NUM		(1)
#define KARAMI_ACT_NUM		(4)
WEST_KARAMITUKU:
	
	///<初期化処理　読み込み処理
	CATS_RES_INIT		KARAMI_RES_NO, KARAMI_ACT_NUM, KARAMI_RES_NUM, KARAMI_RES_NUM, KARAMI_RES_NUM, KARAMI_RES_NUM, 0, 0
	CATS_CAHR_RES_LOAD	KARAMI_RES_NO, EFFECT_132_NCGR_BIN
	CATS_PLTT_RES_LOAD	KARAMI_RES_NO, EFFECT_132_NCLR, 1
	CATS_CELL_RES_LOAD	KARAMI_RES_NO, EFFECT_132_NCER_BIN
	CATS_CELL_ANM_RES_LOAD	KARAMI_RES_NO, EFFECT_132_NANR_BIN
	
	///<動作
	CATS_ACT_ADD		KARAMI_RES_NO, WEST_CSP_WE_132, EFFECT_132_NCGR_BIN, EFFECT_132_NCLR, EFFECT_132_NCER_BIN, EFFECT_132_NANR_BIN, 0, 0, 0,0,

	LOOP_LABEL	5
		WAIT 2
		SE_R			SEQ_SE_DP_W020
	LOOP

	LOOP_LABEL	2
		WAIT 6
		SE_R			SEQ_SE_DP_W030
	LOOP
	WAIT_FLAG

	///<破棄処理
	CATS_RES_FREE		KARAMI_RES_NO
	
	SEQEND
