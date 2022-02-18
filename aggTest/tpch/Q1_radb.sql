SELECT F0."AGG_GB_ARG1" AS "o_custkey", min(F0."AGG_GB_ARG0") AS "count(1)", F0."ub_agg_gb_arg1" AS "ub_o_custkey", F0."lb_agg_gb_arg1" AS "lb_o_custkey", sum((F1."ub_agg_gb_arg0" * (CASE  WHEN (F1."ub_agg_gb_arg0" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_count(1)", sum((F1."lb_agg_gb_arg0" * (CASE  WHEN (F1."lb_agg_gb_arg0" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_count(1)", max((CASE  WHEN (F0."cet_r" > 0) THEN 1 ELSE 0 END)) AS "cet_r", max((CASE  WHEN (F0."bst_r" > 0) THEN 1 ELSE 0 END)) AS "bst_r", sum(F1."pos_r") AS "pos_r"
FROM ((
SELECT max(F0."ub_o_custkey") AS "ub_agg_gb_arg1", min(F0."lb_o_custkey") AS "lb_agg_gb_arg1", sum(F0."bst_r") AS "AGG_GB_ARG0", max(((CASE  WHEN (F0."ub_o_custkey" = F0."lb_o_custkey") THEN 1 ELSE 0 END) * F0."cet_r")) AS "cet_r", max(F0."bst_r") AS "bst_r", F0."o_custkey" AS "AGG_GB_ARG1"
FROM "orders_radb" AS F0
GROUP BY F0."o_custkey") F0 JOIN (
SELECT min(1) AS "AGG_GB_ARG0", (CASE  WHEN (F0."o_custkey" >= 7500) THEN 7500 WHEN ((F0."o_custkey" < 7500) AND (F0."o_custkey" >= 1)) THEN 1 ELSE 1 END) AS "AGG_GB_ARG1", max(1) AS "ub_agg_gb_arg0", min(1) AS "lb_agg_gb_arg0", max(F0."ub_o_custkey") AS "ub_agg_gb_arg1", min(F0."lb_o_custkey") AS "lb_agg_gb_arg1", sum(0) AS "cet_r", sum(0) AS "bst_r", sum(F0."bst_r") AS "pos_r"
FROM "orders_radb" AS F0
GROUP BY (CASE  WHEN (F0."o_custkey" >= 7500) THEN 7500 WHEN ((F0."o_custkey" < 7500) AND (F0."o_custkey" >= 1)) THEN 1 ELSE 1 END)) F1 ON (((F0."ub_agg_gb_arg1" >= F1."lb_agg_gb_arg1") AND (F1."ub_agg_gb_arg1" >= F0."lb_agg_gb_arg1"))))
GROUP BY F0."AGG_GB_ARG1", F0."ub_agg_gb_arg1", F0."lb_agg_gb_arg1";
