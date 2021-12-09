/*
 *------------------------------------------------------------------------------
 *
 * print-proto.h - make declarations external
 *
 *     long-description
 *
 *        AUTHOR: lord_pretzel
 *        DATE: 2019-11-11
 *        SUBDIR:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef _PRINT_PROTO_H_
#define _PRINT_PROTO_H_

#include "dss.h"
#include <stdio.h>

extern FILE *print_prep PROTO((int table, int update));
extern int pr_drange PROTO((int tbl, DSS_HUGE min, DSS_HUGE cnt, long num));


#endif /* _PRINT_PROTO_H_ */
