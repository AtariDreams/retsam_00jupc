//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		laster.h
 *	@brief		ラスターシステム
 *	@author		tomoya takahashi
 *	@data		2005.12.01
 *
 * LABEL	LABEL_LASTER_SYS		ラスターシステム
 * LABEL	LABEL_LASTER_RAP_FUNC	ラスターシステムを使用した汎用関数郡
 * 
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __LASTER_H__
#define __LASTER_H__

#undef GLOBAL
#ifdef	__LASTER_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *
 * ラスターシステム説明		[ LABEL_LASTER_SYS ]
 *	ラスターシステム構造
 *		書き込み用バッファ、参照用バッファの２つを初期化関数で設定します。
 *		１つが書き込み用バッファの時、もう一方は参照用バッファになります。
 *
 *		書き込みが終わったら、書き込みバッファを参照用に参照用を書き込み用に
 *		スイッチします。（ダブルバッファ処理）
 *
 *		初期化関数で設定した２つのバッファはどちらが、書き込み用か参照用か
 *		使用するユーザーからは、わかりません。
 *		バッファを使用するときは必ず＊関数を使用して配列の先頭ポインタを
 *		取得してください。
 *
 *		＊取得関数
 *			void* LASTER_GetWriteBuff( CONST_LASTER_SYS_PTR cp_laster );
 *			const void* LASTER_GetReadBuff( CONST_LASTER_SYS_PTR cp_laster );
 *			
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					定数宣言
*/
//-----------------------------------------------------------------------------

//-------------------------------------
//	
//	スイッチフラグ
//	
//=====================================
enum{
	LASTER_SWITCH_OFF,	// バッファスイッチ無効
	LASTER_SWITCH_ON,	// バッファスイッチ有効
};

//-----------------------------------------------------------------------------
/**
 *					構造体宣言
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	
//	ラスターシステム構造体
//	
//=====================================
typedef struct _LASTER_SYS * LASTER_SYS_PTR;
typedef const struct _LASTER_SYS * CONST_LASTER_SYS_PTR;


//-----------------------------------------------------------------------------
/**
 *					プロトタイプ宣言
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスターシステム初期化
 *
 *	@param	heap	使用するヒープ
 *	@param	p_buff1	バッファ１
 *	@param	p_buff2	バッファ２
 *
 *	@return	作成したラスターシステム
 *
 */
//-----------------------------------------------------------------------------
GLOBAL LASTER_SYS_PTR LASTER_Init( int heap, void* p_buff1, void* p_buff2 );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスターシステム破棄
 *
 *	@param	lster	破棄するラスターシステム
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_Delete( LASTER_SYS_PTR p_laster );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	書き込み用ラスターバッファを取得
 *
 *	@param	cp_laster	ラスタースステム
 *
 *	@return	書き込み用ラスターバッファ
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void* LASTER_GetWriteBuff( CONST_LASTER_SYS_PTR cp_laster );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	参照用ラスターバッファを取得
 *
 *	@param	cp_laster	ラスターシステム
 *
 *	@return	参照用ラスターバッファ
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL const void* LASTER_GetReadBuff( CONST_LASTER_SYS_PTR cp_laster );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	バッファスイッチフラグを設定します。
 *
 *	@param	p_laster	ラスターシステム
 *	@param	switch_flag	スイッチフラグ
 *
 *	@return	none
 *
 * switch_flag	
 *	LASTER_SWITCH_ON,	// バッファスイッチ有効
 *	LASTER_SWITCH_OFF	// バッファスイッチ無効
 * 
 *	LASTERE_Init関数を呼んだ状態でバッファスイッチは有効になっています。
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_SwitchFlagSet( LASTER_SYS_PTR p_laster, u8 switch_flag );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスター　Vブランク関数
 *
 *	@param	p_laster	ラスターシステム
 *
 *	@return	none
 *
 * バッファスイッチフラグが有効のとき
 *		書き込み用バッファを参照用に
 *		参照用バッファを書き込み用に変更します。
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_VBlank( LASTER_SYS_PTR p_laster );



//-----------------------------------------------------------------------------
/**
 * 
 *		ラスターシステムを使用した汎用関数郡	[ LABEL_LASTER_RAP_FUNC ]
 *		
 */
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *
 *		ラスターHブランクDMA転送関数
 * 
 */
//-----------------------------------------------------------------------------
// 通信時やグラフィックデータ転送時に
// おかしなことが起こらないかの実験が必要ですが、
// 今のところは0にしておきます。
#define LASTER_USE_DMA	(0)		// 使用するDMAナンバー
enum{
	LASTER_DMA_TYPE16,
	LASTER_DMA_TYPE32
};

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスターで使用するDMAを停止する
 *
 *	@param	none
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_DmaStop( void );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	HブランクDMA転送パラメータ設定
 *
 *	@param	p_src		転送元
 *	@param	p_dest		転送先
 *	@param	szbyte		転送サイズ	（１回の転送で転送するサイズ）
 *	@param	dma_type
 *
 *	@return	none
 *
 *	LASTER_DMA_TYPE16,	16bit単位
 *	LASTER_DMA_TYPE32	32bit単位
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_DmaSet( const void* p_src, void* p_dest, int szbyte, int dma_type );


