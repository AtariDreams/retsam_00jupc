//==============================================================================================
/**
 * @file	btower_scr.c
 * @brief	バトルタワー　スクリプトコマンド系
 * @author	Miyuki Iwasawa
 * @date	2006.04.17
 *
 * 2007.05.24	Satoshi Nohara
 *
 * frontier/fssc_tower_sub.cに処理を移動
 *
 * 関連ソース	field/scr_btower.c		常駐
 *				field/b_tower_scr.c		フィールドサブ(フィールド、フロンティアがあるので常駐へ)
 *				field/b_tower_ev.c		常駐
 *				field/b_tower_wifi.c	常駐
 *				field/b_tower_fld.c		フィールドサブ(フィールド、フロンティアがあるので常駐へ)
 */
//==============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/record.h"
#include "savedata/frontier_savedata.h"
#include "savedata/fnote_mem.h"
#include "system/buflen.h"
#include "savedata/tv_work.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "system/procsys.h"
#include "system/gamedata.h"
#include "application/pokelist.h"
#include "fieldsys.h"
#include "sysflag.h"
#include "field/situation.h"
#include "field/situation_local.h"
#include "field/tv_topic.h"
#include "field/tvtopic_extern.h"
#include "ev_mapchange.h"
#include "fielddata/base_goods/goods_id_def.h"
#include "savedata/undergrounddata.h"
#include "savedata/b_tower.h"
#define __B_TOWER_SCR_H_GLOBAL
#include "field/b_tower_scr.h"
#include "field/b_tower_ev.h"
#include "b_tower_fld.h"
#include "field/field_encount.h"
#include "syswork.h"

#include "scr_tool.h"

//#include "field/b_tower_deb.h"

///サブ関数プロトタイプ
static u16 towerscr_PokeRibbonSet(SAVEDATA* sv,u8 ribbon,BTOWER_SCRWORK* wk);
static u16 towerscr_IfRenshouPrizeGet(BTOWER_SCRWORK* wk);
static void towerscr_SaveMemberPokeData(BTOWER_SCRWORK* wk,SAVEDATA* sv,BTWR_SCORE_POKE_DATA mode);


//============================================================================================
//
//	関数
//
//============================================================================================

//--------------------------------------------------------------
/**
 *	@brief	プレイモード別のメンバー数を取得する
 *
 *	@param	mode	BTWR_MODE_〜
 */
//--------------------------------------------------------------
u16	TowerScrTools_GetMemberNum(u16 mode)
{
	switch(mode){
	case BTWR_MODE_SINGLE:
	case BTWR_MODE_WIFI:
	case BTWR_MODE_RETRY:
		return 3;
	case BTWR_MODE_DOUBLE:
		return 4;
	case BTWR_MODE_MULTI:
	case BTWR_MODE_COMM_MULTI:
	case BTWR_MODE_WIFI_MULTI:
		return 2;
	}
	return 0;
}

typedef struct POKE_CHK{
	u16	monsno;
	u16	itemno;
}POKE_CHK;

//同じ種類ポケモン＆同じ道具をもったポケモンかチェック
static BOOL pokechk_CheckSamePoke(POKE_CHK* plist,u16 monsno,u16 itemno,int num)
{
	int i;

	for(i = 0;i < num;i++){
		if(plist[i].monsno == monsno){
			if(itemno == 0){
				continue;
			}
			if(plist[i].itemno == itemno){
				return TRUE;
			}
		}
	}
	return FALSE;
}

//その組み合わせで挑戦可能かどうか？
static BOOL pokechk_IsEntryPattern(POKE_CHK* plist,int num)
{
	int i,j;

	for(i = 0;i < num-1;i++){
		for(j = i+1;j < num;j++){
			if(plist[i].monsno == plist[j].monsno){
				return FALSE;
			}
			if(plist[i].itemno == 0){
				continue;
			}
			if(plist[i].itemno == plist[j].itemno){
				return FALSE;
			}
		}
	}
	return TRUE;
}

