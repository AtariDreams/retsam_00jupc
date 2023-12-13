//==============================================================================
/**
 * @file	we_182.s
 * @brief	まもる			182
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_MAMORU
	
// =============================================================================
//
//
//	■まもる			182
//
//
// =============================================================================
WEST_MAMORU:
	
	LOAD_PARTICLE_DROP	0,W_182_SPA

	SE_L			SEQ_SE_DP_W115
	
	ADD_PARTICLE 	0,W_182_182_MAMORU_BALL, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_AT, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
