/*
* $Id: driver.c,v 1.21 2007/03/21 04:41:00 olteanu Exp $
*
* Revision History
* ===================
* $Log: driver.c,v $
* Revision 1.21  2007/03/21 04:41:00  olteanu
* *** empty log message ***
*
* Revision 1.20  2007/03/21 04:05:15  olteanu
* *** empty log message ***
*
* Revision 1.19  2007/03/20 16:54:17  olteanu
* *** empty log message ***
*
* Revision 1.18  2007/03/19 21:07:58  olteanu
* *** empty log message ***
*
* Revision 1.17  2007/03/19 14:52:30  olteanu
* *** empty log message ***
*
* Revision 1.16  2007/03/19 01:03:31  olteanu
* *** empty log message ***
*
* Revision 1.15  2007/03/18 22:46:49  olteanu
* *** empty log message ***
*
* Revision 1.14  2007/03/18 20:55:32  olteanu
* *** empty log message ***
*
* Revision 1.13  2007/03/18 13:04:39  olteanu
* *** empty log message ***
*
* Revision 1.12  2007/03/17 00:37:04  olteanu
* *** empty log message ***
*
* Revision 1.11  2007/03/16 23:50:37  olteanu
* *** empty log message ***
*
* Revision 1.10  2007/03/16 22:57:42  olteanu
* *** empty log message ***
*
* Revision 1.9  2007/03/16 21:30:50  olteanu
* *** empty log message ***
*
* Revision 1.8  2007/03/15 18:49:36  olteanu
* *** empty log message ***
*
* Revision 1.7  2007/03/15 13:09:32  olteanu
* *** empty log message ***
*
* Revision 1.6  2007/03/15 12:31:50  olteanu
* *** empty log message ***
*
* Revision 1.5  2007/03/14 21:03:01  olteanu
* *** empty log message ***
*
* Revision 1.4  2007/03/14 20:21:53  olteanu
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
* Revision 1.4  2007/03/06 15:46:22  olteanu
* *** empty log message ***
*
* Revision 1.3  2007/03/04 18:41:02  olteanu
* *** empty log message ***
*
* Revision 1.2  2007/03/02 18:13:30  olteanu
* *** empty log message ***
*
* Revision 1.1.1.1  2007/03/01 18:11:56  olteanu
* added functionality to generate U-relations
*
*/
/* main driver for dss banchmark */

#define DECLARER				/* EXTERN references get defined here */
#define NO_FUNC (int (*) ()) NULL	/* to clean up tdefs */
#define NO_LFUNC (long (*) ()) NULL		/* to clean up tdefs */

#define V_STR(avg, sd, tgt)  a_rnd((int)(avg * V_STR_LOW),(int)(avg * V_STR_HGH), sd, tgt)
#define TEXT(avg, sd, tgt)  dbg_text(tgt, (int)(avg * V_STR_LOW),(int)(avg * V_STR_HGH), sd)

#include "config.h"
#include "release.h"
#include "dss.h"
#include <stdlib.h>
#if (defined(_POSIX_)||!defined(WIN32))		/* Change for Windows NT */
#include <unistd.h>
#include <sys/wait.h>
#endif /* WIN32 */
#include <stdio.h>				/* */
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#ifdef HP
#include <strings.h>
#endif
#if (defined(WIN32)&&!defined(_POSIX_))
#include <process.h>
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#pragma warning(disable:4514)
#define WIN32_LEAN_AND_MEAN
#define NOATOM
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOMCX
#include <windows.h>
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif

#include "dss.h"
#include "dsstypes.h"
#include "print-proto.h"



extern int optind, opterr;
extern char *optarg;
DSS_HUGE rowcnt = 0, minrow = 0;
long upd_num = 0;
double flt_scale;
#if (defined(WIN32)&&!defined(_POSIX_))
char *spawn_args[25];
#endif


/*
* general table descriptions. See dss.h for details on structure
* NOTE: tables with no scaling info are scaled according to
* another table
*
*
* the following is based on the tdef structure defined in dss.h as:
* typedef struct
* {
* char     *name;            -- name of the table;
*                               flat file output in <name>.tbl
* long      base;            -- base scale rowcount of table;
*                               0 if derived
* int       (*header) ();    -- function to prep output
* int       (*loader[2]) (); -- functions to present output
* long      (*gen_seed) ();  -- functions to seed the RNG
* int       (*verify) ();    -- function to verfiy the data set without building it
* int       child;           -- non-zero if there is an associated detail table
* unsigned long vtotal;      -- "checksum" total
* }         tdef;
*
*/

/*
* flat file print functions; used with -F(lat) option
*/
int pr_cust (customer_t * c, int mode);
int pr_line (order_t * o, int mode);
int pr_order (order_t * o, int mode);
int pr_part (part_t * p, int mode);
int pr_psupp (part_t * p, int mode);
int pr_supp (supplier_t * s, int mode);
int pr_order_line (order_t * o, int mode);
int pr_part_psupp (part_t * p, int mode);
int pr_nation (code_t * c, int mode);
int pr_region (code_t * c, int mode);
int pr_component_value(int table, long tid, int attr, int cid, DSS_HUGE wid, void* value);

/*
* inline load functions; used with -D(irect) option
*/
int ld_cust (customer_t * c, int mode);
int ld_line (order_t * o, int mode);
int ld_order (order_t * o, int mode);
int ld_part (part_t * p, int mode);
int ld_psupp (part_t * p, int mode);
int ld_supp (supplier_t * s, int mode);
int ld_order_line (order_t * o, int mode);
int ld_part_psupp (part_t * p, int mode);
int ld_nation (code_t * c, int mode);
int ld_region (code_t * c, int mode);

/*
* seed generation functions; used with '-O s' option
*/
long sd_cust (int child, DSS_HUGE skip_count);
long sd_line (int child, DSS_HUGE skip_count);
long sd_order (int child, DSS_HUGE skip_count);
long sd_part (int child, DSS_HUGE skip_count);
long sd_psupp (int child, DSS_HUGE skip_count);
long sd_supp (int child, DSS_HUGE skip_count);
long sd_order_line (int child, DSS_HUGE skip_count);
long sd_part_psupp (int child, DSS_HUGE skip_count);

/*
* header output functions); used with -h(eader) option
*/
int hd_cust (FILE * f);
int hd_line (FILE * f);
int hd_order (FILE * f);
int hd_part (FILE * f);
int hd_psupp (FILE * f);
int hd_supp (FILE * f);
int hd_order_line (FILE * f);
int hd_part_psupp (FILE * f);
int hd_nation (FILE * f);
int hd_region (FILE * f);

/*
* data verfication functions; used with -O v option
*/
int vrf_cust (customer_t * c, int mode);
int vrf_line (order_t * o, int mode);
int vrf_order (order_t * o, int mode);
int vrf_part (part_t * p, int mode);
int vrf_psupp (part_t * p, int mode);
int vrf_supp (supplier_t * s, int mode);
int vrf_order_line (order_t * o, int mode);
int vrf_part_psupp (part_t * p, int mode);
int vrf_nation (code_t * c, int mode);
int vrf_region (code_t * c, int mode);


