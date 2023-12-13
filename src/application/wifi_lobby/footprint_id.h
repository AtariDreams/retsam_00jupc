//==============================================================================
/**
 * @file	footprint_id.h
 * @brief	キャラID、セルID等の定義場所
 * @author	matsuda
 * @date	2008.01.26(土)
 */
//==============================================================================
#ifndef __FOOTPRINT_ID_H__
#define __FOOTPRINT_ID_H__


//==============================================================================
//	キャラID
//==============================================================================
enum{
	CHARID_FOOTPRINT_START = 10000,
	
	CHARID_INK_FOOT_0,				//インクの上に置く足跡：0番目
	CHARID_INK_FOOT_1,				//インクの上に置く足跡：1番目
	CHARID_INK_FOOT_2,				//インクの上に置く足跡：2番目
	CHARID_INK_FOOT_3,				//インクの上に置く足跡：3番目
	CHARID_INK_FOOT_4,				//インクの上に置く足跡：4番目
	CHARID_INK_FOOT_5,				//インクの上に置く足跡：5番目
	CHARID_INK,						//インク
	CHARID_INK_FOUNDATION = CHARID_INK,			//インクの下に敷く下地
	CHARID_TOUCH_EFF,				//インクをタッチした時に出すエフェクト
	
	//-- サブ画面 --//
	CHARID_SUB_NAME_FRAME,			///<名前を囲むフレーム
	CHARID_SUB_NAME_FOOT_0,			///<名前の横に置く足跡：0番目
	CHARID_SUB_NAME_FOOT_1,			///<名前の横に置く足跡：1番目
	CHARID_SUB_NAME_FOOT_2,			///<名前の横に置く足跡：2番目
	CHARID_SUB_NAME_FOOT_3,			///<名前の横に置く足跡：3番目
	CHARID_SUB_NAME_FOOT_4,			///<名前の横に置く足跡：4番目
	CHARID_SUB_NAME_FOOT_5,			///<名前の横に置く足跡：5番目
	CHARID_SUB_NAME_FOOT_6,			///<名前の横に置く足跡：6番目
	CHARID_SUB_NAME_FOOT_7,			///<名前の横に置く足跡：7番目
};

//==============================================================================
//	パレットID
//==============================================================================
enum{
	PLTTID_FOOTPRINT_START = 10000,

	PLTTID_OBJ_COMMON,	///<常駐OBJパレット
	PLTTID_OBJ_INK_FOOT,	///<インクパレットに置かれている自分の足跡
	PLTTID_OBJ_FONTOAM,		///<FONTOAM用パレット
	
	//-- サブ画面 --//
	PLTTID_SUB_OBJ_COMMON,	///<サブ画面の常駐OBJパレット
};

//--------------------------------------------------------------
//	常駐OBJパレットオフセット
//--------------------------------------------------------------
enum{
	PALOFS_INK = 1,					///<インク
	PALOFS_INK_FOUNDATION = 2,		///<インクの下に敷く下地
	PALOFS_TOUCH_EFF = 0,			///<インクをタッチした時に出すエフェクト
	
	FOOTPRINT_COMMON_PAL_NUM = 3,	///<常駐OBJパレットの本数
};

//--------------------------------------------------------------
//	サブ画面の常駐OBJパレットオフセット
//--------------------------------------------------------------
enum{
	PALOFS_SUB_NAME_FRAME = 0,		///<名前を囲むフレーム
	PALOFS_SUB_NAME_FOOT = 0,		///<名前の横に配置する足跡

	FOOTPRINT_SUB_COMMON_PAL_NUM = 1,	///<サブ画面の常駐OBJパレットの本数
};

//--------------------------------------------------------------
//	パレットのカラー位置指定
//--------------------------------------------------------------
enum{
	COLOR_NO_INK_START = 1,			///<インクのカラー開始位置
	
	//-- サブ画面 --//
	COLOR_NO_SUB_NAME_FOOT = 7,		///<名前の横に配置する足跡
};

//==============================================================================
//	セルID
//==============================================================================
enum{
	CELLID_FOOTPRINT_START = 10000,

	CELLID_INK_FOOT,				//インクの上に置く足跡
	CELLID_INK,						//インク
	CELLID_INK_FOUNDATION = CELLID_INK,			//インクの下に敷く下地
	CELLID_TOUCH_EFF,				//インクをタッチした時に出すエフェクト

	//-- サブ画面 --//
	//-- サブ画面 --//
	CELLID_SUB_NAME_FRAME,			///<名前を囲むフレーム
	CELLID_SUB_NAME_FOOT,			///<名前の横に置く足跡
};

//==============================================================================
//	セルアニメID
//==============================================================================
enum{
	CELLANMID_FOOTPRINT_START = 10000,

	CELLANMID_INK_FOOT,					//インクの上に置く足跡
	CELLANMID_INK,						//インク
	CELLANMID_INK_FOUNDATION = CELLANMID_INK,			//インクの下に敷く下地
	CELLANMID_TOUCH_EFF,				//インクをタッチした時に出すエフェクト

	//-- サブ画面 --//
	//-- サブ画面 --//
	CELLANMID_SUB_NAME_FRAME,			///<名前を囲むフレーム
	CELLANMID_SUB_NAME_FOOT,			///<名前の横に置く足跡
};

//==============================================================================
//	マルチセルID
//==============================================================================
enum{
	MCELLID_FOOTPRINT_START = 10000,
};

//==============================================================================
//	マルチセルアニメID
//==============================================================================
enum{
	MCELLANMID_FOOTPRINT_START = 10000,
};



#endif	//__FOOTPRINT_ID_H__
