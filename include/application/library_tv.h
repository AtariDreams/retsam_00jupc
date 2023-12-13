//==============================================================================================
/**
 * @file	library_tv.c
 * @brief	図書館TV
 * @author	Satoshi Nohara
 * @date	2008.03.16
 */
//==============================================================================================
#ifndef	__LIBRARY_TV_H__
#define	__LIBRARY_TV_H__

//============================================================================================
//
//	extern宣言
//
//============================================================================================
extern PROC_RESULT LibraryTV_Init(PROC * proc, int * seq);
extern PROC_RESULT LibraryTV_Main(PROC * proc, int * seq);
extern PROC_RESULT LibraryTV_Exit(PROC * proc, int * seq);


#endif


