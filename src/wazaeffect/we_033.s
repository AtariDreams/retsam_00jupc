//==============================================================================
/**
 * @file	we_033.s
 * @brief	たいあたり			33
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_TAIATARI
	
// =============================================================================
//
//
//	■たいあたり			33
//
//
// =============================================================================
#define WE_033_MOVE_X	(14)
#define WE_033_MOVE_Y	(8)
WEST_TAIATARI:
	
	LOAD_PARTICLE_DROP	0,W_033_SPA

	SE_R			SEQ_SE_DP_050
	
	FUNC_CALL		WEST_SP_WE_T10,	4, 2, +WE_033_MOVE_X, -WE_033_MOVE_Y, WE_TOOL_M1 | WE_TOOL_SSP
	WAIT_FLAG

	ADD_PARTICLE 	0,W_033_033_HIT_MARK,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_033_033_BALL_BURN,	EMTFUNC_DEFENCE_POS

	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 1, 0, 1, 2,  WE_TOOL_E1 | WE_TOOL_SSP,	
	FUNC_CALL		WEST_SP_WE_T10,	4, 2, -WE_033_MOVE_X, +WE_033_MOVE_Y, WE_TOOL_M1 | WE_TOOL_SSP
	
	WAIT_FLAG

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
