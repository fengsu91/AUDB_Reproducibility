/*
* $Id: print.c,v 1.16 2007/03/21 04:05:15 olteanu Exp $
*
* Revision History
* ===================
* $Log: print.c,v $
* Revision 1.16  2007/03/21 04:05:15  olteanu
* *** empty log message ***
*
* Revision 1.15  2007/03/21 01:42:57  olteanu
* *** empty log message ***
*
* Revision 1.14  2007/03/19 21:25:20  olteanu
* *** empty log message ***
*
* Revision 1.13  2007/03/19 21:08:53  olteanu
* *** empty log message ***
*
* Revision 1.12  2007/03/19 01:55:52  lublena
* *** empty log message ***
*
* Revision 1.11  2007/03/19 01:53:37  lublena
* *** empty log message ***
*
* Revision 1.10  2007/03/19 01:03:31  olteanu
* *** empty log message ***
*
* Revision 1.9  2007/03/18 20:55:32  olteanu
* *** empty log message ***
*
* Revision 1.8  2007/03/17 00:37:04  olteanu
* *** empty log message ***
*
* Revision 1.7  2007/03/16 23:50:37  olteanu
* *** empty log message ***
*
* Revision 1.6  2007/03/16 21:30:50  olteanu
* *** empty log message ***
*
* Revision 1.5  2007/03/15 18:49:36  olteanu
* *** empty log message ***
*
* Revision 1.4  2007/03/15 12:31:50  olteanu
* *** empty log message ***
*
* Revision 1.3  2007/03/14 20:19:06  olteanu
* *** empty log message ***
*
* Revision 1.2  2007/03/14 17:57:11  olteanu
* *** empty log message ***
*
* Revision 1.1.1.1  2007/03/14 15:01:09  olteanu
*
*
* Revision 1.4  2007/03/04 18:41:02  olteanu
* *** empty log message ***
*
* Revision 1.3  2007/03/03 18:22:05  olteanu
* *** empty log message ***
*
* Revision 1.2  2007/03/02 18:13:30  olteanu
* *** empty log message ***
*
* Revision 1.1.1.1  2007/03/01 18:11:56  olteanu
*
*
* Revision 1.3  2005/10/28 02:56:22  jms
* add platform-specific printf formats to allow for DSS_HUGE data type
*
* Revision 1.2  2005/01/03 20:08:59  jms
* change line terminations
*
* Revision 1.1.1.1  2004/11/24 23:31:47  jms
* re-establish external server
*
* Revision 1.4  2004/02/18 16:26:49  jms
* 32/64 bit changes for overflow handling needed additional changes when ported back to windows
*
* Revision 1.3  2004/02/18 14:05:53  jms
* porting changes for LINUX and 64 bit RNG
*
* Revision 1.2  2004/01/22 05:49:29  jms
* AIX porting (AIX 5.1)
*
* Revision 1.1.1.1  2003/08/07 17:58:34  jms
* recreation after CVS crash
*
* Revision 1.2  2003/08/07 17:58:34  jms
* Convery RNG to 64bit space as preparation for new large scale RNG
*
* Revision 1.1.1.1  2003/04/03 18:54:21  jms
* initial checkin
*
*
*/
/* generate flat files for data load */
#include <stdio.h>
#ifndef VMS
#include <sys/types.h>
#endif
#if defined(SUN)
#include <unistd.h>
#endif
#include <math.h>

#include "dss.h"
#include "dsstypes.h"
#include <string.h>
#include "print-proto.h"

/*
 * Function Prototypes
 */

FILE*
print_prep(int table, int update)
{
  FILE *res = tbl_open(table, "w");
  OPEN_CHECK(res, tdefs[table].name);
  return(res);
}

int
dbg_print(int format, FILE *target, void *data, int len, int sep)
{
	int dollars,
		cents;

	switch(format)
	{
	case DT_STR:
		if (columnar)
			fprintf(target, "%-*s", len, (char *)data);
		else
			fprintf(target, "%s", (char *)data);
		break;
#ifdef MVS
	case DT_VSTR:
		/* note: only used in MVS, assumes columnar output */
		fprintf(target, "%c%c%-*s",
			(len >> 8) & 0xFF, len & 0xFF, len, (char *)data);
		break;
#endif /* MVS */
	case DT_INT:
		if (columnar)
			fprintf(target, "%12ld", (long)data);
		else
			fprintf(target, "%ld", (long)data);
		break;
	case DT_HUGE:
		fprintf(target, HUGE_FORMAT, *(DSS_HUGE *)data);
		break;
	case DT_KEY:
		fprintf(target, "%ld", (long)data);
		break;
	case DT_MONEY:
		cents = (int)*(DSS_HUGE *)data;
		if (cents < 0)
			{
			fprintf(target, "-");
			cents = -cents;
			}
		dollars = cents / 100;
		cents %= 100;
		if (columnar)
			fprintf(target, "%12d.%02d", dollars, cents);
		else
			fprintf(target, "%d.%02d", dollars, cents);
		break;
	case DT_CHR:
		if (columnar)
			fprintf(target, "%c ", *(char *)data);
		else
			fprintf(target, "%c", *(char *)data);
		break;
	}

//#ifdef EOL_HANDLING
//	if (sep)
//#endif /* EOL_HANDLING */
//	if (!columnar)
//		fprintf(target, "%c", SEPARATOR);

	return(0);
}


/* added by olteanu for U-relations */


