SELECT sum(F0."bst_r") AS "count(1)", sum(F0."pos_r") AS "ub_count(1)", sum(F0."cet_r") AS "lb_count(1)", 1 AS "cet_r", 1 AS "bst_r", 1 AS "pos_r"
FROM "customer_2_1000_radb" AS F0;