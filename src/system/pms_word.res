// ���̃t�@�C���̓R���o�[�^�Ŏ�����������Ă��܂�
// ���Ƃł̕ҏW�͍s��Ȃ��悤�ɂ��肢���܂�
// Game Freak   taya


#ifdef __PMS_WORD_RES__	// �P��̃t�@�C������̂�include��������

//----------------------------------------------------------------
/**
 * �egmm�t�@�C���w��̂��߂̒萔
*/
//----------------------------------------------------------------

enum {
	PMSW_SRC_POKEMON,
	PMSW_SRC_SKILL,
	PMSW_SRC_TYPE,
	PMSW_SRC_TOKUSEI,
	PMSW_SRC_TRAINER,
	PMSW_SRC_HITO,
	PMSW_SRC_AISATSU,
	PMSW_SRC_SEIKATSU,
	PMSW_SRC_KIMOTI,
	PMSW_SRC_NANKAI,
	PMSW_SRC_UNION,
};

//----------------------------------------------------------------
/**
 * �egmm�t�@�C�����̗v�f��
*/
//----------------------------------------------------------------

static const u16 PMS_SrcElems[] = {
	 496,	//�|�P������	0 ... 495
	 468,	//�킴��	496 ... 963
	  18,	//�|�P�����^�C�v��	964 ... 981
	 124,	//�Ƃ�������	982 ... 1105
	  38,	//�g���[�i�[	1106 ... 1143
	  38,	//�Ђ�	1144 ... 1181
	 107,	//��������	1182 ... 1288
	 104,	//��������	1289 ... 1392
	  47,	//������	1393 ... 1439
	  32,	//�Ȃ񂩂����Ƃ�	1440 ... 1471
	  23,	//���j�I��	1472 ... 1494
};

//----------------------------------------------------------------
/**
 * �egmm�t�@�C���̃A�[�J�C�u���t�@�C��ID
*/
//----------------------------------------------------------------

static const u16 PMS_SrcFileID[] = {
	NARC_msg_monsname_dat,
	NARC_msg_wazaname_dat,
	NARC_msg_typename_dat,
	NARC_msg_tokusei_dat,
	NARC_msg_pms_word06_dat,
	NARC_msg_pms_word07_dat,
	NARC_msg_pms_word08_dat,
	NARC_msg_pms_word09_dat,
	NARC_msg_pms_word10_dat,
	NARC_msg_pms_word11_dat,
	NARC_msg_pms_word12_dat,
};

//----------------------------------------------------------------
/**
 * �d���P��e�[�u��
*/
//----------------------------------------------------------------

static const PMS_WORD DupWord_00[] = {
	944,1300
};

static const PMS_WORD DupWord_01[] = {
	872,1112,
};

static const PMS_WORD DupWord_02[] = {
	 93,971,
};

static const PMS_WORD DupWord_03[] = {
	1044,1116,
};

static const PMS_WORD DupWord_04[] = {
	1044,1116,
};

static const PMS_WORD DupWord_05[] = {
	1044,1116,
};

static const PMS_WORD DupWord_06[] = {
	1044,1116,
};

static const PMS_WORD DupWord_07[] = {
	1044,1116,
};

static const PMS_WORD DupWord_08[] = {
	1044,1116,
};

static const PMS_WORD DupWord_09[] = {
	1044,1116,
};

static const PMS_WORD DupWord_10[] = {
	1044,1116,
};

static const PMS_WORD DupWord_11[] = {
	1044,1116, 1, 2 // MatchComment: could not be bothered to match the data correctly right now
};


static const struct {
    const PMS_WORD* data;
    int   count;
}DupWordTable[] = {
	{ DupWord_00, 2 },
	{ DupWord_01, 2 },
	{ DupWord_02, 2 },
	{ DupWord_03, 2 },
    { DupWord_04, 2 },
    { DupWord_05, 2 },
    { DupWord_06, 2 },
    { DupWord_07, 2 },
    { DupWord_08, 2 },
    { DupWord_09, 2 },
    { DupWord_10, 2 },
    { DupWord_11, 2 },
};

//----------------------------------------------------------------
/**
 * �֘A�萔
*/
//----------------------------------------------------------------

#define  PMS_SRCFILE_MAX  (11)

#undef __PMS_WORD_RES__
#endif

