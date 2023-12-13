//==============================================================================
/**
 * @file	we_252.s
 * @brief	ねこだまし			252
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_NEKODAMASI
	
// =============================================================================
//
//
//	■ねこだまし			252
//
//
// =============================================================================
#define NEKODAMASI_RES_NO			(0)
#define NEKODAMASI_RES_NUM		(1)
#define NEKODAMASI_ACT_NUM		(1)

#define NEKODAMASI_WNDMSK_WAIT	( 2 )

#define NEKODAMASI_SSP_SC_WAIT	( 16 / 2 )


WEST_NEKODAMASI:

	LOAD_PARTICLE_DROP	0,W_252_SPA

	///<初期化処理　読み込み処理
	CATS_RES_INIT		NEKODAMASI_RES_NO, NEKODAMASI_ACT_NUM, NEKODAMASI_RES_NUM, NEKODAMASI_RES_NUM, NEKODAMASI_RES_NUM, NEKODAMASI_RES_NUM, 0, 0
	CATS_CAHR_RES_LOAD	NEKODAMASI_RES_NO, EFFECT_252_NCGR_BIN
	CATS_PLTT_RES_LOAD	NEKODAMASI_RES_NO, EFFECT_252_NCLR, 1
	CATS_CELL_RES_LOAD	NEKODAMASI_RES_NO, EFFECT_252_NCER_BIN
	CATS_CELL_ANM_RES_LOAD	NEKODAMASI_RES_NO, EFFECT_252_NANR_BIN

	///<動作
	CATS_ACT_ADD		NEKODAMASI_RES_NO, WEST_CSP_WE_252, EFFECT_252_NCGR_BIN, EFFECT_252_NCLR, EFFECT_252_NCER_BIN, EFFECT_252_NANR_BIN, 0, 0, 0,0,
	
	SE_WAITPLAY_C	SEQ_SE_DP_W260,20
	WAIT	NEKODAMASI_WNDMSK_WAIT
	FUNC_CALL WEST_SP_WE_252_BACK,0,0,
	
	WAIT	4
	ADD_PARTICLE 	0,W_252_068_HIT_MARK, EMTFUNC_DEFENCE_POS
	WAIT	4
	WAIT	NEKODAMASI_SSP_SC_WAIT
	FUNC_CALL	WEST_SP_WE_252_POKE, 0,0
	SE_R		SEQ_SE_DP_W166

	WAIT_FLAG

	///<破棄処理
	CATS_RES_FREE		NEKODAMASI_RES_NO

	WAIT_PARTICLE
	EXIT_PARTICLE	0,	
	SEQEND
