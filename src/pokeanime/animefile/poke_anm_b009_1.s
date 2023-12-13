//==============================================================================
/**
 * @file	poke_anm_b009_1.s
 * @brief	三角移動
 * @author	
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME_B009_1
	
// =============================================================================
//
//
//	背面	三角移動クイックｘ2
//
//
// =============================================================================
PAST_ANIME_B009_1:
	START_LOOP	2
		//3回の処理でx,yを+12動かす
		CALL_MF_LINE APPLY_SET,0,TARGET_DX,4,0,3
		CALL_MF_LINE APPLY_SET,0,TARGET_DY,4,0,3
		HOLD_CMD

		//スプライト位置をx=12,y=12に設定 dx,dyを0に設定
		SET_ADD_PARAM		PARAM_X,USE_VAL,12,PARAM_SET
		SET_ADD_PARAM		PARAM_Y,USE_VAL,12,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		SET_ADD_PARAM		PARAM_DY,USE_VAL,0,PARAM_SET
		APPLY_TRANS

		//3回の処理でxを-24動かす
		CALL_MF_LINE APPLY_SET,0,TARGET_DX,-8,0,3
		HOLD_CMD

		//スプライト位置をx=-12,y=12に設定 dx,dyを0に設定
		SET_ADD_PARAM		PARAM_X,USE_VAL,-12,PARAM_SET
		SET_ADD_PARAM		PARAM_Y,USE_VAL,12,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		SET_ADD_PARAM		PARAM_DY,USE_VAL,0,PARAM_SET
		APPLY_TRANS
		
		//3回の処理でxを+12,yを-12動かす
		CALL_MF_LINE APPLY_SET,0,TARGET_DX,4,0,3
		CALL_MF_LINE APPLY_SET,0,TARGET_DY,-4,0,3
		HOLD_CMD

		//スプライト位置をx=0,y=0に設定 dx,dyを0に設定
		SET_ADD_PARAM		PARAM_X,USE_VAL,0,PARAM_SET
		SET_ADD_PARAM		PARAM_Y,USE_VAL,0,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		SET_ADD_PARAM		PARAM_DY,USE_VAL,0,PARAM_SET
		APPLY_TRANS
	END_LOOP
	
	END_ANM
