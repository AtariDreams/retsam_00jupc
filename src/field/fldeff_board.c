//******************************************************************************
/**
 * 
 * @file	fldeff_board.c
 * @brief	tB[hOBJÅÂ
 * @author	kagaya
 * @data	05.07.13
 *
 */
//******************************************************************************
#if 0
#include "common.h"
#include "fieldsys.h"
#include "field_effect.h"
#include "fieldobj.h"

#include "fldeff_board.h"

//==============================================================================
//	define
//==============================================================================
//--------------------------------------------------------------
///	ÅÂ¯Ê
//--------------------------------------------------------------
enum
{
	BOARDNO_A = 0,											///<ÅÂA
	BOARDNO_B,												///<ÅÂB
	BOARDNO_C,												///<ÅÂC
	BOARDNO_D,												///<ÅÂD
	BOARDNO_E,												///<ÅÂE
	BOARDNO_F,												///<ÅÂF
	BOARDNO_MAX,											///<ÅÂÅå
};

//==============================================================================
//	typedef struct
//==============================================================================
//--------------------------------------------------------------
///	FE_BOARD^
//--------------------------------------------------------------
typedef struct _TAG_FE_BOARD * FE_BOARD_PTR;

//--------------------------------------------------------------
///	FE_BOARD\¢Ì
//--------------------------------------------------------------
typedef struct _TAG_FE_BOARD
{
	FE_SYS *fes;
	FRO_OBJ robj[BOARDNO_MAX];
	FRO_MDL rmdl[BOARDNO_MAX];
}FE_BOARD;

#define FE_BOARD_SIZE (sizeof(FE_BOARD)) 					///<FE_BOARDTCY

//--------------------------------------------------------------
///	BOARD_ADD_H\¢Ì
//--------------------------------------------------------------
typedef struct
{
	int board_no;
	FE_SYS *fes;
	FRO_OBJ *robj;
	FIELD_OBJ_PTR fldobj;									///<ÅÂÎÛÅ étB[hOBJ
}BOARD_ADD_H;

//--------------------------------------------------------------
///	EOA_BOARD_WORK\¢Ì
//--------------------------------------------------------------
typedef struct
{
	int seq_no;
	int obj_id;
	int zone_id;
	BOARD_ADD_H head;
}EOA_BOARD_WORK;

#define EOA_BOARD_WORK_SIZE (sizeof(EOA_BOARD_WORK))

//==============================================================================
//	vg^Cv
//==============================================================================
static void Board_GraphicInit( FE_BOARD_PTR bd );
static void Board_GraphicDelete( FE_BOARD_PTR bd );
static FRO_OBJ * Board_NoOBJGet( FE_BOARD_PTR bd, int type );
static int Board_OBJCodeBoardNo( int code );

static const u32 DATA_3DModelArcID_Board[BOARDNO_MAX];
static const EOA_H_NPP DATA_EoaH_Board;

//==============================================================================
//	ÅÂ@VXe
//==============================================================================
//--------------------------------------------------------------
/**
 * ÅÂú»
 * @param	fes		FE_SYS_PTR
 * @retval	FE_BOARD_PTR		FE_FOOTMARK_PTR
 */
//--------------------------------------------------------------
void * FE_Board_Init( FE_SYS *fes )
{
	FE_BOARD_PTR bd;
	
	bd = FE_AllocClearMemory( fes, FE_BOARD_SIZE, ALLOC_FR, 0 );
	bd->fes = fes;
	
	Board_GraphicInit( bd );
	return( bd );
}

//--------------------------------------------------------------
/**
 * ÅÂí
 * @param	bd		FE_BOARD_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
void FE_Board_Delete( void *work )
{
	FE_BOARD_PTR bd = work;
	Board_GraphicDelete( bd );
	FE_FreeMemory( bd );
}

//==============================================================================
//	ÅÂ@OtBbN
//==============================================================================
//--------------------------------------------------------------
/**
 * ÅÂ OtBbNú»
 * @param	bd		FE_BOARD_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void Board_GraphicInit( FE_BOARD_PTR bd )
{
	int i;
	for( i = 0; i < BOARDNO_MAX; i++ ){
		FE_FROMdl_ResSetArcLoadTexTrans(
			bd->fes, &bd->rmdl[i], 0, DATA_3DModelArcID_Board[i], ALLOC_FR );
		FRO_OBJ_InitInMdl( &bd->robj[i], &bd->rmdl[i] );
	}
}

//--------------------------------------------------------------
/**
 * ÅÂ@OtBbNú»
 * @param	bd		FE_BOARD_PTR
 * @retval	nothing
 */
//--------------------------------------------------------------
static void Board_GraphicDelete( FE_BOARD_PTR bd )
{
	int i;
	for( i = 0; i < BOARDNO_MAX; i++ ){
		FRO_MDL_DeleteAll( &bd->rmdl[i] );
	}
}

//--------------------------------------------------------------
/**
 * wè^CvÌ_æ¾
 * @param	bd		FE_BOARD_PTR
 * @param	no		BOARDNO_A
 * @retval	NNSG3dRenderObj wèÌ_
 */
//--------------------------------------------------------------
static FRO_OBJ * Board_NoOBJGet( FE_BOARD_PTR bd, int no )
{
	return( &bd->robj[no] );
}

//--------------------------------------------------------------
/**
 * tB[hOBJ OBJR[h©ç{[híÞ
 * @param	code	OBJ R[h
 * @retval	int		BOARDNO_A
 */
