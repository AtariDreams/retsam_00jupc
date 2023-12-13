//============================================================================================
/**
 * @file	record_mix.c
 * @brief	レコードコーナー：混ぜる処理本体
 * @author	tamada GAME FREAK inc.
 * @date	2006.05.11
 */
//============================================================================================

#include "common.h"
#include "system/gamedata.h"

#include "system/pm_str.h"
#include "system/clact_tool.h"
#include "system/wordset.h"
#include "system/bmp_menu.h"

#include "savedata/randomgroup.h"
#include "savedata/imageclip_data.h"
#include "savedata/tv_work.h"
#include "savedata/guinness.h"
#include "savedata/b_tower.h"
#include "field/b_tower_wifi.h"

#include "system/procsys.h"
#include "application/record_corner.h"
#include "record_corner_local.h"

#include "communication/communication.h"
#include "communication/comm_state.h"


//============================================================================================
//
//
//			定義など
//
//
//============================================================================================
//------------------------------------------------------------------
/**
 * @brief	レコードまぜる関数用引数定義
 */
//------------------------------------------------------------------
typedef struct {
	int heapID;				///<利用可能なヒープ指定ID
	SAVEDATA * sv;			///<セーブデータへのポインタ
	int member;				///<最大通信人数
	int my_id;				///<自分の通信ID
	const void **darray;	///<受信データへのポインタの配列へのポインタ
	const void **ex_darray;	///<プラチナでテレビ拡張の為、追加
}RECORD_MIX_WORK;

//------------------------------------------------------------------
/**
 * @brief	送受信データサイズを取得するための関数型定義
 */
//------------------------------------------------------------------
typedef u32 (*GET_SIZE_FUNC)(SAVEDATA * sv);

//------------------------------------------------------------------
/**
 * @brief	送信データを生成するための関数型定義
 */
//------------------------------------------------------------------
typedef void * (*CREATE_DATA_FUNC)(SAVEDATA * sv, int heapID, u32 size);

//------------------------------------------------------------------
/**
 * @brief	受信データをまぜるための関数型定義
 */
//------------------------------------------------------------------
typedef void (*MIX_DATA_FUNC)(const RECORD_MIX_WORK *);

//------------------------------------------------------------------
/**
 * @brief	レコード処理関数テーブル型定義
 */
//------------------------------------------------------------------
typedef struct {
	GET_SIZE_FUNC get_size;		///<サイズ取得関数
	CREATE_DATA_FUNC get_data;	///<送信データ生成関数
	MIX_DATA_FUNC mixer_func;	///<受信データ処理関数
}RECORD_FUND_TABLE;


//============================================================================================
//
//
//			各レコードデータの処理関数
//
//
//============================================================================================
//------------------------------------------------------------------
///
//------------------------------------------------------------------
static u32 GetRandomGroupSize(SAVEDATA * sv)
{
	RANDOM_GROUP * rg = SaveData_GetRandomGroup(sv);
	return RandomGroup_GetDefaultDataSize(rg);
}
//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void * CreateRandomGroupData(SAVEDATA * sv, int heapID, u32 size)
{
	RANDOM_GROUP * rg = SaveData_GetRandomGroup(sv);
	void * buf = sys_AllocMemoryLo(heapID, size);
	MI_CpuCopyFast(RandomGroup_GetDefaultData(rg), buf, size);
	return buf;
}
//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void MixRandomGroup(const RECORD_MIX_WORK * mwk)
{
	RANDOM_GROUP * rg = SaveData_GetRandomGroup(mwk->sv);
	RandomGroup_RecordMix(mwk->member, mwk->my_id, rg, mwk->darray);
}

