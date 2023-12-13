//=============================================================================
/**
 * @file	dendou_ball_anm.c
 * @bfief	殿堂マシン上ボールアニメ
 * @author	Nozomu Saito
 *
 */
//=============================================================================

#define BALL_ANM_CONT_CODE	(0x10)		//ボールアニメ識別コード 値は適当
#define BALL_PUT_WAIT	(15)			//ボールをおいた後のウェイト

#include "common.h"
#include "fieldsys.h"
#include "field_3d_anime.h"

#include "fielddata/build_model/build_model_id.h"
#include "system/snd_tool.h"

#include "dendou_ball_anm.h"

enum{
	SEQ_START,
	SEQ_BALL_PUT,
	SEQ_PUT_WAIT,
	SEQ_ANM_START,
	SEQ_ANM_WAIT,
	SEQ_END
};

typedef struct DENDOU_BALL_ANM_tag
{
	VecFx32 MachinePos;	//回復マシーン座標
	u8 MyPokeNum;		//手持ちポケモン数	
	u8 NowPos;			//現在配置中ボール数
	u8 Wait;			//ボールを置く間隔
	u8 Seq;				//シーケンサ

	u8 EntryIdx[6];		//ボール最大6個
}DENDOU_BALL_ANM;

static BOOL DendouBallAnimeEvent(GMEVENT_CONTROL * event);

#define BALL_OFS	( (FX32_ONE*4)+(FX32_ONE/2) )		//4.5

//■回復マシンを基準としたボール配置座標（XYZ）
static VecFx32 BallPos[6] = {
	{-BALL_OFS,FX32_ONE*12,-BALL_OFS},		//-4.5,12,-4.5
	{ BALL_OFS,FX32_ONE*12,-BALL_OFS},		//4.5,12,-4.5
	{-BALL_OFS,FX32_ONE*12,0},				//-4.5,12,0
	{ BALL_OFS,FX32_ONE*12,0},				//4.5,12,0
	{-BALL_OFS,FX32_ONE*12,BALL_OFS},		//-4.5,12,4.5
	{ BALL_OFS,FX32_ONE*12,BALL_OFS},		//4.5,12,4.5
};

//==============================================================================
/**
 * 殿堂マシンアニメーションセット
 * 
 * @param	fsys			フィールドシステムポインタ
 * @param	inBallNum		ボール個数
 * 
 * @return	none
 */
//==============================================================================
void DendouBallAnm_StartAnm(FIELDSYS_WORK * fsys, const u8 inBallNum)
{
	BOOL rc;
	M3DO_PTR obj_ptr;
	int block_idx;
	int obj_id = BMID_LEAGE_O03;

	rc = MPTL_CheckMap3DObj(fsys, obj_id, &obj_ptr,&block_idx);
			
	if (rc){
		//殿堂マシンあった
		//データ作成
		DENDOU_BALL_ANM *work = sys_AllocMemoryLo(HEAPID_FIELD,sizeof(DENDOU_BALL_ANM));
		work->MyPokeNum = inBallNum;
		work->NowPos = 0;
		work->Wait = 0;
		work->Seq = 0;
		{
			VecFx32 trans;
			MPTL_CalcMapOffset(block_idx, GetWorldMapMatrixW(fsys->World), &trans);
			work->MachinePos = M3DO_GetGlobalVec(obj_ptr);
			work->MachinePos.x += trans.x;
			work->MachinePos.z += trans.z;
		}
		//アニメイベントコール
		FieldEvent_Call(fsys->event, DendouBallAnimeEvent, work);
	}else{
		GF_ASSERT(0&&"殿堂マシンありません");
	}
}

//==============================================================================
/**
 * 殿堂マシン上ボールアニメイベント
 * 
 * @param	event	イベントポインタ
 * 
 * @return	BOOL	TRUE:イベント終了　FALSE:イベント継続
 */
//==============================================================================
static BOOL DendouBallAnimeEvent(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	DENDOU_BALL_ANM *work = (DENDOU_BALL_ANM *)FieldEvent_GetSpecialWork(event);
	switch(work->Seq){
	case SEQ_START:
		{
			NNSG3dResMdl* ball_model;
			NNSG3dResFileHeader** ball_res_file;
			NNSG3dRenderObj *render_obj;
			
			ball_res_file = GetMapResourceModelRes(BMID_PC_MB, fsys->MapResource);
			ball_model = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*ball_res_file),0);
			//ボールアニメーションセットアップ
			F3DASub_SetUpAnimation( fsys->field_3d_anime,
									fsys->AnimeContMng,
									BALL_ANM_CONT_CODE,	//ボールアニメ識別コード
									BMID_PC_MB,
									NULL,			//後で指定
									ball_model,
									GetMapResourceBmTexturePTR(fsys->MapResource),
									1,				//アニメ数1
									1,		//ループ1回
									FALSE			//反転しない
									);
		}
		(work->Seq)++;
		break;
	case SEQ_BALL_PUT:
		{
			M3DO_PTR obj_ptr;
			NNSG3dRenderObj *render_obj;
			VecFx32 pos;
			VecFx32 rot = {0,0,0 };
			
			pos.x = work->MachinePos.x + BallPos[work->NowPos].x;
			pos.y = work->MachinePos.y + BallPos[work->NowPos].y;
			pos.z = work->MachinePos.z + BallPos[work->NowPos].z;
			//※ボールを置く音は鳴らしません（仕様）
			
			//モンスターボールを置く
			work->EntryIdx[work->NowPos] = 
				M3DO_SetMap3DObjExp(fsys->Map3DObjExp,
									fsys->MapResource,
									BMID_PC_MB,
									&pos,
									&rot,
									fsys->field_3d_anime);
			//ＯＢＪポインタを取り直して、レンダーＯＢＪをセット
			obj_ptr = M3DO_GetMap3DObjDataFromIndex(fsys->Map3DObjExp, work->EntryIdx[work->NowPos]);
			render_obj = M3DO_GetRenderObj(obj_ptr);
			F3DASub_EntryRenderObj(fsys->AnimeContMng, BALL_ANM_CONT_CODE, work->NowPos, render_obj);
			(work->Seq)++;
		}
		break;
	case SEQ_PUT_WAIT:
		if(work->Wait < BALL_PUT_WAIT){
			work->Wait++;
			break;
		}else{
			work->Wait = 0;;
		}
		//置いたボールの数が手持ち分なら次のシーケンスへ
		work->NowPos++;
		if (work->NowPos < work->MyPokeNum){
			(work->Seq) = SEQ_BALL_PUT;
		}else{
			(work->Seq)++;
		}
		break;
	case SEQ_ANM_START:
		//モンスターボールアニメーションスタート
		F3DASub_StartAnimation(fsys->AnimeContMng, BALL_ANM_CONT_CODE, 0);

		(work->Seq)++;
		break;
	case SEQ_ANM_WAIT:
		//アニメ・ジングル待ち
		if ( (F3DASub_WaitAnimation(fsys->AnimeContMng, BALL_ANM_CONT_CODE)) ){
			u8 i;
			//アニメ解放
			F3DASub_RereaseAnimation(fsys->field_3d_anime, fsys->AnimeContMng, BALL_ANM_CONT_CODE);
			//追加したボールＯＢＪを解放する
			for(i=0;i<work->MyPokeNum;i++){
				M3DO_CleanMap3DObj(work->EntryIdx[i], fsys->Map3DObjExp);
			}
			(work->Seq)++;
		}
		break;
	case SEQ_END:
		//終り
		sys_FreeMemoryEz(work);
		return TRUE;
	}

	return FALSE;
}
