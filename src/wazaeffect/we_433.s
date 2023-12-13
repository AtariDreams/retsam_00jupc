//==============================================================================
/**
 * @file	we_001.s
 * @brief	はたく
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_Hataku
	
// =============================================================================
//
//
//	■はたく
//
//
// =============================================================================
WEST_Hataku:
	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_R,  0
	WORK_SET		WEDEF_GP_INDEX_SCREEN_R, 0
	WORK_SET		WEDEF_GP_INDEX_FADE_VALUE, 1
	
	HAIKEI_CHG		BG_ID_433, WEDEF_HAIKEI_MODE_BLEND | WEDEF_HAIKEI_SBIT_NONE
	SE_WAITPLAY_C	SEQ_SE_DP_061,2
	HAIKEI_CHG_WAIT

	WAIT		60

	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_R,  0
	WORK_SET		WEDEF_GP_INDEX_SCREEN_R, 0
	WORK_SET		WEDEF_GP_INDEX_FADE_VALUE, 2
	HAIKEI_RECOVER	BG_ID_433, WEDEF_HAIKEI_RC_MODE_BLEND | WEDEF_HAIKEI_SBIT_NONE
	HAIKEI_CHG_WAIT
	SEQEND