//------------------------------------------------------------------
///
//------------------------------------------------------------------
static u32 GetImcTVDataSize(SAVEDATA * sv)
{
	IMC_SAVEDATA * imc = SaveData_GetImcSaveData(sv);
	return ImcSaveData_GetTelevisionWorkSize();
}
//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void * AllocImcTVData(SAVEDATA * sv, int heapID, u32 size)
{
	IMC_TELEVISION_SAVEDATA * imcsv;
	IMC_SAVEDATA * imc = SaveData_GetImcSaveData(sv);
	void * buf = sys_AllocMemoryLo(heapID, size);
	imcsv = ImcSaveData_GetTelevisionSaveData( imc, IMC_SAVEDATA_TELEVISION_MYDATA );
	MI_CpuCopyFast(imcsv, buf, size);
	return buf;
}

//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void MixImcTVData(const RECORD_MIX_WORK * mwk)
{
	IMC_SAVEDATA * imc = SaveData_GetImcSaveData(mwk->sv);
	ImcSaveData_RecordMix(mwk->member, mwk->my_id, imc, mwk->darray);
}

//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void MixTVWatchData(const RECORD_MIX_WORK * mwk)
{
	TV_WORK * tvwk = SaveData_GetTvWork(mwk->sv);
	TVWORK_MixWatchData(tvwk, mwk->member, mwk->my_id, mwk->darray, mwk->ex_darray);
}

//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void MixTVRecordData(const RECORD_MIX_WORK * mwk)
{
	TV_WORK * tvwk = SaveData_GetTvWork(mwk->sv);
	TVWORK_MixRecordData(tvwk, mwk->member, mwk->my_id, mwk->darray, mwk->ex_darray);
}
//------------------------------------------------------------------
///
//------------------------------------------------------------------
static void MixTVQandAData(const RECORD_MIX_WORK * mwk)
{
	TV_WORK * tvwk = SaveData_GetTvWork(mwk->sv);
	TVWORK_MixQandAData(tvwk, mwk->member, mwk->my_id, mwk->darray, mwk->ex_darray);
}
//------------------------------------------------------------------
/// ギネス iwasawa
//------------------------------------------------------------------
static void * AllocGuinnessRecordData(SAVEDATA * sv, int heapID, u32 size)
{
	return GuinnessRecord_SendDataAlloc(sv,heapID);
}

static void MixGuinnessData(const RECORD_MIX_WORK * mwk)
{
	GUINNESS* gSave = SaveData_GetGuinnessData(mwk->sv);

	GuinnessRecord_RecvDataMix(mwk->sv,mwk->my_id,mwk->member,mwk->darray,mwk->heapID);
}
//------------------------------------------------------------------
/// トレーナーロード iwasawa
//------------------------------------------------------------------
static void * AllocTrainerRoadRecordData(SAVEDATA * sv, int heapID, u32 size)
{
	DPW_BT_PLAYER* dat;

	dat = sys_AllocMemoryLo(heapID,size);
	MI_CpuClear8(dat,size);
	//データ収集
	BtlTowerData_MakeUpdatePlayerData(sv,BTWR_SCORE_POKE_SINGLE,dat);
	return dat;
}

static void MixTrainerRoadData(const RECORD_MIX_WORK * mwk)
{
	/*
	 * 現状何もしない
	*/
}



//============================================================================================
//============================================================================================
//------------------------------------------------------------------
/**
 * @brief	レコードデータテーブル
 *
 * データが増える場合、ここに追加していく
 *
 * 2008.05.17(土) プラチナ以降向けにテレビデータ追加
 * 今までのテレビデータ送信領域にはDP向けとして、プラチナから追加されたトピックなどは
 * 除いたデータを作成して送信する。DPではこれをテレビデータとして受け取る。
 * プラチナ以降では新規に追加したテレビデータ領域を使用してテレビデータを送信し、
 * 受信時はここのデータを参照する。
 * 受信部分でROM内で放送出来ないデータは受け取りをはじいているので、金銀以降でも
 * ここを使用してテレビを送ってもらえれば放送出来ないテレビは受信側で勝手にはじく
 */