int
pr_component_value(unsigned table, long tid, unsigned attr, long cid, DSS_HUGE wid, void* value, int type)
{
  switch(table)
     {
     case PART:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(part_p_partkey);
	   fprintf(part_p_partkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_partkey, "%ld", *((long*)value));
	   PR_END (part_p_partkey);
	   break;

	 case 2:
	   PR_STRT(part_p_name);
	   fprintf(part_p_name, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_name, "%s",  (char*)value);
	   PR_END (part_p_name);
	   break;

	 case 3:
	   PR_STRT(part_p_mfgr);
	   fprintf(part_p_mfgr, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_mfgr, "%s",  (char*)value);
	   PR_END (part_p_mfgr);
	   break;

	 case 4:
	   PR_STRT(part_p_brand);
	   fprintf(part_p_brand, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_brand, "%s",  (char*)value);
	   PR_END (part_p_brand);
	   break;

	 case 5:
	   PR_STRT(part_p_type);
	   fprintf(part_p_type, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_type, "%s",  (char*)value);
	   PR_END (part_p_type);
	   break;

	 case 6:
	   PR_STRT(part_p_size);
	   fprintf(part_p_size, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_size, "%ld", *((long*)value));
	   PR_END (part_p_size);
	   break;

	 case 7:
	   PR_STRT(part_p_container);
	   fprintf(part_p_container, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_container, "%s",  (char*)value);
	   PR_END (part_p_container);
	   break;

	 case 8:
	   PR_STRT(part_p_retailprice);
	   fprintf(part_p_retailprice, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(part_p_retailprice, (long*)value);
	   PR_END (part_p_retailprice);
	   break;

	 case 9:
	   PR_STRT(part_p_comment);
	   fprintf(part_p_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(part_p_comment, "%s",  (char*)value);
	   PR_END (part_p_comment);
	   break;
	 }//end switch
       break;

     case SUPP:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(supp_s_suppkey);
	   fprintf(supp_s_suppkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_suppkey, "%ld", *((long*)value));
	   PR_END (supp_s_suppkey);
	   break;

	 case 2:
	   PR_STRT(supp_s_name);
	   fprintf(supp_s_name, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_name, "%s",  (char*)value);
	   PR_END (supp_s_name);
	   break;

	 case 3:
	   PR_STRT(supp_s_address);
	   fprintf(supp_s_address, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_address, "%s",  (char*)value);
	   PR_END (supp_s_address);
	   break;

	 case 4:
	   PR_STRT(supp_s_nationkey);
	   fprintf(supp_s_nationkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_nationkey, "%ld", *((long*)value));
	   PR_END (supp_s_nationkey);
	   break;

	 case 5:
	   PR_STRT(supp_s_phone);
	   fprintf(supp_s_phone, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_phone, "%s",  (char*)value);
	   PR_END (supp_s_phone);
	   break;

	 case 6:
	   PR_STRT(supp_s_acctbal);
	   fprintf(supp_s_acctbal, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(supp_s_acctbal, (long*)value);
	   PR_END (supp_s_acctbal);
	   break;

	 case 7:
	   PR_STRT(supp_s_comment);
	   fprintf(supp_s_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(supp_s_comment, "%s",  (char*)value);
	   PR_END (supp_s_comment);
	   break;
	 }//end switch
       break;

     case PSUPP:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(psupp_ps_partkey);
	   fprintf(psupp_ps_partkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(psupp_ps_partkey, "%ld", *((long*)value));
	   PR_END (psupp_ps_partkey);
	   break;

	 case 2:
	   PR_STRT(psupp_ps_suppkey);
	   fprintf(psupp_ps_suppkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(psupp_ps_suppkey, "%ld", *((long*)value));
	   PR_END (psupp_ps_suppkey);
	   break;

	 case 3:
	   PR_STRT(psupp_ps_availqty);
	   fprintf(psupp_ps_availqty, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(psupp_ps_availqty, "%ld", *((long*)value));
	   PR_END (psupp_ps_availqty);
	   break;

	 case 4:
	   PR_STRT(psupp_ps_supplycost);
	   fprintf(psupp_ps_supplycost, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(psupp_ps_supplycost, (long*)value);
	   PR_END (psupp_ps_supplycost);
	   break;

	 case 5:
	   PR_STRT(psupp_ps_comment);
	   fprintf(psupp_ps_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(psupp_ps_comment, "%s",  (char*)value);
	   PR_END (psupp_ps_comment);
	   break;
	 }//end switch
       break;

     case CUST:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(cust_c_custkey);
	   fprintf(cust_c_custkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_custkey, "%ld", *((long*)value));
	   PR_END (cust_c_custkey);
	   break;

	 case 2:
	   PR_STRT(cust_c_name);
	   fprintf(cust_c_name, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_name, "%s",  (char*)value);
	   PR_END (cust_c_name);
	   break;

	 case 3:
	   PR_STRT(cust_c_address);
	   fprintf(cust_c_address, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_address, "%s",  (char*)value);
	   PR_END (cust_c_address);
	   break;

	 case 4:
	   PR_STRT(cust_c_nationkey);
	   fprintf(cust_c_nationkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_nationkey, "%ld", *((long*)value));
	   PR_END (cust_c_nationkey);
	   break;

	 case 5:
	   PR_STRT(cust_c_phone);
	   fprintf(cust_c_phone, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_phone, "%s",  (char*)value);
	   PR_END (cust_c_phone);
	   break;

	 case 6:
	   PR_STRT(cust_c_acctbal);
	   fprintf(cust_c_acctbal, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(cust_c_acctbal, (long*)value);
	   PR_END (cust_c_acctbal);
	   break;

	 case 7:
	   PR_STRT(cust_c_mktsegment);
	   fprintf(cust_c_mktsegment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_mktsegment, "%s",  (char*)value);
	   PR_END (cust_c_mktsegment);
	   break;

	 case 8:
	   PR_STRT(cust_c_comment);
	   fprintf(cust_c_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(cust_c_comment, "%s",  (char*)value);
	   PR_END (cust_c_comment);
	   break;
	 }//end switch
       break;

     case ORDER:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(orders_o_orderkey);
	   fprintf(orders_o_orderkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_orderkey, "%ld", *((long*)value));
	   PR_END (orders_o_orderkey);
	   break;

	 case 2:
	   PR_STRT(orders_o_custkey);
	   fprintf(orders_o_custkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_custkey, "%ld", *((long*)value));
	   PR_END (orders_o_custkey);
	   break;

	 case 3:
	   PR_STRT(orders_o_orderstatus);
	   fprintf(orders_o_orderstatus, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_orderstatus, "%ld", *((long*)value));
	   PR_END (orders_o_orderstatus);
	   break;

	 case 4:
	   PR_STRT(orders_o_totalprice);
	   fprintf(orders_o_totalprice, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(orders_o_totalprice, (long*)value);
	   PR_END (orders_o_totalprice);
	   break;

	 case 5:
	   PR_STRT(orders_o_orderdate);
	   fprintf(orders_o_orderdate, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_orderdate, "%s",  (char*)value);
	   PR_END (orders_o_orderdate);
	   break;

	 case 6:
	   PR_STRT(orders_o_orderpriority);
	   fprintf(orders_o_orderpriority, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_orderpriority, "%s",  (char*)value);
	   PR_END (orders_o_orderpriority);
	   break;

	 case 7:
	   PR_STRT(orders_o_clerk);
	   fprintf(orders_o_clerk, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_clerk, "%s",  (char*)value);
	   PR_END (orders_o_clerk);
	   break;

	 case 8:
	   PR_STRT(orders_o_shippriority);
	   fprintf(orders_o_shippriority, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_shippriority, "%ld", *((long*)value));
	   PR_END (orders_o_shippriority);
	   break;

	 case 9:
	   PR_STRT(orders_o_comment);
	   fprintf(orders_o_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(orders_o_comment, "%s",  (char*)value);
	   PR_END (orders_o_comment);
	   break;
	 }//end switch
       break;

     case LINE:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(lineitem_l_orderkey);
	   fprintf(lineitem_l_orderkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_orderkey, "%ld", *((long*)value));
	   PR_END (lineitem_l_orderkey);
	   break;

	 case 2:
	   PR_STRT(lineitem_l_partkey);
	   fprintf(lineitem_l_partkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_partkey, "%ld", *((long*)value));
	   PR_END (lineitem_l_partkey);
	   break;

	 case 3:
	   PR_STRT(lineitem_l_suppkey);
	   fprintf(lineitem_l_suppkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_suppkey, "%ld", *((long*)value));
	   PR_END (lineitem_l_suppkey);
	   break;

	 case 4:
	   PR_STRT(lineitem_l_linenumber);
	   fprintf(lineitem_l_linenumber, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_linenumber, "%ld", *((long*)value));
	   PR_END (lineitem_l_linenumber);
	   break;

	 case 5:
	   PR_STRT(lineitem_l_quantity);
	   fprintf(lineitem_l_quantity, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_quantity, "%ld", *((long*)value));
	   PR_END (lineitem_l_quantity);
	   break;

	 case 6:
	   PR_STRT(lineitem_l_extendedprice);
	   fprintf(lineitem_l_extendedprice, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(lineitem_l_extendedprice, value);
	   PR_END (lineitem_l_extendedprice);
	   break;

	 case 7:
	   PR_STRT(lineitem_l_discount);
	   fprintf(lineitem_l_discount, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(lineitem_l_discount, (long*)value);
	   PR_END (lineitem_l_discount);
	   break;

	 case 8:
	   PR_STRT(lineitem_l_tax);
	   fprintf(lineitem_l_tax, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   PR_MONEY(lineitem_l_tax, (long*)value);
	   PR_END (lineitem_l_tax);
	   break;

	 case 9:
	   PR_STRT(lineitem_l_returnflag);
	   fprintf(lineitem_l_returnflag, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_returnflag, "%s",  (char*)value);
	   PR_END (lineitem_l_returnflag);
	   break;

	 case 10:
	   PR_STRT(lineitem_l_linestatus);
	   fprintf(lineitem_l_linestatus, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_linestatus, "%ld", *((long*)value));
	   PR_END (lineitem_l_linestatus);
	   break;

	 case 11:
	   PR_STRT(lineitem_l_shipdate);
	   fprintf(lineitem_l_shipdate, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_shipdate, "%s",  (char*)value);
	   PR_END (lineitem_l_shipdate);
	   break;

	 case 12:
	   PR_STRT(lineitem_l_commitdate);
	   fprintf(lineitem_l_commitdate, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_commitdate, "%s",  (char*)value);
	   PR_END (lineitem_l_commitdate);
	   break;

	 case 13:
	   PR_STRT(lineitem_l_receiptdate);
	   fprintf(lineitem_l_receiptdate, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_receiptdate, "%s",  (char*)value);
	   PR_END (lineitem_l_receiptdate);
	   break;

	 case 14:
	   PR_STRT(lineitem_l_shipinstruct);
	   fprintf(lineitem_l_shipinstruct, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_shipinstruct, "%s",  (char*)value);
	   PR_END (lineitem_l_shipinstruct);
	   break;

	 case 15:
	   PR_STRT(lineitem_l_shipmode);
	   fprintf(lineitem_l_shipmode, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_shipmode, "%s",  (char*)value);
	   PR_END (lineitem_l_shipmode);
	   break;

	 case 16:
	   PR_STRT(lineitem_l_comment);
	   fprintf(lineitem_l_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(lineitem_l_comment, "%s",  (char*)value);
	   PR_END (lineitem_l_comment);
	   break;
	 }//end switch
       break;

     case NATION:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(nation_n_nationkey);
	   fprintf(nation_n_nationkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(nation_n_nationkey, "%ld", *((long*)value));
	   PR_END (nation_n_nationkey);
	   break;

	 case 2:
	   PR_STRT(nation_n_name);
	   fprintf(nation_n_name, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(nation_n_name, "%s",  (char*)value);
	   PR_END (nation_n_name);
	   break;

	 case 3:
	   PR_STRT(nation_n_regionkey);
	   fprintf(nation_n_regionkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(nation_n_regionkey, "%ld", *((long*)value));
	   PR_END (nation_n_regionkey);
	   break;

	 case 4:
	   PR_STRT(nation_n_comment);
	   fprintf(nation_n_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(nation_n_comment, "%s",  (char*)value);
	   PR_END (nation_n_comment);
	   break;
	 }//end switch
       break;

     case REGION:
       switch(attr)
	 {
	 case 1:
	   PR_STRT(region_r_regionkey);
	   fprintf(region_r_regionkey, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(region_r_regionkey, "%ld", *((long*)value));
	   PR_END (region_r_regionkey);
	   break;

	 case 2:
	   PR_STRT(region_r_name);
	   fprintf(region_r_name, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(region_r_name, "%s",  (char*)value);
	   PR_END (region_r_name);
	   break;

	 case 3:
	   PR_STRT(region_r_comment);
	   fprintf(region_r_comment, "%ld%c%lld%c%ld%c",cid,SEPARATOR,wid,SEPARATOR,tid,SEPARATOR);
	   fprintf(region_r_comment, "%s",  (char*)value);
	   PR_END (region_r_comment);
	   break;
	 }//end switch
       break;
     }//end switch
   return(0);
}



void
add_placeholder(unsigned table, long tid, unsigned attribute)
{

  Placeholder * placeholder = (Placeholder *) malloc(sizeof(Placeholder));
  placeholder->table = table;
  placeholder->tid = tid;
  placeholder->column = attribute;

  if (placeholders_idx == PLACEHOLDERS_LIMIT)
    {
      fprintf (stderr, "reached the placeholders limit. \n");
      gen_components ();
      free (placeholder_set);
      placeholders_idx = 0;
      placeholders_size = PLACEHOLDERS_INIT;
      placeholder_set = (Placeholder**) calloc(placeholders_size,sizeof(Placeholder*));
      if (placeholder_set == NULL)
	  {
	    fprintf (stderr, "out of memory \n");
	    exit(-1);
	  }
    } else
    if (placeholders_idx == placeholders_size)
      {
	fprintf (stderr, "reallocate a bit. %ld not enough.. \n",placeholders_size);
	placeholders_size *=1.5;
	placeholder_set = realloc(placeholder_set, placeholders_size * sizeof(Placeholder*));
	if (placeholder_set == NULL)
	  {
	    fprintf (stderr, "out of memory \n");
	    exit(-1);
	  }
      }

  placeholder_set[placeholders_idx] = placeholder;
  placeholders_idx++;
}


/* the following functions are changed by olteanu for U-relations */

int
pr_cust(customer_t *c, int mode)
{
   DSS_HUGE i;
   threshold = upper * placeholders_ratio;

   RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   //fprintf(stderr,"i is %ld, threshold is %ld, upper is %ld",(long)i,threshold,upper);
   if (TUPLE_LEVEL) {
     if ((long)i <  threshold+1) {
       add_placeholder(CUST, (long)c->custkey, 0);
       return(0);
     }
     threshold = -1;
   }

   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_custkey);
       fprintf(cust_c_custkey, "%ld%c1%c%ld%c%lld",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR,c->custkey);
       PR_END (cust_c_custkey);

       stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
       crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,1);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_name);
       fprintf(cust_c_name, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       if (scale <= 3000)
	 PR_VSTR(cust_c_name, c->name, C_NAME_LEN);
       else
	 PR_VSTR(cust_c_name, c->name, C_NAME_LEN + 3);
       PR_END (cust_c_name);

       stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
       crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,2);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_address);
       fprintf(cust_c_address, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       PR_VSTR(cust_c_address, c->address,
	       (columnar)?(long)(ceil(C_ADDR_LEN * V_STR_HGH)):c->alen);
       PR_END (cust_c_address);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,3);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_nationkey);
       fprintf(cust_c_nationkey, "%ld%c1%c%ld%c%lld",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR,c->nation_code);
       PR_END (cust_c_nationkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,4);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_phone);
       fprintf(cust_c_phone, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       PR_STR(cust_c_phone, c->phone, PHONE_LEN);
       PR_END (cust_c_phone);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,5);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_acctbal);
       fprintf(cust_c_acctbal, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       PR_MONEY(cust_c_acctbal, &c->acctbal);
       PR_END (cust_c_acctbal);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,6);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_mktsegment);
       fprintf(cust_c_mktsegment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       PR_STR(cust_c_mktsegment, c->mktsegment, C_MSEG_LEN);
       PR_END (cust_c_mktsegment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,7);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,CUST_PLACEHOLDERS);
   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(cust_c_comment);
       fprintf(cust_c_comment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->custkey,SEPARATOR);
       PR_VSTR_LAST(cust_c_comment, c->comment,
		    (columnar)?(long)(ceil(C_CMNT_LEN * V_STR_HGH)):c->clen);
       PR_END (cust_c_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(CUST,(long)c->custkey,8);

   return(0);
}

/*
 * print the numbered order
 */
int
pr_order(order_t *o, int mode)
{
   DSS_HUGE i;
   threshold = upper * placeholders_ratio;

   RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);

   if (TUPLE_LEVEL) {
     if ((long)i <  threshold+1) {
       add_placeholder(ORDER,(long)o->okey,0);
       return(0);
     }
     threshold = -1;
   }

   if ((long)i > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(orders_o_orderkey);
       fprintf(orders_o_orderkey, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR,(long)o->okey);
       PR_END (orders_o_orderkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(ORDER,(long)o->okey,1);

   if (!TUPLE_LEVEL)
     RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_custkey);
       fprintf(orders_o_custkey, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR,
	       (long)o->custkey);
       PR_END (orders_o_custkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
       add_placeholder(ORDER,(long)o->okey,2);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_orderstatus);
       fprintf(orders_o_orderstatus, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_CHR(orders_o_orderstatus, &o->orderstatus);
       PR_END (orders_o_orderstatus);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
       add_placeholder(ORDER,(long)o->okey,3);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_totalprice);
       fprintf(orders_o_totalprice, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_MONEY(orders_o_totalprice, &o->totalprice);
       PR_END (orders_o_totalprice);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
      add_placeholder(ORDER,(long)o->okey,4);


    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_orderdate);
       fprintf(orders_o_orderdate, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_STR(orders_o_orderdate, o->odate, DATE_LEN);
       PR_END (orders_o_orderdate);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
      add_placeholder(ORDER,(long)o->okey,5);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_orderpriority);
       fprintf(orders_o_orderpriority, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_STR(orders_o_orderpriority, o->opriority, O_OPRIO_LEN);
       PR_END (orders_o_orderpriority);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
   else
      add_placeholder(ORDER,(long)o->okey,6);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_clerk);
       fprintf(orders_o_clerk, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_STR(orders_o_clerk, o->clerk, O_CLRK_LEN);
       PR_END (orders_o_clerk);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
      add_placeholder(ORDER,(long)o->okey,7);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_shippriority);
       fprintf(orders_o_shippriority, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_INT(orders_o_shippriority, o->spriority);
       PR_END (orders_o_shippriority);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
      add_placeholder(ORDER,(long)o->okey,8);

    if (!TUPLE_LEVEL)
      RANDOM(i,0,upper,ORDERS_PLACEHOLDERS);
    if ((long)i > threshold) {
      if (!COUNT_ONLY) {
       PR_STRT(orders_o_comment);
       fprintf(orders_o_comment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)o->okey,SEPARATOR);
       PR_VSTR_LAST(orders_o_comment, o->comment,
		    (columnar)?(long)(ceil(O_CMNT_LEN * V_STR_HGH)):o->clen);
       PR_END (orders_o_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
      }
    }
    else
      add_placeholder(ORDER,(long)o->okey,9);
    return(0);
}

/*
 * print an order's lineitems
 */
int
pr_line(order_t *o, int mode)
{
  long i;
  DSS_HUGE j;
  threshold = upper * placeholders_ratio;

   for (i = 0; (long)i <  o->lines; i++)
     {

       RANDOM(j,0,upper,LINE_PLACEHOLDERS);
       if (TUPLE_LEVEL) {
	 if ((long)j < threshold+1) {
	   add_placeholder(LINE,tid_line,0);
	   tid_line++;
	   continue;
	 }
       }

       if ((long)j > threshold) {
	 if (!COUNT_ONLY) {
	   PR_STRT(lineitem_l_orderkey);
	   fprintf(lineitem_l_orderkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR,o->l[i].okey);
	   PR_END (lineitem_l_orderkey);
	          stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	 }
       }
       else
	   add_placeholder(LINE,tid_line,1);

       if (!TUPLE_LEVEL)
	 RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_partkey);
	    fprintf(lineitem_l_partkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR,
		    o->l[i].partkey);
	    PR_END (lineitem_l_partkey);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	   add_placeholder(LINE,tid_line,2);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_suppkey);
	    fprintf(lineitem_l_suppkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR,
		    o->l[i].suppkey);
	    PR_END (lineitem_l_suppkey);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	   add_placeholder(LINE,tid_line,3);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_linenumber);
	    fprintf(lineitem_l_linenumber, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR,
		    o->l[i].lcnt);
	    PR_END (lineitem_l_linenumber);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	   add_placeholder(LINE,tid_line,4);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_quantity);
	    fprintf(lineitem_l_quantity, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR,
		    o->l[i].quantity);
	    PR_END (lineitem_l_quantity);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	   add_placeholder(LINE,tid_line,5);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_extendedprice);
	    fprintf(lineitem_l_extendedprice, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_MONEY(lineitem_l_extendedprice, &o->l[i].eprice);
	    PR_END (lineitem_l_extendedprice);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	   add_placeholder(LINE,tid_line,6);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_discount);
	    fprintf(lineitem_l_discount, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_MONEY(lineitem_l_discount, &o->l[i].discount);
	    PR_END (lineitem_l_discount);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,7);


	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_tax);
	    fprintf(lineitem_l_tax, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_MONEY(lineitem_l_tax, &o->l[i].tax);
	    PR_END (lineitem_l_tax);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,8);


	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_returnflag);
	    fprintf(lineitem_l_returnflag, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_CHR(lineitem_l_returnflag, &o->l[i].rflag[0]);
	    PR_END (lineitem_l_returnflag);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,9);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_linestatus);
	    fprintf(lineitem_l_linestatus, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_CHR(lineitem_l_linestatus, &o->l[i].lstatus[0]);
	    PR_END (lineitem_l_linestatus);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,10);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_shipdate);
	    fprintf(lineitem_l_shipdate, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_STR(lineitem_l_shipdate, o->l[i].sdate, DATE_LEN);
	    PR_END (lineitem_l_shipdate);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,11);


	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_commitdate);
	    fprintf(lineitem_l_commitdate, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_STR(lineitem_l_commitdate, o->l[i].cdate, DATE_LEN);
	    PR_END (lineitem_l_commitdate);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,12);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_receiptdate);
	    fprintf(lineitem_l_receiptdate, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_STR(lineitem_l_receiptdate, o->l[i].rdate, DATE_LEN);
	    PR_END (lineitem_l_receiptdate);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,13);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_shipinstruct);
	    fprintf(lineitem_l_shipinstruct, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_STR(lineitem_l_shipinstruct, o->l[i].shipinstruct, L_INST_LEN);
	    PR_END (lineitem_l_shipinstruct);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,14);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_shipmode);
	    fprintf(lineitem_l_shipmode, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_STR(lineitem_l_shipmode, o->l[i].shipmode, L_SMODE_LEN);
	    PR_END (lineitem_l_shipmode);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,15);

	if (!TUPLE_LEVEL)
	  RANDOM(j,0,upper,LINE_PLACEHOLDERS);
	if ((long)j > threshold) {
	  if (!COUNT_ONLY) {
	    PR_STRT(lineitem_l_comment);
	    fprintf(lineitem_l_comment, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_line,SEPARATOR);
	    PR_VSTR_LAST(lineitem_l_comment, o->l[i].comment,
			 (columnar)?(long)(ceil(L_CMNT_LEN * V_STR_HGH)):o->l[i].clen);
	    PR_END (lineitem_l_comment);
	           stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	  }
	}
	else
	  add_placeholder(LINE,tid_line,16);

	tid_line++;
     }

   return(0);
}