//参加チェック組み合わせ総当り
static BOOL pokechk_CheckPattern(POKE_CHK* ps,int num,int pokenum,int max)
{
	int i,j,k,l;
	POKE_CHK	pd[4];

	MI_CpuClear8(pd,sizeof(POKE_CHK)*4);
	
	//残ったポケモンの中で総当り
	for(i = 0;i < max;i++){	//一体目
		pd[0] = ps[i];
		for(j = i+1;j < pokenum;j++){	//二体目
			pd[1] = ps[j];
			if(num == 2){
				if(pokechk_IsEntryPattern(pd,num)){
					return TRUE;
				}
				continue;
			}
			for(k = j+1;k < pokenum;k++){	//３体目
				pd[2] = ps[k];
				if(num == 3){
					if(pokechk_IsEntryPattern(pd,num)){
						return TRUE;
					}
					continue;
				}
				for(l = k+1;l < pokenum;l++){	//４体目
					pd[3] = ps[l];
					if(pokechk_IsEntryPattern(pd,num)){
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 *	@brief	バトルタワー　参加可能なポケモン数のチェック
 *
 *	@param	num			参加に必要なポケモン数
 *	@param	item_flag	アイテムチェックするかフラグ
 *
 *	@retval	true	参加可能
 *	@retval	false	参加不可
 */
//--------------------------------------------------------------
BOOL TowerScrTools_CheckEntryPokeNum(u16 num,SAVEDATA *savedata,u8 item_flag)
{
	u8	i,j,ct,pokenum;
	u16	monsno,itemno;
	POKEPARTY	*party;
	POKEMON_PARAM* pp;
	POKE_CHK	ps[6];
	
	party = SaveData_GetTemotiPokemon(savedata);
	pokenum = PokeParty_GetPokeCount(party);

	if(pokenum < num){
		return FALSE;
	}

	for(i = 0,ct = 0;i < pokenum;i++){
		pp = PokeParty_GetMemberPointer(party,i);
		monsno = PokeParaGet(pp,ID_PARA_monsno,NULL);
		itemno = PokeParaGet(pp,ID_PARA_item,NULL);
		
		//アイテムをチェックしない時は0クリアしておく
		if( item_flag == 0 ){
			itemno = 0;
		}

		//タマゴでないかチェック
		if(PokeParaGet(pp,ID_PARA_tamago_flag,NULL) != 0){
			continue;
		}

		//バトルタワーに参加できないポケモンかをチェック
		if(BattleTowerExPokeCheck_MonsNo(monsno) == TRUE){
			continue;
		}

		//引数にアイテムをチェックするか追加(08.04.28)
		if( item_flag == 1 ){
			//同じ種類ポケモン＆同じ道具をもったポケモンかチェック
			if(pokechk_CheckSamePoke(ps,monsno,itemno,ct) == TRUE){
				//既に選ばれているポケモンと同じ
				continue;
			}
		}

		ps[ct].monsno = monsno;
		ps[ct].itemno = itemno;
		ct++;
	}

	if(ct < num){
		return FALSE;
	}

	//参加チェック組み合わせ総当り
	return pokechk_CheckPattern(ps,num,ct,(ct-num)+1);
}

//--------------------------------------------------------------
/**
 *	@brief	リセットしてタイトルに戻る
 */
//--------------------------------------------------------------
void TowerScrTools_SystemReset(void)
{
	OS_ResetSystem(0);
}

//--------------------------------------------------------------
/**
 *	@brief	new game時にプレイデータをクリアする
 */
//--------------------------------------------------------------
void TowerScrTools_ClearPlaySaveData(BTLTOWER_PLAYWORK* playSave)
{
	TowerPlayData_Clear(playSave);
}

//--------------------------------------------------------------
/**
 * @brief	セーブデータが有効かどうか返す
 */
//--------------------------------------------------------------
BOOL TowerScrTools_IsSaveDataEnable(BTLTOWER_PLAYWORK* playSave)
{
	return TowerPlayData_GetSaveFlag(playSave);
}

//--------------------------------------------------------------
/**
 *	@brief	脱出用に現在のロケーションを記憶する
 */
//--------------------------------------------------------------
void TowerScrTools_PushNowLocation(FIELDSYS_WORK* fsys)
{
	LOCATION_WORK * sp = Situation_GetSpecialLocation(SaveData_GetSituation(fsys->savedata));

	//現在位置を特殊接続先に設定
	SetLocation(sp, fsys->location->zone_id, DOOR_ID_JUMP_CODE,
			Player_NowGPosXGet(fsys->player), Player_NowGPosZGet(fsys->player), DIR_UP);

	//特殊接続フラグON
	SysFlag_CommCounterSet(SaveData_GetEventWork(fsys->savedata));
	return;	
}

/**
 *	@brief	ゲートから出る時に特殊接続先を元に戻す
 */
void TowerScrTools_PopNowLocation(FIELDSYS_WORK* fsys)
{
	//特殊接続フラグOff
	SysFlag_CommCounterReset(SaveData_GetEventWork(fsys->savedata));
}

/**
 *	@brief	入ったゲートの位置を取得する
 */
void TowerScrTools_GetGateLocation(FIELDSYS_WORK* fsys,u16* x,u16* y)
{
	LOCATION_WORK * sp = Situation_GetSpecialLocation(SaveData_GetSituation(fsys->savedata));

	*x = sp->grid_x;
	*y = sp->grid_z;
}

//--------------------------------------------------------------
/**
 *	@brief	プレイモード別レコードセーブデータ連勝数を返す
 */
//--------------------------------------------------------------
u16	TowerScrTools_GetRenshouRecord(SAVEDATA* sv,u16 play_mode)
{
	u16 val;
	if(play_mode == BTWR_MODE_RETRY){
		return 0;
	}

	//専用取得
	if(play_mode == BTWR_MODE_WIFI_MULTI){
		val = FrontierRecord_Get(SaveData_GetFrontier(sv),
			FRID_TOWER_MULTI_WIFI_RENSHOU_CNT, 
			Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_RENSHOU_CNT) );
		return val;
	}

	val = FrontierRecord_Get(SaveData_GetFrontier(sv),
			FRID_TOWER_SINGLE_RENSHOU_CNT+play_mode*2, FRONTIER_RECORD_NOT_FRIEND);

	return val;
}

/**
 *	@brief	スコアデータのフラグを返す
 */
u16 TowerScrTools_GetScoreFlags(SAVEDATA* sv,BTWR_SFLAG_ID flagid)
{
	BTLTOWER_SCOREWORK	*score = SaveData_GetTowerScoreData(sv);

	return (u16)TowerScoreData_SetFlags(score,flagid,BTWR_DATA_get);
}

//--------------------------------------------------------------
/**
 *	@brief	Wifi成績の未アップロードフラグを制御する
 */
//--------------------------------------------------------------
void TowerScrTools_SetWifiUploadFlag(SAVEDATA* sv,u8 flag)
{
	BTLTOWER_SCOREWORK	*score = SaveData_GetTowerScoreData(sv);
	
	if(flag == 0){	//リセット
		TowerScoreData_SetFlags(score,BTWR_SFLAG_WIFI_UPLOAD,BTWR_DATA_reset);
	}else{	//セット
		TowerScoreData_SetFlags(score,BTWR_SFLAG_WIFI_UPLOAD,BTWR_DATA_set);
	}
}

//--------------------------------------------------------------
/**
 *	@brief	WiFiプレイ成績の未アップロードフラグを取得する
 */
//--------------------------------------------------------------
u16 TowerScrTools_GetWifiUploadFlag(SAVEDATA* sv)
{
	BTLTOWER_SCOREWORK	*score = SaveData_GetTowerScoreData(sv);
	return (u16)TowerScoreData_SetFlags(score,BTWR_SFLAG_WIFI_UPLOAD,BTWR_DATA_get);
}

//--------------------------------------------------------------
/**
 *	@brief	セーブせずに止めたときのエラー処理
 *
 *	@return	プレイしていたモードを返す
 */
//--------------------------------------------------------------
u16 TowerScrTools_SetNGScore(SAVEDATA* savedata)
{
	u8	play_mode;
	int id;
	BTLTOWER_PLAYWORK* playSave;
	BTLTOWER_SCOREWORK* scoreSave;
//	RECORD*  record;
	
	//セーブデータ取得
	playSave = SaveData_GetTowerPlayData(savedata);
	scoreSave = SaveData_GetTowerScoreData(savedata);

	//どのモードをプレイしていたか？
	play_mode = (u8)TowerPlayData_Get(playSave,BTWR_PSD_playmode,NULL);
	
	if(play_mode == BTWR_MODE_RETRY){
		return play_mode;
	}

	//レコード挑戦中フラグを落とす
	if(play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		TowerScoreData_SetFlags(scoreSave,BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_reset);
#else
		FrontierRecord_Set(	SaveData_GetFrontier(savedata), 
							FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
							Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT), 0 );
#endif
	}else{
		TowerScoreData_SetFlags(scoreSave,BTWR_SFLAG_SINGLE_RECORD+play_mode,BTWR_DATA_reset);
	}

	//現在の周回数リセット
	TowerScoreData_SetStage(scoreSave,play_mode,BTWR_DATA_reset);

#if 1	//不正終了した時は、現在の連勝数を0にするように変更(プラチナで仕様変更)
	//"現在の連勝数"を0にする
	//シングル、ダブル、AIマルチ、マルチ、WIFIマルチ(DLは別)
	FrontierRecord_Set(	SaveData_GetFrontier(savedata), 
						TowerScr_GetWinRecordID(play_mode),
						Frontier_GetFriendIndex(TowerScr_GetWinRecordID(play_mode)), 0 );
#endif

	//プラチナで追加されたWIFIマルチも除く
	//if(play_mode != BTWR_MODE_WIFI){
	if( (play_mode != BTWR_MODE_WIFI) && (play_mode != BTWR_MODE_WIFI_MULTI) ){
		//プレイランダムシードをひとつ強制で進める
		BtlTower_UpdatePlayRndSeed(savedata);
	}

	OS_Printf("TowerNGScoreSet -> mode = %d\n",play_mode);
	return play_mode;	
}

//--------------------------------------------------------------
/**
 *	@brief	WIFIのプレイヤーDLデータがあるかどうかチェック
 */
//--------------------------------------------------------------
u16	TowerScrTools_IsPlayerDataEnable(SAVEDATA* sv)
{
	BTLTOWER_WIFI_DATA	*wifiSave;
	wifiSave = SaveData_GetTowerWifiData(sv);

	return (u16)TowerWifiData_IsPlayerDataEnable(wifiSave);
}

//***************************************************************
//===============================================================
//	ワークの初期化と解放
//===============================================================
//***************************************************************
/**
 *	@brief	fsys内のバトルタワー制御ワークポインタを初期化する
 *			バトルタワー開始時に必ず呼び出す
 */
void TowerScr_WorkClear(BTOWER_SCRWORK** pp)
{
	GF_ASSERT(*pp==NULL && "btower work is reake");
	*pp = NULL;
}

//--------------------------------------------------------------
/**
 *	@brief	ワークエリアを取得して初期化する
 *
 *	@param	savedata	セーブデータへのポインタ
 *	@param	init		初期化モード BTWR_PLAY_NEW:初めから、BTWR_PLAY_CONTINE:続きから
 *	@param	playmode	プレイモード指定:BTWR_MODE_〜
 *	
 *	＊かならず TowerScr_WorkRelease()で領域を開放すること
 */
//--------------------------------------------------------------
BTOWER_SCRWORK* TowerScr_WorkInit( SAVEDATA* savedata, u16 init, u16 playmode )
{
	u8 buf8;
	u16	i,chg_flg;
	BTOWER_SCRWORK* wk;
	FRONTIER_SAVEWORK* frontier;
	RECORD *record;
	
	/////////////////////
	//HEAPID_WORLDに変更
	/////////////////////
	
	//sys_CreateHeapLo(HEAPID_BASE_APP,HEAPID_BTLTOWER_SCR,0x4000);

	//wk =  sys_AllocMemory(HEAPID_BTLTOWER_SCR,sizeof(BTOWER_SCRWORK));
	wk =  sys_AllocMemory(HEAPID_WORLD,sizeof(BTOWER_SCRWORK));
	MI_CpuClear8(wk,sizeof(BTOWER_SCRWORK));

	//wk->heapID = HEAPID_BTLTOWER_SCR;
	wk->heapID = HEAPID_WORLD;

	//セーブデータ取得
	wk->playSave = SaveData_GetTowerPlayData(savedata);
	wk->scoreSave = SaveData_GetTowerScoreData(savedata);

	//マジックナンバー設定
	wk->magicNo = BTOWER_SCRWORK_MAGIC;

	//セーブフラグを無効状態にリセット
	TowerPlayData_SetSaveFlag(wk->playSave,FALSE);
	
	if(init == BTWR_PLAY_NEW){
		wk->play_mode = playmode;
		wk->member_num = (u8)TowerScrTools_GetMemberNum(wk->play_mode);
		wk->now_round = 1;
		wk->now_win = 0;

		for(i = 0;i < BTOWER_STOCK_MEMBER_MAX;i++){
			wk->member[i] = BTWR_NULL_POKE;
		}

		for(i = 0;i < BTOWER_STOCK_TRAINER_MAX;i++){
			wk->trainer[i] = BTWR_NULL_TRAINER;
		}

		//セーブデータ初期化
		TowerPlayData_Clear(wk->playSave);

		//プレイモードだけはこの時点でセーブデータに書き出す
		buf8 = wk->play_mode;
		TowerPlayData_Put(wk->playSave,BTWR_PSD_playmode,&buf8);

#ifdef BTOWER_AUTO_DEB
		if(wk->play_mode == BTWR_MODE_WIFI){
			OS_Printf(" #WifiTowerStage=%d\n",DebugBTowerAutoStage++);
		}
#endif

	}else{
		//現在のプレイ進行データ取得
		wk->play_mode = (u8)TowerPlayData_Get(wk->playSave,BTWR_PSD_playmode,NULL);
		wk->now_round = (u8)TowerPlayData_Get(wk->playSave,BTWR_PSD_round,NULL);
		wk->now_win = wk->now_round-1;

		wk->member_num = (u8)TowerScrTools_GetMemberNum(wk->play_mode);

		//選ばれているポケモンNo
		TowerPlayData_Get(wk->playSave,BTWR_PSD_pokeno,wk->member);

		//抽選済みのトレーナーNo
		TowerPlayData_Get(wk->playSave,BTWR_PSD_trainer,wk->trainer);

		//プレイランダムシード取得
		wk->play_rnd_seed = TowerPlayData_Get(wk->playSave,BTWR_PSD_rnd_seed,NULL);
		OS_Printf("TowerContinueRndSeed = %d\n",wk->play_rnd_seed);
		
		if(wk->play_mode == BTWR_MODE_MULTI){
			wk->partner = (u8)TowerPlayData_Get(wk->playSave,BTWR_PSD_partner,NULL);

			//パートナートレーナーデータ再生成
			TowerPlayData_Get(wk->playSave,BTWR_PSD_pare_poke,&(wk->five_poke[wk->partner]));
			RomBattleTowerPartnerDataMake(wk,&wk->five_data[wk->partner],
				TOWER_FIVE_FIRST+wk->partner,
				TowerPlayData_Get(wk->playSave,BTWR_PSD_pare_itemfix,NULL),
				&(wk->five_poke[wk->partner]),wk->heapID);
		}
	}

	//自分の性別を取得
	wk->my_sex = MyStatus_GetMySex(SaveData_GetMyStatus(savedata));

	//現在のレコードを取得
	if(wk->play_mode != BTWR_MODE_RETRY){
		frontier = SaveData_GetFrontier(savedata);
		record = SaveData_GetRecord(savedata);
		
		//現在の連勝数は挑戦中フラグがonのときだけ引き継ぐ
		if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
			chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
						BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_get);
#else
			//chg_flg = FrontierRecord_Get(SaveData_GetFrontier(savedata), 
			//						FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
			//						Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT) );
			chg_flg = SysWork_WifiFrClearFlagGet( SaveData_GetEventWork(savedata) );
			OS_Printf( "WIFI chg_flg = %d\n", chg_flg );
#endif
		}else{
			chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
						BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,BTWR_DATA_get);
		}

		if(chg_flg){
			if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
				wk->renshou = FrontierRecord_Get(
				frontier,FRID_TOWER_MULTI_WIFI_RENSHOU_CNT,
				Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_RENSHOU_CNT) );
			}else{
				wk->renshou = FrontierRecord_Get(
				frontier,FRID_TOWER_SINGLE_RENSHOU_CNT+wk->play_mode*2,FRONTIER_RECORD_NOT_FRIEND);
			}

			wk->stage = TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_get);
		}

		wk->win_cnt = RECORD_Get(record,RECID_BTOWER_WIN);

		//wk->stageがchg_flgに関係なく取得していたので変更(08.05.20)
		//wifiのために変更したがwifiでは連勝記録はDPとは別のものを使用しているので、
		//周回数の変数は見ていないから平気と思われるが一応。
		//wk->stage = TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_get);
	}

	//WIFI(32人データがあるので、周回数ワークが足りないため、連勝数から周回数を算出してセット
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
		wk->stage = TowerScoreData_SetStageValue(	wk->scoreSave, BTWR_MODE_WIFI_MULTI, 
													(wk->renshou / 7) );
		OS_Printf( "** wk->renshou = %d\n", wk->renshou );
		OS_Printf( "** stage = %d\n", TowerScoreData_SetStage(wk->scoreSave,
															BTWR_MODE_WIFI_MULTI,BTWR_DATA_get) );
	}

	OS_Printf( "stage = %d\n", wk->stage );
	return wk;
}

