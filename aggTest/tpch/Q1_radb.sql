SELECT F0."AGG_GB_ARG6" AS "l_linestatus", min(F0."AGG_GB_ARG0") AS "sum_qty", min(F0."AGG_GB_ARG1") AS "sum_base_price", min(F0."AGG_GB_ARG2") AS "sum_disc_price", min(F0."AGG_GB_ARG3") AS "sum_charge", min(F0."AGG_GB_ARG4") AS "avg_disc", min(F0."AGG_GB_ARG5") AS "count_order", F0."ub_agg_gb_arg6" AS "ub_l_linestatus", F0."lb_agg_gb_arg6" AS "lb_l_linestatus", sum((F1."ub_agg_gb_arg0" * (CASE  WHEN (F1."ub_agg_gb_arg0" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_sum_qty", sum((F1."lb_agg_gb_arg0" * (CASE  WHEN (F1."lb_agg_gb_arg0" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_sum_qty", sum((F1."ub_agg_gb_arg1" * (CASE  WHEN (F1."ub_agg_gb_arg1" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_sum_base_price", sum((F1."lb_agg_gb_arg1" * (CASE  WHEN (F1."lb_agg_gb_arg1" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_sum_base_price", sum((F1."ub_agg_gb_arg2" * (CASE  WHEN (F1."ub_agg_gb_arg2" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_sum_disc_price", sum((F1."lb_agg_gb_arg2" * (CASE  WHEN (F1."lb_agg_gb_arg2" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_sum_disc_price", sum((F1."ub_agg_gb_arg3" * (CASE  WHEN (F1."ub_agg_gb_arg3" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_sum_charge", sum((F1."lb_agg_gb_arg3" * (CASE  WHEN (F1."lb_agg_gb_arg3" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_sum_charge", sum((F1."ub_agg_gb_arg4" * (CASE  WHEN (F1."ub_agg_gb_arg4" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_avg_disc", sum((F1."lb_agg_gb_arg4" * (CASE  WHEN (F1."lb_agg_gb_arg4" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_avg_disc", sum((F1."ub_agg_gb_arg5" * (CASE  WHEN (F1."ub_agg_gb_arg5" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_count_order", sum((F1."lb_agg_gb_arg5" * (CASE  WHEN (F1."lb_agg_gb_arg5" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_count_order", max((CASE  WHEN (F0."cet_r" > 0) THEN 1 ELSE 0 END)) AS "cet_r", max((CASE  WHEN (F0."bst_r" > 0) THEN 1 ELSE 0 END)) AS "bst_r", sum(F1."pos_r") AS "pos_r"
FROM ((
SELECT max(F0."ub_l_linestatus") AS "ub_agg_gb_arg6", min(F0."lb_l_linestatus") AS "lb_agg_gb_arg6", sum((F0."l_quantity" * (CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "AGG_GB_ARG0", sum((F0."l_extendedprice" * (CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "AGG_GB_ARG1", sum(((F0."l_extendedprice" * (1 - F0."l_discount")) * (CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "AGG_GB_ARG2", sum((((F0."l_extendedprice" * (1 - F0."l_discount")) * (1 + F0."l_tax")) * (CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "AGG_GB_ARG3", sum((F0."l_discount" * (CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "AGG_GB_ARG4", sum((CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END)) AS "AGG_GB_ARG5", max(((CASE  WHEN (F0."ub_l_linestatus" = F0."lb_l_linestatus") THEN 1 ELSE 0 END) * (CASE  WHEN (F0."ub_l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END))) AS "cet_r", max((CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END)) AS "bst_r", F0."l_linestatus" AS "AGG_GB_ARG6"
FROM (
SELECT F0."tid" AS "tid", F0."l_orderkey" AS "l_orderkey", F0."l_partkey" AS "l_partkey", F0."l_suppkey" AS "l_suppkey", F0."l_linenumber" AS "l_linenumber", F0."l_quantity" AS "l_quantity", F0."l_extendedprice" AS "l_extendedprice", F0."l_discount" AS "l_discount", F0."l_tax" AS "l_tax", F0."l_returnflag" AS "l_returnflag", F0."l_linestatus" AS "l_linestatus", F0."l_shipdate" AS "l_shipdate", F0."l_commitdate" AS "l_commitdate", F0."l_receiptdate" AS "l_receiptdate", F0."l_shipinstruct" AS "l_shipinstruct", F0."l_shipmode" AS "l_shipmode", F0."l_comment" AS "l_comment", F0."ub_tid" AS "ub_tid", F0."lb_tid" AS "lb_tid", F0."ub_l_orderkey" AS "ub_l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", F0."ub_l_partkey" AS "ub_l_partkey", F0."lb_l_partkey" AS "lb_l_partkey", F0."ub_l_suppkey" AS "ub_l_suppkey", F0."lb_l_suppkey" AS "lb_l_suppkey", F0."ub_l_linenumber" AS "ub_l_linenumber", F0."lb_l_linenumber" AS "lb_l_linenumber", F0."ub_l_quantity" AS "ub_l_quantity", F0."lb_l_quantity" AS "lb_l_quantity", F0."ub_l_extendedprice" AS "ub_l_extendedprice", F0."lb_l_extendedprice" AS "lb_l_extendedprice", F0."ub_l_discount" AS "ub_l_discount", F0."lb_l_discount" AS "lb_l_discount", F0."ub_l_tax" AS "ub_l_tax", F0."lb_l_tax" AS "lb_l_tax", F0."ub_l_returnflag" AS "ub_l_returnflag", F0."lb_l_returnflag" AS "lb_l_returnflag", F0."ub_l_linestatus" AS "ub_l_linestatus", F0."lb_l_linestatus" AS "lb_l_linestatus", F0."ub_l_shipdate" AS "ub_l_shipdate", F0."lb_l_shipdate" AS "lb_l_shipdate", F0."ub_l_commitdate" AS "ub_l_commitdate", F0."lb_l_commitdate" AS "lb_l_commitdate", F0."ub_l_receiptdate" AS "ub_l_receiptdate", F0."lb_l_receiptdate" AS "lb_l_receiptdate", F0."ub_l_shipinstruct" AS "ub_l_shipinstruct", F0."lb_l_shipinstruct" AS "lb_l_shipinstruct", F0."ub_l_shipmode" AS "ub_l_shipmode", F0."lb_l_shipmode" AS "lb_l_shipmode", F0."ub_l_comment" AS "ub_l_comment", F0."lb_l_comment" AS "lb_l_comment", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM "lineitem_radb" AS F0) F0
WHERE (F0."lb_l_shipdate" <= '1998-12-01')
GROUP BY F0."l_linestatus") F0 JOIN (
SELECT min(F0."l_quantity") AS "AGG_GB_ARG0", min(F0."l_extendedprice") AS "AGG_GB_ARG1", min((F0."l_extendedprice" * (1 - F0."l_discount"))) AS "AGG_GB_ARG2", min(((F0."l_extendedprice" * (1 - F0."l_discount")) * (1 + F0."l_tax"))) AS "AGG_GB_ARG3", min(F0."l_discount") AS "AGG_GB_ARG4", min(1) AS "AGG_GB_ARG5", (CASE  WHEN (F0."l_linestatus" >= '?') THEN '?' WHEN ((F0."l_linestatus" < '?') AND (F0."l_linestatus" >= '0')) THEN '0' ELSE '0' END) AS "AGG_GB_ARG6", max(F0."ub_l_quantity") AS "ub_agg_gb_arg0", min(F0."lb_l_quantity") AS "lb_agg_gb_arg0", max(F0."ub_l_extendedprice") AS "ub_agg_gb_arg1", min(F0."lb_l_extendedprice") AS "lb_agg_gb_arg1", max(greatest(greatest((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), greatest((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount"))))) AS "ub_agg_gb_arg2", min(least(least((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), least((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount"))))) AS "lb_agg_gb_arg2", max(greatest(greatest((greatest(greatest((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), greatest((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."ub_l_tax")), (greatest(greatest((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), greatest((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."lb_l_tax"))), greatest((least(least((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), least((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."ub_l_tax")), (least(least((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), least((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."lb_l_tax"))))) AS "ub_agg_gb_arg3", min(least(least((greatest(greatest((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), greatest((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."ub_l_tax")), (greatest(greatest((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), greatest((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."lb_l_tax"))), least((least(least((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), least((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."ub_l_tax")), (least(least((F0."ub_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."ub_l_extendedprice" * (1 - F0."ub_l_discount"))), least((F0."lb_l_extendedprice" * (1 - F0."lb_l_discount")), (F0."lb_l_extendedprice" * (1 - F0."ub_l_discount")))) * (1 + F0."lb_l_tax"))))) AS "lb_agg_gb_arg3", max(F0."ub_l_discount") AS "ub_agg_gb_arg4", min(F0."lb_l_discount") AS "lb_agg_gb_arg4", max(1) AS "ub_agg_gb_arg5", min(1) AS "lb_agg_gb_arg5", max(F0."ub_l_linestatus") AS "ub_agg_gb_arg6", min(F0."lb_l_linestatus") AS "lb_agg_gb_arg6", sum(0) AS "cet_r", sum(0) AS "bst_r", sum((CASE  WHEN (F0."l_shipdate" <= '1998-12-01') THEN 1 ELSE 0 END)) AS "pos_r"
FROM (
SELECT F0."tid" AS "tid", F0."l_orderkey" AS "l_orderkey", F0."l_partkey" AS "l_partkey", F0."l_suppkey" AS "l_suppkey", F0."l_linenumber" AS "l_linenumber", F0."l_quantity" AS "l_quantity", F0."l_extendedprice" AS "l_extendedprice", F0."l_discount" AS "l_discount", F0."l_tax" AS "l_tax", F0."l_returnflag" AS "l_returnflag", F0."l_linestatus" AS "l_linestatus", F0."l_shipdate" AS "l_shipdate", F0."l_commitdate" AS "l_commitdate", F0."l_receiptdate" AS "l_receiptdate", F0."l_shipinstruct" AS "l_shipinstruct", F0."l_shipmode" AS "l_shipmode", F0."l_comment" AS "l_comment", F0."ub_tid" AS "ub_tid", F0."lb_tid" AS "lb_tid", F0."ub_l_orderkey" AS "ub_l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", F0."ub_l_partkey" AS "ub_l_partkey", F0."lb_l_partkey" AS "lb_l_partkey", F0."ub_l_suppkey" AS "ub_l_suppkey", F0."lb_l_suppkey" AS "lb_l_suppkey", F0."ub_l_linenumber" AS "ub_l_linenumber", F0."lb_l_linenumber" AS "lb_l_linenumber", F0."ub_l_quantity" AS "ub_l_quantity", F0."lb_l_quantity" AS "lb_l_quantity", F0."ub_l_extendedprice" AS "ub_l_extendedprice", F0."lb_l_extendedprice" AS "lb_l_extendedprice", F0."ub_l_discount" AS "ub_l_discount", F0."lb_l_discount" AS "lb_l_discount", F0."ub_l_tax" AS "ub_l_tax", F0."lb_l_tax" AS "lb_l_tax", F0."ub_l_returnflag" AS "ub_l_returnflag", F0."lb_l_returnflag" AS "lb_l_returnflag", F0."ub_l_linestatus" AS "ub_l_linestatus", F0."lb_l_linestatus" AS "lb_l_linestatus", F0."ub_l_shipdate" AS "ub_l_shipdate", F0."lb_l_shipdate" AS "lb_l_shipdate", F0."ub_l_commitdate" AS "ub_l_commitdate", F0."lb_l_commitdate" AS "lb_l_commitdate", F0."ub_l_receiptdate" AS "ub_l_receiptdate", F0."lb_l_receiptdate" AS "lb_l_receiptdate", F0."ub_l_shipinstruct" AS "ub_l_shipinstruct", F0."lb_l_shipinstruct" AS "lb_l_shipinstruct", F0."ub_l_shipmode" AS "ub_l_shipmode", F0."lb_l_shipmode" AS "lb_l_shipmode", F0."ub_l_comment" AS "ub_l_comment", F0."lb_l_comment" AS "lb_l_comment", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM "lineitem_radb" AS F0) F0
WHERE (F0."lb_l_shipdate" <= '1998-12-01')
GROUP BY (CASE  WHEN (F0."l_linestatus" >= '?') THEN '?' WHEN ((F0."l_linestatus" < '?') AND (F0."l_linestatus" >= '0')) THEN '0' ELSE '0' END)) F1 ON (((F0."ub_agg_gb_arg6" >= F1."lb_agg_gb_arg6") AND (F1."ub_agg_gb_arg6" >= F0."lb_agg_gb_arg6"))))
GROUP BY F0."AGG_GB_ARG6", F0."ub_agg_gb_arg6", F0."lb_agg_gb_arg6";
