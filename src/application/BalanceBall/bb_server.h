//==============================================================================
/**
 * @file	bb_server.h
 * @brief	簡単な説明を書く
 * @author	goto
 * @date	2007.09.25(火)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================

#ifndef __BB_SERVER_H__
#define __BB_SERVER_H__

#include "bb_common.h"

extern BB_SERVER* BB_Server_AllocMemory( int comm_num, BB_SYS* sys );

extern void		  BB_Server_FreeMemory( BB_SERVER* wk );
extern BOOL		  BB_Server_Main( BB_SERVER* wk );


#endif

