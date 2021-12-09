SELECT F0."AGG_GB_ARG1" AS "o_custkey", sum(F1."bst_r") AS "count(1)", F0."ub_agg_gb_arg1" AS "ub_o_custkey", F0."lb_agg_gb_arg1" AS "lb_o_custkey", sum(F1."pos_r") AS "ub_count(1)", min(F1."cet_r") AS "lb_count(1)", min((CASE  WHEN ((F0."ub_agg_gb_arg1" = F1."ub_agg_gb_arg1") AND ((F0."ub_agg_gb_arg1" = F0."lb_agg_gb_arg1") AND (F1."ub_agg_gb_arg1" = F1."lb_agg_gb_arg1"))) THEN 1 ELSE 0 END)) AS "cet_r", max((CASE  WHEN (F0."AGG_GB_ARG1" = F1."AGG_GB_ARG1") THEN 1 ELSE 0 END)) AS "bst_r", sum(F1."pos_r") AS "pos_r"
FROM ((
SELECT max(F0."ub_o_custkey") AS "ub_agg_gb_arg1", min(F0."lb_o_custkey") AS "lb_agg_gb_arg1", F0."o_custkey" AS "AGG_GB_ARG1"
FROM "orders_radb" AS F0
GROUP BY F0."o_custkey") F0 JOIN (
SELECT 1 AS "AGG_GB_ARG0", F0."o_custkey" AS "AGG_GB_ARG1", 1 AS "ub_agg_gb_arg0", 1 AS "lb_agg_gb_arg0", F0."ub_o_custkey" AS "ub_agg_gb_arg1", F0."lb_o_custkey" AS "lb_agg_gb_arg1", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM "orders_radb" AS F0) F1 ON (((F0."ub_agg_gb_arg1" >= F1."lb_agg_gb_arg1") AND (F1."ub_agg_gb_arg1" >= F0."lb_agg_gb_arg1"))))
GROUP BY F0."AGG_GB_ARG1", F0."ub_agg_gb_arg1", F0."lb_agg_gb_arg1";