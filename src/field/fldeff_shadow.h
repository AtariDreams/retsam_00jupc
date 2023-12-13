//==============================================================================
/**
 * @file	fldeff_shadow.h
 * @brief	フィールドOBJ影
 * @author	kagaya
 * @data	05.07.13
 */
//==============================================================================
#ifndef FLDEFF_SHADOW_H_FILE
#define FLDEFF_SHADOW_H_FILE

#include "field_effect.h"

//==============================================================================
//	define
//==============================================================================
//--------------------------------------------------------------
///	影種類
//--------------------------------------------------------------
enum
{
	SHADOW_BLACK = 0,										///<黒色の影
	SHADOW_RED,												///<赤色の影
	SHADOW_BLUE,											///<青色の影
	SHADOW_MAX,												///<最大
};

//==============================================================================
//	外部参照
//==============================================================================
extern void * FE_Shadow_Init( FE_SYS *fes );
extern void FE_Shadow_Delete( void *work );

extern void FE_FldOBJShadow_Add( FIELD_OBJ_PTR fldobj );
extern EOA_PTR FE_FldOBJShadowSet( FIELD_OBJ_PTR fldobj, int type );

#endif //FLDEFF_SHADOW_H_FILE
