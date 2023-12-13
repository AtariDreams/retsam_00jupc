//============================================================================================
/**
 * @file	field_place_name.h
 * @brief	地名取得
 * @author	Saito
 * @date	05.12.01
 */
//============================================================================================
#ifndef	__FIELD_MATRIX_H__
#define	__FIELD_MATRIX_H__

//実体はplace_name_core.c
extern void PNC_GetPlaceNameFromZoneID(	const int inZoneID,
										const u32 inHeapID,
										STRBUF *outName);

#endif	//__FIELD_MATRIX_H__