//--------------------------------------------------------------
static int Board_OBJCodeBoardNo( int code )
{
	switch( code ){
	case BOARD_A:	return( BOARDNO_A );
	case BOARD_B:	return( BOARDNO_B );
	case BOARD_C:	return( BOARDNO_C );
	case BOARD_D:	return( BOARDNO_D );
	case BOARD_E:	return( BOARDNO_E );
	case BOARD_F:	return( BOARDNO_F );
	}
	
	GF_ASSERT( 0 && "Board_OBJCodeBoardNo()³øOBJR[h" );
	return( 0 );
}

//==============================================================================
//	ÅÂ@EOA
//==============================================================================
//--------------------------------------------------------------
/**
 * tB[hOBJÅÂÇÁ
 * @param	fldobj	FIELD_OBJ_PTR
 * @retval	eoa		EOA_PTR
 */
//--------------------------------------------------------------
EOA_PTR FE_FldOBJBoard_Add( FIELD_OBJ_PTR fldobj )
{
	EOA_PTR eoa;
	FE_SYS *fes;
	FE_BOARD_PTR bd;
	BOARD_ADD_H head;
	
	fes = FE_FieldOBJ_FE_SYS_Get( fldobj );
	bd = FE_EffectWorkGet( fes, FE_FLD_BOARD );
	
	head.board_no = Board_OBJCodeBoardNo( FieldOBJ_OBJCodeGet(fldobj) );
	head.robj = Board_NoOBJGet( bd, head.board_no );
	head.fes = fes;
	head.fldobj = fldobj;
	
	{
		VecFx32 mtx = {0,0,0};
		int pri = FieldOBJ_TCBPriGet( fldobj, FLDOBJ_TCBPRI_OFFS_AFTER );
		eoa = FE_EoaAddNpp( fes, &DATA_EoaH_Board, &mtx, 0, &head, pri );
	}
	
	return( eoa );
}

//--------------------------------------------------------------
/**
 * EOA ÅÂ@ú»
 * @param	eoa	EOA_PTR
 * @param	wk	eoa work *
 * @retval	int TRUE=³íI¹BFALSE=ÙíI¹
 */
//--------------------------------------------------------------
static int EoaBoard_Init( EOA_PTR eoa, void *wk )
{
	EOA_BOARD_WORK *work;
	const BOARD_ADD_H *head;
	
	work = wk;
	head = EOA_AddPtrGet( eoa );
	work->head = *head;
	
	work->obj_id = FieldOBJ_OBJIDGet( work->head.fldobj );
	
	if( FieldOBJ_StatusBitCheck_Alies(work->head.fldobj) == TRUE ){
		work->zone_id = FieldOBJ_ZoneIDGetAlies( work->head.fldobj );
	}else{
		work->zone_id = FieldOBJ_ZoneIDGet( work->head.fldobj );
	}
	
	return( TRUE );
}

//--------------------------------------------------------------
/**
 * EOA ÅÂ@í
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaBoard_Delete( EOA_PTR eoa, void *wk )
{
}

//--------------------------------------------------------------
/**
 * EOA ÅÂ@®ì
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaBoard_Move( EOA_PTR eoa, void *wk )
{
	EOA_BOARD_WORK *work;
	FIELD_OBJ_PTR fldobj;
	
	work = wk;
	fldobj = work->head.fldobj;
	
	if( FieldOBJ_CheckSameID(fldobj,work->obj_id,work->zone_id) == FALSE ){
		FE_EoaDelete( eoa );
		return;
	}
	
	{
		VecFx32 mtx,d_mtx;
			
		FieldOBJ_VecPosGet( fldobj, &mtx );
		FieldOBJ_VecDrawOffsGet( fldobj, &d_mtx );
		
		mtx.x += d_mtx.x + FLDOBJ_BLACT_X_GROUND_OFFS_FX32;
		mtx.y += d_mtx.y + FLDOBJ_BLACT_Y_GROUND_OFFS_FX32;
		mtx.z += d_mtx.z + FLDOBJ_BLACT_Z_GROUND_OFFS_FX32 - (FX32_ONE * 6) - (FX32_ONE*2);
		
		EOA_MatrixSet( eoa, &mtx );
	}
}

//--------------------------------------------------------------
/**
 * EOA ÅÂ@`æ
 * @param	eoa		EOA_PTR
 * @param	wk		eoa work *
 * @retval	nothing
 */
//--------------------------------------------------------------
static void EoaBoard_Draw( EOA_PTR eoa, void *wk )
{
	VecFx32 mtx;
	EOA_BOARD_WORK *work = wk;
	EOA_MatrixGet( eoa, &mtx );
	FRO_OBJ_DrawPos( work->head.robj, &mtx );
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	ÅÂimde[u@ÀÑÍBOARD_WALK_UPÌlÉêv
//--------------------------------------------------------------
static const u32 DATA_3DModelArcID_Board[BOARDNO_MAX] =
{
	NARC_fldeff_board_a_nsbmd,
	NARC_fldeff_board_b_nsbmd,
	NARC_fldeff_board_c_nsbmd,
	NARC_fldeff_board_d_nsbmd,
	NARC_fldeff_board_e_nsbmd,
	NARC_fldeff_board_f_nsbmd,
};

//--------------------------------------------------------------
///	ÅÂEOA_H
//--------------------------------------------------------------
static const EOA_H_NPP DATA_EoaH_Board =
{
	EOA_BOARD_WORK_SIZE,
	EoaBoard_Init,
	EoaBoard_Delete,
	EoaBoard_Move,
	EoaBoard_Draw,
};
#endif
