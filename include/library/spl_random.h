/*---------------------------------------------------------------------------*
  Project:  simple particle library
  File:     spl_random.h

  乱数処理

  $Id: spl_random.h,v 1.3 2005/11/09 09:53:26 matsuda Exp $
  $Log: spl_random.h,v $
  Revision 1.3  2005/11/09 09:53:26  matsuda
  SPLライブラリが更新されたので更新

  Revision 1.1  2004/04/08 00:23:26  konoh
  (none)

 *---------------------------------------------------------------------------*/

#ifndef __SPL_RANDOM_H__
#define __SPL_RANDOM_H__

/* 乱数生成 ---------------------------------------------------------------- */

extern u32 spl_rndm_seed;
#define SPL_SRND(x) ((spl_rndm_seed)=(x))
#define SPL_RNDI()  ((spl_rndm_seed)=(spl_rndm_seed*1592653589UL)+453816691UL)
#define SPL_RNDF()  (SPL_RNDI() >> 20) // 小数部１２ビット分で０以上１未満？

/* ------------------------------------------------------------------------- */


/* 任意ベクトルの取得 ------------------------------------------------------ */

extern void spl_rndm_get_arb_vec_xyz(VecFx32* p_vec);
extern void spl_rndm_get_arb_vec_xy(VecFx32* p_vec);

/* ------------------------------------------------------------------------- */


/* 各種の範囲乱数（rangeは８ビットで０〜１の範囲の数とする）---------------- */

// [num * (1 - range), num)
#define SPL_RNDM_GET_01(num, range)\
        (((num) * (0xff - (((range) * (int) (SPL_RNDI() >> 24)) >> 8))) >> 8)

// [num * (1 - range), num * 2 * (1 - range))
#define SPL_RNDM_GET_02(num, range)\
        ((num) * (0xff + (range) - (((range) * (int) (SPL_RNDI() >> 24)) >> 7)) >> 8)

// [-num, num)
#define SPL_RNDM_GET_M11(num)\
        (((num) * ((int) (SPL_RNDI() >> 23)) - ((num) << 8)) >> 8)

/* ------------------------------------------------------------------------- */

#endif
