/**
 *	@file	nutmixer.c
 *	@brief	木の実まぜまぜ料理
 *	@author	Miyuki Iwasawa
 *	@date	06.04.06
 */

#include "common.h"
#include "system/procsys.h"
#include "system/snd_tool.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/palanm.h"
#include "system/savedata.h"
#include "itemtool/itemsym.h"
#include "itemtool/item.h"
#include "gflib/strbuf_family.h"

#include "application/nutmixer.h"
#include "nutmixer_sys.h"

#include "nmix_mseq.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_porutogame1.h"

#include "wifi/dwc_rap.h"
#include "wifi/vchat.h"

#include "communication/communication.h"
#include "communication/comm_state.h"

//プロトタイプ宣言　ローカル
//================================================================

//#define VCHATPAUSE_ON
#define VCHATPAUSE_WAIT		(3)
#define VCHATPAUSE_TCBPRI	(64)
static void VChatPauseStart( NUTMIXER_WORK* p_wk );
static void VChatPauseOffTcb( TCB_PTR tcb, void* pwork );


//------------------------------------------------------------------
/**
 * @brief	サブプロセス呼び出し処理
 * @param	proc	サブプロセスポインタを保持するワークへのポインタ
 */
//------------------------------------------------------------------
BOOL NutMixerProcCall(PROC ** proc)
{
	if (*proc) {
		if (ProcMain(*proc)) {
			PROC_Delete(*proc);
			*proc = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

/**
 *	@brief	ナッツミキサー　プロセス初期化
 */
PROC_RESULT	NutMixerProc_Init(PROC* proc,int *seq)
{
	NUTMIXER_WORK* wk = NULL;
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);
	
	HeapStatePush();

	//ヒープ作成
	sys_CreateHeap(HEAPID_BASE_APP,HEAPID_NUT_MIXER,0x20000);
	wk = PROC_AllocWork(proc,sizeof(NUTMIXER_WORK),HEAPID_NUT_MIXER);
	MI_CpuClear8(wk,sizeof(NUTMIXER_WORK));

	wk->heapID = HEAPID_NUT_MIXER;

	//引継ぎパラメータ
	wk->param = pp;

	if( (pp->wifi_pofin) && (pp->wifi_vchat) ){
		// ボイスチャット開始
		mydwc_startvchat( HEAPID_NUT_MIXER );
		OS_Printf( "vct on\n" );
	}

	// wifiかどうかを設定
	wk->wifi = pp->wifi_pofin;
	wk->wifi_vchat = pp->wifi_vchat;

	// バッグカーソル位置記憶領域初期化
	wk->bag_cursor = MyItem_BagCursorAlloc( wk->heapID );

	//動作モード取得
	if(pp->sys_mode == 1){
		wk->comm_f = TRUE;
	}

	return PROC_RES_FINISH;
}

/**
 *	@brief	ナッツミキサー　プロセスメイン
 */
PROC_RESULT	NutMixerProc_Main(PROC* proc,int *seq)
{
	int	old_seq;
	NUTMIXER_WORK* wk = PROC_GetWork(proc);
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);

	old_seq = *seq;
	switch(*seq){
	case NMSEQ_BAG_MAIN:
#ifdef VCHATPAUSE_ON
		VChatPauseStart( wk );	// vchat pause
#endif
		*seq =  NMixMainSeq_Bag(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_CALL:
#ifdef VCHATPAUSE_ON
		VChatPauseStart( wk );	// vchat pause
#endif
		*seq = NMixMainSeq_GameCall(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_WAIT:
		*seq = NMixMainSeq_GameWait(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_END:
		*seq = NMixMainSeq_GameEnd(wk,&(wk->m_seq));
		break;
	case NMSEQ_MAIN_END:
		return PROC_RES_FINISH;
	}

	if(old_seq != *seq){
		wk->m_seq = 0;
	}

	return PROC_RES_CONTINUE;
}

/**
 *	@brief	ナッツミキサー　プロセス終了
 */
PROC_RESULT NutMixerProc_End(PROC* proc,int *seq)
{
	int	heapID = 0;
	NUTMIXER_WORK* wk = PROC_GetWork(proc);
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);


	switch( *seq ){
	case 0:
		
		//ワークエリア解放
		heapID = wk->heapID;

		// バッグカーソル位置記憶領域破棄
		sys_FreeMemoryEz( wk->bag_cursor );

		MI_CpuClear8(wk,sizeof(NUTMIXER_WORK));
		PROC_FreeWork(proc);

		if( (pp->wifi_pofin) && (pp->wifi_vchat) ){
			// ボイスチャット終了
			mydwc_stopvchat();
		}
		
		HeapStatePop();
		HeapStateCheck(heapID);

		sys_DeleteHeap(heapID);

		// Wi-Fiでなければこのまま抜ける
		if( pp->wifi_pofin == FALSE ){
			return PROC_RES_FINISH;
		}

		// 以下Wi-Fiのときのみ
		CommStateSetErrorCheck(FALSE,TRUE);
		CommTimingSyncStart(SYNCHROID_PROC_END);

		(*seq) ++;
		break;

	case 1:
		if( (CommIsTimingSync(SYNCHROID_PROC_END)) ||
			(CommGetConnectNum() < CommInfoGetEntryNum()) ){	// 人数が少なくなったらそのまま抜ける
			return PROC_RES_FINISH;
		}
		break;

	default:
		GF_ASSERT(0);
		break;
	}
	return PROC_RES_CONTINUE;
}

//====================================================================
//ローカルサブ関数
//====================================================================


//----------------------------------------------------------------------------
/**
 *	@brief	Pause開始
 *
 *	@param	p_wk	ワーク
 */
//-----------------------------------------------------------------------------
static void VChatPauseStart( NUTMIXER_WORK* p_wk )
{
	if( (p_wk->wifi_vchat == TRUE) && (p_wk->wifi == TRUE) ){
		mydwc_pausevchat( TRUE );
		p_wk->vchat_wait = VCHATPAUSE_WAIT;
		TCB_Add( VChatPauseOffTcb, p_wk, VCHATPAUSE_TCBPRI );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	ボイスチャットPauseOFFTCB
 */
//-----------------------------------------------------------------------------
static void VChatPauseOffTcb( TCB_PTR tcb, void* pwork )
{
	NUTMIXER_WORK* wk = pwork;
	if( --wk->vchat_wait <= 0 ){
		mydwc_pausevchat( FALSE );
		TCB_Delete( tcb );
	}
}