//------------------------------------------------------------------
static const RECORD_FUND_TABLE RecordFuncTable[] = {
	{	//ランダムグループ
		GetRandomGroupSize,
		CreateRandomGroupData,
		MixRandomGroup,
	},
	{	//イメージクリップ
		GetImcTVDataSize,
		AllocImcTVData,
		MixImcTVData,
	},
	{	//テレビデータ：監視型(DP向け)
		(GET_SIZE_FUNC)TVWORK_GetSendWatchDataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendWatchData,
		NULL,
	},
	{	//テレビデータ：ギネス型(DP向け)
		(GET_SIZE_FUNC)TVWORK_GetSendRecordDataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendRecordData,
		NULL,
	},
	{	//テレビデータ：質問型(DP向け)
		(GET_SIZE_FUNC)TVWORK_GetSendQandADataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendQandAData,
		NULL,
	},
	{	//ギネスデータ
		(GET_SIZE_FUNC)GuinnessRecord_GetWorkSize,
		(CREATE_DATA_FUNC)AllocGuinnessRecordData,
		MixGuinnessData,
	},
	{	//トレーナーロードデータ
		(GET_SIZE_FUNC)TowerDpwBtPlayer_GetWorkSize,
		(CREATE_DATA_FUNC)AllocTrainerRoadRecordData,
		MixTrainerRoadData,
	},
	
	//---- ここからプラチナで追加 ----
	
	{	//テレビデータ：監視型(プラチナ以降向け)
		(GET_SIZE_FUNC)TVWORK_GetSendWatchDataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendWatchDataEx,
		MixTVWatchData,
	},
	{	//テレビデータ：ギネス型(プラチナ以降向け)
		(GET_SIZE_FUNC)TVWORK_GetSendRecordDataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendRecordDataEx,
		MixTVRecordData,
	},
	{	//テレビデータ：質問型(プラチナ以降向け)
		(GET_SIZE_FUNC)TVWORK_GetSendQandADataSize,
		(CREATE_DATA_FUNC)TVWORK_AllocSendQandADataEx,
		MixTVQandAData,
	},
};

enum{
	RECORD_TBL_ID_RANDOM,		//ランダムグループ
	RECORD_TBL_ID_IMC,			//イメージクリップ
	RECORD_TBL_ID_TV_WATCH_DP,	//テレビデータ：監視型(DP向け)
	RECORD_TBL_ID_TV_RECORD_DP,	//テレビデータ：ギネス型(DP向け)
	RECORD_TBL_ID_TV_QAND_DP,	//テレビデータ：質問型(DP向け)
	RECORD_TBL_ID_GUINNESS,		//ギネスデータ
	RECORD_TBL_ID_TRAINER_ROAD,	//トレーナーロードデータ
	RECORD_TBL_ID_TV_WATCH_EX,	//テレビデータ：監視型(プラチナ以降向け)
	RECORD_TBL_ID_TV_RECORD_EX,	//テレビデータ：ギネス型(プラチナ以降向け)
	RECORD_TBL_ID_TV_QAND_EX,	//テレビデータ：質問型(プラチナ以降向け)
};

//============================================================================================
//============================================================================================
//------------------------------------------------------------------
/**
 * @brief	送信データを生成する
 * @param	sv				セーブデータへのポインタ
 * @param	send_data		送信データ構造体
 */
//------------------------------------------------------------------
void MakeSendData(SAVEDATA * sv, RECORD_DATA * send_data)
{
	u32 data_size;
	void * data_adrs;
	int i, max;
	u32 buf_size = RECORD_SEND_DATASIZE;
	u8 * buffer = send_data->data;
	const RECORD_FUND_TABLE * tbl = RecordFuncTable;
	max = NELEMS(RecordFuncTable);

	for (i = 0; i < max; i++) {
		data_size = tbl[i].get_size(sv);
		data_adrs = tbl[i].get_data(sv, HEAPID_RECORD, data_size);
		GF_ASSERT(buf_size > data_size);
		OS_TPrintf("src:%08x dst:%08x siz:%04x\n",data_adrs, buffer, data_size);
		MI_CpuCopyFast(data_adrs, buffer, data_size);
		sys_FreeMemoryEz(data_adrs);
		buffer += data_size;
		buf_size -= data_size;

	}
	
}

//------------------------------------------------------------------
/**
 * @brief	受信データを混ぜる
 * @param	sv				セーブデータへのポインタ
 * @param	record			受信データ構造体
 */
