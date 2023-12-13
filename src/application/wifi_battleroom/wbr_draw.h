//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_draw.h
 *	@brief		wifi	バトルルーム
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_DRAW_H__
#define __WBR_DRAW_H__

#include "system/procsys.h"

#undef GLOBAL
#ifdef	__WBR_DRAW_H_GLOBAL
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
///	プロセス必要データ
//=====================================
typedef struct {
	u32 dummy;
} WBR_PROC_PARAM;

//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------
GLOBAL PROC_RESULT WbrProc_Init( PROC *proc,int *seq);
GLOBAL PROC_RESULT WbrProc_Main( PROC *proc,int *seq);
GLOBAL PROC_RESULT WbrProc_End( PROC *proc,int *seq);

#undef	GLOBAL
#endif		// __WBR_DRAW_H__

