//============================================================================
/**
 *
 *@file		be_274.s
 *@brief	戦闘シーケンス（BattleEffect）
 *			274　こおりの追加効果＋怯ませる。間接追加効果確率をみて、それぞれチェックする。
 *
 *@author	HisashiSogabe
 *@data		2006.05.05
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

BE_274:
	VALUE			VAL_SET,BUF_PARA_ADD_STATUS_INDIRECT,ADD_COND2_KOORI_HIRUMU|ADD_STATUS_DEFENCE|ADD_STATUS_WAZAKOUKA
	CRITICAL_CHECK
	DAMAGE_CALC
	SEQ_END
