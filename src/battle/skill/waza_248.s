
//==============================================================================
/**
 * @file	waza_248.s
 * @brief	みらいよち
 * @author	soga
 * @date	2005.10.06
 */
//==============================================================================

	.text

	.include	"waza_seq_def.h"

WAZA_248:
	MESSAGE_PARAM	MiraiyochiMineMsg,TAG_NICK,SIDE_ATTACK
	JUMP_BE_SEQ