tdef tdefs[] =
{
	{"part.tbl", "part table", 200000, hd_part,
		{pr_part, ld_part}, sd_part, vrf_part, PSUPP, 0},
	{"partsupp.tbl", "partsupplier table", 200000, hd_psupp,
		{pr_psupp, ld_psupp}, sd_psupp, vrf_psupp, NONE, 0},
	{"supplier.tbl", "suppliers table", 10000, hd_supp,
		{pr_supp, ld_supp}, sd_supp, vrf_supp, NONE, 0},
	{"customer.tbl", "customers table", 150000, hd_cust,
		{pr_cust, ld_cust}, sd_cust, vrf_cust, NONE, 0},
	{"orders.tbl", "order table", 150000, hd_order,
		{pr_order, ld_order}, sd_order, vrf_order, LINE, 0},
	{"lineitem.tbl", "lineitem table", 150000, hd_line,
		{pr_line, ld_line}, sd_line, vrf_line, NONE, 0},
	{"orders.tbl", "orders/lineitem tables", 150000, hd_order_line,
		{pr_order_line, ld_order_line}, sd_order, vrf_order_line, LINE, 0},
	{"part.tbl", "part/partsupplier tables", 200000, hd_part_psupp,
		{pr_part_psupp, ld_part_psupp}, sd_part, vrf_part_psupp, PSUPP, 0},
	{"nation.tbl", "nation table", NATIONS_MAX, hd_nation,
		{pr_nation, ld_nation}, NO_LFUNC, vrf_nation, NONE, 0},
	{"region.tbl", "region table", NATIONS_MAX, hd_region,
		{pr_region, ld_region}, NO_LFUNC, vrf_region, NONE, 0},
	//
	{"part_p_partkey.tbl", "Part.P_Partkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_name.tbl", "Part.P_Name", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_mfgr.tbl", "Part.P_Mfgr", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_brand.tbl", "Part.P_Brand", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_type.tbl", "Part.P_Type", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_size.tbl", "Part.P_Size", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_container.tbl", "Part.P_Container", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_retailprice.tbl", "Part.P_Retailprice", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"part_p_comment.tbl", "Part.P_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"supp_s_suppkey.tbl", "Supp.S_Suppkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_name.tbl", "Supp.S_Name", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_address.tbl", "Supp.S_Address", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_nationkey.tbl", "Supp.S_Nationkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_phone.tbl", "Supp.S_Phone", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_acctbal.tbl", "Supp.S_Acctbal", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"supp_s_comment.tbl", "Supp.S_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"psupp_ps_partkey.tbl", "Psupp.PS_Partkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"psupp_ps_suppkey.tbl", "Psupp.PS_Suppkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"psupp_ps_availqty.tbl", "Psupp.PS_Availqty", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"psupp_ps_supplycost.tbl", "Psupp.PS_Supplycost", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"psupp_ps_comment.tbl", "Psupp.PS_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"cust_c_custkey.tbl", "Cust.C_Custkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_name.tbl", "Cust.C_Name", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_address.tbl", "Cust.C_Address", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_nationkey.tbl", "Cust.C_Nationkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_phone.tbl", "Cust.C_Phone", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_acctbal.tbl", "Cust.C_Acctbal", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_mktsegment.tbl", "Cust.C_Mktsegment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"cust_c_comment.tbl", "Cust.C_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"orders_o_orderkey.tbl", "Orders.O_Orderkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_custkey.tbl", "Orders.O_Custkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_orderstatus.tbl", "Orders.O_Orderstatus", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_totalprice.tbl", "Orders.O_Totalprice", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_orderdate.tbl", "Orders.O_Orderdate", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_orderpriority.tbl", "Orders.O_Orderpriority", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_clerk.tbl", "Orders.O_Clerk", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_shippriority.tbl", "Orders.O_Shippriority", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"orders_o_comment.tbl", "Orders.O_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"lineitem_l_orderkey.tbl", "Lineitem.L_Orderkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_partkey.tbl", "Lineitem.L_Partkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_suppkey.tbl", "Lineitem.L_Suppkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_linenumber.tbl", "Lineitem.L_Linenumber", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_quantity.tbl", "Lineitem.L_Quantity", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_extendedprice.tbl", "Lineitem.L_Extendedprice", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_discount.tbl", "Lineitem.L_Discount", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_tax.tbl", "Lineitem.L_Tax", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_returnflag.tbl", "Lineitem.L_Returnflag", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_linestatus.tbl", "Lineitem.L_Linestatus", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_shipdate.tbl", "Lineitem.L_Shipdate", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_commitdate.tbl", "Lineitem.L_Commitdate", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_receiptdate.tbl", "Lineitem.L_Receiptdate", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_shipinstruct.tbl", "Lineitem.L_Shipinstruct", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_shipmode.tbl", "Lineitem.L_Shipmode", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"lineitem_l_comment.tbl", "Lineitem.L_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"nation_n_nationkey.tbl", "Nation.N_Nationkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"nation_n_name.tbl", "Nation.N_Name", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"nation_n_regionkey.tbl", "Nation.N_Regionkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"nation_n_comment.tbl", "Nation.N_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	//
	{"region_r_regionkey.tbl", "Region.R_Nationkey", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"region_r_name.tbl", "Region.R_Name", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0},
	{"region_r_comment.tbl", "Region.R_Comment", 0, hd_part,
		{pr_component_value, ld_part}, NO_LFUNC, vrf_part, NONE, 0}
};

int *pids;


/*
* routines to handle the graceful cleanup of multi-process loads
*/

void
stop_proc (int signum)
{
	exit (0);
}

void
kill_load (void)
{
	int i;

#if !defined(U2200) && !defined(DOS)
	for (i = 0; i < children; i++)
		if (pids[i])
			KILL (pids[i]);
#endif /* !U2200 && !DOS */
		return;
}

/*
* re-set default output file names
*/
int
set_files (int i, int pload)
{
	char line[80], *new_name;

	if (table & (1 << i))
child_table:
	{
		if (pload != -1)
			sprintf (line, "%s.%d", tdefs[i].name, pload);
		else
		{
			printf ("Enter new destination for %s data: ",
				tdefs[i].name);
			if (fgets (line, sizeof (line), stdin) == NULL)
				return (-1);;
			if ((new_name = strchr (line, '\n')) != NULL)
				*new_name = '\0';
			if (strlen (line) == 0)
				return (0);
		}
		new_name = (char *) malloc (strlen (line) + 1);
		MALLOC_CHECK (new_name);
		strcpy (new_name, line);
		tdefs[i].name = new_name;
		if (tdefs[i].child != NONE)
		{
			i = tdefs[i].child;
			tdefs[i].child = NONE;
			goto child_table;
		}
	}

	return (0);
}



/*
* read the distributions needed in the benchamrk
*/
void
load_dists (void)
{
	read_dist (env_config (DIST_TAG, DIST_DFLT), "p_cntr", &p_cntr_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "colors", &colors);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "p_types", &p_types_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "nations", &nations);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "regions", &regions);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "o_oprio",
		&o_priority_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "instruct",
		&l_instruct_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "smode", &l_smode_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "category",
		&l_category_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "rflag", &l_rflag_set);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "msegmnt", &c_mseg_set);

	/* load the distributions that contain text generation */
	read_dist (env_config (DIST_TAG, DIST_DFLT), "nouns", &nouns);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "verbs", &verbs);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "adjectives", &adjectives);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "adverbs", &adverbs);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "auxillaries", &auxillaries);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "terminators", &terminators);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "articles", &articles);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "prepositions", &prepositions);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "grammar", &grammar);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "np", &np);
	read_dist (env_config (DIST_TAG, DIST_DFLT), "vp", &vp);

}

/* added by olteanu for U-relations */

/*
 * do some shuffling on the array of placeholders
 */

void
shuffle_placeholders()
{
  if (placeholders_idx < SWAP_SIZE)
    return;

  DSS_HUGE range1, range2, size;
  Placeholder *placeholder;

  int iterations = floor(placeholders_idx * SHUFFLE_ITE_RATIO);
  int i = 0;


  fprintf (stderr, "Shuffle %ld placeholders in %d iterations.\n",placeholders_idx, iterations);

  while (i < iterations)
    {
      RANDOM(size,3,(long)SWAP_SIZE,WSD_PLACEHOLDERS);

      RANDOM(range1,0,placeholders_idx-size-1,WSD_PLACEHOLDERS);
      RANDOM(range2,0,placeholders_idx-size-1,WSD_PLACEHOLDERS);
      //fprintf (stderr, "Shuffle %d placeholders from position %ld to position %ld.\n",size, (long)range1, (long)range2);

      int j = 0;
      while (j < size)
	{
	  placeholder = placeholder_set[range1+j];
	  placeholder_set[range1+j] = placeholder_set[range2+size-j];
	  placeholder_set[range2+size-j] = placeholder;
	  if (placeholder == NULL)
	    {
	      fprintf (stderr, "Problem with swapping: %ld in [0,%ld].\n",(long)(range1+j), placeholders_idx);
	      exit(-1);
	    }
	  if (placeholder_set[range1+j] == NULL)
	    {
	      fprintf (stderr, "Problem with swapping %ld in [0,%ld].\n",(long)(range2+SWAP_SIZE-j), placeholders_idx);
	      exit(-1);
	    }
	  j++;
	}
      i++;
    }
}



/*
 * approximate the number of or-sets and the maximal arity of generated components.
 */
long
approx_zipf_distr(long size, double zipf, int delta)
{
  int ready = 0;
  long x = size, sum;
  int arity;
  double temp;

  while (!ready)
    {
      sum = x;
      temp = x;
      arity = 2;
      //fprintf(stderr,"Try with %ld..",x);

      while (temp > 2 && sum < size+1)
	{
	  temp = temp*zipf;
	  sum += (long)floor(temp)*arity;
	  arity++;
	  //fprintf(stderr,"(temp %ld,sum %ld)",(long)floor(temp),sum);
	}
      if (sum > size)
	x -= delta;
      else ready = 1;
      //fprintf(stderr,"\n",x);
    }
  fprintf(stderr,"Zipf SOLUTION: init size %ld, computed size %ld, or-sets %ld, ratio %g \n",size, sum, x, zipf);

  return x;
}

/*
 * insert a value in the component relation
 */
long
prepare_output(unsigned table, long tid, unsigned attr, void* value, long lworlds, long nvalues, long worlds_used)
{
  if (ONE_WORLD)
    {
      pr_component_value(table,tid,attr,crt_cid,1,value);
      return 1;
    }

  long t = (long)lworlds/nvalues;
  if (t == 0 && worlds_used < lworlds)
    t = 1;
  long s = 0;
  while (s < t)
    {
      pr_component_value(table,tid,attr,crt_cid,s+1+worlds_used,value);
      s++;
    }
  worlds_used += t;
  return worlds_used;
}



/*
 * generate a component relation defining values for all placeholders from columns
 */