//--------------------------------------------------------------
/**
 *	@brief	ワークエリアを開放する
 */
//--------------------------------------------------------------
void TowerScr_WorkRelease(BTOWER_SCRWORK* wk)
{
	int	heap;

	if(wk == NULL){
		return;
	}
	GF_ASSERT(wk->magicNo == BTOWER_SCRWORK_MAGIC && "btower word is crash");
	heap = wk->heapID;

	MI_CpuClear8(wk,sizeof(BTOWER_SCRWORK));
	sys_FreeMemoryEz(wk);
	wk = NULL;

	//sys_DeleteHeap(heap);
}


//***************************************************************
//===============================================================
//	ワークに依存するサブツール
//===============================================================
//***************************************************************

//--------------------------------------------------------------
/**
 *	@brief	バトルタワー用ポケモン選択呼出し
 */
//--------------------------------------------------------------
void TowerScr_SelectPoke(BTOWER_SCRWORK* wk,GMEVENT_CONTROL *ev_work,void** proc_wk)
{
	EventCmd_BTowerPokeSelectCall(ev_work,proc_wk,
		PL_MODE_BATTLE_TOWER,PST_MODE_NORMAL,
		wk->member_num,wk->member_num,100,0);
}

//--------------------------------------------------------------
/**
 *	@brief	選択したポケモンを取得
 *	
 *	@retval	TRUE	選択した
 *	@retval	FALSE	選択せずにやめた
 */