/*
 * print the numbered order *and* its associated lineitems
 */
int
pr_order_line(order_t *o, int mode)
{
    tdefs[ORDER].name = tdefs[ORDER_LINE].name;
    pr_order(o, mode);
    pr_line(o, mode);

    return(0);
}

/*
 * print the given part
 */
int
pr_part(part_t *part, int mode)
{
   DSS_HUGE j;
   threshold = upper * placeholders_ratio;

   RANDOM(j,0,upper,PART_PLACEHOLDERS);

   if (TUPLE_LEVEL) {
     if ((long)j <threshold+1) {
       add_placeholder(PART,(long)part->partkey,0);
       return(0);
     }
     threshold = -1;
   }

   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_partkey);
       fprintf(part_p_partkey, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR,
	       (long)part->partkey);
       PR_END (part_p_partkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,1);


   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_name);
       fprintf(part_p_name, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_VSTR(part_p_name, part->name,
	       (columnar)?(long)P_NAME_LEN:part->nlen);
       PR_END (part_p_name);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,2);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_mfgr);
       fprintf(part_p_mfgr, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_STR(part_p_mfgr, part->mfgr, P_MFG_LEN);
       PR_END (part_p_mfgr);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,3);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_brand);
       fprintf(part_p_brand, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_STR(part_p_brand, part->brand, P_BRND_LEN);
       PR_END (part_p_brand);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,4);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_type);
       fprintf(part_p_type, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_VSTR(part_p_type, part->type,
	       (columnar)?(long)P_TYPE_LEN:part->tlen);
       PR_END (part_p_type);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,5);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_size);
       fprintf(part_p_size, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR,
	       (long)part->size);
       PR_END (part_p_size);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,6);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_container);
       fprintf(part_p_container, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_STR(part_p_container, part->container, P_CNTR_LEN);
       PR_END (part_p_container);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,7);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_retailprice);
       fprintf(part_p_retailprice, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_MONEY(part_p_retailprice, &part->retailprice);
       PR_END (part_p_retailprice);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,8);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,PART_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(part_p_comment);
       fprintf(part_p_comment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)part->partkey,SEPARATOR);
       PR_VSTR(part_p_comment, part->comment,
		    (columnar)?(long)(ceil(P_CMNT_LEN * V_STR_HGH)):part->clen);
       PR_END (part_p_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(PART,(long)part->partkey,9);

   return(0);
}

