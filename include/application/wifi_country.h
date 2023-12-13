//============================================================================================
/**
 * @file	wifi_country.h
 * @brief	WI-FI地球画面で使用する文字列等のデータを外部公開するための仕組み
 * @author	taya
 * @date	2006.04.22
 */
//============================================================================================
#ifndef __WIFI_COUNTRY_H__
#define __WIFI_COUNTRY_H__


extern u32 WIFI_COUNTRY_GetDataLen( void );
extern u32 WIFI_COUNTRY_CountryCodeToDataIndex( u32 countryCode );
extern u32 WIFI_COUNTRY_CountryCodeToPlaceMsgDataID( u32 countryCode );
extern u32 WIFI_COUNTRY_DataIndexToPlaceMsgDataID( u32 dataIndex );
extern u32 WIFI_COUNTRY_DataIndexToCountryCode( u32 dataIndex );
extern u32 WIFI_COUNTRY_DataIndexToPlaceDataID( u32 dataIndex );
extern u32 WIFI_COUNTRY_CountryCodeToPlaceIndexMax( u32 countryCode );
// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2007/01/26
// 地名をアルファベット順にソートして表示するように変更
extern const u8 *WIFI_COUNTRY_DataIndexToPlaceSortTable( u32 dataIndex );
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2007/02/21
// 地名リストとソートテーブルの実際の要素数が食い違う場合に対応
extern u32 WIFI_COUNTRY_DataIndexToPlaceSortTableSize( u32 dataIndex );
// ----------------------------------------------------------------------------



#endif
