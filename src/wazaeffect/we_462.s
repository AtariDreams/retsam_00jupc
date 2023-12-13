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

	LOAD_PARTICLE_DROP	0,W_462_SPA
	
	SE_FLOW_LR			SEQ_SE_DP_W462

	CONTEST_JP	WEST_CONTEST
	
	SIDE_JP		0, WEST_462_SIDE_MINE, WEST_462_SIDE_ENEMY
	
	SEQEND

WEST_CONTEST:


	ADD_PARTICLE		0, W_462_462_BIG_HAND2, EMTFUNC_ATTACK_POS
	FUNC_CALL			WEST_SP_EMIT_STRAIGHT, 6, 0, 0, 0, 0, 10, 64,
	
	WAIT				5
	WAIT				6
	SE_R				SEQ_SE_DP_W173C
	FUNC_CALL			WEST_SP_WE_SSP_POKE_SCALE_UPDOWN, 8, (WE_TOOL_E1 | WE_TOOL_SSP), 100, 100, 100, 70, 100, (20 << 16) | 1, (4 << 16) | 4
	WAIT				5
	
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_HIT_MARK,		EMTFUNC_DEFENCE_POS
	
	SE_WAITPLAY_R		SEQ_SE_DP_030,7
	SE_WAITPLAY_R		SEQ_SE_DP_030,8
	SE_WAITPLAY_R		SEQ_SE_DP_030,10
	WAIT_FLAG
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
	

WEST_462_SIDE_MINE:

	ADD_PARTICLE		0, W_462_462_BIG_HAND1, EMTFUNC_ATTACK_POS
	FUNC_CALL			WEST_SP_EMIT_STRAIGHT, 6, 0, 0, 0, 0, 10, 64,

	WAIT				5
	WAIT				6
	SE_R				SEQ_SE_DP_W173C
	FUNC_CALL			WEST_SP_WE_SSP_POKE_SCALE_UPDOWN, 8, (WE_TOOL_E1 | WE_TOOL_SSP), 100, 100, 100, 70, 100, (20 << 16) | 1, (4 << 16) | 4
	WAIT				5
		
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_HIT_MARK,		EMTFUNC_DEFENCE_POS
	
	SE_WAITPLAY_R		SEQ_SE_DP_030,7
	SE_WAITPLAY_R		SEQ_SE_DP_030,8
	SE_WAITPLAY_R		SEQ_SE_DP_030,10
	WAIT_FLAG
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND

WEST_462_SIDE_ENEMY:

	ADD_PARTICLE		0, W_462_462_BIG_HAND2, EMTFUNC_ATTACK_POS
	FUNC_CALL			WEST_SP_EMIT_STRAIGHT, 6, 0, 0, 0, 0, 10, 64,
	
	WAIT				5
	WAIT				6
	SE_R				SEQ_SE_DP_W173C
	FUNC_CALL			WEST_SP_WE_SSP_POKE_SCALE_UPDOWN, 8, (WE_TOOL_E1 | WE_TOOL_SSP), 100, 100, 100, 70, 100, (20 << 16) | 1, (4 << 16) | 4
	WAIT				5
	
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_STONE_BURN01,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE		0, W_462_462_HIT_MARK,		EMTFUNC_DEFENCE_POS
	
	SE_WAITPLAY_R		SEQ_SE_DP_030,7
	SE_WAITPLAY_R		SEQ_SE_DP_030,8
	SE_WAITPLAY_R		SEQ_SE_DP_030,10
	WAIT_FLAG
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
