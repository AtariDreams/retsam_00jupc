//==============================================================================
/**
 * @file	we_201.s
 * @brief	すなあらし			201
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_SUNAARASI
	
// =============================================================================
//
//
//	■すなあらし			201
//
//
// =============================================================================
WEST_SUNAARASI:
	
	LOAD_PARTICLE_DROP	0,W_201_SPA
	ADD_PARTICLE 	0,W_201_201_SUNAARASI_KONA, EMTFUNC_DUMMY
	ADD_PARTICLE 	0,W_201_201_SUNAARASI_MIST, EMTFUNC_DUMMY

	SE_C			SEQ_SE_DP_W201

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
