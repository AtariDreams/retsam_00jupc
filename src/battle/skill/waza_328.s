
//==============================================================================
/**
 * @file	waza_328.s
 * @brief	すなじごく
 * @author	soga
 * @date	2005.10.06
 */
//==============================================================================

	.text

	.include	"waza_seq_def.h"

WAZA_328:
	MESSAGE_PARAM	SunazigokuMineMsg,TAG_NICK,SIDE_DEFENCE
	JUMP_BE_SEQ