//--------------------------------------------------------------
BOOL TowerScr_GetEntryPoke(BTOWER_SCRWORK* wk,void** app_work,SAVEDATA* sv)
{
	u16	i = 0;
	PLIST_DATA* pld = *app_work;
	POKEPARTY* party;
	POKEMON_PARAM* pp;

#ifdef BTOWER_AUTO_DEB
	if(pld->ret_mode != 0){
		sys_FreeMemoryEz(*app_work);
		*app_work = NULL;
		return FALSE;
	}
#else	//BTOWER_AUTO_DEB
	//データ取得
	if(pld->ret_mode != 0 || pld->ret_sel == PL_SEL_POS_EXIT){
		sys_FreeMemoryEz(*app_work);
		*app_work = NULL;
		return FALSE;
	}
#endif	//BTOWER_AUTO_DEB
	party = SaveData_GetTemotiPokemon(sv);
	
	for(i = 0;i < wk->member_num;i++){
		wk->member[i] = pld->in_num[i]-1;
		pp = PokeParty_GetMemberPointer(party,wk->member[i]);
		wk->mem_poke[i] = PokeParaGet(pp,ID_PARA_monsno,NULL);	
		wk->mem_item[i] = PokeParaGet(pp,ID_PARA_item,NULL);	
	}
	//ワーク領域解放
	sys_FreeMemoryEz(*app_work);
	*app_work = NULL;
	return TRUE;
}

//--------------------------------------------------------------
/**
 *	@brief	参加指定した手持ちポケモンの条件チェック
 *
 *	@retval	0	参加OK
 *	@retval	1	同じポケモンがいる
 *	@retval 2	同じアイテムを持っている
 */
//--------------------------------------------------------------
int TowerScr_CheckEntryPoke(BTOWER_SCRWORK* wk,SAVEDATA* savedata)
{
#if 1
	u16	i = 0,j = 0;
	u16	monsno[BTOWER_STOCK_MEMBER_MAX],itemno[BTOWER_STOCK_MEMBER_MAX];
	POKEPARTY* party;
	POKEMON_PARAM* poke;

	party = SaveData_GetTemotiPokemon(savedata);

	for(i = 0;i < wk->member_num;i++){
		poke = PokeParty_GetMemberPointer(party,wk->member[i]);
		monsno[i] = PokeParaGet(poke,ID_PARA_monsno,NULL);
		itemno[i] = PokeParaGet(poke,ID_PARA_item,NULL);

		if(i == 0){
			continue;
		}
		for(j = 0;j < i;j++){
			if(monsno[i] == monsno[j]){
				//同じポケモンがいる
				return 1;
			}
			if((itemno[i] != 0) && (itemno[i] == itemno[j])){
				//同じアイテムをもっている
				return 2;
			}
		}
	}
	return 0;
#else
	return TowerScr_CheckEntryPokeSub( wk->member, wk->member_num, savedata );
#endif
}

static BOOL is_trainer_conflict(u16* trainer,u16 id,u16 num)
{
	u16 i;
	for(i = 0;i < num;i++){
		if(trainer[i] == id){
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 *	@brief　対戦トレーナーNo抽選
 */
//--------------------------------------------------------------
void TowerScr_BtlTrainerNoSet(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	int i;
	u16	no,stage;
	
	if(	wk->play_mode == BTWR_MODE_MULTI ||
		wk->play_mode == BTWR_MODE_WIFI_MULTI ||
		wk->play_mode == BTWR_MODE_COMM_MULTI){
		//if(wk->play_mode == BTWR_MODE_COMM_MULTI && wk->pare_stage > wk->stage){
		if( (wk->play_mode == BTWR_MODE_COMM_MULTI && wk->pare_stage > wk->stage) ||
			(wk->play_mode == BTWR_MODE_WIFI_MULTI && wk->pare_stage > wk->stage) ){
			stage = wk->pare_stage;	//通信時には周回数の多いほうで抽選
		}else{
			stage = wk->stage;
		}
		for(i = 0;i < BTOWER_STOCK_TRAINER_MAX;i++){
			do{
				no = BattleTowerTrainerNoGet(wk,stage,i/2,wk->play_mode);
			}while(is_trainer_conflict(wk->trainer,no,i));
#ifdef BTOWER_AUTO_DEB
#if 0
			wk->trainer[i] = DebugBTowerAutoTrainer;
			OS_Printf(" #TowerStage = %d,%d\n",DebugBTowerAutoStage,DebugBTowerAutoTrainer);
			if(DebugBTowerAutoTrainer++ >= 299){
				DebugBTowerAutoTrainer = 0;
				DebugBTowerAutoStage++;
			}
#endif
			wk->trainer[i] = no;
			if(DebugBTowerAutoTrainer++ >= 299){
				DebugBTowerAutoStage++;
			}
			OS_Printf(" #TowerStage = %d,%d\n",DebugBTowerAutoStage,DebugBTowerAutoTrainer);
#else		
			wk->trainer[i] = no;
#endif
		}
	}else{
		for(i = 0;i < (BTOWER_STOCK_TRAINER_MAX/2);i++){
			do{
				no = BattleTowerTrainerNoGet(wk,wk->stage,i,wk->play_mode);
			}while(is_trainer_conflict(wk->trainer,no,i));
#ifdef BTOWER_AUTO_DEB
#if 0
			wk->trainer[i] = DebugBTowerAutoTrainer;
			OS_Printf(" #TowerStage = %d,%d\n",DebugBTowerAutoStage,DebugBTowerAutoTrainer);
			if(DebugBTowerAutoTrainer++ >= 299){
				DebugBTowerAutoTrainer = 0;
				DebugBTowerAutoStage++;
			}
#endif
			if(DebugBTowerAutoTrainer++ >= 299){
				DebugBTowerAutoStage++;
			}
			OS_Printf(" #TowerStage = %d,%d\n",DebugBTowerAutoStage,DebugBTowerAutoTrainer);
			wk->trainer[i] = 25;//no;
#else		
			wk->trainer[i] = no;
#endif
		}
	}
}

//--------------------------------------------------------------
/**
 *	@brief	現在のラウンド数をスクリプトワークに取得する
 */
//--------------------------------------------------------------
u16	TowerScr_GetNowRound(BTOWER_SCRWORK* wk)
{
	return wk->now_round;
}

//--------------------------------------------------------------
/**
 *	@brief	7連勝しているかどうかチェック
 */
//--------------------------------------------------------------
BOOL TowerScr_IsClear(BTOWER_SCRWORK* wk)
{
	if(wk->clear_f){
		return TRUE;
	}
	if(wk->now_round > BTWR_CLEAR_WINCNT){
		//クリアフラグon
		wk->clear_f = 1;
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 *	@brief	最大連勝記録更新番組作成関数
 */
//--------------------------------------------------------------
static void towerscr_MakeTVRenshouMaxUpdate(BTOWER_SCRWORK* wk,SAVEDATA* savedata,u16 renshou)
{
	POKEPARTY *party;

	if(	wk->play_mode != BTWR_MODE_SINGLE &&
		wk->play_mode != BTWR_MODE_DOUBLE){
		return;
	}
	party = SaveData_GetTemotiPokemon(savedata);

	if(wk->play_mode == BTWR_MODE_SINGLE){
		//TVTOPIC_Entry_Record_BTower(fsys,
		TVTOPIC_Entry_Record_BTowerEx(savedata,
			renshou,PokeParty_GetMemberPointer(party,wk->member[0]), TRUE);
	}else{
		//TVTOPIC_Entry_Record_BTower(fsys,
		TVTOPIC_Entry_Record_BTowerEx(savedata,
			renshou,PokeParty_GetMemberPointer(party,wk->member[0]), FALSE);
	}
}

//--------------------------------------------------------------
/**
 *	@brief	7人抜き成功または負けた時の共通データセーブ処理
 *
 *	@param	now_renshou	現在の連勝数
 */
//--------------------------------------------------------------
static void towerscr_SetCommonScore(BTOWER_SCRWORK* wk,SAVEDATA* sv,u8 win_f,u16 now_renshou)
{
	u8	buf8;

	switch(wk->play_mode){
	case BTWR_MODE_SINGLE:
		//ポケモンデータセット
		towerscr_SaveMemberPokeData(wk,sv,BTWR_SCORE_POKE_SINGLE);
	case BTWR_MODE_DOUBLE:
		if(now_renshou >= 7){
			//TVインタビューデータセット(シングルとダブルで実行)
			TVTOPIC_BTowerTemp_Set(SaveData_GetTvWork(sv),win_f,now_renshou);
		}
		break;
	case BTWR_MODE_WIFI:
		//ポケモンデータセット
		towerscr_SaveMemberPokeData(wk,sv,BTWR_SCORE_POKE_WIFI);
		//スコア押し出し
		TowerPlayData_WifiRecordAdd(wk->playSave,wk->rec_down,wk->rec_turn,wk->rec_damage);

		//プレイモード書き出し
		buf8 = wk->play_mode;
		TowerPlayData_Put(wk->playSave,BTWR_PSD_playmode,&buf8);
		//ラウンド数書き出し
		buf8 = wk->now_round;
		TowerPlayData_Put(wk->playSave,BTWR_PSD_round,&buf8);
		
		TowerScoreData_SetWifiScore(wk->scoreSave,wk->playSave);
		break;
	default:
		break;
	}

}

//--------------------------------------------------------------
/**
 *	@brief	敗戦処理	
 */
//--------------------------------------------------------------
void TowerScr_SetLoseScore(BTOWER_SCRWORK* wk,SAVEDATA* savedata)
{
	u32	ret = 0;
	int	id;
	u16	before,after,chg_flg;
	RECORD* record = SaveData_GetRecord(savedata);
	FRONTIER_SAVEWORK *frontier = SaveData_GetFrontier(savedata);
	
	if(wk->play_mode == BTWR_MODE_RETRY){
		return;
	}
	
	OS_Printf("TowerLoseScoreSet -> mode = %d\n",wk->play_mode);

	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
		id = FRID_TOWER_MULTI_WIFI_RENSHOU;				//最大
	}else{
		id = wk->play_mode*2+FRID_TOWER_SINGLE_RENSHOU;	//最大
	}

	//現在の最大連勝数取得
	before = FrontierRecord_Get(frontier,id,Frontier_GetFriendIndex(id));

	OS_Printf( "before = %d\n", before );
	OS_Printf( "wk->renshou = %d\n", wk->renshou );
	OS_Printf( "wk->now_win = %d\n", wk->now_win );

	//最大連勝数更新
	after = FrontierRecord_SetIfLarge(frontier, id, Frontier_GetFriendIndex(id), 
										wk->renshou+wk->now_win);

	//更新している||(記録が7の倍数&&前後の値が同じ)なら番組作成
	if(after > 1){
		if(	(before < after) ||
			((before == after) && (after%7==0)) ){
			towerscr_MakeTVRenshouMaxUpdate(wk,savedata,after);
		}
	}

	//成績モニタ用に現在の連勝数も書き出しておく
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_get);
#else
		chg_flg = FrontierRecord_Get(SaveData_GetFrontier(savedata), 
									FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT) );
#endif
	}else{
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,BTWR_DATA_get);
	}

	OS_Printf( "chg_flg = %d\n", chg_flg );
	OS_Printf( "now win = %d\n", FrontierRecord_Get(frontier,id+1,Frontier_GetFriendIndex(id+1)) );

#if 0
	if(chg_flg){	//連勝記録挑戦中なら加算
		ret = FrontierRecord_Add(frontier, id+1, Frontier_GetFriendIndex(id+1), wk->now_win);
	}else{	//前回負けていればセット
		ret = FrontierRecord_Set(frontier, id+1, Frontier_GetFriendIndex(id+1), wk->now_win);
	}
#else
	//連勝してから、負けた時に、wk->renshouが必ず0になっていれば、
	//(renshou + now_win)をセットでいいはず(08.05.25)
	//7+7=14勝利(勝ち終わり)
	//7+3=10勝利(負け終わり)
	//0+3=3勝利(負け終わり)
	ret = FrontierRecord_Set(frontier,id+1,Frontier_GetFriendIndex(id+1),(wk->renshou+wk->now_win));
#endif

	OS_Printf( "now win = %d\n", FrontierRecord_Get(frontier,id+1,Frontier_GetFriendIndex(id+1)) );

	//レコード挑戦中フラグを落とす
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_WIFI_MULTI_RECORD,
								BTWR_DATA_reset);
