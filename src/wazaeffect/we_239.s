//==============================================================================
/**
 * @file	we_239.s
 * @brief	たつまき			239
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_TATUMAKI
	
// =============================================================================
//
//
//	■たつまき			239
//
//
// =============================================================================
WEST_TATUMAKI:
	
	LOAD_PARTICLE_DROP	0,W_239_SPA
	CAMERA_CHG		0,WE_CAMERA_TYPE_PERSPECTIV
	ADD_PARTICLE 	0,W_239_239_TORNADO_ROLL1, EMTFUNC_DF_SIDE
	ADD_PARTICLE 	0,W_239_239_TORNADO_ROLL2, EMTFUNC_DF_SIDE

	WAIT			10
	SE_R			SEQ_SE_DP_060
	WAIT			10
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 3, 0, 1, 15,  WE_TOOL_E1 | WE_TOOL_SSP
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, 3, 0, 1, 15,  WE_TOOL_E2 | WE_TOOL_SSP

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
