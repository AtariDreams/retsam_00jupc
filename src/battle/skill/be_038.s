//============================================================================
/**
 *
 *@file		be_038.s
 *@brief	戦闘シーケンス（BattleEffect）
 *			038　一撃で敵を戦闘不能にする（敵のポケモンのレベルが自分より高いと当たらない。低いほど命中率が高くなる）
 *
 *@author	HisashiSogabe
 *@data		2005.10.03
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

BE_038:
	ICHIGEKI
	SEQ_END
