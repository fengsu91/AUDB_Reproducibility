SELECT F0."c_mktsegment" AS "c_mktsegment", min(F0."c_acctbal") AS "sum(c_acctbal)", F0."ub_c_mktsegment" AS "ub_c_mktsegment", F0."lb_c_mktsegment" AS "lb_c_mktsegment", sum((F1."ub_c_acctbal" * (CASE  WHEN (F1."ub_c_acctbal" > 0) THEN F1."pos_r" ELSE 0 END))) AS "ub_sum(c_acctbal)", sum((F1."lb_c_acctbal" * (CASE  WHEN (F1."lb_c_acctbal" > 0) THEN 0 ELSE F1."pos_r" END))) AS "lb_sum(c_acctbal)", max((CASE  WHEN (F0."cet_r" > 0) THEN 1 ELSE 0 END)) AS "cet_r", max((CASE  WHEN (F0."bst_r" > 0) THEN 1 ELSE 0 END)) AS "bst_r", sum(F1."pos_r") AS "pos_r"
FROM ((
SELECT max(F0."ub_c_mktsegment") AS "ub_c_mktsegment", min(F0."lb_c_mktsegment") AS "lb_c_mktsegment", sum((F0."c_acctbal" * F0."bst_r")) AS "c_acctbal", max(((CASE  WHEN (F0."ub_c_mktsegment" = F0."lb_c_mktsegment") THEN 1 ELSE 0 END) * F0."cet_r")) AS "cet_r", max(F0."bst_r") AS "bst_r", F0."c_mktsegment" AS "c_mktsegment"
FROM "customer_2_1000_radb" AS F0
GROUP BY F0."c_mktsegment") F0 JOIN (
SELECT min(F0."tid") AS "tid", min(F0."c_custkey") AS "c_custkey", min(F0."c_name") AS "c_name", min(F0."c_address") AS "c_address", min(F0."c_nationkey") AS "c_nationkey", min(F0."c_phone") AS "c_phone", min(F0."c_acctbal") AS "c_acctbal", (CASE  WHEN (F0."c_mktsegment" >= 'GKKKKNCMRb') THEN 'GKKKKNCMRb' WHEN ((F0."c_mktsegment" < 'GKKKKNCMRb') AND (F0."c_mktsegment" >= 'AUTOMOBILE')) THEN 'AUTOMOBILE' ELSE 'AUTOMOBILE' END) AS "c_mktsegment", min(F0."c_comment") AS "c_comment", max(F0."ub_tid") AS "ub_tid", min(F0."lb_tid") AS "lb_tid", max(F0."ub_c_custkey") AS "ub_c_custkey", min(F0."lb_c_custkey") AS "lb_c_custkey", max(F0."ub_c_name") AS "ub_c_name", min(F0."lb_c_name") AS "lb_c_name", max(F0."ub_c_address") AS "ub_c_address", min(F0."lb_c_address") AS "lb_c_address", max(F0."ub_c_nationkey") AS "ub_c_nationkey", min(F0."lb_c_nationkey") AS "lb_c_nationkey", max(F0."ub_c_phone") AS "ub_c_phone", min(F0."lb_c_phone") AS "lb_c_phone", max(F0."ub_c_acctbal") AS "ub_c_acctbal", min(F0."lb_c_acctbal") AS "lb_c_acctbal", max(F0."ub_c_mktsegment") AS "ub_c_mktsegment", min(F0."lb_c_mktsegment") AS "lb_c_mktsegment", max(F0."ub_c_comment") AS "ub_c_comment", min(F0."lb_c_comment") AS "lb_c_comment", sum(0) AS "cet_r", sum(0) AS "bst_r", sum(F0."bst_r") AS "pos_r"
FROM "customer_2_1000_radb" AS F0
GROUP BY (CASE  WHEN (F0."c_mktsegment" >= 'GKKKKNCMRb') THEN 'GKKKKNCMRb' WHEN ((F0."c_mktsegment" < 'GKKKKNCMRb') AND (F0."c_mktsegment" >= 'AUTOMOBILE')) THEN 'AUTOMOBILE' ELSE 'AUTOMOBILE' END)) F1 ON (((F0."ub_c_mktsegment" >= F1."lb_c_mktsegment") AND (F1."ub_c_mktsegment" >= F0."lb_c_mktsegment"))))
GROUP BY F0."c_mktsegment", F0."ub_c_mktsegment", F0."lb_c_mktsegment";