/*
 * print the given part's suppliers
 */
int
pr_psupp(part_t *part, int mode)
{
  long      i;
  DSS_HUGE j;
  threshold = upper * placeholders_ratio;


  for (i = 0; i < SUPP_PER_PART; i++)
    {
      RANDOM(j,0,upper,PSUPP_PLACEHOLDERS);

      if (TUPLE_LEVEL) {
	if ((long)j <threshold+1) {
	  add_placeholder(PSUPP,(long)tid_partsupp,0);
	  tid_partsupp++;
	  continue;
	}
      }

      if ((long)j > threshold) {
	if (!COUNT_ONLY) {
	  PR_STRT(psupp_ps_partkey);
	  fprintf(psupp_ps_partkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_partsupp,SEPARATOR,
		  part->s[i].partkey);
	  PR_END (psupp_ps_partkey);
	         stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	}
      }
      else
	add_placeholder(PSUPP,(long)tid_partsupp,1);

      if (!TUPLE_LEVEL)
	RANDOM(j,0,upper,PSUPP_PLACEHOLDERS);
      if ((long)j > threshold) {
	if (!COUNT_ONLY) {
	  PR_STRT(psupp_ps_suppkey);
	  fprintf(psupp_ps_suppkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_partsupp,SEPARATOR,
		  part->s[i].suppkey);
	  PR_END (psupp_ps_suppkey);
	         stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	}
      }
      else
	add_placeholder(PSUPP,(long)tid_partsupp,2);

      if (!TUPLE_LEVEL)
	RANDOM(j,0,upper,PSUPP_PLACEHOLDERS);
      if ((long)j > threshold) {
	if (!COUNT_ONLY) {
	  PR_STRT(psupp_ps_availqty);
	  fprintf(psupp_ps_availqty, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,tid_partsupp,SEPARATOR,
		  part->s[i].qty);
	  PR_END (psupp_ps_availqty);
	         stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	}
      }
      else
	add_placeholder(PSUPP,(long)tid_partsupp,3);

      if (!TUPLE_LEVEL)
	RANDOM(j,0,upper,PSUPP_PLACEHOLDERS);
      if ((long)j > threshold) {
	if (!COUNT_ONLY) {
	  PR_STRT(psupp_ps_supplycost);
	  fprintf(psupp_ps_supplycost, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_partsupp,SEPARATOR);
	  PR_MONEY(psupp_ps_supplycost, &part->s[i].scost);
	  PR_END (psupp_ps_supplycost);
	         stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	}
      }
      else
	add_placeholder(PSUPP,(long)tid_partsupp,4);

      if (!TUPLE_LEVEL)
	RANDOM(j,0,upper,PSUPP_PLACEHOLDERS);
      if ((long)j > threshold) {
	if (!COUNT_ONLY) {
	  PR_STRT(psupp_ps_comment);
	  fprintf(psupp_ps_comment, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,tid_partsupp,SEPARATOR);
	  PR_VSTR(psupp_ps_comment, part->s[i].comment,
		       (columnar)?(long)(ceil(PS_CMNT_LEN * V_STR_HGH)):part->s[i].clen);
	  PR_END (psupp_ps_comment);
	         stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
	}
      }
      else
	add_placeholder(PSUPP,(long)tid_partsupp,5);

      tid_partsupp++;
      }

   return(0);
}

