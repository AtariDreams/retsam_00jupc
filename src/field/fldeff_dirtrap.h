//==============================================================================
/**
 * @file	fldeff_dirtrap.h
 * @brief	一枚ポリゴン 方向矢印罠
 * @author	k.ohno
 * @data	05.11.30
 */
//==============================================================================
#ifndef FLDEFF_DIRTRAP_H_FILE
#define FLDEFF_DIRTRAP_H_FILE

#include "fieldobj.h"
#include "field_effect.h"

//==============================================================================
//	typedef
//==============================================================================

//==============================================================================
//	外部参照
//==============================================================================
extern void * FE_DirTrap_Init( FE_SYS *fes );
extern void FE_DirTrap_Delete( void *work );

extern EOA_PTR FE_DirTrap_Add( FIELDSYS_WORK *fsys, int x, int z, int dir );

#endif //FLDEFF_DIRTRAP_H_FILE
