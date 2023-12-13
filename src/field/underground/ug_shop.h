//=============================================================================
/**
 * @file	ug_shop.h
 * @brief	地下のお店を管理するクラス
 * @author	Katsumi Ohno
 * @date    2006.04.03
 */
//=============================================================================

#ifndef __UG_SHOP_H__
#define __UG_SHOP_H__

extern void UgShopMiniInfoDispWindow(_START_WORK* pShop,int msg);
extern void UgShopMiniInfoDispWindowDel(_START_WORK* pShop);
extern void UgShopMiniInfoDispWindowDelOn(_START_WORK* pShop);

extern void UgShopManInit(FIELDSYS_WORK* pFSys);

#endif __UG_SHOP_H__