/*
 * print the given part *and* its suppliers
 */
int
pr_part_psupp(part_t *part, int mode)
{
    tdefs[PART].name = tdefs[PART_PSUPP].name;
    pr_part(part, mode);
    pr_psupp(part, mode);

    return(0);
}

int
pr_supp(supplier_t *supp, int mode)
{
  DSS_HUGE j;
  threshold = upper * placeholders_ratio;

  RANDOM(j,0,upper,SUPP_PLACEHOLDERS);

  if (TUPLE_LEVEL) {
    if ((long)j <threshold+1) {
      add_placeholder(SUPP,(long)supp->suppkey,0);
      return(0);
    }
    threshold = -1;
  }


  if ((long)j > threshold) {
    if (!COUNT_ONLY) {
      PR_STRT(supp_s_suppkey);
      fprintf(supp_s_suppkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR,
	      supp->suppkey);
      PR_END (supp_s_suppkey);
             stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
    }
  }
  else
      add_placeholder(SUPP,(long)supp->suppkey,1);


  if (!TUPLE_LEVEL)
    RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(supp_s_name);
       fprintf(supp_s_name, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR);
       PR_STR(supp_s_name, supp->name, S_NAME_LEN);
       PR_END (supp_s_name);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,2);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(supp_s_address);
       fprintf(supp_s_address, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR);
       PR_VSTR(supp_s_address, supp->address,
	       (columnar)?(long)(ceil(S_ADDR_LEN * V_STR_HGH)):supp->alen);
       PR_END (supp_s_address);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,3);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY)  {
       PR_STRT(supp_s_nationkey);
       fprintf(supp_s_nationkey, "%ld%c1%c%lld%c%lld",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR,
	       supp->nation_code);
       PR_END (supp_s_nationkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,4);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(supp_s_phone);
       fprintf(supp_s_phone, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR);
       PR_STR(supp_s_phone, supp->phone, PHONE_LEN);
       PR_END (supp_s_phone);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,5);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(supp_s_acctbal);
       fprintf(supp_s_acctbal, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR);
       PR_MONEY(supp_s_acctbal, &supp->acctbal);
       PR_END (supp_s_acctbal);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,6);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,SUPP_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(supp_s_comment);
       fprintf(supp_s_comment, "%ld%c1%c%lld%c",crt_cid,SEPARATOR,SEPARATOR,supp->suppkey,SEPARATOR);
       PR_VSTR_LAST(supp_s_comment, supp->comment,
		    (columnar)?(long)(ceil(S_CMNT_LEN * V_STR_HGH)):supp->clen);
       PR_END (supp_s_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(SUPP,(long)supp->suppkey,7);

   return(0);
}