#else
		FrontierRecord_Set(	SaveData_GetFrontier(savedata), 
							FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
							Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT), 0 );
#endif
	}else{
		TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,
								BTWR_DATA_reset);
	}

	//延べ勝利数更新
	RECORD_Add(record,RECID_BTOWER_WIN,wk->now_win);
	//現在の周回数リセット
	TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_reset);

	//バトルタワーへのチャレンジ数追加
	if(wk->play_mode != BTWR_MODE_WIFI_MULTI){
		RECORD_Add(SaveData_GetRecord(savedata),RECID_BTOWER_CHALLENGE,1);
	}
	
	//連勝リボンをもらえるかどうかのフラグをセット
	towerscr_IfRenshouPrizeGet(wk);

	//勝ち負け共通データ処理
	ret+=1;
	if(ret > 9999){
		ret = 9999;	
	}
	towerscr_SetCommonScore(wk,savedata,FALSE,ret);
}

//--------------------------------------------------------------
/**
 *	@brief	クリア処理
 */
//--------------------------------------------------------------
void TowerScr_SetClearScore(BTOWER_SCRWORK* wk,SAVEDATA* savedata, FNOTE_DATA* fnote)
{
	u32	ret = 0;
	int	id;
	void* note;
	u16	before,after,chg_flg;
	RECORD*  record;
	FRONTIER_SAVEWORK *frontier;
		
	if(wk->play_mode == BTWR_MODE_RETRY){
		return;
	}
	OS_Printf("TowerClearScoreSet -> mode = %d\n",wk->play_mode);
	
	record = SaveData_GetRecord(savedata);
	frontier = SaveData_GetFrontier(savedata);

	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
		id = FRID_TOWER_MULTI_WIFI_RENSHOU;
	}else{
		id = wk->play_mode*2+FRID_TOWER_SINGLE_RENSHOU;
	}
	
	//現在の連勝数を書き出し
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_get);
#else
		chg_flg = FrontierRecord_Get(SaveData_GetFrontier(savedata), 
									FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT) );
#endif
	}else{
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,BTWR_DATA_get);
	}

#if 0
	if(chg_flg){	//現在も連勝記録挑戦中なら加算
		//ret = FrontierRecord_Add(frontier,id+1,Frontier_GetFriendIndex(id+1),BTWR_CLEAR_WINCNT);

		ret = FrontierRecord_Get(	SaveData_GetFrontier(savedata), 
									(id+1),
									Frontier_GetFriendIndex(id+1) );

		FrontierRecord_Set(	SaveData_GetFrontier(savedata), 
							(id+1),
							Frontier_GetFriendIndex(id+1), (ret+BTWR_CLEAR_WINCNT) );

		ret = FrontierRecord_Get(	SaveData_GetFrontier(savedata), 
									(id+1),
									Frontier_GetFriendIndex(id+1) );

	}else{	//前回負けてた場合は一旦リセット
		ret = FrontierRecord_Set(frontier,id+1,Frontier_GetFriendIndex(id+1),BTWR_CLEAR_WINCNT);
	}
