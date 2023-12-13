//==============================================================================================
/**
 * @file	backup.h
 * @brief	バックアップ警告画面共通ヘッダ
 * @date	2006.07.07
 * @author	watanabe / tamada
 */
//==============================================================================================
#ifndef	__BACKUP_H__
#define	__BACKUP_H__

#include "system/procsys.h"


//---------------------------------------------------------------------------
/**
 * @brief	セーブ失敗エラーの指定
 */
//---------------------------------------------------------------------------
enum {
	SAVEERROR_MSG_DISABLE_WRITE = 0,		///<書き込み失敗の時
	SAVEERROR_MSG_DISABLE_READ,				///<書き込み前のリードチェック失敗のとき
};

//---------------------------------------------------------------------------
/**
 * @brief	セーブ失敗画面呼び出し
 * @param	heapID			使用するヒープのID
 * @param	error_msg_id	表示するエラーの種類指定（SAVEERROR_MSG_〜を指定する）
 */
//---------------------------------------------------------------------------
extern void SaveErrorWarningCall( int heapID, int error_msg_id );

//---------------------------------------------------------------------------
/**
 * @brief	ロード失敗画面呼び出し
 * @param	heapID			使用するヒープのID
 */
//---------------------------------------------------------------------------
extern void BackupErrorWarningCall( int heapID );

#if AFTERMASTER_070123_GBACARTRIDGE_BUG_FIX
extern void GbaCartridgeErrorWarningCall( int heapID );
#endif

#endif	//__BACKUP_H__
