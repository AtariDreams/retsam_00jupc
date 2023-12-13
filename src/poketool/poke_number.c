//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		poke_number.c
 *	@brief		シンオウ　全国　ポケモンナンバー　取得	
 *	@author		tomoya takahashi
 *	@data		2006.03.24
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "system.h"
#include "assert.h"
#include <string.h>

#include "include/savedata/zukanwork.h"

#include "include/poketool/poke_tool.h"

#define	__POKE_NUMBER_H_GLOBAL
#include "include/poketool/poke_number.h"

//-----------------------------------------------------------------------------
/**
 *					コーディング規約
 *		●関数名
 *				１文字目は大文字それ以降は小文字にする
 *		●変数名
 *				・変数共通
 *						constには c_ を付ける
 *						staticには s_ を付ける
 *						ポインタには p_ を付ける
 *						全て合わさると csp_ となる
 *				・グローバル変数
 *						１文字目は大文字
 *				・関数内変数
 *						小文字と”＿”と数字を使用する 関数の引数もこれと同じ
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					定数宣言
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					構造体宣言
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	セーブデータから現在の表示ポケモンナンバーモード取得
 *
 *	@param	p_savedata		セーブデータワーク
 *
 *	@return	ポケモンナンバーモード
 */
//-----------------------------------------------------------------------------
u32 PMNumber_GetMode( SAVEDATA* p_savedata )
{
	ZUKAN_WORK* p_zkn_w;
	
	// セーブデータから図鑑ワーク取得
	p_zkn_w = SaveData_GetZukanWork( p_savedata );
	return PMNumber_GetMode_ZW( p_zkn_w );
}
u32 PMNumber_GetMode_ZW( const ZUKAN_WORK* cp_zukan )
{
	BOOL zenkoku_get;
	// 全国図鑑保持フラグ取得
	zenkoku_get = ZukanWork_GetZenkokuZukanFlag( cp_zukan );
	// ポケモンナンバーを選ぶ
	if( zenkoku_get == TRUE ){
		return PM_NUMBER_ZENKOKU;
	}
	return PM_NUMBER_SHINOU;
}

//----------------------------------------------------------------------------
/**
 *	@brief	モードのポケモンナンバー取得
 *
 *	@param	mode		ポケモンナンバーモード
 *	@param	monsno		モンスターナンバー
 *
 *	@return	モードのポケモンナンバー
 */
//-----------------------------------------------------------------------------
u32 PMNumber_GetPokeNumber( u32 mode, u32 monsno )
{
	if( mode == PM_NUMBER_SHINOU ){
		return PokeZenkokuNo2ShinouNo( monsno );
	}
	return monsno;
}

