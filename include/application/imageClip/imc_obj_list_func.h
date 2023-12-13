//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		imc_obj_list_func.h
 *	@brief		沢山のシステムが使用するオブジェリスト用関数を宣言
 *	@author		tomoya takahashi
 *	@data		2005.09.30
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __IMC_OBJ_LIST_FUNC_H__
#define __IMC_OBJ_LIST_FUNC_H__

#include "include/application/imageClip/imc_obj_list.h"

#undef GLOBAL
#ifdef	__IMC_OBJ_LIST_FUNC_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					定数宣言
*/
//-----------------------------------------------------------------------------
#define IMC_OBJ_LIST_FUNC_TP_NOW	(0xffffffff)

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

//-----------------------------------------------------------------------------
/**
*	アクセサリとポケモンオブジェの表示優先順位、当たり判定、座標変更
*	を行う関数
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief	表示優先順位設定
 *
 *	@param	work	オブジェリストワーク
 *	@param	pri		表示優先順位
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_ACCEPOKE_Pri( IMC_OBJLIST* work, int pri );
//----------------------------------------------------------------------------
/**
 *
 *	@brief	タッチパネル当たり判定
 *
 *	@param	work	オブジェリストワーク
 *	@param	x		判定ｘ座標	（今の状態をしよう{sys}IMC_OBJ_LIST_FUNC_TP_NOW ）
 *	@param	y		判定ｙ座標
 *
 *	@retval	TRUE	あたった
 *	@retval	FALSE	あたらなかった
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL BOOL IMC_OBJLIST_ACCEPOKE_Hit( IMC_OBJLIST* work, int x, int y );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	細かいタッチパネル当たり判定
 *
 *	@param	work	オブジェリストワーク
 *	@param	offs_x	左上からのオフセットｘ座標
 *	@param	offs_y	左上からのオフセットｙ座標 
 *	@param	drawData キャラクタデータテーブル
 *
 *	@retval	TRUE	あたった
 *	@retval	FALSE	あたらなかった
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL BOOL IMC_OBJLIST_ACCEPOKE_HitEx( IMC_OBJLIST* work, int* offs_x, int* offs_y, NNSG2dCharacterData** drawData );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	座標設定
 *
 *	@param	work	オブジェクトリストワーク
 *	@param	x		ｘ座標
 *	@param	y		ｙ座標
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_ACCEPOKE_Mat( IMC_OBJLIST* work, int x, int y );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	座標を取得
 *
 *	@param	obj		オブジェワーク
 *	@param	x		ｘ座標格納先
 *	@param	y		ｙ座標格納先
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_ACCEPOKE_MatGet( IMC_OBJLIST* obj, int* x, int* y );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	サイズを取得
 *
 *	@param	obj		オブジェワーク
 *	@param	x		ｘサイズ格納先
 *	@param	y		ｙサイズ格納先
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_ACCEPOKE_SizeGet( IMC_OBJLIST* obj, int* x, int* y );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	食い込んでよいサイズを取得
 *
 *	@param	obj		オブジェワーク
 *	@param	x		ｘサイズ格納先
 *	@param	y		ｙサイズ格納先
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
#if PLFIX_S2407
GLOBAL void IMC_OBJLIST_ACCEPOKE_InSizeGet( IMC_OBJLIST* obj, int* p_left, int* p_right, int* p_top, int* p_bottom );
#else
GLOBAL void IMC_OBJLIST_ACCEPOKE_InSizeGet( IMC_OBJLIST* obj, int* x, int* y );
#endif

//----------------------------------------------------------------------------
/**
 *
 *	@brief	リストごと足し算移動させる
 *	
 *	@param	top		リスト先頭ポインタ
 *	@param	x		足すX座標
 *	@param	y		足すY座標
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_ACCEPOKE_ListMove( IMC_OBJLIST* top, int x, int y );


//----------------------------------------------------------------------------
/**
 *	@brief	オブジェクト矩形取得　食い込んでよいサイズ分減らし済み
 *	
 *	@param	p_obj	ワーク
 *	@param	top		上
 *	@param	bottom	下
 *	@param	left	左
 *	@param	right	右
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_GetObjRectSubInSize( IMC_OBJLIST* p_obj, int* top, int* bottom, int* left, int* right );

//----------------------------------------------------------------------------
/**
 *	@brief	中心座標を取得する
 *
 *	@param	p_obj	オブジェクト
 *	@param	x		ｘ座標格納先
 *	@param	y		ｙ座標格納先
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
GLOBAL void IMC_OBJLIST_GetObjCenterMat( IMC_OBJLIST* p_obj, int* x, int* y );
	

#undef	GLOBAL
#endif		// __IMC_OBJ_LIST_FUNC_H__

