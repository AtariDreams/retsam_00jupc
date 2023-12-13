//============================================================================================
/**
 * @file		scr_pokepark.h
 * @brief
 * @author		tamada GAME FREAK inc.
 * @date		2006.05.15
 */
//============================================================================================

#ifndef	__SCR_POKEPARK_H__
#define	__SCR_POKEPARK_H__

//============================================================================================
//
//		スクリプト・プログラム共通で参照する定義
//
//============================================================================================

#define	POKEPARK_SCORE_CAPTURE	0		/* スコア：捕獲 */
#define	POKEPARK_SCORE_TIME		1		/* スコア：時間 */
#define	POKEPARK_SCORE_TYPE		2		/* スコア：タイプ */
#define	POKEPARK_SCORE_TOTAL	3		/* スコア：トータル */

#define	POKEPARK_MODE_START		0		/* 制御モード：開始 */
#define	POKEPARK_MODE_END		1		/* 制御モード：終了 */

//============================================================================================
//
//		プログラムでのみ参照する定義
//
//============================================================================================
//__ASM_NO_DEF_が定義されている場合、アセンブラソースなので以下は無効にする
#ifndef	__ASM_NO_DEF_

#include "vm.h"

//-----------------------------------------------------------------------------
///	ポケパーク用コマンド：開始・終了制御
//-----------------------------------------------------------------------------
extern BOOL EvCmdPokeParkControl(VM_MACHINE * core);

//-----------------------------------------------------------------------------
///	ポケパーク用コマンド：預けたポケモンを数える
//-----------------------------------------------------------------------------
extern BOOL EvCmdPokeParkDepositCount(VM_MACHINE * core);

//-----------------------------------------------------------------------------
///	ポケパーク用コマンド：預けたポケモンをボックスに転送
//-----------------------------------------------------------------------------
extern BOOL EvCmdPokeParkTransMons(VM_MACHINE * core);

//-----------------------------------------------------------------------------
///	ポケパーク用コマンド：スコア取得
//-----------------------------------------------------------------------------
extern BOOL EvCmdPokeParkGetScore(VM_MACHINE * core);


#endif	//__ASM_NO_DEF_

#endif	/* __SCR_POKEPARK_H__ */
