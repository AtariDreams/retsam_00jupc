//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		imc_sys.h
 *	@brief		イメージクリップメインシステム
 *	@author		tomoya takahashi
 *	@data		2005.09.21
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __IMC_SYS_H__
#define __IMC_SYS_H__

#include "include/system/procsys.h"
#include "include/savedata/imageclip_data.h"
#include "include/savedata/config.h"
#include "include/contest/contest.h"
#include "include/savedata/record.h"

#undef GLOBAL
#ifdef	__IMC_SYS_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif


//-----------------------------------------------------------------------------
/**
 *	【説明】
 *		イメージクリッププロックは2つあります。
 *		１：通常の作成プロック			[JAMP_NORMAL]
 *		２：コンテスト用プロック		[JAMP_CONTEST]
 *		です。
 */
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
/**
 *					通常
 * [JAMP_NORMAL]
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
//-------------------------------------
//	イメージクリップ外部データ
//=====================================
typedef struct {
	// 編集するポケモンデータ
	POKEMON_PARAM* p_poke_para;

	// イメージクリップセーブ領域
	IMC_TELEVISION_SAVEDATA* p_imc_one_data;

	// イメージクリップアイテムセーブ領域
	const IMC_ITEM_SAVEDATA* cp_imc_item;

	// コンフィグ
	const CONFIG* cp_config;

	// レコードデータ
	RECORD* p_record;	

	// トレーナーデータ
	const MYSTATUS* cp_tr;

	// セーブ有無格納先
	BOOL* p_save_flg;

	// 最初の説明テキストを出すかチェック
	BOOL reference;
} IMC_PROC_WORK;


//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロック生成
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcInit( PROC* proc, int* seq );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロックメイン
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcMain( PROC* proc, int* seq );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロック終了
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcEnd( PROC* proc, int* seq );




//-----------------------------------------------------------------------------
/**
 *					コンテスト用
 * [JAMP_CONTEST]
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					定数宣言
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	カウンタ定数
//=====================================
#define IMC_COUNT_MAX	(60)

//-----------------------------------------------------------------------------
/**
 *					構造体宣言
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	イメージクリップ外部データ
//=====================================
typedef struct {
	// 編集するポケモンデータ
	POKEMON_PARAM* p_poke_para;

	// イメージクリップセーブ領域
	IMC_CONTEST_SAVEDATA* p_imc_one_data;

	int theme;			// テーマ
	int rank;			// ランク
	int mode;			// モード
	int type;			// タイプ

	// イメージクリップアイテムセーブ領域
	const IMC_ITEM_SAVEDATA* cp_imc_item;

	// 通信用データ
	CON_IMC_LOCAL* p_comm_data;	

	// コンフィグ
	const CONFIG* cp_config;	

	// トレーナーデータ
	const MYSTATUS* cp_tr;

} IMC_CONTEST_PROC_WORK;


//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロック生成
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcContestInit( PROC* proc, int* seq );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロックメイン
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcContestMain( PROC* proc, int* seq );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	イメージクリッププロック終了
 *
 *	@param	proc		プロックデータ	
 *	@param	seq			シーケンス
 *
 *	@return	処理状況
 *
 *	typedef enum {
 *		PROC_RES_CONTINUE = 0,		///<動作継続中
 *		PROC_RES_FINISH,			///<動作終了
 *	}PROC_RESULT;
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT IMC_ProcContestEnd( PROC* proc, int* seq );



#undef	GLOBAL
#endif		// __IMC_SYS_H__

