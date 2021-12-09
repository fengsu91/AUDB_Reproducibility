SELECT F0."o_orderkey" AS "o_orderkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority", (F0."cet_r" * F1."cet_r") AS "cet_r", (F0."bst_r" * F1."bst_r") AS "bst_r", (F0."pos_r" * F1."pos_r") AS "pos_r"
FROM ((
SELECT F1."o_orderkey" AS "o_orderkey", F1."o_orderdate" AS "o_orderdate", F1."o_shippriority" AS "o_shippriority", F1."ub_o_orderkey" AS "ub_o_orderkey", F1."lb_o_orderkey" AS "lb_o_orderkey", F1."ub_o_orderdate" AS "ub_o_orderdate", F1."lb_o_orderdate" AS "lb_o_orderdate", F1."ub_o_shippriority" AS "ub_o_shippriority", F1."lb_o_shippriority" AS "lb_o_shippriority", (F0."cet_r" * F1."cet_r") AS "cet_r", (F0."bst_r" * F1."bst_r") AS "bst_r", (F0."pos_r" * F1."pos_r") AS "pos_r"
FROM ((
SELECT F0."c_custkey" AS "c_custkey", F0."lb_c_custkey" AS "lb_c_custkey", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM (
SELECT F0."c_custkey" AS "c_custkey", F0."c_custkey" AS "lb_c_custkey", (CASE  WHEN (F0."ub_c_custkey" = F0."lb_c_custkey") THEN F0."cet_r" ELSE 0 END) AS "cet_r", F0."bst_r" AS "bst_r", F0."bst_r" AS "pos_r"
FROM (
SELECT F0."c_custkey" AS "c_custkey", F0."ub_c_custkey" AS "ub_c_custkey", F0."lb_c_custkey" AS "lb_c_custkey", (CASE  WHEN ((F0."c_mktsegment" = 'BUILDING') AND ((F0."ub_c_mktsegment" = F0."lb_c_mktsegment") AND ('BUILDING' = 'BUILDING'))) THEN 1 ELSE 0 END) AS "cet_r", (CASE  WHEN (F0."c_mktsegment" = 'BUILDING') THEN 1 ELSE 0 END) AS "bst_r"
FROM (
SELECT F0."c_custkey" AS "c_custkey", F0."c_mktsegment" AS "c_mktsegment", F0."ub_c_custkey" AS "ub_c_custkey", F0."lb_c_custkey" AS "lb_c_custkey", F0."ub_c_mktsegment" AS "ub_c_mktsegment", F0."lb_c_mktsegment" AS "lb_c_mktsegment"
FROM "customer_radb" AS F0) F0
WHERE ((F0."lb_c_mktsegment" <= 'BUILDING') AND (F0."ub_c_mktsegment" >= 'BUILDING'))) F0
WHERE (F0."bst_r" > 0)) F0
WHERE (F0."c_custkey" = F0."lb_c_custkey")) F0 JOIN (
SELECT F0."o_custkey" AS "o_custkey", F0."o_orderkey" AS "o_orderkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."lb_o_custkey" AS "lb_o_custkey", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM (
SELECT F0."o_custkey" AS "o_custkey", F0."o_orderkey" AS "o_orderkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."o_custkey" AS "lb_o_custkey", F0."o_orderkey" AS "ub_o_orderkey", F0."o_orderkey" AS "lb_o_orderkey", F0."o_orderdate" AS "ub_o_orderdate", F0."o_orderdate" AS "lb_o_orderdate", F0."o_shippriority" AS "ub_o_shippriority", F0."o_shippriority" AS "lb_o_shippriority", (CASE  WHEN ((((F0."ub_o_custkey" = F0."lb_o_custkey") AND (F0."ub_o_orderkey" = F0."lb_o_orderkey")) AND (F0."ub_o_orderdate" = F0."lb_o_orderdate")) AND (F0."ub_o_shippriority" = F0."lb_o_shippriority")) THEN F0."cet_r" ELSE 0 END) AS "cet_r", F0."bst_r" AS "bst_r", F0."bst_r" AS "pos_r"
FROM (
SELECT F0."o_custkey" AS "o_custkey", F0."o_orderkey" AS "o_orderkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."ub_o_custkey" AS "ub_o_custkey", F0."lb_o_custkey" AS "lb_o_custkey", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority", (CASE  WHEN (F0."lb_o_orderdate" > '1995-03-15') THEN 1 ELSE 0 END) AS "cet_r", (CASE  WHEN (F0."o_orderdate" > '1995-03-15') THEN 1 ELSE 0 END) AS "bst_r"
FROM (
SELECT F0."o_orderkey" AS "o_orderkey", F0."o_custkey" AS "o_custkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_custkey" AS "ub_o_custkey", F0."lb_o_custkey" AS "lb_o_custkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority"
FROM "orders_radb" AS F0) F0
WHERE (F0."ub_o_orderdate" > '1995-03-15')) F0
WHERE (F0."bst_r" > 0)) F0
WHERE ((((F0."o_orderkey" = F0."lb_o_orderkey") AND (F0."o_custkey" = F0."lb_o_custkey")) AND (F0."o_orderdate" = F0."lb_o_orderdate")) AND (F0."o_shippriority" = F0."lb_o_shippriority"))) F1 ON ((F0."c_custkey" = F1."o_custkey")))) F0 JOIN (
SELECT F0."l_orderkey" AS "l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", F0."cet_r" AS "cet_r", F0."bst_r" AS "bst_r", F0."pos_r" AS "pos_r"
FROM (
SELECT F0."l_orderkey" AS "l_orderkey", F0."l_orderkey" AS "lb_l_orderkey", (CASE  WHEN (F0."ub_l_orderkey" = F0."lb_l_orderkey") THEN F0."cet_r" ELSE 0 END) AS "cet_r", F0."bst_r" AS "bst_r", F0."bst_r" AS "pos_r"
FROM (
SELECT F0."l_orderkey" AS "l_orderkey", F0."ub_l_orderkey" AS "ub_l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", (CASE  WHEN (F0."ub_l_shipdate" < '1995-03-17') THEN 1 ELSE 0 END) AS "cet_r", (CASE  WHEN (F0."l_shipdate" < '1995-03-17') THEN 1 ELSE 0 END) AS "bst_r"
FROM (
SELECT F0."l_orderkey" AS "l_orderkey", F0."l_shipdate" AS "l_shipdate", F0."ub_l_orderkey" AS "ub_l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", F0."ub_l_shipdate" AS "ub_l_shipdate", F0."lb_l_shipdate" AS "lb_l_shipdate"
FROM "lineitem_radb" AS F0) F0
WHERE (F0."lb_l_shipdate" < '1995-03-17')) F0
WHERE (F0."bst_r" > 0)) F0
WHERE (F0."l_orderkey" = F0."lb_l_orderkey")) F1 ON ((F0."o_orderkey" = F1."l_orderkey"))) UNION ALL
SELECT F0."o_orderkey" AS "o_orderkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority", ((F0."cet_r" + F1."cet_r") + (CASE  WHEN ((F0."o_orderkey" = F1."l_orderkey") AND ((F0."ub_o_orderkey" = F0."lb_o_orderkey") AND (F1."ub_l_orderkey" = F1."lb_l_orderkey"))) THEN 1 ELSE 0 END)) AS "cet_r", ((F0."bst_r" + F1."bst_r") + (CASE  WHEN (F0."o_orderkey" = F1."l_orderkey") THEN 1 ELSE 0 END)) AS "bst_r", (F0."pos_r" + F1."pos_r") AS "pos_r"
FROM ((
SELECT F1."o_orderkey" AS "o_orderkey", F1."o_orderdate" AS "o_orderdate", F1."o_shippriority" AS "o_shippriority", F1."ub_o_orderkey" AS "ub_o_orderkey", F1."lb_o_orderkey" AS "lb_o_orderkey", F1."ub_o_orderdate" AS "ub_o_orderdate", F1."lb_o_orderdate" AS "lb_o_orderdate", F1."ub_o_shippriority" AS "ub_o_shippriority", F1."lb_o_shippriority" AS "lb_o_shippriority", ((F0."cet_r" + F1."cet_r") + (CASE  WHEN ((F0."c_custkey" = F1."o_custkey") AND ((F0."ub_c_custkey" = F0."lb_c_custkey") AND (F1."ub_o_custkey" = F1."lb_o_custkey"))) THEN 1 ELSE 0 END)) AS "cet_r", ((F0."bst_r" + F1."bst_r") + (CASE  WHEN (F0."c_custkey" = F1."o_custkey") THEN 1 ELSE 0 END)) AS "bst_r", (F0."pos_r" + F1."pos_r") AS "pos_r"
FROM ((
SELECT (CASE  WHEN (F0."c_custkey" >= 7500) THEN 7500 WHEN ((F0."c_custkey" < 7500) AND (F0."c_custkey" >= 1)) THEN 1 ELSE 1 END) AS "c_custkey", max(F0."ub_c_custkey") AS "ub_c_custkey", min(F0."lb_c_custkey") AS "lb_c_custkey", sum(0) AS "cet_r", sum(0) AS "bst_r", sum((CASE  WHEN (F0."c_mktsegment" = 'BUILDING') THEN 1 ELSE 0 END)) AS "pos_r"
FROM (
SELECT F0."c_custkey" AS "c_custkey", F0."c_mktsegment" AS "c_mktsegment", F0."ub_c_custkey" AS "ub_c_custkey", F0."lb_c_custkey" AS "lb_c_custkey", F0."ub_c_mktsegment" AS "ub_c_mktsegment", F0."lb_c_mktsegment" AS "lb_c_mktsegment"
FROM "customer_radb" AS F0) F0
WHERE ((F0."lb_c_mktsegment" <= 'BUILDING') AND (F0."ub_c_mktsegment" >= 'BUILDING'))
GROUP BY (CASE  WHEN (F0."c_custkey" >= 7500) THEN 7500 WHEN ((F0."c_custkey" < 7500) AND (F0."c_custkey" >= 1)) THEN 1 ELSE 1 END)) F0 JOIN (
SELECT (CASE  WHEN (F0."o_custkey" >= 7500) THEN 7500 WHEN ((F0."o_custkey" < 7500) AND (F0."o_custkey" >= 1)) THEN 1 ELSE 1 END) AS "o_custkey", min(F0."o_orderkey") AS "o_orderkey", min(F0."o_orderdate") AS "o_orderdate", min(F0."o_shippriority") AS "o_shippriority", max(F0."ub_o_custkey") AS "ub_o_custkey", min(F0."lb_o_custkey") AS "lb_o_custkey", max(F0."ub_o_orderkey") AS "ub_o_orderkey", min(F0."lb_o_orderkey") AS "lb_o_orderkey", max(F0."ub_o_orderdate") AS "ub_o_orderdate", min(F0."lb_o_orderdate") AS "lb_o_orderdate", max(F0."ub_o_shippriority") AS "ub_o_shippriority", min(F0."lb_o_shippriority") AS "lb_o_shippriority", sum(0) AS "cet_r", sum(0) AS "bst_r", sum((CASE  WHEN (F0."o_orderdate" > '1995-03-15') THEN 1 ELSE 0 END)) AS "pos_r"
FROM (
SELECT F0."o_orderkey" AS "o_orderkey", F0."o_custkey" AS "o_custkey", F0."o_orderdate" AS "o_orderdate", F0."o_shippriority" AS "o_shippriority", F0."ub_o_orderkey" AS "ub_o_orderkey", F0."lb_o_orderkey" AS "lb_o_orderkey", F0."ub_o_custkey" AS "ub_o_custkey", F0."lb_o_custkey" AS "lb_o_custkey", F0."ub_o_orderdate" AS "ub_o_orderdate", F0."lb_o_orderdate" AS "lb_o_orderdate", F0."ub_o_shippriority" AS "ub_o_shippriority", F0."lb_o_shippriority" AS "lb_o_shippriority"
FROM "orders_radb" AS F0) F0
WHERE (F0."ub_o_orderdate" > '1995-03-15')
GROUP BY (CASE  WHEN (F0."o_custkey" >= 7500) THEN 7500 WHEN ((F0."o_custkey" < 7500) AND (F0."o_custkey" >= 1)) THEN 1 ELSE 1 END)) F1 ON (((F0."lb_c_custkey" <= F1."ub_o_custkey") AND (F0."ub_c_custkey" >= F1."lb_o_custkey"))))) F0 JOIN (
SELECT (CASE  WHEN (F0."l_orderkey" >= 300000) THEN 300000 WHEN ((F0."l_orderkey" < 300000) AND (F0."l_orderkey" >= 1)) THEN 1 ELSE 1 END) AS "l_orderkey", max(F0."ub_l_orderkey") AS "ub_l_orderkey", min(F0."lb_l_orderkey") AS "lb_l_orderkey", sum(0) AS "cet_r", sum(0) AS "bst_r", sum((CASE  WHEN (F0."l_shipdate" < '1995-03-17') THEN 1 ELSE 0 END)) AS "pos_r"
FROM (
SELECT F0."l_orderkey" AS "l_orderkey", F0."l_shipdate" AS "l_shipdate", F0."ub_l_orderkey" AS "ub_l_orderkey", F0."lb_l_orderkey" AS "lb_l_orderkey", F0."lb_l_shipdate" AS "lb_l_shipdate"
FROM "lineitem_radb" AS F0) F0
WHERE (F0."lb_l_shipdate" < '1995-03-17')
GROUP BY (CASE  WHEN (F0."l_orderkey" >= 300000) THEN 300000 WHEN ((F0."l_orderkey" < 300000) AND (F0."l_orderkey" >= 1)) THEN 1 ELSE 1 END)) F1 ON (((F0."lb_o_orderkey" <= F1."ub_l_orderkey") AND (F0."ub_o_orderkey" >= F1."lb_l_orderkey"))));