void
gen_component_tuplelevel (int arity, long idx)
{
    if (idx+arity > placeholders_size+1)
	arity = placeholders_size-idx+1;

    if (arity == 0)
    {
	fprintf(stderr,"nullary component for cid %ld !\n",crt_cid);
	return;
    }

    DSS_HUGE j,l,p;

    static long x = 0;
    long i,k,m,worlds_used;

    long *nvalues = (long*)malloc(arity * sizeof(long));
    double local_worlds = 1.0;

    /* compute the number of different values for each column of the component */
    m = 0;
    while (m < arity)
    {
	if (ONE_WORLD)
	    nvalues[m] = 1;
	else
	{
	    RANDOM(j,2,max_placeholders,WSD_PLACEHOLDERS);
	    nvalues[m] = (long)j;
	    local_worlds *= nvalues[m]*LOCAL_WORLDS_RATIO_INIT;
	}
	m++;
    }
    if (!ONE_WORLD)
        local_worlds /= LOCAL_WORLDS_RATIO_INIT;

  /* compute the number of local worlds for this component */

  //RANDOM(j,1,(DSS_HUGE)max_worlds/worlds_drop-1,WSD_PLACEHOLDERS);

    long lworlds = (long)ceil(local_worlds+0.5);
    stat_total_amount_worlds += log10(lworlds);

    if (lworlds > stat_max_lworlds)
        stat_max_lworlds = lworlds;

    stat_avg_lworlds += (lworlds-stat_avg_lworlds)/crt_cid;


  if (COUNT_ONLY) {
      free (nvalues);
      for(i=0;i<arity;i++)
	  free (placeholder_set[idx+i]);
      return;
  }

  static char **asc_date = NULL;
  char **mk_ascdate PROTO((void));
  static int bInit = 0;
  static char szFormat[100];
  static char szBrandFormat[100];

  if (!bInit)
    {
      sprintf(szFormat, O_CLRK_FMT, 9, HUGE_FORMAT + 1);
      sprintf(szBrandFormat, P_BRND_FMT, 2, HUGE_FORMAT + 1);
      bInit = 1;
    }
  if (asc_date == NULL)
    asc_date = mk_ascdate();

  char *value;

  /* pick up values for each column and output them */
  for(i=0; i < arity;i++)
    {
      unsigned table = placeholder_set[idx+i]->table;
      long tid       = placeholder_set[idx+i]->tid;

      worlds_used = 0;

      for(k=0;k<nvalues[i];k++)
	{
	  switch(table)
	    {
	    case CUST:

	      //CUSTKEY
	      RANDOM(l,1,(tdefs[CUST].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1,&l,j,nvalues[i], worlds_used);

	      //NAME
	      RANDOM(l,1,(tdefs[CUST].base * scale - 1),WSD_PLACEHOLDERS);
	      value =  (char*)malloc((C_NAME_LEN+3)*sizeof(char));
	      sprintf(value, szFormat, C_NAME_TAG, (long)l);
	      prepare_output(table,tid,2,value,j,nvalues[i], worlds_used);
	      free (value);

	      //ADDRESS
	      value =  (char*)malloc((C_ADDR_MAX + 1)*sizeof(char));
	      V_STR(C_ADDR_LEN, C_ADDR_SD, value);
	      prepare_output(table,tid,3,value,j,nvalues[i], worlds_used);
	      free (value);

	      // NATIONKEY
	      RANDOM(l, 0, (nations.count - 1), C_NTRG_SD);
	      prepare_output(table,tid,4,&l,j,nvalues[i], worlds_used);

	      // PHONE
	      value =  (char*)malloc((PHONE_LEN + 1)*sizeof(char));
	      gen_phone((DSS_HUGE)tid, value, (long)C_PHNE_SD);
	      prepare_output(table,tid,5,value,j,nvalues[i], worlds_used);
	      free (value);

	      //ACCTBAL
	      RANDOM(l, C_ABAL_MIN, C_ABAL_MAX, C_ABAL_SD);
	      prepare_output(table,tid,6,&l,j,nvalues[i], worlds_used);

	      // MKTSEGMENT
	      value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
	      pick_str(&c_mseg_set, C_MSEG_SD, value);
	      prepare_output(table,tid,7,value,j,nvalues[i], worlds_used);
	      free (value);

	      // COMMENT
	      value =  (char*)malloc((C_CMNT_MAX + 1)*sizeof(char));
	      TEXT(C_CMNT_LEN, C_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,8,value,j,nvalues[i], worlds_used);
	      free (value);

	      break;

	    case ORDER:
	      // ORDERKEY
	      RANDOM(l,1,(tdefs[ORDER].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1,&l,j,nvalues[i], worlds_used);

	      // CUSTKEY
	      if (scale >= 30000)
		RANDOM64(l, O_CKEY_MIN, O_CKEY_MAX, O_CKEY_SD);
	      else
		RANDOM(l, O_CKEY_MIN, O_CKEY_MAX, O_CKEY_SD);
	      prepare_output(table,tid,2,&l,j,nvalues[i], worlds_used);

	      // ORDERSTATUS -- SUBJECT TO CHANGE
	      prepare_output(table,tid,3, &x,j,nvalues[i], worlds_used);

	      // TOTALPRICE
	      RANDOM(l,1,3000000,WSD_PLACEHOLDERS);
	      prepare_output(table,tid,4,&l,j,nvalues[i], worlds_used);

	      // ORDERDATE
	      RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
	      value =  (char*)malloc((DATE_LEN)*sizeof(char));
	      strcpy(value, asc_date[l - STARTDATE]);
	      prepare_output(table,tid,5, value,j,nvalues[i], worlds_used);
	      free (value);

	      // ORDER_PRIORITY
	      value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
	      pick_str(&o_priority_set, O_PRIO_SD, value);
	      prepare_output(table,tid,6, value,j,nvalues[i], worlds_used);
	      free (value);

	      // CLERK
	      RANDOM(l, 1, MAX((scale * O_CLRK_SCL), O_CLRK_SCL), O_CLRK_SD);
	      value =  (char*)malloc((O_CLRK_LEN + 1)*sizeof(char));
	      sprintf(value, szFormat, O_CLRK_TAG, l);
	      prepare_output(table,tid,7, value,j,nvalues[i], worlds_used);
	      free (value);

	      // SHIP_PRIORITY
	      prepare_output(table,tid,8, &x,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((O_CMNT_MAX + 1)*sizeof(char));
	      TEXT(O_CMNT_LEN, O_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,9, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;


	    case LINE:
	      // ORDERKEY
	      RANDOM(l,1,(tdefs[ORDER].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // PARTKEY
	      RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,2, &l,j,nvalues[i], worlds_used);

	      // SUPPKEY
	      RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,3, &l,j,nvalues[i], worlds_used);

	      // LINENUMBER
	      RANDOM(l, O_LCNT_MIN, O_LCNT_MAX, O_LCNT_SD);
	      prepare_output(table,tid,4, &l,j,nvalues[i], worlds_used);

	      // QUANTITY
	      RANDOM(l, L_QTY_MIN, L_QTY_MAX, L_QTY_SD);
	      prepare_output(table,tid,5, &l,j,nvalues[i], worlds_used);

	      // EXTENDEDPRICE
	      RANDOM(l,1,3000000,WSD_PLACEHOLDERS);
	      prepare_output(table,tid,6, &l,j,nvalues[i], worlds_used);

	      // DISCOUNT
	      RANDOM(l, L_DCNT_MIN, L_DCNT_MAX, L_DCNT_SD);
	      prepare_output(table,tid,7, &l,j,nvalues[i], worlds_used);

	      // TAX
	      RANDOM(l, L_TAX_MIN, L_TAX_MAX, L_TAX_SD);
	      prepare_output(table,tid,8, &l,j,nvalues[i], worlds_used);

	      // RETURNFLAG
	      value =  (char*)malloc(2*sizeof(char));
	      pick_str(&l_rflag_set, L_RFLG_SD, value);
	      prepare_output(table,tid,9, value,j,nvalues[i], worlds_used);
	      free (value);

	      // LINESTATUS
	      prepare_output(table,tid,10, &x,j,nvalues[i], worlds_used);

	      // SHIPDATE; make it more particular, see build.c
	      RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
	      value =  (char*)malloc((DATE_LEN)*sizeof(char));
	      strcpy(value, asc_date[l - STARTDATE]);
	      prepare_output(table,tid,11, value,j,nvalues[i], worlds_used);
	      free (value);

	      // COMMITDATE
	      //RANDOM(l, L_CDTE_MIN, L_CDTE_MAX, L_CDTE_SD);
	      RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
	      value =  (char*)malloc((DATE_LEN)*sizeof(char));
	      strcpy(value, asc_date[l - STARTDATE]);
	      prepare_output(table,tid,12, value,j,nvalues[i], worlds_used);
	      free (value);

	      // RECEIPTDATE
	      //RANDOM(l, L_RDTE_MIN, L_RDTE_MAX, L_RDTE_SD);
	      value =  (char*)malloc((DATE_LEN)*sizeof(char));
	      RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
	      strcpy(value, asc_date[l - STARTDATE]);
	      prepare_output(table,tid,13, value,j,nvalues[i], worlds_used);
	      free (value);

	      // SHIPINSTRUCT
	      value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
	      pick_str(&l_instruct_set, L_SHIP_SD, value);
	      prepare_output(table,tid,14, value,j,nvalues[i], worlds_used);
	      free (value);

	      // SHIPMODE
	      value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
	      pick_str(&l_smode_set, L_SMODE_SD, value);
	      prepare_output(table,tid,15, value,j,nvalues[i], worlds_used);
	      free (value);

	      // COMMENT
	      value =  (char*)malloc((L_CMNT_MAX + 1)*sizeof(char));
	      TEXT(L_CMNT_LEN, L_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,16, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;


	    case PART:
	      // PARTKEY
	      RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // NAME
	      value =  (char*)malloc((P_NAME_LEN+1)*sizeof(char));
	      agg_str(&colors, (long)P_NAME_SCL, (long)P_NAME_SD, value);
	      prepare_output(table,tid,2, value,j,nvalues[i], worlds_used);
	      free (value);

	      // MFGR
	      RANDOM(l, P_MFG_MIN, P_MFG_MAX, P_MFG_SD);
	      value =  (char*)malloc((P_MFG_LEN+1)*sizeof(char));
	      sprintf(value, szFormat, P_MFG_TAG, l);
	      prepare_output(table,tid,3, value,j,nvalues[i], worlds_used);
	      free (value);

	      // BRAND
	      RANDOM(p, P_MFG_MIN, P_MFG_MAX, P_MFG_SD);
	      RANDOM(l, P_BRND_MIN, P_BRND_MAX, P_BRND_SD);
	      value =  (char*)malloc((P_BRND_LEN+1)*sizeof(char));
	      sprintf(value, szBrandFormat, P_BRND_TAG, (p * 10 + l));
	      prepare_output(table,tid,4, value,j,nvalues[i], worlds_used);
	      free (value);

	      // TYPE
	      value =  (char*)malloc((P_TYPE_LEN+1)*sizeof(char));
	      pick_str(&p_types_set, P_TYPE_SD, value);
	      prepare_output(table,tid,5, value,j,nvalues[i], worlds_used);
	      free (value);

	      // SIZE
	      RANDOM(l, P_SIZE_MIN, P_SIZE_MAX, P_SIZE_SD);
	      prepare_output(table,tid,6, &l,j,nvalues[i], worlds_used);

	      // CONTAINER
	      value =  (char*)malloc((P_CNTR_LEN+1)*sizeof(char));
	      pick_str(&p_cntr_set, P_CNTR_SD, value);
	      prepare_output(table,tid,7, value,j,nvalues[i], worlds_used);
	      free (value);

	      // RETAILPRICE
	      l = rpb_routine(tid);
	      prepare_output(table,tid,8, &l,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((P_CMNT_MAX+1)*sizeof(char));
	      TEXT(P_CMNT_LEN, P_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,9, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;

	    case PSUPP:
	      // PARTKEY
	      RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // SUPPKEY
	      RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,2, &l,j,nvalues[i], worlds_used);

	      // AVAILQTY
	      RANDOM(l, PS_QTY_MIN, PS_QTY_MAX, PS_QTY_SD);
	      prepare_output(table,tid,3, &l,j,nvalues[i], worlds_used);

	      // SUPPLYCOST
	      RANDOM(l, PS_SCST_MIN, PS_SCST_MAX, PS_SCST_SD);
	      prepare_output(table,tid,4, &l,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((PS_CMNT_MAX+1)*sizeof(char));
	      TEXT(PS_CMNT_LEN, PS_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,5, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;

	    case SUPP:
	      // SUPPKEY
	      RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // NAME
	      value =  (char*)malloc((S_NAME_LEN+1)*sizeof(char));
	      sprintf(value, szFormat, S_NAME_TAG, tid);
	      prepare_output(table,tid,2, value,j,nvalues[i], worlds_used);
	      free (value);

	      // ADDRESS
	      value =  (char*)malloc((S_ADDR_MAX+1)*sizeof(char));
	      V_STR(C_ADDR_LEN, C_ADDR_SD, value);
	      prepare_output(table,tid,3, value,j,nvalues[i], worlds_used);
	      free (value);

	      // NATIONKEY
	      RANDOM(l,1,(nations.count - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,4, &l,j,nvalues[i], worlds_used);

	      // PHONE
	      value =  (char*)malloc((PHONE_LEN+1)*sizeof(char));
	      gen_phone((DSS_HUGE)tid, value, (long)C_PHNE_SD);
	      prepare_output(table,tid,5, value,j,nvalues[i], worlds_used);
	      free (value);

	      // ACCTBAL
	      RANDOM(l, C_ABAL_MIN, C_ABAL_MAX, C_ABAL_SD);
	      prepare_output(table,tid,6, &l,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((S_CMNT_MAX+1)*sizeof(char));
	      TEXT(S_CMNT_LEN, S_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,7, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;

	    case NATION:
	      // NATIONKEY
	      RANDOM(l,1,(nations.count - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // NAME
	      prepare_output(table,tid,2, nations.list[(long)l - 1].text,j,nvalues[i], worlds_used);

	      // REGIONKEY
	      RANDOM(l,1,(regions.count - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,3, &l,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((N_CMNT_MAX+1)*sizeof(char));
	      TEXT(N_CMNT_LEN, N_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,4, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;

	    case REGION:
	      // REGIONKEY
	      RANDOM(l,1,(regions.count - 1),WSD_PLACEHOLDERS);
	      prepare_output(table,tid,1, &l,j,nvalues[i], worlds_used);

	      // NAME
	      prepare_output(table,tid,2,regions.list[(long)l - 1].text,j,nvalues[i], worlds_used);

	      // COMMENT
	      value =  (char*)malloc((N_CMNT_MAX+1)*sizeof(char));
	      TEXT(R_CMNT_LEN, R_CMNT_SD, value);
	      worlds_used = prepare_output(table,tid,3, value,j,nvalues[i], worlds_used);
	      free (value);

	      break;
	    }//end switch
	}// end for

      free (placeholder_set[idx+i]);
    }

  free ((DSS_HUGE*)nvalues);
  nvalues = NULL;
}





/*
 * generate a component relation defining values for all placeholders from columns
 */
void
gen_component_attrlevel (int arity, long idx)
{
    if (idx+arity > placeholders_size+1)
	arity = placeholders_size-idx+1;

    if (arity == 0)
    {
	fprintf(stderr,"nullary component for cid %ld !\n",crt_cid);
	return;
    }

    DSS_HUGE l,p;
    long i,k,worlds_used;
    int m;

    static long x = 0;

    long *nvalues = (long*)malloc(arity * sizeof(long));
    double   local_worlds = 1.0;

    /* compute the number of different values for each column of the component */
    m = 0;
    while (m < arity)
    {
	if (ONE_WORLD)
	    nvalues[m] = 1;
	else
	{
	    RANDOM(l,2,max_placeholders,WSD_PLACEHOLDERS);
	    nvalues[m] = (long)l;
	    local_worlds *= nvalues[m]*LOCAL_WORLDS_RATIO_INIT;
	}
	//if (arity > 18)
	//    fprintf(stderr, "add %ld\t",nvalues[m]);
	m++;
    }
    if (!ONE_WORLD)
	local_worlds /= LOCAL_WORLDS_RATIO_INIT;

    //if (arity > 18)
    //	fprintf(stderr, "..finally: %g\n",local_worlds);

    /* compute the number of local worlds for this component */

    //RANDOM(j,1,max_worlds,WSD_PLACEHOLDERS);

    long lworlds = (long)ceil(local_worlds+0.5);
    stat_total_amount_worlds += log10(lworlds);

    if (lworlds > stat_max_lworlds)
	stat_max_lworlds = lworlds;

    stat_avg_lworlds += (lworlds-stat_avg_lworlds)/crt_cid;

    if (COUNT_ONLY) {
	free (nvalues);
	for(i=0;i<arity;i++)
	    free (placeholder_set[idx+i]);
	return;
    }

  static char **asc_date = NULL;
  char **mk_ascdate PROTO((void));
  static int bInit = 0;
  static char szFormat[100];
  static char szBrandFormat[100];

  if (!bInit)
    {
      sprintf(szFormat, O_CLRK_FMT, 9, HUGE_FORMAT + 1);
      sprintf(szBrandFormat, P_BRND_FMT, 2, HUGE_FORMAT + 1);
      bInit = 1;
    }
  if (asc_date == NULL)
    asc_date = mk_ascdate();

  char *value;

  /* pick up values for each column and output them */
  for(i=0; i < arity;i++)
    {
      unsigned table = placeholder_set[idx+i]->table;
      unsigned  attr = placeholder_set[idx+i]->column;
      long tid       = placeholder_set[idx+i]->tid;

      worlds_used = 0;

      for(k=0;k<nvalues[i];k++)
	{
	  switch(table)
	    {
	    case CUST:
	      switch(attr)
		{
		case 1: //CUSTKEY
		  RANDOM(l,1,(tdefs[CUST].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: //NAME
		  RANDOM(l,1,(tdefs[CUST].base * scale - 1),WSD_PLACEHOLDERS);
		  value =  (char*)malloc((C_NAME_LEN+3)*sizeof(char));
		  sprintf(value, szFormat, C_NAME_TAG, (long)l);
		  worlds_used = prepare_output(table,tid,attr,value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 3: //ADDRESS
		  value =  (char*)malloc((C_ADDR_MAX + 1)*sizeof(char));
		  V_STR(C_ADDR_LEN, C_ADDR_SD, value);
		  worlds_used = prepare_output(table,tid,attr,value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 4: // NATIONKEY
		  RANDOM(l, 0, (nations.count - 1), C_NTRG_SD);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 5: // PHONE
		  value =  (char*)malloc((PHONE_LEN + 1)*sizeof(char));
		  gen_phone((DSS_HUGE)tid, value, (long)C_PHNE_SD);
		  worlds_used = prepare_output(table,tid,attr,value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 6: //ACCTBAL
		  RANDOM(l, C_ABAL_MIN, C_ABAL_MAX, C_ABAL_SD);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 7: // MKTSEGMENT
		  value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
		  pick_str(&c_mseg_set, C_MSEG_SD, value);
		  worlds_used = prepare_output(table,tid,attr,value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 8: // COMMENT
		  value =  (char*)malloc((C_CMNT_MAX + 1)*sizeof(char));
		  TEXT(C_CMNT_LEN, C_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr,value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;
		}// end switch
	      break;

	    case ORDER:
	      switch(attr)
		{
		case 1: // ORDERKEY
		  RANDOM(l,1,(tdefs[ORDER].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // CUSTKEY
		  if (scale >= 30000)
		    RANDOM64(l, O_CKEY_MIN, O_CKEY_MAX, O_CKEY_SD);
		  else
		    RANDOM(l, O_CKEY_MIN, O_CKEY_MAX, O_CKEY_SD);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 3: // ORDERSTATUS -- SUBJECT TO CHANGE
		  worlds_used = prepare_output(table,tid,attr, &x, lworlds,nvalues[i], worlds_used);
		  break;

		case 4: // TOTALPRICE
		  RANDOM(l,1,3000000,WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr,&l,lworlds,nvalues[i], worlds_used);
		  break;

		case 5: // ORDERDATE
		  RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
		  value =  (char*)malloc((DATE_LEN)*sizeof(char));
		  strcpy(value, asc_date[l - STARTDATE]);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 6: // ORDER_PRIORITY
		  value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
		  pick_str(&o_priority_set, O_PRIO_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 7: // CLERK
		  RANDOM(l, 1, MAX((scale * O_CLRK_SCL), O_CLRK_SCL), O_CLRK_SD);
		  value =  (char*)malloc((O_CLRK_LEN + 1)*sizeof(char));
		  sprintf(value, szFormat, O_CLRK_TAG, l);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 8: // SHIP_PRIORITY
		  worlds_used = prepare_output(table,tid,attr, &x,lworlds,nvalues[i], worlds_used);
		  break;

		case 9: // COMMENT
		  value =  (char*)malloc((O_CMNT_MAX + 1)*sizeof(char));
		  TEXT(O_CMNT_LEN, O_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;
		}// end switch
	      break;


	    case LINE:
	      switch(attr)
		{
		case 1: // ORDERKEY
		  RANDOM(l,1,(tdefs[ORDER].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // PARTKEY
		  RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 3: // SUPPKEY
		  RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 4: // LINENUMBER
		  RANDOM(l, O_LCNT_MIN, O_LCNT_MAX, O_LCNT_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 5: // QUANTITY
		  RANDOM(l, L_QTY_MIN, L_QTY_MAX, L_QTY_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;
		case 6: // EXTENDEDPRICE
		  RANDOM(l,1,3000000,WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 7: // DISCOUNT
		  RANDOM(l, L_DCNT_MIN, L_DCNT_MAX, L_DCNT_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 8: // TAX
		  RANDOM(l, L_TAX_MIN, L_TAX_MAX, L_TAX_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 9: // RETURNFLAG
		  value =  (char*)malloc(2*sizeof(char));
		  pick_str(&l_rflag_set, L_RFLG_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 10: // LINESTATUS
		  worlds_used = prepare_output(table,tid,attr, &x,lworlds,nvalues[i], worlds_used);
		  break;

		case 11: // SHIPDATE; make it more particular, see build.c
		  RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
		  value =  (char*)malloc((DATE_LEN)*sizeof(char));
		  strcpy(value, asc_date[l - STARTDATE]);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 12: // COMMITDATE
		  //RANDOM(l, L_CDTE_MIN, L_CDTE_MAX, L_CDTE_SD);
		  RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
		  value =  (char*)malloc((DATE_LEN)*sizeof(char));
		  strcpy(value, asc_date[l - STARTDATE]);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 13: // RECEIPTDATE
		  //RANDOM(l, L_RDTE_MIN, L_RDTE_MAX, L_RDTE_SD);
		  value =  (char*)malloc((DATE_LEN)*sizeof(char));
		  RANDOM(l, O_ODATE_MIN, O_ODATE_MAX, O_ODATE_SD);
		  strcpy(value, asc_date[l - STARTDATE]);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 14: // SHIPINSTRUCT
		  value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
		  pick_str(&l_instruct_set, L_SHIP_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 15: // SHIPMODE
		  value =  (char*)malloc((MAXAGG_LEN + 1)*sizeof(char));
		  pick_str(&l_smode_set, L_SMODE_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 16: // COMMENT
		  value =  (char*)malloc((L_CMNT_MAX + 1)*sizeof(char));
		  TEXT(L_CMNT_LEN, L_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}// end switch
	      break;


	    case PART:
	      switch(attr)
		{
		case 1: // PARTKEY
		  RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // NAME
		  value =  (char*)malloc((P_NAME_LEN+1)*sizeof(char));
		  agg_str(&colors, (long)P_NAME_SCL, (long)P_NAME_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 3: // MFGR
		  RANDOM(l, P_MFG_MIN, P_MFG_MAX, P_MFG_SD);
		  value =  (char*)malloc((P_MFG_LEN+1)*sizeof(char));
		  sprintf(value, szFormat, P_MFG_TAG, l);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 4: // BRAND
		  RANDOM(p, P_MFG_MIN, P_MFG_MAX, P_MFG_SD);
		  RANDOM(l, P_BRND_MIN, P_BRND_MAX, P_BRND_SD);
		  value =  (char*)malloc((P_BRND_LEN+1)*sizeof(char));
		  sprintf(value, szBrandFormat, P_BRND_TAG, (p * 10 + l));
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 5: // TYPE
		  value =  (char*)malloc((P_TYPE_LEN+1)*sizeof(char));
		  pick_str(&p_types_set, P_TYPE_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 6: // SIZE
		  RANDOM(l, P_SIZE_MIN, P_SIZE_MAX, P_SIZE_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 7: // CONTAINER
		  value =  (char*)malloc((P_CNTR_LEN+1)*sizeof(char));
		  pick_str(&p_cntr_set, P_CNTR_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 8: // RETAILPRICE
		  l = rpb_routine(tid);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 9: // COMMENT
		  value =  (char*)malloc((P_CMNT_MAX+1)*sizeof(char));
		  TEXT(P_CMNT_LEN, P_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}//end switch
	      break;

	    case PSUPP:
	      switch(attr)
		{
		case 1: // PARTKEY
		  RANDOM(l,1,(tdefs[PART].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // SUPPKEY
		  RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 3: // AVAILQTY
		  RANDOM(l, PS_QTY_MIN, PS_QTY_MAX, PS_QTY_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 4: // SUPPLYCOST
		  RANDOM(l, PS_SCST_MIN, PS_SCST_MAX, PS_SCST_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 5: // COMMENT
		  value =  (char*)malloc((PS_CMNT_MAX+1)*sizeof(char));
		  TEXT(PS_CMNT_LEN, PS_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}//end switch
	      break;

	    case SUPP:
	      switch(attr)
		{
		case 1: // SUPPKEY
		  RANDOM(l,1,(tdefs[SUPP].base * scale - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // NAME
		  value =  (char*)malloc((S_NAME_LEN+1)*sizeof(char));
		  sprintf(value, szFormat, S_NAME_TAG, tid);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 3: // ADDRESS
		  value =  (char*)malloc((S_ADDR_MAX+1)*sizeof(char));
		  V_STR(C_ADDR_LEN, C_ADDR_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 4: // NATIONKEY
		  RANDOM(l,1,(nations.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 5: // PHONE
		  value =  (char*)malloc((PHONE_LEN+1)*sizeof(char));
		  gen_phone((DSS_HUGE)tid, value, (long)C_PHNE_SD);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		case 6: // ACCTBAL
		  RANDOM(l, C_ABAL_MIN, C_ABAL_MAX, C_ABAL_SD);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 7: // COMMENT
		  value =  (char*)malloc((S_CMNT_MAX+1)*sizeof(char));
		  TEXT(S_CMNT_LEN, S_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}//end switch

	      break;

	    case NATION:
	      switch(attr)
		{
		case 1: // NATIONKEY
		  RANDOM(l,1,(nations.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // NAME
		  RANDOM(l,1,(nations.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, nations.list[(long)l - 1].text,lworlds,nvalues[i], worlds_used);
		  break;

		case 3: // REGIONKEY
		  RANDOM(l,1,(regions.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 4: // COMMENT
		  value =  (char*)malloc((N_CMNT_MAX+1)*sizeof(char));
		  TEXT(N_CMNT_LEN, N_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}//end switch
	      break;

	    case REGION:
	      switch(attr)
		{
		case 1: // REGIONKEY
		  RANDOM(l,1,(regions.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, &l,lworlds,nvalues[i], worlds_used);
		  break;

		case 2: // NAME
		  RANDOM(l,1,(regions.count - 1),WSD_PLACEHOLDERS);
		  worlds_used = prepare_output(table,tid,attr, regions.list[(long)l - 1].text,lworlds,nvalues[i], worlds_used);
		  break;

		case 3: // COMMENT
		  value =  (char*)malloc((R_CMNT_MAX+1)*sizeof(char));
		  TEXT(R_CMNT_LEN, R_CMNT_SD, value);
		  worlds_used = prepare_output(table,tid,attr, value,lworlds,nvalues[i], worlds_used);
		  free (value);
		  break;

		}//end switch
	      break;
	    }//end switch
	}// end for

      free (placeholder_set[idx+i]);
    }

  free (nvalues);
  nvalues = NULL;
}


/*
 * generate a component relation defining values for all placeholders from placeholders_list
 */
void
gen_components (void)
{
  DSS_HUGE j;
  int i, l, k;

  int arity = 1;
  long amount;

  if (!COUNT_ONLY)
    shuffle_placeholders();

  fprintf (stderr, "Generate alternatives for %ld placeholders (allocated %ld).\n",placeholders_idx,placeholders_size);

  placeholders_size = placeholders_idx;

  /* compute the initial number of or-sets */
  amount = approx_zipf_distr(placeholders_size, Zipf_component_ratio,1);
  placeholders_idx = 0;

  while (amount > 0)
    {

      fprintf (stderr, "Generate %ld components [start with cid %ld] with arity %d.\n", amount, crt_cid, arity);

      i = 0;
      while (i < amount)
	{
	    //fprintf (stderr, "component (%ld,%d,%ld)..",crt_cid,arity,placeholders_idx);
	    if (TUPLE_LEVEL)
		gen_component_tuplelevel(arity,placeholders_idx);
	    else
		gen_component_attrlevel(arity,placeholders_idx);
	    //fprintf (stderr, "done.\n");

	    placeholders_idx += arity;
	    i++;
	    crt_cid++;
	}

      amount = (long)floor((double)amount*Zipf_component_ratio);
      arity++;
    }

  /* everything that is left is processed here */
    if (placeholders_idx < placeholders_size)
    {
      fprintf (stderr, "REMAINING HOLES:%ld\n",(placeholders_size-placeholders_idx));

      while (placeholders_idx < placeholders_size) {
	  //fprintf (stderr, "component (%ld,%d,%ld)..",crt_cid,1,placeholders_idx);
	  if (TUPLE_LEVEL)
	    gen_component_tuplelevel(1,placeholders_idx);
	  else
	    gen_component_attrlevel(1,placeholders_idx);
	  //fprintf (stderr, "done.\n");
	  crt_cid++;
	  placeholders_idx++;
      }
    }
}

/* finish addition by olteanu for U-relations */

/*
* generate a particular template table with placeholders
*/
void
gen_template (int tnum, DSS_HUGE start, DSS_HUGE count, long upd_num)
{
	static order_t o;
	supplier_t supp;
	customer_t cust;
	part_t part;
	code_t code;
	static int completed = 0;
	DSS_HUGE i;

	DSS_HUGE rows_per_segment=0;
	DSS_HUGE rows_this_segment=-1;
	DSS_HUGE residual_rows=0;

	if (insert_segments)
	  {
	    rows_per_segment = count / insert_segments;
	    residual_rows = count - (rows_per_segment * insert_segments);
	  }

	for (i = start; count; count--, i++)
	{
		LIFENOISE (1000, i);
		row_start(tnum);

		switch (tnum)
		{
		case LINE:
		case ORDER:
  		case ORDER_LINE:
			mk_order (i, &o, upd_num % 10000);

		  if (insert_segments  && (upd_num > 0))
          {
			if((upd_num / 10000) < residual_rows)
				{
				if((++rows_this_segment) > rows_per_segment)
					{
					rows_this_segment=0;
					upd_num += 10000;
					}
				}
			else
				{
				if((++rows_this_segment) >= rows_per_segment)
					{
					rows_this_segment=0;
					upd_num += 10000;
					}
				}
          }

			if (set_seeds == 0)
            {
				if (validate)
					tdefs[tnum].verify(&o, 0);
				else
					tdefs[tnum].loader[direct] (&o, upd_num);
            }
			break;
		case SUPP:
			mk_supp (i, &supp);
			if (set_seeds == 0)
            {
				if (validate)
					tdefs[tnum].verify(&supp, 0);
				else
					tdefs[tnum].loader[direct] (&supp, upd_num);
            }
			break;
		case CUST:
			mk_cust (i, &cust);
			if (set_seeds == 0)
            {
				if (validate)
					tdefs[tnum].verify(&cust, 0);
				else
					tdefs[tnum].loader[direct] (&cust, upd_num);
            }
			break;
		case PSUPP:
		case PART:
  		case PART_PSUPP:
			mk_part (i, &part);
			if (set_seeds == 0) {
				if (validate)
					tdefs[tnum].verify(&part, 0);
				else
					tdefs[tnum].loader[direct] (&part, upd_num);
            }
			break;
		case NATION:
			mk_nation (i, &code);
			if (set_seeds == 0) {
				if (validate)
					tdefs[tnum].verify(&code, 0);
				else
					tdefs[tnum].loader[direct] (&code, 0);
            }
			break;
		case REGION:
			mk_region (i, &code);
			if (set_seeds == 0) {
				if (validate)
					tdefs[tnum].verify(&code, 0);
				else
					tdefs[tnum].loader[direct] (&code, 0);
            }
			break;
		}
		row_stop(tnum);
		if (set_seeds && (i % tdefs[tnum].base) < 2)
		{
			printf("\nSeeds for %s at rowcount %lld\n", tdefs[tnum].comment, i);
			dump_seeds(tnum);
		}
	}
	completed |= 1 << tnum;
}



void
usage (void)
{
	fprintf (stderr, "%s\n%s\n\t%s\n%s %s\n\n",
		"USAGE:",
		"dbgen [-{vfFD}] [-O {fhmsv}][-T {pcsoPSOL}]",
		"[-s <scale>][-C <procs>][-S <step>]",
		"dbgen [-v] [-O {dfhmr}] [-s <scale>]",
		"[-U <updates>] [-r <percent>]");
	fprintf (stderr, "-b <s> -- load distributions for <s>\n");
	fprintf (stderr, "-C <n> -- use <n> processes to generate data\n");
	fprintf (stderr, "          [Under DOS, must be used with -S]\n");
	fprintf (stderr, "-D     -- do database load in line\n");
    fprintf (stderr, "-d <n> -- split deletes between <n> files\n");
	fprintf (stderr, "-f     -- force. Overwrite existing files\n");
	fprintf (stderr, "-F     -- generate flat files output\n");
	fprintf (stderr, "-h     -- display this message\n");
    fprintf (stderr, "-i <n> -- split inserts between <n> files\n");
	fprintf (stderr, "-n <s> -- inline load into database <s>\n");
	fprintf (stderr, "-O d   -- generate SQL syntax for deletes\n");
	fprintf (stderr, "-O f   -- over-ride default output file names\n");
	fprintf (stderr, "-O h   -- output files with headers\n");
	fprintf (stderr, "-O m   -- produce columnar output\n");
	fprintf (stderr, "-O r   -- generate key ranges for deletes.\n");
	fprintf (stderr, "-O v   -- Verify data set without generating it.\n");
	fprintf (stderr, "-q     -- enable QUIET mode\n");
	fprintf (stderr, "-r <n> -- updates refresh (n/100)%% of the\n");
	fprintf (stderr, "          data set\n");
	fprintf (stderr, "-s <n> -- set Scale Factor (SF) to  <n> \n");
	fprintf (stderr, "-S <n> -- build the <n>th step of the data/update set\n");
	fprintf (stderr, "-T c   -- generate cutomers ONLY\n");
	fprintf (stderr, "-T l   -- generate nation/region ONLY\n");
	fprintf (stderr, "-T L   -- generate lineitem ONLY\n");
	fprintf (stderr, "-T n   -- generate nation ONLY\n");
	fprintf (stderr, "-T o   -- generate orders/lineitem ONLY\n");
	fprintf (stderr, "-T O   -- generate orders ONLY\n");
	fprintf (stderr, "-T p   -- generate parts/partsupp ONLY\n");
	fprintf (stderr, "-T P   -- generate parts ONLY\n");
	fprintf (stderr, "-T r   -- generate region ONLY\n");
	fprintf (stderr, "-T s   -- generate suppliers ONLY\n");
	fprintf (stderr, "-T S   -- generate partsupp ONLY\n");
	fprintf (stderr, "-U <s> -- generate <s> update sets\n");
	fprintf (stderr, "-v     -- enable VERBOSE mode\n");
	fprintf (stderr,
		"\nTo generate the SF=1 (1GB), validation database population, use:\n");
	fprintf (stderr, "\tdbgen -vfF -s 1\n");
	fprintf (stderr, "\nTo generate updates for a SF=1 (1GB), use:\n");
	fprintf (stderr, "\tdbgen -v -U 1 -s 1\n");
}

/*
* pload() -- handle the parallel loading of tables
*/
#ifndef DOS
/*
* int partial(int tbl, int s) -- generate the s-th part of the named tables data
*/
int
partial (int tbl, int s)
{
	DSS_HUGE rowcnt;
	DSS_HUGE extra;

	if (verbose > 0)
	{
		fprintf (stderr, "\tStarting to load stage %d of %ld for %s...",
			s, children, tdefs[tbl].comment);
	}

	if (direct == 0)
		set_files (tbl, s);

	rowcnt = set_state(tbl, scale, children, s, &extra);

	if (s == children)
		gen_template (tbl, rowcnt * (s - 1) + 1, rowcnt + extra, upd_num);
	else
		gen_template (tbl, rowcnt * (s - 1) + 1, rowcnt, upd_num);

	if (verbose > 0)
		fprintf (stderr, "done.\n");

	return (0);
}


int
pload (int tbl)
{
	int c = 0, i, status;

	if (verbose > 0)
	{
		fprintf (stderr, "Starting %ld children to load %s",
			children, tdefs[tbl].comment);
	}
	for (c = 0; c < children; c++)
	{
		pids[c] = SPAWN ();
		if (pids[c] == -1)
		{
			perror ("Child loader not created");
			kill_load ();
			exit (-1);
		}
		else if (pids[c] == 0)	/* CHILD */
		{
			SET_HANDLER (stop_proc);
			verbose = 0;
			partial (tbl, c+1);
			exit (0);
		}
		else if (verbose > 0)			/* PARENT */
			fprintf (stderr, ".");
	}

	if (verbose > 0)
		fprintf (stderr, "waiting...");

	c = children;
	while (c)
	{
		i = WAIT (&status, pids[c - 1]);
		if (i == -1 && children)
		{
			if (errno == ECHILD)
				fprintf (stderr, "\nCould not wait on pid %d\n", pids[c - 1]);
			else if (errno == EINTR)
				fprintf (stderr, "\nProcess %d stopped abnormally\n", pids[c - 1]);
			else if (errno == EINVAL)
				fprintf (stderr, "\nProgram bug\n");
		}
		if (! WIFEXITED(status)) {
			(void) fprintf(stderr, "\nProcess %d: ", i);
			if (WIFSIGNALED(status)) {
				(void) fprintf(stderr, "rcvd signal %d\n",
					WTERMSIG(status));
				} else if (WIFSTOPPED(status)) {
				(void) fprintf(stderr, "stopped, signal %d\n",
					WSTOPSIG(status));
					}

			}
		c--;
	}

	if (verbose > 0)
		fprintf (stderr, "done\n");
	return (0);
}
#endif /* !DOS */

/* added by olteanu for U-relations */
/* new parameters to tune the generation of U-relations (see below): x, z, m */

void
process_options (int count, char **vector)
{
	int option;

	while ((option = getopt (count, vector,
		"b:C:Dd:Ffi:hn:O:P:qr:s:S:T:U:vx:z:m:")) != -1) //modified optstring, see below
	switch (option)
		{
		case 'x':
		    placeholders_ratio = atof (optarg);
		    fprintf (stderr, "Ratio of placeholders: %f\n",placeholders_ratio);
		    break;
		case 'z':
		    Zipf_component_ratio = atof (optarg);
		    fprintf (stderr, "Ratio of Zipf components: %f\n",Zipf_component_ratio);
		    break;
		case 'm':
		    max_placeholders = atoi (optarg);
		    fprintf (stderr, "Maximum number of placeholders: %i\n",max_placeholders);
		    break;
		/* end added */

		case 'b':				/* load distributions from named file */
			d_path = (char *)malloc(strlen(optarg) + 1);
			MALLOC_CHECK(d_path);
			strcpy(d_path, optarg);
			break;
		case 'q':				/* all prompts disabled */
			verbose = -1;
			break;
		case 'i':
			insert_segments = atoi (optarg);
			break;
		case 'd':
			delete_segments = atoi (optarg);
			break;
	  case 'S':				/* generate a particular STEP */
		  step = atoi (optarg);
		  break;
	  case 'v':				/* life noises enabled */
		  verbose = 1;
		  break;
	  case 'f':				/* blind overwrites; Force */
		  force = 1;
		  break;
	  case 'T':				/* generate a specifc table */
		  switch (*optarg)
		  {
		  case 'c':			/* generate customer ONLY */
			  table = 1 << CUST;
			  break;
		  case 'L':			/* generate lineitems ONLY */
			  table = 1 << LINE;
			  break;
		  case 'l':			/* generate code table ONLY */
			  table = 1 << NATION;
			  table |= 1 << REGION;
			  break;
		  case 'n':			/* generate nation table ONLY */
			  table = 1 << NATION;
			  break;
		  case 'O':			/* generate orders ONLY */
			  table = 1 << ORDER;
			  break;
		  case 'o':			/* generate orders/lineitems ONLY */
			  table = 1 << ORDER_LINE;
			  break;
		  case 'P':			/* generate part ONLY */
			  table = 1 << PART;
			  break;
		  case 'p':			/* generate part/partsupp ONLY */
			  table = 1 << PART_PSUPP;
			  break;
		  case 'r':			/* generate region table ONLY */
			  table = 1 << REGION;
			  break;
		  case 'S':			/* generate partsupp ONLY */
			  table = 1 << PSUPP;
			  break;
		  case 's':			/* generate suppliers ONLY */
			  table = 1 << SUPP;
			  break;
		  default:
			  fprintf (stderr, "Unknown table name %s\n",
				  optarg);
			  usage ();
			  exit (1);
		  }
		  break;
		  case 's':				/* scale by Percentage of base rowcount */
		  case 'P':				/* for backward compatibility */
			  flt_scale = atof (optarg);
			  if (flt_scale < MIN_SCALE)
			  {
				  int i;
				  int int_scale;

				  scale = 1;
				  int_scale = (int)(1000 * flt_scale);
				  for (i = PART; i < REGION; i++)
				  {
					  tdefs[i].base = (DSS_HUGE)(int_scale * tdefs[i].base)/1000;
					  if (tdefs[i].base < 1)
						  tdefs[i].base = 1;
				  }
			  }
			  else
				  scale = (long) flt_scale;
			  if (scale > MAX_SCALE)
			  {
				  fprintf (stderr, "%s %5.0f %s\n\t%s\n\n",
					  "NOTE: Data generation for scale factors >",
					  MAX_SCALE,
					  "GB is still in development,",
					  "and is not yet supported.\n");
				  fprintf (stderr,
					  "Your resulting data set MAY NOT BE COMPLIANT!\n");
			  }
			  break;
		  case 'O':				/* optional actions */
			  switch (tolower (*optarg))
			  {
			  case 'd':			/* generate SQL for deletes */
				  gen_sql = 1;
				  break;
			  case 'f':			/* over-ride default file names */
				  fnames = 1;
				  break;
			  case 'h':			/* generate headers */
				  header = 1;
				  break;
			  case 'm':			/* generate columnar output */
				  columnar = 1;
				  break;
			  case 'r':			/* generate key ranges for delete */
				  gen_rng = 1;
				  break;
			  case 's':			/* calibrate the RNG usage */
				  set_seeds = 1;
				  break;
			  case 'v':			/* validate the data set */
				  validate = 1;
				  break;
			  default:
				  fprintf (stderr, "Unknown option name %s\n",
					  optarg);
				  usage ();
				  exit (1);
			  }
			  break;
			  case 'D':				/* direct load of generated data */
				  direct = 1;
				  break;
			  case 'F':				/* generate flat files for later loading */
				  direct = 0;
				  break;
			  case 'U':				/* generate flat files for update stream */
				  updates = atoi (optarg);
				  break;
			  case 'r':				/* set the refresh (update) percentage */
				  refresh = atoi (optarg);
				  break;
#ifndef DOS
			  case 'C':
				  children = atoi (optarg);
				  break;
#endif /* !DOS */
			  case 'n':				/* set name of database for direct load */
				  db_name = (char *) malloc (strlen (optarg) + 1);
				  MALLOC_CHECK (db_name);
				  strcpy (db_name, optarg);
				  break;
			  default:
				  printf ("ERROR: option '%c' unknown.\n",
					  *(vector[optind] + 1));
			  case 'h':				/* something unexpected */
				  fprintf (stderr,
					  "%s Population Generator (Version %d.%d.%d build %d)\n",
					  NAME, VERSION, RELEASE, PATCH, BUILD);
				  fprintf (stderr, "Copyright %s %s\n", TPC, C_DATES);
				  usage ();
				  exit (1);
	  }

#ifndef DOS
	if (children != 1 && step == -1)
		{
		pids = malloc(children * sizeof(pid_t));
		MALLOC_CHECK(pids)
		}
#else
	if (children != 1 && step < 0)
		{
		fprintf(stderr, "ERROR: -C must be accompanied by -S on this platform\n");
		exit(1);
		}
#endif /* DOS */

	return;
}

/*
* MAIN
*
* assumes the existance of getopt() to clean up the command
* line handling
*/
int
main (int ac, char **av)
{
	DSS_HUGE i;

	table = (1 << CUST) |
		(1 << SUPP) |
		(1 << NATION) |
		(1 << REGION) |
		(1 << PART_PSUPP) |
		(1 << ORDER_LINE);
	force = 0;
    insert_segments=0;
    delete_segments=0;
    insert_orders_segment=0;
    insert_lineitem_segment=0;
    delete_segment=0;

	verbose = 0;
	columnar = 0;
	set_seeds = 0;
	header = 0;
	direct = 0;
	scale = 1;
	flt_scale = 1.0;
	updates = 0;
	refresh = UPD_PCT;
	step = -1;
	tdefs[ORDER].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	tdefs[LINE].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	tdefs[ORDER_LINE].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	fnames = 0;
	db_name = NULL;
	gen_sql = 0;
	gen_rng = 0;
	children = 1;
	d_path = NULL;



	/* added by olteanu for U-relations */

	stat_total_amount_worlds = 0;
	stat_avg_lworlds = 0;
	stat_max_lworlds = 0;
	crt_cid = 1;

	if (TUPLE_LEVEL)
	  fprintf (stderr, "mode: tuple-level\n");
	else
	  fprintf (stderr, "mode: attribute-level\n");

	placeholders_ratio = PLACEHOLDERS_RATIO_INIT;
	Zipf_component_ratio = ZIPF_COMP_RATIO_INIT;
	max_placeholders = MAX_PLACEHOLDERS_INIT;

	tid_line = 1;
	tid_partsupp = 1;

	placeholders_idx = 0;
	placeholders_size = PLACEHOLDERS_INIT;
	placeholder_set = (Placeholder**) calloc(placeholders_size,sizeof(Placeholder*));

	upper = 10000;
	threshold = upper * placeholders_ratio;


	part_p_partkey = print_prep(PART_p_PARTKEY , 0);
	part_p_name = print_prep(PART_p_NAME , 0);
	part_p_mfgr = print_prep(PART_p_MFGR , 0);
	part_p_brand = print_prep(PART_p_BRAND , 0);
	part_p_type = print_prep(PART_p_TYPE , 0);
	part_p_size = print_prep(PART_p_SIZE , 0);
	part_p_container = print_prep(PART_p_CONTAINER , 0);
	part_p_retailprice = print_prep(PART_p_RETAILPRICE , 0);
	part_p_comment = print_prep(PART_p_COMMENT , 0);

	supp_s_suppkey = print_prep(SUPP_s_SUPPKEY , 0);
	supp_s_name = print_prep(SUPP_s_NAME , 0);
	supp_s_address = print_prep(SUPP_s_ADDRESS , 0);
	supp_s_nationkey = print_prep(SUPP_s_NATIONKEY , 0);
	supp_s_phone = print_prep(SUPP_s_PHONE , 0);
	supp_s_acctbal = print_prep(SUPP_s_ACCTBAL , 0);
	supp_s_comment = print_prep(SUPP_s_COMMENT , 0);

	psupp_ps_partkey = print_prep(PSUPP_ps_PARTKEY , 0);
	psupp_ps_suppkey = print_prep(PSUPP_ps_SUPPKEY , 0);
	psupp_ps_availqty = print_prep(PSUPP_ps_AVAILQTY , 0);
	psupp_ps_supplycost = print_prep(PSUPP_ps_SUPPLYCOST , 0);
	psupp_ps_comment = print_prep(PSUPP_ps_COMMENT , 0);

	cust_c_custkey = print_prep(CUST_c_CUSTKEY , 0);
	cust_c_name = print_prep(CUST_c_NAME , 0);
	cust_c_address = print_prep(CUST_c_ADDRESS , 0);
	cust_c_nationkey = print_prep(CUST_c_NATIONKEY , 0);
	cust_c_phone = print_prep(CUST_c_PHONE , 0);
	cust_c_acctbal = print_prep(CUST_c_ACCTBAL , 0);
	cust_c_mktsegment = print_prep(CUST_c_MKTSEGMENT , 0);
	cust_c_comment = print_prep(CUST_c_COMMENT , 0);

	orders_o_orderkey = print_prep(ORDERS_o_ORDERKEY , 0);
	orders_o_custkey = print_prep(ORDERS_o_CUSTKEY , 0);
	orders_o_orderstatus = print_prep(ORDERS_o_ORDERSTATUS , 0);
	orders_o_totalprice = print_prep(ORDERS_o_TOTALPRICE , 0);
	orders_o_orderdate = print_prep(ORDERS_o_ORDERDATE , 0);
	orders_o_orderpriority = print_prep(ORDERS_o_ORDERPRIORITY , 0);
	orders_o_clerk = print_prep(ORDERS_o_CLERK , 0);
	orders_o_shippriority = print_prep(ORDERS_o_SHIPPRIORITY , 0);
	orders_o_comment = print_prep(ORDERS_o_COMMENT , 0);

	lineitem_l_orderkey = print_prep(LINEITEM_l_ORDERKEY , 0);
	lineitem_l_partkey = print_prep(LINEITEM_l_PARTKEY , 0);
	lineitem_l_suppkey = print_prep(LINEITEM_l_SUPPKEY , 0);
	lineitem_l_linenumber = print_prep(LINEITEM_l_LINENUMBER , 0);
	lineitem_l_quantity = print_prep(LINEITEM_l_QUANTITY , 0);
	lineitem_l_extendedprice = print_prep(LINEITEM_l_EXTENDEDPRICE , 0);
	lineitem_l_discount = print_prep(LINEITEM_l_DISCOUNT , 0);
	lineitem_l_tax = print_prep(LINEITEM_l_TAX , 0);
	lineitem_l_returnflag = print_prep(LINEITEM_l_RETURNFLAG , 0);
	lineitem_l_linestatus = print_prep(LINEITEM_l_LINESTATUS , 0);
	lineitem_l_shipdate = print_prep(LINEITEM_l_SHIPDATE , 0);
	lineitem_l_commitdate = print_prep(LINEITEM_l_COMMITDATE , 0);
	lineitem_l_receiptdate = print_prep(LINEITEM_l_RECEIPTDATE , 0);
	lineitem_l_shipinstruct = print_prep(LINEITEM_l_SHIPINSTRUCT , 0);
	lineitem_l_shipmode = print_prep(LINEITEM_l_SHIPMODE , 0);
	lineitem_l_comment = print_prep(LINEITEM_l_COMMENT , 0);

	nation_n_nationkey = print_prep(NATION_n_NATIONKEY , 0);
	nation_n_name = print_prep(NATION_n_NAME , 0);
	nation_n_regionkey = print_prep(NATION_n_REGIONKEY , 0);
	nation_n_comment = print_prep(NATION_n_COMMENT , 0);

	region_r_regionkey = print_prep(REGION_r_REGIONKEY , 0);
	region_r_name = print_prep(REGION_r_NAME , 0);
	region_r_comment = print_prep(REGION_r_COMMENT , 0);



	/* finished addition by olteanu for U-relations */



#ifdef NO_SUPPORT
	signal (SIGINT, exit);
#endif /* NO_SUPPORT */
	process_options (ac, av);
#if (defined(WIN32)&&!defined(_POSIX_))
	for (i = 0; i < ac; i++)
	{
		spawn_args[i] = malloc ((strlen (av[i]) + 1) * sizeof (char));
		MALLOC_CHECK (spawn_args[i]);
		strcpy (spawn_args[i], av[i]);
	}
	spawn_args[ac] = NULL;
#endif


	if (verbose >= 0)
		{
		fprintf (stderr,
			"%s Population Generator (Version %d.%d.%d build %d)\n",
			NAME, VERSION, RELEASE, PATCH, BUILD);
		fprintf (stderr, "Copyright %s %s\n", TPC, C_DATES);
		}

	load_dists ();

	/* have to do this after init */
	tdefs[NATION].base = nations.count;
	tdefs[REGION].base = regions.count;



	/*
	* updates are never parallelized
	*/
	if (updates)
		{
		/*
		 * set RNG to start generating rows beyond SF=scale
		 */
		set_state (ORDER, scale, children, children + 1, &i);
		rowcnt = (int)(tdefs[ORDER_LINE].base / 10000 * scale * refresh);
		if (step > 0)
			{
			/*
			 * adjust RNG for any prior update generation
			 */
	      for (i=1; i < step; i++)
         {
			sd_order(0, rowcnt);
			sd_line(0, rowcnt);
         }
			upd_num = step - 1;
			}
		else
			upd_num = 0;

		while (upd_num < updates)
			{
			if (verbose > 0)
				fprintf (stderr,
				"Generating update pair #%ld for %s [pid: %d]",
				upd_num + 1, tdefs[ORDER_LINE].comment, DSS_PROC);
			insert_orders_segment=0;
			insert_lineitem_segment=0;
			delete_segment=0;
			minrow = upd_num * rowcnt + 1;
			gen_template (ORDER_LINE, minrow, rowcnt, upd_num + 1);
			if (verbose > 0)
				fprintf (stderr, "done.\n");
			pr_drange (ORDER_LINE, minrow, rowcnt, upd_num + 1);
			upd_num++;
			}

		exit (0);
		}

	/**
	** actual data generation section starts here
	**/
/*
 * open database connection or set all the file names, as appropriate
 */
	if (direct)
		prep_direct ((db_name) ? db_name : DBNAME);
	else if (fnames)
		for (i = PART; i <= REGION; i++)
		{
			if (table & (1 << i))
				if (set_files ((int)i, -1))
				{
					fprintf (stderr, "Load aborted!\n");
					exit (1);
				}
		}

/*
 * traverse the tables, invoking the appropriate data generation routine for any to be built
 */
	for (i = PART; i <= REGION; i++)
		if (table & (1 << i))
		{
			if (children > 1 && i < NATION)
				if (step >= 0)
				{
					if (validate)
					{
						INTERNAL_ERROR("Cannot validate parallel data generation");
					}
					else
						partial ((int)i, step);
				}
#ifdef DOS
				else
				{
					fprintf (stderr,
						"Parallel load is not supported on your platform.\n");
					exit (1);
				}
#else
				else
				{
					if (validate)
					{
						INTERNAL_ERROR("Cannot validate parallel data generation");
					}
					else
						pload ((int)i);
				}
#endif /* DOS */
				else
				{
					minrow = 1;
					if (i < NATION)
						rowcnt = tdefs[i].base * scale;
					else
						rowcnt = tdefs[i].base;
					if (verbose > 0)
						fprintf (stderr, "%s data for %s [pid: %d]",
						(validate)?"Validating":"Generating", tdefs[i].comment, DSS_PROC);

					gen_template ((int)i, minrow, rowcnt, upd_num);
					if (verbose > 0)
						fprintf (stderr, "done.\n");

				}
				if (validate)
					printf("Validation checksum for %s at %ld GB: %0x\n",
                           tdefs[i].name, scale, (unsigned int) tdefs[i].vtotal);
		}

		if (direct)
			close_direct ();


		/* added by olteanu for U-relations */
		/* now we generate the components */
		gen_components ();

		fprintf(stderr, "10^%g worlds were created.\n",stat_total_amount_worlds);
		fprintf(stderr, "%ld components were created.\n",crt_cid-1);
		fprintf(stderr, "Maximum %g and avg %g amount of local worlds were created.\n",stat_max_lworlds,stat_avg_lworlds);

		return (0);
}