//------------------------------------------------------------------
void MixReceiveData(SAVEDATA * sv, const RECORD_DATA * record)
{
	RECORD_MIX_WORK mwk;
	u32 data_size;
	u32 top_ofs = 0;
	u32 buf_size = RECORD_SEND_DATASIZE;
	int i, j, s;
	const void * adrs_array[RECORD_CORNER_MEMBER_MAX];
	const void * ex_array[RECORD_CORNER_MEMBER_MAX];
	u32 ex_offs[RECORD_TBL_ID_TV_QAND_DP - RECORD_TBL_ID_TV_WATCH_DP + 1];
	
	mwk.heapID = HEAPID_RECORD;
	mwk.sv = sv;
	mwk.my_id = CommGetCurrentID();
	mwk.member = RECORD_CORNER_MEMBER_MAX;
	mwk.darray = adrs_array;
	mwk.ex_darray = ex_array;
	
	ex_offs[RECORD_TBL_ID_TV_WATCH_DP - RECORD_TBL_ID_TV_WATCH_DP] = 0;
	ex_offs[RECORD_TBL_ID_TV_RECORD_DP - RECORD_TBL_ID_TV_WATCH_DP] = 0;
	ex_offs[RECORD_TBL_ID_TV_QAND_DP - RECORD_TBL_ID_TV_WATCH_DP] = 0;
	//プラチナ以降ではプラチナから追加されたテレビ領域の方でDPのテレビもまとめて混ぜる為、
	//参照先アドレスだけを先に取得しておく
	for(i = 0; i < RECORD_TBL_ID_TV_WATCH_DP; i++){
		ex_offs[RECORD_TBL_ID_TV_WATCH_DP - RECORD_TBL_ID_TV_WATCH_DP] += RecordFuncTable[i].get_size(sv);
	}
	for(i = 0; i < RECORD_TBL_ID_TV_RECORD_DP; i++){
		ex_offs[RECORD_TBL_ID_TV_RECORD_DP - RECORD_TBL_ID_TV_WATCH_DP] += RecordFuncTable[i].get_size(sv);
	}
	for(i = 0; i < RECORD_TBL_ID_TV_QAND_DP; i++){
		ex_offs[RECORD_TBL_ID_TV_QAND_DP - RECORD_TBL_ID_TV_WATCH_DP] += RecordFuncTable[i].get_size(sv);
	}


	for (i = 0; i < NELEMS(RecordFuncTable); i++) {
		data_size = RecordFuncTable[i].get_size(sv);
		GF_ASSERT(buf_size > data_size);
		for (j = 0; j < RECORD_CORNER_MEMBER_MAX; j++) {
			if (CommInfoGetMyStatus(j) != NULL) {
				mwk.darray[j] = &(record[j].data[top_ofs]);
			} else {
				mwk.darray[j] = NULL;
			}
		}
		
		switch(i){
		case RECORD_TBL_ID_TV_WATCH_EX:
		case RECORD_TBL_ID_TV_RECORD_EX:
		case RECORD_TBL_ID_TV_QAND_EX:
			for (s = 0; s < RECORD_CORNER_MEMBER_MAX; s++) {
				if (CommInfoGetMyStatus(s) != NULL) {
					mwk.ex_darray[s] = &(record[s].data[ex_offs[i - RECORD_TBL_ID_TV_WATCH_EX]]);
				} else {
					mwk.ex_darray[s] = NULL;
				}
			}
			break;
		}
		
		if(RecordFuncTable[i].mixer_func != NULL){
			RecordFuncTable[i].mixer_func(&mwk);
		}
		
		top_ofs += data_size;
		buf_size -= data_size;
	}


	//以下はレコード交換以外の特殊処理
	
	//テレビ番組刷新の処理
	{
		TV_WORK * tvwk = SaveData_GetTvWork(sv);
		TVWORK_ClearMyTopic(tvwk);	//自分データを削除
		TVWORK_StartProgram(tvwk);	//放送データのリセット
	}
}






