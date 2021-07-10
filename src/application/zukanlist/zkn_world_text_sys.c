//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		zkn_world_text_sys.c
 *	@brief		�}�ӂ̊O����e�L�X�g�A�N�Z�X
 *	@author		tomoya takahashi 
 *	@data		2006.02.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "system.h"
#include "assert.h"
#include <string.h>
#include "include/system/fontproc.h"
#include "include/system/msgdata_util.h"
#include "include/msgdata/msg.naix"

#include "include/application/zukanlist/zkn_defain.h"
#include "include/application/zukanlist/zkn_world_text_data.h"
//#include "src/application/zukanlist/zkn_worldtext.h"

#include "include/application/zukanlist/zkn_version.h"

#define	__ZKN_WORLD_TEXT_SYS_H_GLOBAL
#include "include/application/zukanlist/zkn_world_text_sys.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define ZKN_WT_ALLOC_STR_NUM		( 256 )		// �m�ۂ��镶����


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 *					�O���[�o���f�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
//	�|�P�����O����e�L�X�g�p���R�[�h��
//	���ۂ�GMM�t�@�C���ɓ����Ă���f�[�^�̊O���ꍀ�ڐ�
//	�Ή��f�[�^
//	IDX�͊O����e�L�X�g�p���R�[�h
//
//	zukan_data.xls�ɂ͉��̐����̏����ް����i�[���Ă���
//=====================================
// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2006/10/05
// �O���ꂸ����̃e�L�X�g�����������������`���ɕύX

enum {
	TEXTCODE_JAPAN,
	TEXTCODE_ENGLISH,
	TEXTCODE_FRANCE,
	TEXTCODE_GERMANY,
	TEXTCODE_ITALY,
	TEXTCODE_SPAIN
};

#if (PM_LANG == LANG_JAPAN)
#define TEXTCODE_OWN	TEXTCODE_JAPAN
#elif (PM_LANG == LANG_ENGLISH)
#define TEXTCODE_OWN	TEXTCODE_ENGLISH
#elif (PM_LANG == LANG_FRANCE)
#define TEXTCODE_OWN	TEXTCODE_FRANCE
#elif (PM_LANG == LANG_GERMANY)
#define TEXTCODE_OWN	TEXTCODE_GERMANY
#elif (PM_LANG == LANG_ITALY)
#define TEXTCODE_OWN	TEXTCODE_ITALY
#elif (PM_LANG == LANG_SPAIN)
#define TEXTCODE_OWN	TEXTCODE_SPAIN
#endif

static const u8 ZKN_WORLD_TEXT_LangBtnOrder[ZKN_WORLD_TEXT_NUM] = {
#if (PM_LANG == LANG_JAPAN)
	TEXTCODE_JAPAN, TEXTCODE_ENGLISH, TEXTCODE_FRANCE, TEXTCODE_GERMANY, TEXTCODE_ITALY, TEXTCODE_SPAIN
#elif (PM_LANG == LANG_ENGLISH)
	TEXTCODE_ENGLISH, TEXTCODE_FRANCE, TEXTCODE_GERMANY, TEXTCODE_ITALY, TEXTCODE_SPAIN, TEXTCODE_JAPAN
#elif (PM_LANG == LANG_FRANCE)
	TEXTCODE_FRANCE, TEXTCODE_ENGLISH, TEXTCODE_GERMANY, TEXTCODE_ITALY, TEXTCODE_SPAIN, TEXTCODE_JAPAN
#elif (PM_LANG == LANG_GERMANY)
	TEXTCODE_GERMANY, TEXTCODE_ENGLISH, TEXTCODE_FRANCE, TEXTCODE_ITALY, TEXTCODE_SPAIN, TEXTCODE_JAPAN
#elif (PM_LANG == LANG_ITALY)
	TEXTCODE_ITALY, TEXTCODE_ENGLISH, TEXTCODE_FRANCE, TEXTCODE_GERMANY, TEXTCODE_SPAIN, TEXTCODE_JAPAN
#elif (PM_LANG == LANG_SPAIN)
	TEXTCODE_SPAIN, TEXTCODE_ENGLISH, TEXTCODE_FRANCE, TEXTCODE_GERMANY, TEXTCODE_ITALY, TEXTCODE_JAPAN
#endif
};

// ----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static inline int ZknWt_GetGmmIdx( int zkn_text_code );
static inline BOOL ZknWt_CheckWorldData( int textverpokenum, int gmm_idx );
static STRBUF* ZknWt_GetSTRData( int file_idx, int data_idx, int heap );
static void ZknWt_GetCountryPokeData( int monsno, int country, int* p_country_poke_num, int* p_country_text_code, int* p_country_gmm_idx );

//----------------------------------------------------------------------------
/**
 *	@brief	������o�b�t�@��j������
 *
 *	@param	buf		�o�b�t�@
 *
 *	@return	none
 */
//-----------------------------------------------------------------------------
void ZKN_WT_DeleteStrBuf( STRBUF* buf )
{
	STRBUF_Delete( buf );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���R�[�h����Ή�����O����GMM�C���f�b�N�X���擾
 *
 *	@param	country	���R�[�h
 *
 *	@return	�O����ԍ�
 */
//-----------------------------------------------------------------------------
int ZKN_WT_LANG_Code_WORLD_TEXT_GMM_Idx( int country )
{
	int country_text_code;
	int country_gmm_idx;
	
	// �O����e�L�X�g�p���R�[�h�ɕϊ�
	country_text_code = ZKN_WT_GetLANG_Code_ZKN_WORLD_TEXT_Code( country );
	GF_ASSERT( country_text_code < ZKN_WORLD_TEXT_NUM );	// �����Ƃ������Ƃ̓f�[�^������
	// �O����e�L�X�g�p���R�[�h����GMM�f�[�^�O���ꍀ�ڐ��擾
	country_gmm_idx = ZknWt_GetGmmIdx( country_text_code );

	return country_gmm_idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O����ԍ�����Ή����鍑�R�[�h���擾����
 *
 *	@param	county	�O����ԍ�	(gmm�t�@�C���ɕ���ł���ԍ�)
 *
 *	@return	���R�[�h		// ����������Ȃ��Ƃ��̓z�[���^�E����Ԃ�
 */
//-----------------------------------------------------------------------------
int ZKN_WT_WORLD_TEXT_GMM_Idx_LANG_Code( int country )
{
	// ----------------------------------------------------------------------------
	// localize_spec_mark(LANG_ALL) imatake 2006/10/05
	// �O���ꂸ����̃e�L�X�g�����������������`���ɕύX

	return ZKN_WT_GetZKN_WORLD_TEXT_Code_LANG_Code( ZKN_WORLD_TEXT_LangBtnOrder[country+1] );

	// ----------------------------------------------------------------------------
}


//----------------------------------------------------------------------------
/**
 *	@brief	���̃|�P��������Ԃ�
 *
 *	@param	monsno		�����X�^�[�i���o�[
 *	@param	country		�Q�[�������R�[�h
 *	@param	heap		�q�[�v
 *
 *	@return	������f�[�^
 */
//-----------------------------------------------------------------------------
#ifdef NONEQUIVALENT
STRBUF* ZKN_WT_GetPokeName( int monsno, int country, int heap )
{
	int country_poke_num;
	int country_text_code;
	int country_gmm_idx;
	int gmm_file_idx;

	// �����X�^�[�i���o�[�ƃQ�[�������R�[�h����
	// �O����e�L�X�g�f�[�^���ڐ�
	// �O����e�L�X�g�f�[�^�p���R�[�h
	// �O����e�L�X�g�f�[�^GMM�O�����ڐ��擾
	ZknWt_GetCountryPokeData( monsno, country, &country_poke_num, &country_text_code, &country_gmm_idx );

	// �����`�F�b�N
	if( country_gmm_idx == ZKN_WORLD_TEXT_NUM ){
		// ����
		// GMM�̒��̃f�[�^idx��monsno
		// ----------------------------------------------------------------------------
		// localize_spec_mark(LANG_ALL) imatake 2007/01/19
		// �|�P�������͂��ׂđS�啶����
		return MSGDAT_UTIL_GetMonsName( monsno, heap );
		// ----------------------------------------------------------------------------
	}else{
		// �O��
		// GMM�̒��̃f�[�^�C���f�b�N�X�����߂�
		gmm_file_idx = NARC_msg_zkn_worldname_dat + country_gmm_idx;
		country_gmm_idx = country_poke_num;
	}

	// gmm_file_idx��country_gmm_idx����STRBUF���擾����
	return ZknWt_GetSTRData( gmm_file_idx, country_gmm_idx, heap );
}
#else

static const u32 sOv21_21E9CCC[] = {
    0x2cd,
    0x2c8,
    0x2c9,
    0x2ca,
    0x2cb,
    0x2cc
};

asm STRBUF* ZKN_WT_GetPokeName( int monsno, int country, int heap )
{
	push {r4, r5, r6, lr}
	sub sp, #0x28
	add r4, r2, #0
	add r2, sp, #4
	str r2, [sp]
	add r2, sp, #0xc
	add r3, sp, #8
	add r5, r0, #0
	bl ZknWt_GetCountryPokeData
	ldr r2, [sp, #4]
	cmp r2, #6
	bne _021D5642
	add r0, r5, #0
	add r1, r4, #0
	bl MSGDAT_UTIL_GetMonsName
	add sp, #0x28
	pop {r4, r5, r6, pc}
_021D5642:
	ldr r6, =sOv21_21E9CCC // _021D5668
	add r5, sp, #0x10
	add r3, r5, #0
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	lsl r0, r2, #2
	ldr r1, [sp, #0xc]
	ldr r0, [r3, r0]
	add r2, r4, #0
	str r1, [sp, #4]
	bl ZknWt_GetSTRData
	add sp, #0x28
	pop {r4, r5, r6, pc}
	nop
// _021D5668: .4byte 0x021E9CCC
}
#endif

//----------------------------------------------------------------------------
/**
 *	@brief	���̃|�P�����^�C�v��Ԃ�
 *
 *	@param	monsno		�����X�^�[�i���o�[
 *	@param	country		�Q�[�������R�[�h
 *	@param	heap		�q�[�v
 *
 *	@return	������f�[�^
 */
//-----------------------------------------------------------------------------
#ifdef NONEQUIVALENT
STRBUF* ZKN_WT_GetPokeType( int monsno, int country, int heap )
{
	int country_poke_num;
	int country_text_code;
	int country_gmm_idx;
	int gmm_file_idx;

	// �����X�^�[�i���o�[�ƃQ�[�������R�[�h����
	// �O����e�L�X�g�f�[�^���ڐ�
	// �O����e�L�X�g�f�[�^�p���R�[�h
	// �O����e�L�X�g�f�[�^GMM�O�����ڐ��擾
	ZknWt_GetCountryPokeData( monsno, country, &country_poke_num, &country_text_code, &country_gmm_idx );

	// �����`�F�b�N
	if( country_gmm_idx == ZKN_WORLD_TEXT_NUM ){
		// ����
		// GMM�̒��̃f�[�^idx��monsno
		country_gmm_idx = monsno;
		gmm_file_idx = NARC_msg_zkn_type_dat;
	}else{
		// �O��
		// GMM�̒��̃f�[�^�C���f�b�N�X�����߂�
		gmm_file_idx = NARC_msg_zkn_worldtype_dat + country_gmm_idx;
		country_gmm_idx = country_poke_num;
	}

	// gmm_file_idx��country_gmm_idx����STRBUF���擾����
	return ZknWt_GetSTRData( gmm_file_idx, country_gmm_idx, heap );
}
#else
static const u32 sOv21_21E9CE4[] = {
    0x2d3,
    0x2ce,
    0x2cf,
    0x2d0,
    0x2d1,
    0x2d2
};

asm STRBUF* ZKN_WT_GetPokeType( int monsno, int country, int heap )
{
	push {r4, r5, r6, lr}
	sub sp, #0x28
	add r4, r2, #0
	add r2, sp, #4
	str r2, [sp]
	add r2, sp, #0xc
	add r3, sp, #8
	add r5, r0, #0
	bl ZknWt_GetCountryPokeData
	ldr r2, [sp, #4]
	cmp r2, #6
	bne _021D568C
	str r5, [sp, #4]
	ldr r0, =0x000002C7 // _021D56B4
	b _021D56A6
_021D568C:
	ldr r6, =sOv21_21E9CE4 // _021D56B8
	add r5, sp, #0x10
	add r3, r5, #0
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r5!, {r0, r1}
	lsl r0, r2, #2
	ldr r1, [sp, #0xc]
	ldr r0, [r3, r0]
	str r1, [sp, #4]
_021D56A6:
	ldr r1, [sp, #4]
	add r2, r4, #0
	bl ZknWt_GetSTRData
	add sp, #0x28
	pop {r4, r5, r6, pc}
}
#endif

//----------------------------------------------------------------------------
/**
 *	@brief	���̃e�L�X�g�f�[�^��Ԃ�
 *
 *	@param	monsno		�����X�^�[�i���o�[
 *	@param	country		�Q�[�������R�[�h
 *	@param	page		�y�[�W
 *	@param	heap		�q�[�v
 *	
 *	@return	������f�[�^
 */
//-----------------------------------------------------------------------------
#ifdef NONEQUIVALENT
STRBUF* ZKN_WT_GetText( int monsno, int country, int page, int heap )
{
	int country_poke_num;
	int country_text_code;
	int country_gmm_idx;
	int gmm_file_idx;

	// �����X�^�[�i���o�[�ƃQ�[�������R�[�h����
	// �O����e�L�X�g�f�[�^���ڐ�
	// �O����e�L�X�g�f�[�^�p���R�[�h
	// �O����e�L�X�g�f�[�^GMM�O�����ڐ��擾
	ZknWt_GetCountryPokeData( monsno, country, &country_poke_num, &country_text_code, &country_gmm_idx );

	// �����`�F�b�N
	if( country_gmm_idx == ZKN_WORLD_TEXT_NUM ){

		// �y�[�W���`�F�b�N
		GF_ASSERT( page < ZKN_WT_HOME_TEXT_PAGE_NUM );
		// ����
		// GMM�̒��̃f�[�^idx��monsno
		country_gmm_idx = (monsno * ZKN_WT_HOME_TEXT_PAGE_NUM) + page;
		gmm_file_idx = ZKN_TEXT_DAT;
	}else{

		// �y�[�W���`�F�b�N
		GF_ASSERT( page < ZKN_WT_WORLD_TEXT_PAGE_NUM );
		// �O��
		// GMM�̒��̃f�[�^�C���f�b�N�X�����߂�
		gmm_file_idx = NARC_msg_zkn_comment_02_dat + country_gmm_idx;
		country_gmm_idx = (country_poke_num * ZKN_WT_WORLD_TEXT_PAGE_NUM) + page;
	}

	// gmm_file_idx��country_gmm_idx����STRBUF���擾����
	return ZknWt_GetSTRData( gmm_file_idx, country_gmm_idx, heap );
}
#else
static const u32 sOv21_21E9CFC[] = {
    0x2c1,
    0x2bc,
    0x2bd,
    0x2be,
    0x2bf,
    0x2c0
};

asm STRBUF* ZKN_WT_GetText( int monsno, int country, int page, int heap )
{
	push {r4, r5, r6, lr}
	sub sp, #0x28
	add r4, r2, #0
	add r2, sp, #4
	add r6, r3, #0
	str r2, [sp]
	add r2, sp, #0xc
	add r3, sp, #8
	add r5, r0, #0
	bl ZknWt_GetCountryPokeData
	ldr r0, [sp, #4]
	cmp r0, #6
	bne _021D56E8
	cmp r4, #1
	blt _021D56E0
	bl GF_AssertFailedWarningCall
_021D56E0:
	add r0, r5, r4
	str r0, [sp, #4]
	ldr r0, =0x000002C2 // _021D571C
	b _021D570E
_021D56E8:
	ldr r3, =sOv21_21E9CFC // _021D5720
	add r2, sp, #0x10
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	cmp r4, #1
	blt _021D5700
	bl GF_AssertFailedWarningCall
_021D5700:
	ldr r0, [sp, #4]
	lsl r1, r0, #2
	add r0, sp, #0x10
	ldr r0, [r0, r1]
	ldr r1, [sp, #0xc]
	add r1, r1, r4
	str r1, [sp, #4]
_021D570E:
	ldr r1, [sp, #4]
	add r2, r6, #0
	bl ZknWt_GetSTRData
	add sp, #0x28
	pop {r4, r5, r6, pc}
	nop
// _021D571C: .4byte 0x000002C2
// _021D5720: .4byte sOv21_21E9CFC
}
#endif

//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	�֐��ł̖߂�l[�O����GMM���̃f�[�^����][GMM�f�[�^�O���ꍀ�ڐ�]���炻�̃f�[�^������̂��`�F�b�N����
 *
 *	@param	textverpokenum
 *	@param	gmm_idx 
 *
 *	@retval	TRUE	�f�[�^������
 *	@retval	FALSE	���̃|�P�����̂��̍��̃f�[�^�͂Ȃ�
 */
//-----------------------------------------------------------------------------
static inline BOOL ZknWt_CheckWorldData( int textverpokenum, int gmm_idx )
{
	// �����̃f�[�^�����߂Ă��Ȃ��̂ɂ��̃|�P�����̊O����f�[�^�������Ƃ�
	// �̓f�[�^���Ȃ��Ƃ������ƂɂȂ�
	if( (textverpokenum > MONSNO_END) && (gmm_idx != ZKN_WORLD_TEXT_NUM) ){
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�@�C���C���f�b�N�X���當����f�[�^���擾����
 *
 *	@param	file_idx
 *	@param	data_idx
 *	@param	heap 
 *
 *	@return
 */
//-----------------------------------------------------------------------------
static STRBUF* ZknWt_GetSTRData( int file_idx, int data_idx, int heap )
{
	MSGDATA_MANAGER* man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, file_idx, heap );
	if( man )
	{
		STRBUF* buf = STRBUF_Create(ZKN_WT_ALLOC_STR_NUM, heap );
		if( buf )
		{
			MSGMAN_GetString(man, data_idx, buf);
		}
		MSGMAN_Delete(man);
		return buf;
	}
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����X�^�[�i���o�[�ƃQ�[�������R�[�h����
 *			�O����e�L�X�g�f�[�^���ڐ�
 *			�O����e�L�X�g�f�[�^�p���R�[�h
 *			�O����e�L�X�g�f�[�^GMM�O�����ڐ�
 *			�擾
 *
 *	@param	monsno				�����X�^�[�i���o�[
 *	@param	country				�Q�[�������R�[�h
 *	@param	p_country_poke_num	�O����e�L�X�g�f�[�^���ڐ�
 *	@param	p_country_text_code	�O����e�L�X�g�f�[�^�p���R�[�h
 *	@param	p_country_gmm_idx	�O����e�L�X�g�f�[�^GMM�O�����ڐ�
 */
//-----------------------------------------------------------------------------
static void ZknWt_GetCountryPokeData( int monsno, int country, int* p_country_poke_num, int* p_country_text_code, int* p_country_gmm_idx )
{
	// �O����e�L�X�g�p���R�[�h�ɕϊ�
	*p_country_text_code = ZKN_WT_GetLANG_Code_ZKN_WORLD_TEXT_Code( country );
	GF_ASSERT( *p_country_text_code < ZKN_WORLD_TEXT_NUM );	// �����Ƃ������Ƃ̓f�[�^������
	// �����X�^�[�i���o�[����O����GMM���̃f�[�^���ڐ������߂�
//	*p_country_poke_num = ZKN_WT_GetMonsNo_TEXTVERPokeNum( monsno );
	*p_country_poke_num = monsno;	// �S�����ێ����Ă���

	// �O����e�L�X�g�p���R�[�h����GMM�f�[�^�O���ꍀ�ڐ��擾
	*p_country_gmm_idx = ZknWt_GetGmmIdx( *p_country_text_code );

	// �f�[�^�����邩�`�F�b�N
	GF_ASSERT( ZknWt_CheckWorldData( *p_country_poke_num, *p_country_gmm_idx ) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�ӊO����e�L�X�g�p���R�[�h����O����e�L�X�gGMM�f�[�^�̊O���ꍀ�ڐ����擾
 *
 *	@param	zkn_text_code	�}�ӊO����e�L�X�g�p���R�[�h
 *
 *	@retval	�O����e�L�X�gGMM�f�[�^�O���ꍀ�ڐ�
 *	@retval ZKN_WORLD_TEXT_NUM�̎��͎���GMM�Ƀf�[�^������
 */
//-----------------------------------------------------------------------------
static inline int ZknWt_GetGmmIdx( int zkn_text_code )
{
	GF_ASSERT( zkn_text_code < ZKN_WORLD_TEXT_NUM );
	
	// ----------------------------------------------------------------------------
	// localize_spec_mark(LANG_ALL) imatake 2006/10/05
	// �O���ꂸ����̃e�L�X�g�����������������`���ɕύX
	return zkn_text_code == TEXTCODE_OWN ? ZKN_WORLD_TEXT_NUM : zkn_text_code;
	// ---------------------------------------------------------------------------
}

