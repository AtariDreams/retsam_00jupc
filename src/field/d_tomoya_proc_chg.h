//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_tomoya_proc_chg.h
 *	@brief		プロック変更するデバックデータ
 *	@author		tomoya takahashi
 *	@data		2006.03.27
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __D_TOMOYA_PROC_CHG_H__
#define __D_TOMOYA_PROC_CHG_H__


#include "field_trade_cmd.h"

#undef GLOBAL
#ifdef	__D_TOMOYA_PROC_CHG_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

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
//	
//	スプライトを表示させるために必要な物
//
//	その後プロックチェンジに必要なものになりました
//	
//	
//=====================================
typedef struct _DT_SPRITE_CHECK{
	int seq;
	int sub;
	int tmp_seq;
	void* p_pearent;

	FIELDSYS_WORK* p_fsys;

	void* p_work;

	void (*pStartFunc)( struct _DT_SPRITE_CHECK* p_data );
	void (*pEndFunc)( struct _DT_SPRITE_CHECK* p_data );
} DT_SPRITE_CHECK;

//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------
// プロセス変更処理
GLOBAL void DTomoya_ProcChenge( TCB_PTR tcb, void* work );


// 交換でも
typedef struct{
	FLD_TRADE_WORK* p_work;
	u32 seq;
} D_TOMOYA_EVENT_WORK;
GLOBAL BOOL EventScr_D_FldTrade(GMEVENT_CONTROL * event);

#undef	GLOBAL
#endif		// __D_TOMOYA_PROC_CHG_H__

