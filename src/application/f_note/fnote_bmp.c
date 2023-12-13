//============================================================================================
/**
 * @file	fnote_bmp.c
 * @brief	冒険ノート　BMP処理
 * @author	Hiroyuki Nakamura
 * @date	2006.03.06
 */
//============================================================================================
#include "common.h"
#include "gflib/strbuf_family.h"
#include "savedata/fnote_mem.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/wordset.h"
#include "poketool/tr_tool.h"
#include "battle/trtype_def.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_fantasy_note.h"
#include "msgdata/msg_place_name.h"
#include "field/field_place_name.h"
#include "../../src/field/zonedata.h"

#define	FNOTE_BMP_H_GLOBAL
#include "fnote_main.h"
#include "fnote_bmp.h"


//============================================================================================
//	定数定義
//============================================================================================
// BMPウィンドウデータ
// 日付１
#define	WIN_TIME1_PX	( 2 )		// 表示X座標
#define	WIN_TIME1_PY	( 2 )		// 表示Y座標
#define	WIN_TIME1_SX	( 24 )		// Xサイズ
#define	WIN_TIME1_SY	( 2 )		// Yサイズ
#define	WIN_TIME1_PAL	( 15 )		// 使用パレット
#define	WIN_TIME1_CGX	( 1 )		// キャラ開始位置
// 内容１
#define	WIN_STR1_PX		( 4 )
#define	WIN_STR1_PY		( 4 )
#define	WIN_STR1_SX		( 27 )
#define	WIN_STR1_SY		( 18 )
#define	WIN_STR1_PAL	( 15 )
#define	WIN_STR1_CGX	( WIN_TIME1_CGX + WIN_TIME1_SX * WIN_TIME1_SY )
// 日付２（スワップ用）
#define	WIN_TIME2_PX	( 2 )
#define	WIN_TIME2_PY	( 2 )
#define	WIN_TIME2_SX	( 24 )
#define	WIN_TIME2_SY	( 2 )
#define	WIN_TIME2_PAL	( 15 )
#define	WIN_TIME2_CGX	( 1 )
// 内容２（スワップ用）
#define	WIN_STR2_PX		( 4 )
#define	WIN_STR2_PY		( 4 )
#define	WIN_STR2_SX		( 27 )
#define	WIN_STR2_SY		( 18 )
#define	WIN_STR2_PAL	( 15 )
#define	WIN_STR2_CGX	( WIN_TIME2_CGX + WIN_TIME2_SX * WIN_TIME2_SY )

#define	COL_N_BLACK		( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )		// フォントカラー：黒

#define	WEEK_SPC_X	( 12 )		// 曜日スペース

#define	ACTION_PY	( 16 )					// 項目「行動」表示Y座標
#define	POKEMON_PY	( ACTION_PY * 4 + 16 )	// 項目「ポケモン」表示Y座標
#define	TRAINER_PY	( POKEMON_PY + 16 )		// 項目「トレーナー」表示Y座標
#define	SIO_PY		( TRAINER_PY + 16 )		// 項目「通信」表示Y座標


//============================================================================================
//	プロトタイプ宣言
//============================================================================================
static u8 FNOTE_HederPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * time, GF_BGL_BMPWIN * info );
static void FNOTE_ActionPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info );
static void FNOTE_PokemonPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info );
static void FNOTE_TrainerPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info );
static void FNOTE_SioPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info );

static void FNOTE_ActionHouseDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionLaboDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionPokeCenterDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionShopBuyDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionShopBuy2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionShopSaleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionShopSale2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionShopTradeDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionGymBeforeDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionGymAfterDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionSitennouDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionChampionDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionCityDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionDungeonDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionBuildDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionGameCornerDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionSafariDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionItemGetDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionIwakudakiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionIaigiriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionSorawotobuDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionKiribaraiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionKairikiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionNaminoriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionRockckimbDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionTakinoboriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionFlashDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionTeleportDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionAnawohoruDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionAmiaikaoriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionTamagoumiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionMilknomiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionUGFossilDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionUGBaseDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos );
static void FNOTE_ActionFrontierDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos, int id );
static void FNOTE_PokeGetDataPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_POKE * mem );
static void FNOTE_PokeDownDataPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_POKE * mem );
static void FNOTE_SioBtlSingleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioBtlDoubleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioBtlMultiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioBtlMix1DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioBtlMix2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionGreetDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionChgDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionPictureDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionMultiChgDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionTransceiverDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioUnionBtlDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioRecordDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioContestDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioPorutoDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioGtcDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioBtlTowerDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioGuruguruDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioGDSDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos, int id );
static void FNOTE_SioLobbyChatDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioLobbyToyDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioLobbyMiniGameDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );
static void FNOTE_SioLobbyAppliDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos, int id );
static void FNOTE_SioWifiClubDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos );


