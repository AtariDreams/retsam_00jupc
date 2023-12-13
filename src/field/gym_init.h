//=============================================================================
/**
 * @file	gym_init.h
 * @bfief	ジム仕掛け初期化とか
 * @author	Nozomu Saito
 *
 */
//=============================================================================
#ifndef __GYM_INIT_H__
#define __GYM_INIT_H__

#include "field_common.h"

//---------------------------------------------------------------------------
/**
 * @brief	水ジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
void GYM_InitWaterGym(FIELDSYS_WORK *fsys);

//---------------------------------------------------------------------------
/**
 * @brief	ゴーストジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
void GYM_InitGhostGym(FIELDSYS_WORK *fsys);

//---------------------------------------------------------------------------
/**
 * @brief	鋼ジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitSteelGym(FIELDSYS_WORK *fsys);

#if 0	//DP
//---------------------------------------------------------------------------
/**
 * @brief	格闘ジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitCombatGym(FIELDSYS_WORK *fsys);
#endif

//---------------------------------------------------------------------------
/**
 * @brief	電気ジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitElecGym(FIELDSYS_WORK *fsys, const u8 inRoomNo);

//---------------------------------------------------------------------------
/**
 * @brief	PL 草ジム:初期化 kaga
 * 
 * @param	inBit				対象ビット
 * @param	inVal				値
 * @param	outData			データ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitPLGrassGym( FIELDSYS_WORK *fsys );

//---------------------------------------------------------------------------
/**
 * @brief	PL 格闘ジム:初期化 kaga
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitPLFightGym(FIELDSYS_WORK *fsys);

//---------------------------------------------------------------------------
/**
 * @brief	PLゴーストジム:初期化
 * 
 * @param	fsys	フィールドシステムポインタ
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
extern void GYM_InitPLGhostGym(FIELDSYS_WORK *fsys);

//--------------------------------------------------------------
/**
 * 別荘　初期化
 * @param	fsys	FIELDSYS_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
extern void FldVilla_Init( FIELDSYS_WORK *fsys );

//--------------------------------------------------------------
/**
 * やぶれたせかい　初期化
 * @param	fsys	FIELDSYS_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
extern void FldTornWorld_Init( FIELDSYS_WORK *fsys );

//--------------------------------------------------------------
/**
 * @brief   ギミックコードが指定IDのコードと同一かチェック
 *
 * @param   gimmick_id		ギミックコード
 *
 * @retval  TRUE:一致。　FALSE:不一致
 *
 * gym.cを専用のオーバーレイ領域(fieldmap_sub_gym)に移した為、
 * gym.cの関数を呼ぶ前にこれでチェックする
 */
//--------------------------------------------------------------
extern BOOL GYM_GimmickCodeCheck(FIELDSYS_WORK *fsys, int gimmick_id);

#endif //__GYM_INIT_H__