#else
	//連勝してから、負けた時に、wk->renshouが必ず0になっていれば、
	//(renshou + now_win)をセットでいいはず(08.05.25)
	//7+7=14勝利(勝ち終わり)
	//7+3=10勝利(負け終わり)
	//0+3=3勝利(負け終わり)
	OS_Printf( "wk->renshou = %d\n", wk->renshou );
	OS_Printf( "wk->now_win = %d\n", wk->now_win );
	ret = FrontierRecord_Set(frontier,id+1,Frontier_GetFriendIndex(id+1),(wk->renshou+wk->now_win));
#endif

	//レコード挑戦中フラグを立てる
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_set);
#else
		FrontierRecord_Set(	SaveData_GetFrontier(savedata), 
							FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
							Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT), 1 );
#endif
	}else{
		TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,BTWR_DATA_set);
	}

	//現在の最大連勝数取得
	before = FrontierRecord_Get(frontier,id,Frontier_GetFriendIndex(id));
	//最大連勝数更新
	after = FrontierRecord_SetIfLarge(frontier,id,Frontier_GetFriendIndex(id),ret);

	//更新してたら番組作成
#if 0
	if(before < after){
		towerscr_MakeTVRenshouMaxUpdate(wk,fsys->savedata,after);
	}
#endif
	//延べ勝利数更新
	RECORD_Add(record,RECID_BTOWER_WIN,BTWR_CLEAR_WINCNT);
	//周回数プラス
	TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_inc);

	//バトルタワーへのチャレンジ数追加
	if(wk->play_mode != BTWR_MODE_WIFI_MULTI){
		RECORD_Add(record,RECID_BTOWER_CHALLENGE,1);
	}

	//スコア加算
	RECORD_Score_Add(record,SCORE_ID_BTOWER_7WIN);
	
	//連勝リボンをもらえるかどうかのフラグをセット
	towerscr_IfRenshouPrizeGet(wk);

	//勝ち負け共通データ処理
	towerscr_SetCommonScore(wk,savedata,TRUE,ret);

#if 0
	//冒険ノート(通信マルチ&wifiのみ)
	if(	wk->play_mode == BTWR_MODE_COMM_MULTI ||
		wk->play_mode == BTWR_MODE_WIFI_MULTI ||
		wk->play_mode == BTWR_MODE_WIFI){
#else
	//冒険ノート(WIFI_DLのみ)
	if( wk->play_mode == BTWR_MODE_WIFI ){
#endif
		note = FNOTE_SioBattleTowerDataMake(wk->heapID);
		FNOTE_DataSave( fnote, note, FNOTE_TYPE_SIO );
	}
}

//--------------------------------------------------------------
/**
 *	@brief	休むときに現在のプレイ状況をセーブに書き出す
 */
//--------------------------------------------------------------
void TowerScr_SaveRestPlayData(BTOWER_SCRWORK* wk)
{
	u16	i;
	u8	buf8[4];

	//プレイモード書き出し
	buf8[0] = wk->play_mode;
	TowerPlayData_Put(wk->playSave,BTWR_PSD_playmode,buf8);
	
	//ラウンド数書き出し
	buf8[0] = wk->now_round;
	TowerPlayData_Put(wk->playSave,BTWR_PSD_round,buf8);

	//選んだポケモンNo
	TowerPlayData_Put(wk->playSave,BTWR_PSD_pokeno,wk->member);
	
	//バトル成績書き出し
	TowerPlayData_WifiRecordAdd(wk->playSave,wk->rec_down,wk->rec_turn,wk->rec_damage);

	//抽選されたトレーナーNo書き出し
	TowerPlayData_Put(wk->playSave,BTWR_PSD_trainer,wk->trainer);
	
	//プレイランダムシード保存
	TowerPlayData_Put(wk->playSave,BTWR_PSD_rnd_seed,&(wk->play_rnd_seed));
	OS_Printf("TowerRestRndSeed = %d\n",wk->play_rnd_seed);
	
	//セーブフラグを有効状態にリセット
	TowerPlayData_SetSaveFlag(wk->playSave,TRUE);

	if(wk->play_mode != BTWR_MODE_MULTI){
		return;
	}
	//AIマルチモードならパートナーを覚えておく
	buf8[0] = wk->partner;
	TowerPlayData_Put(wk->playSave,BTWR_PSD_partner,buf8);

	//パートナーのポケモンパラメータを憶えておく
	TowerPlayData_Put(wk->playSave,BTWR_PSD_pare_poke,&(wk->five_poke[wk->partner]));
	//アイテムが固定だったかどうか憶えておく
	TowerPlayData_Put(wk->playSave,BTWR_PSD_pare_itemfix,&(wk->five_item[wk->partner]));
	//プレイランダムシードを憶えておく
	
}

/**
 *	@brief	AIマルチペアポケモン抽選
 */
void TowerScr_ChoiceBtlSeven(BTOWER_SCRWORK* wk)
{
	int i;

	for(i = 0;i < BTWR_FIVE_NUM;i++){
		wk->five_item[i] = (u8)RomBattleTowerTrainerDataMake(wk,&(wk->five_data[i]),
			TOWER_FIVE_FIRST+i,wk->member_num,wk->mem_poke,wk->mem_item,&(wk->five_poke[i]),wk->heapID);
	}
}

/**
 *	@brief	対戦トレーナーOBJコード取得
 */
u16 TowerScr_GetEnemyObj(BTOWER_SCRWORK* wk,u16 idx)
{
	//トレーナータイプからOBJコードを取得してくる
	return BtlTower_TrType2ObjCode(wk->tr_data[idx].bt_trd.tr_type);
}

/**
 *	@brief	戦闘呼び出し
 */
void TowerScr_LocalBattleCall(GMEVENT_CONTROL* event,BTOWER_SCRWORK* wk,BOOL* win_flag)
{
	EventCmd_TowerLocalBattle(event,wk,win_flag);
}

//--------------------------------------------------------------
/**
 *	@brief	現在のプレイモードを返す
 */
//--------------------------------------------------------------
u16	TowerScr_GetPlayMode(BTOWER_SCRWORK* wk)
{
	return wk->play_mode;
}

/**
 *	@brief	リーダークリアフラグを取得する
 */
u16	TowerScr_GetLeaderClearFlag(BTOWER_SCRWORK* wk)
{
	return (u16)wk->leader_f;
}

/**
 *	@brief	タワークリア時にバトルポイントを加算する
 *
 *	@return	新たに取得したバトルポイント
 */