//============================================================================================
//	グローバル変数
//============================================================================================
// BMPウィンドウデータ
static const BMPWIN_DAT BmpData[] =
{
	{	// 日付１
		GF_BGL_FRAME0_M, WIN_TIME1_PX, WIN_TIME1_PY,
		WIN_TIME1_SX, WIN_TIME1_SY, WIN_TIME1_PAL, WIN_TIME1_CGX
	},
	{	// 情報１
		GF_BGL_FRAME0_M, WIN_STR1_PX, WIN_STR1_PY,
		WIN_STR1_SX, WIN_STR1_SY, WIN_STR1_PAL, WIN_STR1_CGX
	},
	{	// 日付２
		GF_BGL_FRAME1_M, WIN_TIME2_PX, WIN_TIME2_PY,
		WIN_TIME2_SX, WIN_TIME2_SY, WIN_TIME2_PAL, WIN_TIME2_CGX
	},
	{	// 情報２
		GF_BGL_FRAME1_M, WIN_STR2_PX, WIN_STR2_PY,
		WIN_STR2_SX, WIN_STR2_SY, WIN_STR2_PAL, WIN_STR2_CGX
	}
};


//--------------------------------------------------------------------------------------------
/**
 * BMP追加
 *
 * @param	wk		ワーク
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FantasyNote_BmpAdd( FNOTE_WORK * wk )
{
	u32	i;

	for( i=0; i<WIN_MAX; i++ ){
		GF_BGL_BmpWinAddEx( wk->bgl, &wk->win[i], &BmpData[i] );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * BMP削除
 *
 * @param	wk		ワーク
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FantasyNote_BmpFree( FNOTE_WORK * wk )
{
	u32	i;

	for( i=0; i<WIN_MAX; i++ ){
		GF_BGL_BmpWinDel( &wk->win[i] );
	}
}

//--------------------------------------------------------------------------------------------
/**
 * 文字列表示
 *
 * @param	wk		ワーク
 * @param	flg		表示するBMPのフラグ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FantasyNote_BmpPut( FNOTE_WORK * wk, u32 flg )
{
	GF_BGL_BMPWIN * time;
	GF_BGL_BMPWIN * info;

	if( flg == 0 ){
		time = &wk->win[WIN_TIME1];
		info = &wk->win[WIN_STR1];
	}else{
		time = &wk->win[WIN_TIME2];
		info = &wk->win[WIN_STR2];
	}

	GF_BGL_BmpWinDataFill( time, 0 );
	GF_BGL_BmpWinDataFill( info, 0 );

	if( FNOTE_HederPut( wk, time, info ) == TRUE ){
		FNOTE_ActionPut( wk, info );
		FNOTE_PokemonPut( wk, info );
		FNOTE_TrainerPut( wk, info );
		FNOTE_SioPut( wk, info );
	}

	GF_BGL_BmpWinOnVReq( time );
	GF_BGL_BmpWinOnVReq( info );
}

//--------------------------------------------------------------------------------------------
/**
 * ヘッダーデータ表示
 *
 * @param	wk		ワーク
 * @param	time	日付を表示するBMPウィンドウ
 * @param	info	開始位置を表示するBMPウィンドウ
 *
 * @retval	"TRUE = データあり"
 * @retval	"FALSE = データなし"
 */