//-----------------------------------------------------------------------------
/**
 *		ラスタースクロール
 *			DMA転送使用
*/
//-----------------------------------------------------------------------------
#define LASTER_SCROLL_BUFF_SIZE	(192)	// バッファサイズ

// BG面定数
// スクロールレジスタ　ｘ座標アドレス
enum{
	LASTER_SCROLL_MBG0 = REG_BG0HOFS_ADDR,
	LASTER_SCROLL_MBG1 = REG_BG1HOFS_ADDR,
	LASTER_SCROLL_MBG2 = REG_BG2HOFS_ADDR,
	LASTER_SCROLL_MBG3 = REG_BG3HOFS_ADDR,
	LASTER_SCROLL_SBG0 = REG_DB_BG0HOFS_ADDR,
	LASTER_SCROLL_SBG1 = REG_DB_BG1HOFS_ADDR,
	LASTER_SCROLL_SBG2 = REG_DB_BG2HOFS_ADDR,
	LASTER_SCROLL_SBG3 = REG_DB_BG3HOFS_ADDR
};
typedef struct _LASTER_SCROLL* LASTER_SCROLL_PTR;
typedef const struct _LASTER_SCROLL* CONST_LASTER_SCROLL_PTR;

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスタースクロールオブジェ生成
 *
 *	@param	heap	ヒープ
 *
 *	@return	作成したラスタースクロールオブジェ
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL LASTER_SCROLL_PTR LASTER_ScrollInit( int heap );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスタースクロール開始
 *
 *	@param	start		ラスタースクロールさせる開始ｙ座標
 *	@param	end			ラスタースクロールさせる終了ｙ座標
 *	@param	add_r		sinカーブテーブル作成時の角度を足していく値(0〜65535)
 *	@param	r_w			sinカーブの半径
 *	@param	scr_sp		ラスタースクロールスピード	整数*100した値	マイナス有効
 *	@param	bg_no		ラスターをかけるBGの番号
 *	@param	init_num	ワークを初期化する値
 *	@param	tsk_pri		タスク優先順位
 *
 *	@return	none
 *
 * start <= スクロール範囲 <= end
 * 
 * bg_no
 * 		LASTER_SCROLL_MBG0,		// メイン画面
 *		LASTER_SCROLL_MBG1,
 *		LASTER_SCROLL_MBG2,
 *		LASTER_SCROLL_MBG3,
 *		LASTER_SCROLL_SBG0,		// サブ画面
 *		LASTER_SCROLL_SBG1,
 *		LASTER_SCROLL_SBG2,
 *		LASTER_SCROLL_SBG3
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_ScrollStart( LASTER_SCROLL_PTR p_lstscr, u8 start, u8 end, u16 add_r, fx32 r_w, s16 scr_sp, u32 bg_no, u32 init_num, u32 tsk_pri );
GLOBAL void LASTER_YScrollStart( LASTER_SCROLL_PTR p_lstscr, u8 start, u8 end, u16 add_r, fx32 r_w, s16 scr_sp, u32 bg_no, u32 init_num, u32 tsk_pri );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスタースクロール終了
 *
 *	@param	p_lstscr	終了させるラスタースクロールワーク
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_ScrollEnd( LASTER_SCROLL_PTR p_lstscr );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスタースクロール破棄
 *
 *	@param	p_lstscr	ラスタースクロールワーク
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_ScrollDelete( LASTER_SCROLL_PTR p_lstscr );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	バッファ取得
 *
 *	@param	p_lstscr	ラスタースクロールワーク
 *
 *	@return	バッファポインタ
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void* LASTER_GetScrollWriteBuff( LASTER_SCROLL_PTR p_lstscr );
GLOBAL const void* LASTER_GetScrollReadBuff( LASTER_SCROLL_PTR p_lstscr );

//----------------------------------------------------------------------------
/**
 *
 *	@brief	ラスタースクロールVブランク関数		
 *
 *	@param	p_lstscr	ラスタースクロールワーク
 *
 *	@return	none
 *
 * バッファスイッチ
 *		この処理は30フレームで動いているときは2回目の
 *		VBlank期間で呼んでください。
 *
 *	HblankDma転送設定
 *		この処理はVBlank期間が来るたびに呼んでください。
 *
 */
//-----------------------------------------------------------------------------
// バッファスイッチ
GLOBAL void LASTER_ScrollVBuffSwitch( LASTER_SCROLL_PTR p_lstscr );

// HblankDma転送設定
GLOBAL void LASTER_ScrollVDma( LASTER_SCROLL_PTR p_lstscr );

//-----------------------------------------------------------------------------
/**
 *	そのた
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *	@brief	サインカーブテーブルを作成する
 *
 *	@param	sinTbl	格納先配列
 *	@param	num		配列要素数	s16 buff[ 10 ]; なら10を指定
 *	@param	add_r	１回に回転角を変化させる値
 *	@param	r_w		半径
 *
 *	@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void LASTER_ScrollMakeSinTbl( s16* sinTbl, u32 num, u16 add_r, fx32 r_w );


#undef	GLOBAL
#endif		// __LASTER_H__