u16	TowerScr_AddBtlPoint(BTOWER_SCRWORK* wk)
{
	u16	stage;
	u16	point = 0;
	static const u8	bpoint_wifi[] = {0,
		BTLPOINT_VAL_TOWER_WIFI1,BTLPOINT_VAL_TOWER_WIFI2,BTLPOINT_VAL_TOWER_WIFI3,
		BTLPOINT_VAL_TOWER_WIFI4,BTLPOINT_VAL_TOWER_WIFI5,BTLPOINT_VAL_TOWER_WIFI6,
		BTLPOINT_VAL_TOWER_WIFI7,BTLPOINT_VAL_TOWER_WIFI8,BTLPOINT_VAL_TOWER_WIFI9,
		BTLPOINT_VAL_TOWER_WIFI10,
	};
	static const u8 bpoint_normal[] = {0,
		BTLPOINT_VAL_TOWER_STAGE1,BTLPOINT_VAL_TOWER_STAGE2,BTLPOINT_VAL_TOWER_STAGE3,
		BTLPOINT_VAL_TOWER_STAGE4,BTLPOINT_VAL_TOWER_STAGE5,BTLPOINT_VAL_TOWER_STAGE6,
		BTLPOINT_VAL_TOWER_STAGE7,
	};
	static const u8 bpoint_comm[] = {0,
		BTLPOINT_VAL_TOWER_COMM1,BTLPOINT_VAL_TOWER_COMM2,BTLPOINT_VAL_TOWER_COMM3,
		BTLPOINT_VAL_TOWER_COMM4,BTLPOINT_VAL_TOWER_COMM5,BTLPOINT_VAL_TOWER_COMM6,
		BTLPOINT_VAL_TOWER_COMM7,
	};

	if(wk->play_mode == BTWR_MODE_RETRY){
		return 0;
	}
	if(wk->play_mode == BTWR_MODE_WIFI){
		//ランクごと
		point = bpoint_wifi[TowerScoreData_SetWifiRank(wk->scoreSave,BTWR_DATA_get)];
	}else{

		//ワイアレスマルチ、WIFI
		//if( wk->play_mode == BTWR_MODE_COMM_MULTI ){
		if( (wk->play_mode == BTWR_MODE_COMM_MULTI) || (wk->play_mode == BTWR_MODE_WIFI_MULTI) ){

			//周回数ごと
			stage = TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_get);
			if(stage >= 7){								//この比較文はDPと合わせる
				point = BTLPOINT_VAL_TOWER_COMM8;
			}else{
				point = bpoint_comm[stage];
			}

#if 0
		//WIFI(32人データがあるので、周回数ワークが足りないため、連勝数から周回数を算出する
		}else if( wk->play_mode == BTWR_MODE_WIFI_MULTI ){

			OS_Printf( "wk->renshou = %d\n", wk->renshou );
			OS_Printf( "wk->now_win = %d\n", wk->now_win );
			stage = ( wk->renshou / 7 );
			if(stage >= 7){								//この比較文はDPと合わせる
				point = BTLPOINT_VAL_TOWER_COMM8;
			}else{
				point = bpoint_comm[stage];
			}
#endif

		//シングル、ダブル、AIマルチ
		}else{

			//周回数ごと
			stage = TowerScoreData_SetStage(wk->scoreSave,wk->play_mode,BTWR_DATA_get);
			if(wk->leader_f){
				point = BTLPOINT_VAL_TOWER_LEADER;
			}else if(stage >= 7){
				point = BTLPOINT_VAL_TOWER_STAGE8;
			}else{
				point = bpoint_normal[stage];
			}
		}
	}
	//バトルポイントを加算する
	TowerScoreData_SetBattlePoint(wk->scoreSave,point,BTWR_DATA_add);
	return point;
}

/**
 *	@brief	シングルで連勝した時のご褒美のトロフィーをあげるフラグを立てておく
 */
u16	TowerScr_GoodsFlagSet(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	u8	id,fid;
	u16	record,goods,ret;

	record = TowerScrTools_GetRenshouRecord(sv,wk->play_mode);

	if(record < BTWR_50_RENSHOU_CNT){
		return 0;
	}
	if(record >= BTWR_100_RENSHOU_CNT){
		if(TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_GOLD_GET,BTWR_DATA_get)){
			return 0;
		}
	}else{
		if(TowerScoreData_SetFlags(wk->scoreSave,BTWR_SFLAG_SILVER_GET,BTWR_DATA_get)){
			return 0;
		}
	}
	return 1;
}

//--------------------------------------------------------------
/**
 *	@brief	現在のWIFIランクを操作して返す
 */
//--------------------------------------------------------------
u16	TowerScr_SetWifiRank(BTOWER_SCRWORK* wk,SAVEDATA* sv,u8 mode)
{
	u8	ct,rank;
	BTLTOWER_SCOREWORK* score = SaveData_GetTowerScoreData(sv);
	
	static const u8	btower_wifi_rankdown[] = {
	 0,5,4,4,3,3,2,2,1,1,
//	 0,2,1,1,1,1,1,1,1,1,
	};

	switch(mode){
	//Get
	case 0:
		return (u16)TowerScoreData_SetWifiRank(score,BTWR_DATA_get);
	case 1:	//Inc
		//連続敗戦フラグを落とす
		TowerScoreData_SetFlags(score,BTWR_SFLAG_WIFI_LOSE_F,BTWR_DATA_reset);
		//現在のランクを取得
		rank = TowerScoreData_SetWifiRank(score,BTWR_DATA_get);

		if(rank == 10){	
			wk->prize_f = 1;				//ランク10なら、リボンを貰える条件はクリア(08.06.01)
			return 0;	//もう上がらない
		}
#if 0
		//rank10の時も、リボンをもらえるフラグを立てる必要がある
		//ランクアップ処理は呼んではいけない
		//戻り値は、ランクアップしたか、ランクアップしていないかを返している
		//ランクアップした時しか、リボンを渡す流れに行っていないのは正しいのか？＞てつじさん
		//
		//msg_tower_56
		//「そして●さんは　ランク●に　なりました！」
		//
		//続いて、リボンを渡す流れへ
		//リボンをすでに持っているとか、全員持っているとかは、コメントを見る限りチェックされている
#endif

		//ランクアップ処理
		TowerScoreData_SetWifiRank(score,BTWR_DATA_inc);

		//ランク5以上にアップしてたらリボンがもらえる
		if(rank+1 >= 5){
			wk->prize_f = 1;
		}
		return 1;
	case 2:	//dec
		//現在の連続敗戦数をカウント
		ct = TowerScoreData_SetWifiLoseCount(score,BTWR_DATA_inc);
		rank = TowerScoreData_SetWifiRank(score,BTWR_DATA_get);

		if(rank == 1){
			return 0;
		}
		//ランク別敗戦カウントチェック
		if(ct >= btower_wifi_rankdown[rank-1] ){
			//ランクダウン
			TowerScoreData_SetWifiRank(score,BTWR_DATA_dec);
			//連続敗戦数と連続敗戦フラグをリセット
			TowerScoreData_SetWifiLoseCount(score,BTWR_DATA_reset);
			TowerScoreData_SetFlags(score,BTWR_SFLAG_WIFI_LOSE_F,BTWR_DATA_reset);
			return 1;
		}
		return 0;
	}
	return 0;
}

/**
 *	@brief	リーダーを倒したご褒美リボンをあげる
 */
u16 TowerScr_LeaderRibbonSet(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	if(wk->play_mode != BTWR_MODE_SINGLE){
		return 0;
	}
	
	//メンバーにリボンセット
	switch(wk->leader_f){
	case 1:
		return towerscr_PokeRibbonSet(sv,ID_PARA_sinou_battle_tower_ttwin_first,wk);
	case 2:
		return towerscr_PokeRibbonSet(sv,ID_PARA_sinou_battle_tower_ttwin_second,wk);
	}
	return 0;
}

/**
 *	@brief	フラグが立っていたら連勝のご褒美リボンをあげる	
 */
u16 TowerScr_RenshouRibbonSet(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	u8	id,fid;
	u16	record,goods,ret;
	UNDERGROUNDDATA* gSave;

	if(wk->play_mode == BTWR_MODE_RETRY){
		return 0;
	}

	/////////////////////////////////////////
	//とりあえず仕様が決まっていないので保留
	/////////////////////////////////////////
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
		return 0;
	}

	if(!wk->prize_f){
		return 0;
	}
	switch(wk->play_mode){
	case BTWR_MODE_DOUBLE:
		id = ID_PARA_sinou_battle_tower_2vs2_win50;
		break;
	case BTWR_MODE_MULTI:
		id = ID_PARA_sinou_battle_tower_aimulti_win50;
		break;
	case BTWR_MODE_COMM_MULTI:
		id = ID_PARA_sinou_battle_tower_siomulti_win50;
		break;
	case BTWR_MODE_WIFI:
		id = ID_PARA_sinou_battle_tower_wifi_rank5;
		break;
	}
	//メンバーにリボンセット
	return towerscr_PokeRibbonSet(sv,id,wk);
}

//--------------------------------------------------------------
/**
 *	@biref	プレイランダムシードを更新する
 *
 *	d31r0201.ev
 *	case BTWR_SUB_UPDATE_RANDOM で呼ばれる
 *	ここは、WIFIマルチは通過しないので、このままでよい
 */