//--------------------------------------------------------------------------------------------
static u8 FNOTE_HederPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * time, GF_BGL_BMPWIN * info )
{
	FNOTE_WK_START	mem;
	STRBUF * str;
	u32	siz;

	FNOTE_DataGet( wk->dat, &mem, FNOTE_TYPE_START, wk->page );

	if( mem.year == 0 && mem.month == 0 && mem.day == 0 ){
		return FALSE;
	}

	// 日付
	str = MSGMAN_AllocString( wk->mman, msg_fnote_001 );
	// ----------------------------------------------------------------------------
	// localize_spec_mark(LANG_ALL) imatake 2007/01/26
	// 月の表示を単語表記に変更
	WORDSET_RegisterMonthName( wk->wset, 0, mem.month );
	// ----------------------------------------------------------------------------
	WORDSET_RegisterNumber(
		wk->wset, 1, mem.day, 2, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	STRBUF_Delete( str );
	GF_STR_PrintColor( time, FONT_SYSTEM, wk->msg_buf, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
	// 曜日
	siz = FontProc_GetPrintStrWidth( FONT_SYSTEM, wk->msg_buf, 0 );
	str = MSGMAN_AllocString( wk->mman, msg_fnote_010+mem.week );
	GF_STR_PrintColor(
		time, FONT_SYSTEM, str, siz+WEEK_SPC_X, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );

	// スタート！
	str = MSGMAN_AllocString( wk->mman, msg_fnote_000 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem.start) );

	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor( info, FONT_SYSTEM, wk->msg_buf, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );

	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * 行動データ表示
 *
 * @param	wk		ワーク
 * @param	info	表示するBMPウィンドウ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info )
{
	FNOTE_WK_ACTION	mem[FNOTE_ACTION_MAX];
	u32	i;

	FNOTE_DataGet( wk->dat, &mem[0], FNOTE_TYPE_ACTION, wk->page );

	for( i=0; i<FNOTE_ACTION_MAX; i++ ){
		switch( mem[i].id ){
		case 0:						// データなし
			return;

		case FNOTE_ID_MYHOUSE:		// 行動：自宅
			FNOTE_ActionHouseDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_LABO:			// 行動：研究所
			FNOTE_ActionLaboDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_POKECENTER:	// 行動：ポケモンセンター
			FNOTE_ActionPokeCenterDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FDSHOP_BUY:	// 行動：ショップ・買う
			FNOTE_ActionShopBuyDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FDSHOP_BUY2:	// 行動：ショップ・買う（複数）
			FNOTE_ActionShopBuy2DataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FDSHOP_SALE:	// 行動：ショップ・売る
			FNOTE_ActionShopSaleDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FDSHOP_SALE2:	// 行動：ショップ・売る（複数）
			FNOTE_ActionShopSale2DataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FDSHOP_TRADE:	// 行動：ショップ・売買
			FNOTE_ActionShopTradeDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_GYMBEFORE:	// 行動：ジム（リーダーを倒す前）
			FNOTE_ActionGymBeforeDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_GYMAFTER:		// 行動：ジム（リーダーを倒した後）
			FNOTE_ActionGymAfterDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SITENNOU:		// 行動：四天王
			FNOTE_ActionSitennouDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_CHAMPION:		// 行動：チャンピオン
			FNOTE_ActionChampionDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_CITY:			// 行動：街
			FNOTE_ActionCityDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_DUNGEON:		// 行動：ダンジョン
			FNOTE_ActionDungeonDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_BUILD:		// 行動：建物
			FNOTE_ActionBuildDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_GAMECORNER:		// 行動：ゲームコーナー
			FNOTE_ActionGameCornerDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SAFARIZONE:		// 行動：サファリゾーン
			FNOTE_ActionSafariDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_ITEMGET:		// 行動：アイテム取得
			FNOTE_ActionItemGetDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_IWAKUDAKI:	// 行動：技・いわくだき
			FNOTE_ActionIwakudakiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_IAIGIRI:		// 行動：技・いあいぎり
			FNOTE_ActionIaigiriDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SORAWOTOBU:	// 行動：技・そらをとぶ
			FNOTE_ActionSorawotobuDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_KIRIBARAI:	// 行動：技・きりばらい
			FNOTE_ActionKiribaraiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_KAIRIKI:		// 行動：技・かいりき
			FNOTE_ActionKairikiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_NAMINORI:		// 行動：技・なみのり
			FNOTE_ActionNaminoriDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_ROCKCLIMB:	// 行動：技・ロッククライム
			FNOTE_ActionRockckimbDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_TAKINOBORI:	// 行動：技・たきのぼり
			FNOTE_ActionTakinoboriDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_FLASH:		// 行動：技・フラッシュ
			FNOTE_ActionFlashDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_TELEPORT:		// 行動：技・テレポート
			FNOTE_ActionTeleportDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_ANAWOHORU:	// 行動：技・あなをほる
			FNOTE_ActionAnawohoruDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_AMAIKAORI:	// 行動：技・あまいかおり
			FNOTE_ActionAmiaikaoriDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_TAMAGOUMI:	// 行動：技・タマゴうみ
			FNOTE_ActionTamagoumiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_MIRUKUNOMI:	// 行動：技・ミルクのみ
			FNOTE_ActionMilknomiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_UG_FOSSIL:	// 行動：地下・化石掘り
			FNOTE_ActionUGFossilDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_UG_BASE:		// 行動：地下・秘密基地
			FNOTE_ActionUGBaseDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_PL_TOWER:		// 行動：フロンティア施設・タワー
		case FNOTE_ID_PL_FACTORY:	// 行動：フロンティア施設・ファクトリー
		case FNOTE_ID_PL_CASTLE:	// 行動：フロンティア施設・キャッスル
		case FNOTE_ID_PL_STAGE:		// 行動：フロンティア施設・ステージ
		case FNOTE_ID_PL_ROULETTE:	// 行動：フロンティア施設・ルーレット
			FNOTE_ActionFrontierDataPut( wk, info, &mem[i], i, mem[i].id );
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
/**
 * ポケモンデータ表示
 *
 * @param	wk		ワーク
 * @param	info	表示するBMPウィンドウ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_PokemonPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info )
{
	FNOTE_WK_POKE	mem;

	FNOTE_DataGet( wk->dat, &mem, FNOTE_TYPE_POKEMON, wk->page );

	switch( mem.id ){
	case 0:						// データなし
		return;

	case FNOTE_ID_POKEGET:		// ポケモン：捕獲
		FNOTE_PokeGetDataPut( wk, info, &mem );
		break;

	case FNOTE_ID_POKEDOWN:		// ポケモン：倒した
		FNOTE_PokeDownDataPut( wk, info, &mem );
		break;
	}
}

//--------------------------------------------------------------------------------------------
/**
 * トレーナーデータ表示
 *
 * @param	wk		ワーク
 * @param	info	表示するBMPウィンドウ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2007/01/10
// 対戦ログ出し分けの境界値を、英語版の文字数の出現分布を考慮して変更
enum {
	LETNUM_SHORTEST = 14,
	LETNUM_SHORTER  = 16,
	LETNUM_LONGER   = 19,
	LETNUM_LONGEST  = 26,
};
// ----------------------------------------------------------------------------

static void FNOTE_TrainerPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info )
{
	FNOTE_WK_TRAINER	mem;
	STRBUF * str;
	u32	i;

	FNOTE_DataGet( wk->dat, &mem, FNOTE_TYPE_TRAINER, wk->page );

	if( mem.id == 0 ){ return; }

	{
		STRBUF * str;
		
		// 地名
		str = MSGDAT_GetStrDirectAlloc(
				ARC_MSG, NARC_msg_place_name_dat,
				ZoneData_GetPlaceNameID(mem.map), HEAPID_FANTASYNOTE );
		i = STRBUF_GetLen( str );
		STRBUF_Delete( str );

		// トレーナー名
		// ライバル
		if( TT_TrainerDataParaGet( mem.trid, ID_TD_tr_type ) == TRTYPE_RIVAL ){
			str = MSGMAN_AllocString( wk->mman, msg_fnote_304 );
			WORDSET_RegisterRivalName( wk->wset, 1, wk->sv );
			WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
			i += STRBUF_GetLen( wk->msg_buf );
			STRBUF_Delete( str );
		// ぞれ以外
		}else{
			str = MSGDAT_GetStrDirectAlloc(
					ARC_MSG, NARC_msg_trname_dat, mem.trid, HEAPID_FANTASYNOTE );
			i += STRBUF_GetLen( str );
			STRBUF_Delete( str );
			WORDSET_RegisterTrainerName( wk->wset, 1, mem.trid );
		}
	}

	// ----------------------------------------------------------------------------
	// localize_spec_mark(LANG_ALL) imatake 2007/01/10
	// 対戦ログ出し分けの境界値を、英語版の文字数の出現分布を考慮して変更
	if( i <= LETNUM_SHORTEST ){
		str = MSGMAN_AllocString( wk->mman, msg_fnote_300 );
	}else if( i <= LETNUM_SHORTER ){
		str = MSGMAN_AllocString( wk->mman, msg_fnote_301 );
	}else if( i <= LETNUM_LONGER ){
		str = MSGMAN_AllocString( wk->mman, msg_fnote_302 );
	}else{		// LETNUM_LONGEST
		str = MSGMAN_AllocString( wk->mman, msg_fnote_303 );
	}
	// ----------------------------------------------------------------------------

	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem.map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		info, FONT_SYSTEM, wk->msg_buf, 0, TRAINER_PY, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信データ表示
 *
 * @param	wk		ワーク
 * @param	info	表示するBMPウィンドウ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * info )
{
	FNOTE_WK_SIO	mem[FNOTE_SIO_MAX];
	u32	i;

	FNOTE_DataGet( wk->dat, &mem[0], FNOTE_TYPE_SIO, wk->page );

	for( i=0; i<FNOTE_SIO_MAX; i++ ){
		switch( mem[i].id ){
		case 0:
			return;

		case FNOTE_ID_SB_SINGLE:		// 通信：通信対戦・シングル
			FNOTE_SioBtlSingleDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SB_DOUBLE:		// 通信：通信対戦・ダブル
			FNOTE_SioBtlDoubleDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SB_MULTI:			// 通信：通信対戦・マルチ
			FNOTE_SioBtlMultiDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SB_MIX1:			// 通信：通信対戦・ミックス1vs1
			FNOTE_SioBtlMix1DataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_SB_MIX2:			// 通信：通信対戦・ミックス2vs2
			FNOTE_SioBtlMix2DataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_GREETING:		// 通信：ユニオンルーム・挨拶
			FNOTE_SioUnionGreetDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_POKECHG:		// 通信：ユニオンルーム・交換
			FNOTE_SioUnionChgDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_PICTURE:		// 通信：ユニオンルーム・お絵かき
			FNOTE_SioUnionPictureDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_MLTCHG:			// 通信：ユニオンルーム・複数人交換
			FNOTE_SioUnionMultiChgDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_TRANSCEIVER:	// 通信：ユニオンルーム・トランシーバー
			FNOTE_SioUnionTransceiverDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_U_BATTLE:			// 通信：ユニオンルーム・対戦
			FNOTE_SioUnionBtlDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_RECORD:			// 通信：レコードコーナー
			FNOTE_SioRecordDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_CONTEST:			// 通信：コンテスト
			FNOTE_SioContestDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_PORUTO:			// 通信：ポルト作成
			FNOTE_SioPorutoDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_GTC:				// 通信：GTC
			FNOTE_SioGtcDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_BTLTOWER:			// 通信：バトルタワー
			FNOTE_SioBtlTowerDataPut( wk, info, &mem[i], i );
			break;

		case FNOTE_ID_PL_GURUGURU:				// 通信：ぐるぐる交換
			FNOTE_SioGuruguruDataPut( wk, info, &mem[i], i );
			break;
		case FNOTE_ID_PL_GDS_VIDEO:				// 通信：GDSバトルビデオ
		case FNOTE_ID_PL_GDS_RANKING:			// 通信：GDSランキング
		case FNOTE_ID_PL_GDS_DRESS:				// 通信：GDSドレスアップ
		case FNOTE_ID_PL_GDS_BOX:				// 通信：GDSボックスショット
			FNOTE_SioGDSDataPut( wk, info, &mem[i], i, mem[i].id );
			break;
		case FNOTE_ID_PL_LOBBY_CHAT:			// 通信：WIFI広場おしゃべり
			FNOTE_SioLobbyChatDataPut( wk, info, &mem[i], i );
			break;
		case FNOTE_ID_PL_LOBBY_TOY_GET:				// 通信：WIFI広場タッチトイ
			FNOTE_SioLobbyToyDataPut( wk, info, &mem[i], i );
			break;
		case FNOTE_ID_PL_LOBBY_MINIGAME:		// 通信：WIFI広場ミニゲーム
			FNOTE_SioLobbyMiniGameDataPut( wk, info, &mem[i], i );
			break;
		case FNOTE_ID_PL_LOBBY_FOOT_BOARD:		// 通信：WIFI広場足跡ボード
		case FNOTE_ID_PL_LOBBY_WORLDTIMER:		// 通信：WIFI広場世界時計
		case FNOTE_ID_PL_LOBBY_NEWS:			// 通信：WIFI広場ニュース
		case FNOTE_ID_PL_LOBBY_PARADE:			// 通信：WIFI広場パレード
			FNOTE_SioLobbyAppliDataPut( wk, info, &mem[i], i, mem[i].id );
			break;
		case FNOTE_ID_PL_WIFICLUB:				// 通信：WIFIクラブ
			FNOTE_SioWifiClubDataPut( wk, info, &mem[i], i );
			break;
		}
	}
}


//--------------------------------------------------------------------------------------------
/**
 * 行動：自宅表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionHouseDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_100 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：研究所表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionLaboDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_101 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ポケモンセンター表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionPokeCenterDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_110 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ショップ・買う表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionShopBuyDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_120 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ショップ・買う（複数）表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionShopBuy2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_121 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ショップ・売る表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionShopSaleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_122 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ショップ・売る（複数）表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionShopSale2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_123 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ショップ・売買表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionShopTradeDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_124 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ジム（リーダーを倒す前）表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionGymBeforeDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_130 );
	WORDSET_RegisterGymName( wk->wset, 0, mem->map );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ジム（リーダーを倒した後）表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionGymAfterDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_131 );
	WORDSET_RegisterGymName( wk->wset, 0, mem->map );
	WORDSET_RegisterTrainerName( wk->wset, 1, mem->trainer );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：四天王表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionSitennouDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_132 );
	WORDSET_RegisterTrainerName( wk->wset, 0, mem->trainer );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：チャンピオン表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionChampionDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_133 );
	WORDSET_RegisterTrainerName( wk->wset, 0, mem->trainer );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：街表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionCityDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_140 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ダンジョン表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionDungeonDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str;
	u32	name;

	name = mem->map;
	// 「たにまのはつでんしょ」「キッサキしんでん」「タタラせいてつじょ」は特殊
	if( name == MAPNAME_D2HATUDEN || name == MAPNAME_D20KISSAKI || name == MAPNAME_D4TATARA ){
		str = MSGMAN_AllocString( wk->mman, msg_fnote_142 );
	}else{
		str = MSGMAN_AllocString( wk->mman, msg_fnote_141 );
	}
	WORDSET_RegisterPlaceName( wk->wset, 0, name );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：建物表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionBuildDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str;
	u32	name;

	name = mem->map;
	if( FNOTE_ExBuildPutTypeGet( name ) == 0 ){
		str = MSGMAN_AllocString( wk->mman, msg_fnote_142 );
	}else{
		str = MSGMAN_AllocString( wk->mman, msg_fnote_143 );
	}
	WORDSET_RegisterPlaceName( wk->wset, 0, name );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：ゲームコーナー表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionGameCornerDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_144 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：サファリゾーン表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionSafariDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_145 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：アイテム取得表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionItemGetDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_150 );
	WORDSET_RegisterItemName( wk->wset, 0, mem->item );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・いわくだき表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionIwakudakiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_165 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・いあいぎり表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionIaigiriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_160 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・そらをとぶ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionSorawotobuDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_161 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・きりばらい表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionKiribaraiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_168 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・かいりき表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionKairikiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_163 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・なみのり表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionNaminoriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_162 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・ロッククライム表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionRockckimbDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_167 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・たきのぼり表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionTakinoboriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_166 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・フラッシュ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionFlashDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_164 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・テレポート表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionTeleportDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_172 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・あなをほる表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionAnawohoruDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_170 );
	WORDSET_RegisterPlaceName( wk->wset, 0, ZoneData_GetPlaceNameID(mem->map) );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・あまいかおり表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionAmiaikaoriDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_171 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・タマゴうみ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionTamagoumiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_173 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：技・ミルクのみ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionMilknomiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_174 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：地下・化石掘り表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionUGFossilDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_146 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：地下・秘密基地表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionUGBaseDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_147 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 行動：フロンティア施設表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		行動データ
 * @param	pos		表示位置
 * @param	id		ID
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_ActionFrontierDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_ACTION * mem, u8 pos, int id )
{
	u32 msg_id;
	STRBUF * str;
	
	switch(id){
	case FNOTE_ID_PL_TOWER:
		msg_id = msg_fnote_490;
		break;
	case FNOTE_ID_PL_FACTORY:
		msg_id = msg_fnote_491;
		break;
	case FNOTE_ID_PL_CASTLE:
		msg_id = msg_fnote_492;
		break;
	case FNOTE_ID_PL_STAGE:
		msg_id = msg_fnote_493;
		break;
	case FNOTE_ID_PL_ROULETTE:
	default:
		msg_id = msg_fnote_494;
		break;
	}

	str = MSGMAN_AllocString( wk->mman, msg_id );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, ACTION_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}


//--------------------------------------------------------------------------------------------
/**
 * ポケモン名セット
 *
 * @param	wk			冒険ノートのワーク
 * @param	mons		ポケモン番号
 * @param	sex			性別
 * @param	buf_id		セットする単語ID
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_PokeMonsNameSet( FNOTE_WORK * wk, u16 mons, u8 sex, u8 buf_id )
{
	POKEMON_PARAM * pp = PokemonParam_AllocWork( HEAPID_FANTASYNOTE );

	PokeParaSetSexChr( pp, mons, 1, POW_RND, sex, 0, 0 );
	WORDSET_RegisterPokeMonsName( wk->wset, buf_id, PPPPointerGet( pp ) );
	sys_FreeMemoryEz( pp );
}

//--------------------------------------------------------------------------------------------
/**
 * ポケモン：捕獲表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		ポケモンデータ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_PokeGetDataPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_POKE * mem )
{
	STRBUF * str;

	switch( mem->type ){
	case 0:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_200 );
		break;
	case 1:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_201 );
		break;
	default:
		if( mem->sex == PARA_MALE ){
			str = MSGMAN_AllocString( wk->mman, msg_fnote_202 );
		}else if( mem->sex == PARA_FEMALE ){
			str = MSGMAN_AllocString( wk->mman, msg_fnote_203 );
		}else{
			str = MSGMAN_AllocString( wk->mman, msg_fnote_200 );
		}
	}

	FNOTE_PokeMonsNameSet( wk, mem->mons, mem->sex, 0 );
	WORDSET_RegisterTimeType( wk->wset, 1, mem->time );

	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, POKEMON_PY, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * ポケモン：倒した表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		ポケモンデータ
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_PokeDownDataPut( FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_POKE * mem )
{
	STRBUF * str;

	switch( mem->type ){
	case 0:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_210 );
		break;
	case 1:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_211 );
		break;
	default:
		if( mem->sex == PARA_MALE ){
			str = MSGMAN_AllocString( wk->mman, msg_fnote_212 );
		}else if( mem->sex == PARA_FEMALE ){
			str = MSGMAN_AllocString( wk->mman, msg_fnote_213 );
		}else{
			str = MSGMAN_AllocString( wk->mman, msg_fnote_210 );
		}
	}

	FNOTE_PokeMonsNameSet( wk, mem->mons, mem->sex, 0 );
	WORDSET_RegisterTimeType( wk->wset, 1, mem->time );

	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, POKEMON_PY, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}


//--------------------------------------------------------------------------------------------
/**
 * STRCODE型の単語をセット
 *
 * @param	wk		冒険ノートのワーク
 * @param	str		セットする単語
 * @param	sex		性別
 * @param	buf_id	セットする単語ID
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_StrWordSet( FNOTE_WORK * wk, STRCODE * str, u8 sex, u8 buf_id )
{
	STRBUF * buf = STRBUF_Create( 32, HEAPID_FANTASYNOTE );

	STRBUF_SetStringCode( buf, str );
	WORDSET_RegisterWord( wk->wset, buf_id, buf, sex, TRUE, PM_LANG );
	STRBUF_Delete( buf );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：通信対戦・シングル表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlSingleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_400 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_401 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_402 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：通信対戦・ダブル表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlDoubleDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_403 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_404 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_405 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：通信対戦・マルチ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlMultiDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_406 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_407 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_408 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	FNOTE_StrWordSet( wk, mem->name2, mem->sex2, 1 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：通信対戦・ミックス1vs1表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlMix1DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_409 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_410 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_411 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：通信対戦・ミックス2vs2表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlMix2DataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_412 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_413 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_414 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	FNOTE_StrWordSet( wk, mem->name2, mem->sex2, 1 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・挨拶表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionGreetDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_420 );
	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・交換表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionChgDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_421 );
	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	FNOTE_StrWordSet( wk, mem->nickname, mem->poke_sex, 1 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・お絵かき表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionPictureDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_422 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・複数人交換表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionMultiChgDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_423 );
	FNOTE_StrWordSet( wk, mem->nickname, mem->poke_sex, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・トランシーバー表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionTransceiverDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_424 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ユニオンルーム・対戦表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioUnionBtlDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str;
	
	switch( mem->result ){
	case FNOTE_RESULT_WIN:		// 勝ち
		str = MSGMAN_AllocString( wk->mman, msg_fnote_425 );
		break;
	case FNOTE_RESULT_LOSE:		// 負け
		str = MSGMAN_AllocString( wk->mman, msg_fnote_426 );
		break;
	case FNOTE_RESULT_DRAW:		// 引き分け
	default:
		str = MSGMAN_AllocString( wk->mman, msg_fnote_427 );
		break;
	}

	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：レコードコーナー表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioRecordDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_440 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：コンテスト表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioContestDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_450 );
	WORDSET_RegisterNumber(
		wk->wset, 0, mem->result, 1, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ポルト作成表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioPorutoDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_460 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：GTC表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioGtcDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_470 );
	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	FNOTE_StrWordSet( wk, mem->nickname, mem->poke_sex, 1 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：バトルタワー表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioBtlTowerDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_480 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：ぐるぐる交換表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioGuruguruDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_495 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：GDS表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 * @param   id		ID
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioGDSDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos, int id )
{
	STRBUF * str;
	u32 msg_id;
	
	switch(id){
	case FNOTE_ID_PL_GDS_VIDEO:				// 通信：GDSバトルビデオ
		msg_id = msg_fnote_496;
		break;
	case FNOTE_ID_PL_GDS_RANKING:			// 通信：GDSランキング
		msg_id = msg_fnote_497;
		break;
	case FNOTE_ID_PL_GDS_DRESS:				// 通信：GDSドレスアップ
		msg_id = msg_fnote_498;
		break;
	case FNOTE_ID_PL_GDS_BOX:				// 通信：GDSボックスショット
	default:
		msg_id = msg_fnote_499;
		break;
	}

	str = MSGMAN_AllocString( wk->mman, msg_id );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：WIFI広場・おしゃべり
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioLobbyChatDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_500 );
	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：WIFI広場・タッチトイ
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioLobbyToyDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_501 );
	FNOTE_StrWordSet( wk, mem->name1, mem->sex1, 0 );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：WIFI広場・ミニゲーム
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioLobbyMiniGameDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_502 );
	WORDSET_RegisterWiFiLobbyGameName( wk->wset, 0, mem->result );
	WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, wk->msg_buf, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：WIFI広場・アプリ(足跡ボード、世界時計、ニュース、パレード)
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioLobbyAppliDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos, int id )
{
	STRBUF * str;
	u32 msg_id;
	
	switch(id){
	case FNOTE_ID_PL_LOBBY_FOOT_BOARD:		// 通信：WIFI広場足跡ボード
		msg_id = msg_fnote_503;
		break;
	case FNOTE_ID_PL_LOBBY_WORLDTIMER:		// 通信：WIFI広場世界時計
		msg_id = msg_fnote_504;
		break;
	case FNOTE_ID_PL_LOBBY_NEWS:			// 通信：WIFI広場ニュース
		msg_id = msg_fnote_505;
		break;
	case FNOTE_ID_PL_LOBBY_PARADE:			// 通信：WIFI広場パレード
	default:
		msg_id = msg_fnote_506;
		break;
	}

	str = MSGMAN_AllocString( wk->mman, msg_id );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}

//--------------------------------------------------------------------------------------------
/**
 * 通信：WIFIクラブ表示
 *
 * @param	wk		冒険ノートのワーク
 * @param	win		表示するBMPウィンドウ
 * @param	mem		通信データ
 * @param	pos		表示位置
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void FNOTE_SioWifiClubDataPut(
				FNOTE_WORK * wk, GF_BGL_BMPWIN * win, FNOTE_WK_SIO * mem, u8 pos )
{
	STRBUF * str = MSGMAN_AllocString( wk->mman, msg_fnote_507 );
	GF_STR_PrintColor(
		win, FONT_SYSTEM, str, 0, SIO_PY+pos*16, MSG_NO_PUT, COL_N_BLACK, NULL );
	STRBUF_Delete( str );
}





















/*
void Debug_FNOTE_BmpPut( FNOTE_WORK * wk, u32 flg )
{
	GF_BGL_BMPWIN * time;
	GF_BGL_BMPWIN * info;
	STRBUF * str;

	if( flg == 0 ){
		time = &wk->win[WIN_TIME1];
		info = &wk->win[WIN_STR1];

		GF_BGL_BmpWinDataFill( time, 0 );
		GF_BGL_BmpWinDataFill( info, 0 );

		// 日付
		str = MSGMAN_AllocString( wk->mman, msg_fnote_001 );
		WORDSET_RegisterNumber(
			wk->wset, 0, 7, 2, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
		WORDSET_RegisterNumber(
			wk->wset, 1, 18, 2, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
		WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
		STRBUF_Delete( str );
		GF_STR_PrintColor( time, FONT_SYSTEM, wk->msg_buf, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
		// スタート！
		str = MSGMAN_AllocString( wk->mman, msg_fnote_000 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動１
		str = MSGMAN_AllocString( wk->mman, msg_fnote_100 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 16, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動２
		str = MSGMAN_AllocString( wk->mman, msg_fnote_121 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 32, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動３
		str = MSGMAN_AllocString( wk->mman, msg_fnote_144 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 48, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動４
		str = MSGMAN_AllocString( wk->mman, msg_fnote_171 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 64, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// ポケモン
		str = MSGMAN_AllocString( wk->mman, msg_fnote_200 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 80, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// トレーナー
		str = MSGMAN_AllocString( wk->mman, msg_fnote_300 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 96, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 通信
		str = MSGMAN_AllocString( wk->mman, msg_fnote_460 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 112, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );

	}else{
		time = &wk->win[WIN_TIME2];
		info = &wk->win[WIN_STR2];

		GF_BGL_BmpWinDataFill( time, 0 );
		GF_BGL_BmpWinDataFill( info, 0 );

		// 日付
		str = MSGMAN_AllocString( wk->mman, msg_fnote_001 );
		WORDSET_RegisterNumber(
			wk->wset, 0, 11, 2, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
		WORDSET_RegisterNumber(
			wk->wset, 1, 3, 2, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
		WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
		STRBUF_Delete( str );
		GF_STR_PrintColor( time, FONT_SYSTEM, wk->msg_buf, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
		// スタート！
		str = MSGMAN_AllocString( wk->mman, msg_fnote_000 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 0, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動１
		str = MSGMAN_AllocString( wk->mman, msg_fnote_101 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 16, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動２
		str = MSGMAN_AllocString( wk->mman, msg_fnote_110 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 32, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動３
		str = MSGMAN_AllocString( wk->mman, msg_fnote_145 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 48, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 行動４
		str = MSGMAN_AllocString( wk->mman, msg_fnote_173 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 64, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// ポケモン
		str = MSGMAN_AllocString( wk->mman, msg_fnote_203 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 80, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// トレーナー
		str = MSGMAN_AllocString( wk->mman, msg_fnote_302 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 96, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
		// 通信
		str = MSGMAN_AllocString( wk->mman, msg_fnote_480 );
		GF_STR_PrintColor( info, FONT_SYSTEM, str, 0, 112, MSG_NO_PUT, COL_N_BLACK, NULL );
		STRBUF_Delete( str );
	}

	GF_BGL_BmpWinOnVReq( time );
	GF_BGL_BmpWinOnVReq( info );
}
*/
