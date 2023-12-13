//==============================================================================
/**
 * @file	poke_anm_b022_1.s
 * @brief	膨らむ
 * @author	
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME_B022_1
	
// =============================================================================
//
//
//	背面	膨らむクイックｘ2
//
//
// =============================================================================
#define SIZE	(0x40)
#define CALC_NUM	(16)
PAST_ANIME_B022_1:
	START_LOOP		2
		CALL_MF_CURVE_DIVTIME	APPLY_ADD,0,CURVE_SIN,TARGET_RX,SIZE,0x8000,0,CALC_NUM
		CALL_MF_CURVE_DIVTIME	APPLY_ADD,0,CURVE_SIN,TARGET_RY,SIZE,0x8000,0,CALC_NUM
		HOLD_CMD
	END_LOOP
	
	END_ANM