//--------------------------------------------------------------
u16 TowerScr_PlayRandUpdate(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	u8	chg_flg;

	//現在チャレンジ継続中かどうか？
	if(wk->play_mode == BTWR_MODE_WIFI_MULTI){
#if 0
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_WIFI_MULTI_RECORD,BTWR_DATA_get);
#else
		chg_flg = FrontierRecord_Get(SaveData_GetFrontier(sv), 
									FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_TOWER_MULTI_WIFI_CLEAR_BIT) );
#endif
	}else{
		chg_flg = TowerScoreData_SetFlags(wk->scoreSave,
				BTWR_SFLAG_SINGLE_RECORD+wk->play_mode,BTWR_DATA_get);
	}
	
	if(!chg_flg){
		//プレイ更新ランダムシード初期値取得&日付更新シード更新
		wk->play_rnd_seed = BtlTower_UpdatePlayRndSeed(sv);
	}else{
		//記録挑戦開始時のdayシードを基点に、現在のplayシードを取得する
		wk->play_rnd_seed = BtlTower_GetContinuePlayRndSeed(sv);
	}
	return (wk->play_rnd_seed/65535);
}

//////////////////////////////////////////////////////////////////////////////
//ローカル関数群
//
/**
 *	@brief	ポケモンにリボンをつける　サブ
 */
static u16 towerscr_PokeRibbonSet(SAVEDATA* sv,u8 ribbon,BTOWER_SCRWORK* wk)
{
	u8	flag = 1;
	u8 ct;
	int i;
	POKEPARTY	*party;
	POKEMON_PARAM* pp;
	
	party = SaveData_GetTemotiPokemon(sv);

	ct = 0;
	for(i = 0;i < wk->member_num;i++){
		pp = PokeParty_GetMemberPointer(party,wk->member[i]);
		if(PokeParaGet(pp,ribbon,NULL)){
			continue;	//既に持ってる
		}

		PokeParaPut(pp,ribbon,&flag);

		//TVトピック作成：リボンコレクター
		TVTOPIC_Entry_Record_Ribbon(sv, pp, ribbon);

		++ct;
	}
	if(ct == 0){
		//みんなもう持ってた
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------
/**
 *	@brief	連勝のご褒美リボンをもらえるかどうかのフラグを立てておく
 */
//--------------------------------------------------------------
static u16 towerscr_IfRenshouPrizeGet(BTOWER_SCRWORK* wk)
{
	u16 win;
	if(	wk->play_mode == BTWR_MODE_RETRY ||
		wk->play_mode == BTWR_MODE_SINGLE ||
		wk->play_mode == BTWR_MODE_WIFI_MULTI ||
		wk->play_mode == BTWR_MODE_WIFI){
		return 0;
	}
	win = wk->renshou+wk->now_win;

	//50連勝以上でもらえる
	if(win < BTWR_50_RENSHOU_CNT){
		return 0;
	}
	//prize getフラグを立てておく
	wk->prize_f = 1;
	return 1;
}

//--------------------------------------------------------------
/**
 *	@brief	参加したポケモンのパラメータをB_TOWER_POKEMON型にパックする
 */
//--------------------------------------------------------------
static void towerscr_PokeDataPack(B_TOWER_POKEMON* dat,POKEMON_PARAM* pp)
{
	int i;
	
	//mons_no/form_no/item_no
	dat->mons_no = PokeParaGet(pp,ID_PARA_monsno,NULL);
	dat->form_no = PokeParaGet(pp,ID_PARA_form_no,NULL);
	dat->item_no = PokeParaGet(pp,ID_PARA_item,NULL);

	//waza/pp_count
	for(i = 0;i < WAZA_TEMOTI_MAX;i++){
		dat->waza[i] = PokeParaGet(pp,ID_PARA_waza1+i,NULL);
		dat->pp_count |= ((PokeParaGet(pp,ID_PARA_pp_count1+i,NULL)) << (i*2));
	}
	//country,id,personal
	dat->country_code = PokeParaGet(pp,ID_PARA_country_code,NULL);
	dat->id_no = PokeParaGet(pp,ID_PARA_id_no,NULL);
	dat->personal_rnd = PokeParaGet(pp,ID_PARA_personal_rnd,NULL);

	//power_rnd
	dat->power_rnd = PokeParaGet(pp,ID_PARA_power_rnd,NULL);

	//exp
	for(i = 0;i < 6;i++){
		dat->exp[i] = PokeParaGet(pp,ID_PARA_hp_exp+i,NULL);
	}
	//tokusei,natukido
	dat->tokusei = PokeParaGet(pp,ID_PARA_speabino,NULL);
	dat->natuki = PokeParaGet(pp,ID_PARA_friend,NULL);
	
	//ニックネーム
	PokeParaGet(pp,ID_PARA_nickname,dat->nickname);
}

//--------------------------------------------------------------
/**
 *	@brief	参加したポケモンのパラメータを保存する
 */
//--------------------------------------------------------------
static void towerscr_SaveMemberPokeData(BTOWER_SCRWORK* wk,SAVEDATA* sv,BTWR_SCORE_POKE_DATA mode)
{
	int i = 0;
	B_TOWER_POKEMON* dat;
	POKEPARTY	*party;
	POKEMON_PARAM* pp;
	
	dat = sys_AllocMemoryLo(wk->heapID,sizeof(B_TOWER_POKEMON)*3);
	MI_CpuClear8(dat,sizeof(B_TOWER_POKEMON)*3);

	party = SaveData_GetTemotiPokemon(sv);
	for(i = 0;i < 3;i++){
		pp = PokeParty_GetMemberPointer(party,wk->member[i]);
		towerscr_PokeDataPack(&(dat[i]),pp);	
	}
	TowerScoreData_SetUsePokeData(wk->scoreSave,mode,dat);

	MI_CpuClear8(dat,sizeof(B_TOWER_POKEMON)*3);
	sys_FreeMemoryEz(dat);
}


//=============================================================================================
//
//	フロンティアとフィールドで共通で使用するものを移動
//
//=============================================================================================

//---------------------------------------------------------------------------------------------
/**
 * バトルタワートレーナーの持ちポケモンのパワー乱数を決定する
 *
 * @param	tr_no	トレーナーナンバー
 *
 * @return	パワー乱数
 *
 * b_tower_fld.c→b_tower_evに移動
 */
//---------------------------------------------------------------------------------------------
u8	BattleTowerPowRndGet(u16 tr_no)
{
	u8	pow_rnd;

	if(tr_no<100){
		pow_rnd=(0x1f/8)*1;
	}
	else if(tr_no<120){
		pow_rnd=(0x1f/8)*2;
	}
	else if(tr_no<140){
		pow_rnd=(0x1f/8)*3;
	}
	else if(tr_no<160){
		pow_rnd=(0x1f/8)*4;
	}
	else if(tr_no<180){
		pow_rnd=(0x1f/8)*5;
	}
	else if(tr_no<200){
		pow_rnd=(0x1f/8)*6;
	}
	else if(tr_no<220){
		pow_rnd=(0x1f/8)*7;
	}
	else{
		pow_rnd=0x1f;
	}
	return pow_rnd;
}

//---------------------------------------------------------------------------------------------
/**
 * @brief	タワープレイ更新ランダムシード取得 ラウンド更新時に呼ぶ
 *
 * b_tower_fld.c→b_tower_evに移動
 */
//---------------------------------------------------------------------------------------------
u16	btower_rand(BTOWER_SCRWORK* wk)
{
	//プラチナで追加されたWIFIマルチは通常のランダムを使用
	if( wk->play_mode == BTWR_MODE_WIFI_MULTI ){
		return ( gf_rand() );
	}

	wk->play_rnd_seed = BtlTower_PlayFixRand(wk->play_rnd_seed);

	OS_Printf("btower_rand = %d\n",wk->play_rnd_seed);
	return (wk->play_rnd_seed/65535);
}


