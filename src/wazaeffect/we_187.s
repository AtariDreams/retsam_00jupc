//==============================================================================
/**
 * @file	we_187.s
 * @brief	はらだいこ			187
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_HARADAIKO
	
// =============================================================================
//
//
//	■はらだいこ			187
//
//
// =============================================================================
WEST_HARADAIKO:
	
	LOAD_PARTICLE_DROP	0,W_187_SPA
	ADD_PARTICLE 	0,W_187_187_HARADAI_HANDR, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_187_187_HARADAI_HANDL, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_187_187_HARADAI_ONPU,  EMTFUNC_ATTACK_POS
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 2, 0, 1, 8,  WE_TOOL_M1 | WE_TOOL_SSP,

	SE_REPEAT_L		SEQ_SE_DP_W187,8,4

	LOOP_LABEL		6
	
		WAIT			8
	
	LOOP

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
