//============================================================================================
/**
 * @file	ashiato.c
 * @bfief	フトマキさん用足跡関数郡
 * @author	Tomomichi Ohta
 * @date	06.05.19
 */
//============================================================================================
#include <nitro.h>
#include <nnsys.h>
#include "common.h"
#include "system/bmp_list.h"
#include "system/bmp_menu.h"
#include "fld_bmp.h"
#include "system/lib_pack.h"
#include "fntsys.h"
#include "system/fontproc.h"
#include "talk_msg.h"
#include "fieldsys.h"
#include "poketool/monsno.h"
#include "battle/battle_server.h"

#include "ashiato.h"
#include "ashiato_def.h"

//============================================================================================
//
//	定義
//
//============================================================================================

typedef struct{
	u16	group;
	u16	ato;
}ASHIATO_TT;


//============================================================================================
//
//	プロトタイプ宣言
//
//============================================================================================



//============================================================================================
//
//	変数
//
//============================================================================================
//060609版
static const ASHIATO_TT ashi_data[MONSNO_END+1]={
	// 分類			 足跡の有無
	{ 0	, 0},			//dummy(1originの為数合わせ)
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号1
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号2
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号3
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号4
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号5
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号6
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号7
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号8
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号9
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号10
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号11
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号12
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号13
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号14
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号15
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号16
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号17
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号18
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号19
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号20
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号21
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号22
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号23
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号24
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号25
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号26
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号27
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号28
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号29
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号30
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号31
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号32
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号33
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号34
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号35
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号36
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号37
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号38
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号39
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号40
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号41
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号42
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号43
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号44
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号45
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号46
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号47
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号48
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号49
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号50
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号51
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号52
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号53
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号54
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号55
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号56
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号57
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号58
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号59
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号60
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号61
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号62
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号63
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号64
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号65
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号66
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号67
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号68
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号69
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号70
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号71
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号72
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号73
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号74
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号75
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号76
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号77
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号78
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号79
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号80
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号81
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号82
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号83
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号84
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号85
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号86
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号87
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号88
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号89
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号90
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号91
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号92
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号93
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号94
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号95
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号96
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号97
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号98
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号99
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号100
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号101
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号102
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号103
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号104
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号105
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号106
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号107
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号108
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号109
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号110
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号111
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号112
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号113
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号114
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号115
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号116
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号117
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号118
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号119
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号120
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号121
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号122
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号123
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号124
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号125
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号126
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号127
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号128
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号129
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号130
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号131
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号132
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号133
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号134
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号135
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号136
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号137
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号138
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号139
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号140
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号141
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号142
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号143
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号144
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号145
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号146
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号147
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号148
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号149
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号150
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号151
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号152
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号153
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号154
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号155
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号156
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号157
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号158
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号159
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号160
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号161
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号162
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号163
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号164
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号165
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号166
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号167
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号168
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号169
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号170
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号171
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号172
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号173
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号174
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号175
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号176
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号177
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号178
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号179
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号180
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号181
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号182
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号183
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号184
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号185
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号186
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号187
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号188
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号189
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号190
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号191
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号192
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号193
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号194
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号195
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号196
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号197
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号198
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号199
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号200
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号201
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号202
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号203
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号204
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号205
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号206
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号207
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号208
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号209
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号210
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号211
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号212
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号213
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号214
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号215
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号216
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号217
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号218
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号219
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号220
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号221
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号222
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号223
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号224
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号225
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号226
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号227
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号228
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号229
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号230
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号231
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号232
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号233
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号234
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号235
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号236
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号237
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号238
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号239
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号240
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号241
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号242
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号243
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号244
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号245
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号246
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号247
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号248
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号249
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号250
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号251
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号252
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号253
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号254
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号255
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号256
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号257
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号258
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号259
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号260
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号261
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号262
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号263
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号264
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号265
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号266
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号267
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号268
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号269
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号270
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号271
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号272
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号273
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号274
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号275
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号276
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号277
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号278
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号279
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号280
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号281
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号282
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号283
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号284
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号285
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号286
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号287
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号288
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号289
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号290
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号291
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号292
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号293
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号294
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号295
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号296
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号297
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号298
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号299
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号300
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号301
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号302
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号303
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号304
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号305
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号306
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号307
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号308
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号309
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号310
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号311
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号312
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号313
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号314
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号315
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号316
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号317
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号318
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号319
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号320
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号321
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号322
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号323
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号324
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号325
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号326
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号327
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号328
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号329
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号330
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号331
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号332
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号333
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号334
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号335
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号336
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号337
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号338
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号339
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号340
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号341
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号342
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号343
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号344
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号345
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号346
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号347
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号348
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号349
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号350
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号351
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号352
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号353
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号354
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号355
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号356
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号357
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号358
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号359
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号360
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号361
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号362
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号363
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号364
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号365
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号366
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号367
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号368
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号369
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号370
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号371
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号372
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号373
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号374
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号375
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号376
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号377
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号378
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号379
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号380
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号381
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号382
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号383
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号384
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号385
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号386
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号387
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号388
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号389
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号390
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号391
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号392
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号393
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号394
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号395
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号396
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号397
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号398
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号399
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号400
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号401
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号402
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号403
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号404
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号405
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号406
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号407
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号408
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号409
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号410
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号411
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号412
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号413
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号414
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号415
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号416
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号417
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号418
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号419
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号420
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号421
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号422
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号423
	{ KAWAII_KEI	,ASHIATO_ARI},		//図鑑番号424
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号425
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号426
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号427
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号428
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号429
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号430
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号431
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号432
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号433
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号434
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号435
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号436
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号437
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号438
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号439
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号440
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号441
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号442
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号443
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号444
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号445
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号446
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号447
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号448
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号449
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号450
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号451
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号452
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号453
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号454
	{ OTOBOKE_KEI		,ASHIATO_NASHI},		//図鑑番号455
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号456
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号457
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号458
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号459
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号460
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号461
	{ UNOWN_KEI		,ASHIATO_NASHI},		//図鑑番号462
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号463
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号464
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号465
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号466
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号467
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号468
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号469
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号470
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号471
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号472
	{ OTOBOKE_KEI		,ASHIATO_ARI},		//図鑑番号473
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号474
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号475
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号476
	{ KOWAI_KEI		,ASHIATO_NASHI},		//図鑑番号477
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号478
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号479
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号480
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号481
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号482
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号483
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号484
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号485
	{ UNOWN_KEI		,ASHIATO_ARI},		//図鑑番号486
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号487
	{ KAKKOII_KEI		,ASHIATO_NASHI},		//図鑑番号488
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号489
	{ KAWAII_KEI		,ASHIATO_NASHI},		//図鑑番号490
	{ KOWAI_KEI		,ASHIATO_ARI},		//図鑑番号491
	{ KAWAII_KEI		,ASHIATO_ARI},		//図鑑番号492
	{ KAKKOII_KEI		,ASHIATO_ARI},		//図鑑番号493
};

//============================================================================================
//
//	関数群
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 *	ポケモン番号から足跡の分類に変換
 *
 * @param	monsno		ポケモン番号
 *
 * @return	分類(1〜5)
 */
//--------------------------------------------------------------------------------------------
u16 ashiato_group_chk(u16 mons_no)
{
	return ashi_data[mons_no].group;
}

//--------------------------------------------------------------------------------------------
/**
 *	ポケモン番号から足跡の有無をチェック
 *
 * @param	monsno		ポケモン番号
 *
 * @return	0:足跡なし 1:足跡あり
 */
//--------------------------------------------------------------------------------------------
u16 ashiato_ato_chk(u16 mons_no, u16 form_no)
{
	//PLで追加されたギラティナの新フォルム対応(08.06.25)
	if( (mons_no == MONSNO_KIMAIRAN) && (form_no == FORMNO_GIRATINA_ORIGIN) ){
		return ASHIATO_NASHI;
	}

	return ashi_data[mons_no].ato;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