int
pr_nation(code_t *c, int mode)
{
  DSS_HUGE j;
  threshold = upper * placeholders_ratio;

  RANDOM(j,0,upper,NATION_PLACEHOLDERS);

  if (TUPLE_LEVEL) {
    if ((long)j <threshold+1) {
      add_placeholder(NATION,(long)c->code,0);
      return(0);
    }
    threshold = -1;
  }


  if ((long)j > threshold) {
    if (!COUNT_ONLY) {
       PR_STRT(nation_n_nationkey);
       fprintf(nation_n_nationkey, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR,
	       (long)c->code);
       PR_END (nation_n_nationkey);
       stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
       crt_cid++;
     }
  }
  else
      add_placeholder(NATION,(long)c->code,1);


   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,NATION_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(nation_n_name);
       fprintf(nation_n_name, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR);
       PR_STR(nation_n_name, c->text, NATION_LEN);
       PR_END (nation_n_name);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(NATION,(long)c->code,2);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,NATION_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(nation_n_regionkey);
       fprintf(nation_n_regionkey, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR);
       PR_INT(nation_n_regionkey, c->join);
       PR_END (nation_n_regionkey);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(NATION,(long)c->code,3);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,NATION_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(nation_n_comment);
       fprintf(nation_n_comment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR);
       PR_VSTR_LAST(nation_n_comment, c->comment,
		    (columnar)?(long)(ceil(N_CMNT_LEN * V_STR_HGH)):c->clen);
       PR_END (nation_n_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
      add_placeholder(NATION,(long)c->code,4);

   return(0);
}

int
pr_region(code_t *c, int mode)
{
  DSS_HUGE j;
  threshold = upper * placeholders_ratio;

  RANDOM(j,0,upper,REGION_PLACEHOLDERS);

  if (TUPLE_LEVEL) {
    if ((long)j <threshold+1) {
      add_placeholder(REGION,(long)c->code,0);
      return(0);
    }
    threshold = -1;
  }

  if ((long)j > threshold) {
    if (!COUNT_ONLY) {
      PR_STRT(region_r_regionkey);
      fprintf(region_r_regionkey, "%ld%c1%c%ld%c%ld",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR,(long)c->code);
      PR_END (region_r_regionkey);
             stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
    }
  }
  else
    add_placeholder(REGION,(long)c->code,1);

   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,REGION_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(region_r_name);
       fprintf(region_r_name, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR);
       PR_STR(region_r_name, c->text, REGION_LEN);
       PR_END (region_r_name);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
    add_placeholder(REGION,(long)c->code,2);


   if (!TUPLE_LEVEL)
     RANDOM(j,0,upper,REGION_PLACEHOLDERS);
   if ((long)j > threshold) {
     if (!COUNT_ONLY) {
       PR_STRT(region_r_comment);
       fprintf(region_r_comment, "%ld%c1%c%ld%c",crt_cid,SEPARATOR,SEPARATOR,(long)c->code,SEPARATOR);
       PR_VSTR_LAST(region_r_comment, c->comment,
		    (columnar)?(long)(ceil(R_CMNT_LEN * V_STR_HGH)):c->clen);
       PR_END (region_r_comment);
              stat_avg_lworlds += (1-stat_avg_lworlds)/crt_cid;
crt_cid++;
     }
   }
   else
     add_placeholder(REGION,(long)c->code,3);

   return(0);
}

/*
 * NOTE: this routine does NOT use the BCD2_* routines. As a result,
 * it WILL fail if the keys being deleted exceed 32 bits. Since this
 * would require ~660 update iterations, this seems an acceptable
 * oversight
 */
int
pr_drange(int tbl, DSS_HUGE min, DSS_HUGE cnt, long num)
{
    static int  last_num = 0;
    static FILE *dfp = NULL;
    DSS_HUGE child = -1;
    DSS_HUGE start, last, new;

	static DSS_HUGE rows_per_segment=0;
	static DSS_HUGE rows_this_segment=0;

    if (last_num != num)
        {
        if (dfp)
            fclose(dfp);
        dfp = print_prep(tbl, -num);
        if (dfp == NULL)
            return(-1);
        last_num = num;
		rows_this_segment=0;
        }

    start = MK_SPARSE(min, num/ (10000 / refresh));
    last = start - 1;
    for (child=min; cnt > 0; child++, cnt--)
        {
        new = MK_SPARSE(child, num/ (10000 / refresh));
        if (gen_rng == 1 && new - last == 1)
            {
            last = new;
            continue;
            }
	if (gen_sql)
	    {
	    fprintf(dfp,
		"delete from %s where %s between %lld and %lld;\n",
		    tdefs[ORDER].name, "o_orderkey", start, last);
	    fprintf(dfp,
		"delete from %s where %s between %lld and %lld;\n",
		    tdefs[LINE].name, "l_orderkey", start, last);
	    fprintf(dfp, "commit work;\n");
	    }
	else
	    if (gen_rng)
                {
                PR_STRT(dfp);
                PR_HUGE(dfp, &start);
                PR_HUGE(dfp, &last);
                PR_END(dfp);
                }
            else
                {
				if (delete_segments)
					{

					if(rows_per_segment==0)
						rows_per_segment = (cnt / delete_segments) + 1;
					if((++rows_this_segment) > rows_per_segment)
						{
						fclose(dfp);
						dfp = print_prep(tbl, -num);
						if (dfp == NULL) return(-1);
						last_num = num;
						rows_this_segment=1;
						}
					}
                PR_STRT(dfp);
                PR_HUGE(dfp, &new);
                PR_END(dfp);
                }
	start = new;
	last = new;
        }
    if (gen_rng)
	{
	PR_STRT(dfp);
	PR_HUGE(dfp, &start);
	PR_HUGE(dfp, &last);
	PR_END(dfp);
	}

    return(0);
}

/*
 * verify functions: routines which replace the pr_routines and generate a pseudo checksum
 * instead of generating the actual contents of the tables. Meant to allow large scale data
 * validation without requiring a large amount of storage
 */
int
vrf_cust(customer_t *c, int mode)
{
   VRF_STRT(CUST);
   VRF_INT(CUST, c->custkey);
   VRF_STR(CUST, c->name);
   VRF_STR(CUST, c->address);
   VRF_INT(CUST, c->nation_code);
   VRF_STR(CUST, c->phone);
   VRF_MONEY(CUST, c->acctbal);
   VRF_STR(CUST, c->mktsegment);
   VRF_STR(CUST, c->comment);
   VRF_END(CUST);

   return(0);
}

/*
 * print the numbered order
 */
int
vrf_order(order_t *o, int mode)
{
    VRF_STRT(ORDER);
    VRF_HUGE(ORDER, o->okey);
    VRF_INT(ORDER, o->custkey);
    VRF_CHR(ORDER, o->orderstatus);
    VRF_MONEY(ORDER, o->totalprice);
    VRF_STR(ORDER, o->odate);
    VRF_STR(ORDER, o->opriority);
    VRF_STR(ORDER, o->clerk);
    VRF_INT(ORDER, o->spriority);
    VRF_STR(ORDER, o->comment);
    VRF_END(ORDER);

    return(0);
}

/*
 * print an order's lineitems
 */
int
vrf_line(order_t *o, int mode)
{
	int i;

    for (i = 0; i < o->lines; i++)
        {
        VRF_STRT(LINE);
        VRF_HUGE(LINE, o->l[i].okey);
        VRF_INT(LINE, o->l[i].partkey);
        VRF_INT(LINE, o->l[i].suppkey);
        VRF_INT(LINE, o->l[i].lcnt);
        VRF_INT(LINE, o->l[i].quantity);
        VRF_MONEY(LINE, o->l[i].eprice);
        VRF_MONEY(LINE, o->l[i].discount);
        VRF_MONEY(LINE, o->l[i].tax);
        VRF_CHR(LINE, o->l[i].rflag[0]);
        VRF_CHR(LINE, o->l[i].lstatus[0]);
        VRF_STR(LINE, o->l[i].sdate);
        VRF_STR(LINE, o->l[i].cdate);
        VRF_STR(LINE, o->l[i].rdate);
        VRF_STR(LINE, o->l[i].shipinstruct);
        VRF_STR(LINE, o->l[i].shipmode);
        VRF_STR(LINE, o->l[i].comment);
        VRF_END(LINE);
        }

   return(0);
}

/*
 * print the numbered order *and* its associated lineitems
 */
int
vrf_order_line(order_t *o, int mode)
{
    vrf_order(o, mode);
    vrf_line(o, mode);

    return(0);
}

/*
 * print the given part
 */
int
vrf_part(part_t *part, int mode)
{

   VRF_STRT(PART);
   VRF_INT(PART, part->partkey);
   VRF_STR(PART, part->name);
   VRF_STR(PART, part->mfgr);
   VRF_STR(PART, part->brand);
   VRF_STR(PART, part->type);
   VRF_INT(PART, part->size);
   VRF_STR(PART, part->container);
   VRF_MONEY(PART, part->retailprice);
   VRF_STR(PART, part->comment);
   VRF_END(PART);

   return(0);
}

/*
 * print the given part's suppliers
 */
int
vrf_psupp(part_t *part, int mode)
{
    long      i;

   for (i = 0; i < SUPP_PER_PART; i++)
      {
      VRF_STRT(PSUPP);
      VRF_INT(PSUPP, part->s[i].partkey);
      VRF_INT(PSUPP, part->s[i].suppkey);
      VRF_INT(PSUPP, part->s[i].qty);
      VRF_MONEY(PSUPP, part->s[i].scost);
      VRF_STR(PSUPP, part->s[i].comment);
      VRF_END(PSUPP);
      }

   return(0);
}

/*
 * print the given part *and* its suppliers
 */
int
vrf_part_psupp(part_t *part, int mode)
{
    vrf_part(part, mode);
    vrf_psupp(part, mode);

    return(0);
}

int
vrf_supp(supplier_t *supp, int mode)
{
   VRF_STRT(SUPP);
   VRF_INT(SUPP, supp->suppkey);
   VRF_STR(SUPP, supp->name);
   VRF_STR(SUPP, supp->address);
   VRF_INT(SUPP, supp->nation_code);
   VRF_STR(SUPP, supp->phone);
   VRF_MONEY(SUPP, supp->acctbal);
   VRF_STR(SUPP, supp->comment);
   VRF_END(SUPP);

   return(0);
}

int
vrf_nation(code_t *c, int mode)
{
   VRF_STRT(NATION);
   VRF_INT(NATION, c->code);
   VRF_STR(NATION, c->text);
   VRF_INT(NATION, c->join);
   VRF_STR(NATION, c->comment);
   VRF_END(NATION);

   return(0);
}

int
vrf_region(code_t *c, int mode)
{
   VRF_STRT(REGION);
   VRF_INT(REGION, c->code);
   VRF_STR(REGION, c->text);
   VRF_STR(REGION, c->comment);
   VRF_END(fp);

   return(0);
}
