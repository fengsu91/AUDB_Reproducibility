select l1.tid,o_orderkey,o_custkey,o_orderstatus,o_totalprice,o_orderdate,o_orderpriority,o_clerk,o_shippriority,o_comment,
u_o_orderkey,u_o_custkey,u_o_orderstatus,u_o_totalprice,u_o_orderdate,u_o_orderpriority,u_o_clerk,u_o_shippriority,u_o_comment from
(Select c1.tid,o_orderkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_o_orderkey from
(select DISTINCT ON (tid) tid, o_orderkey, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_orderkey) c1) l1
JOIN
(Select c2.tid,o_custkey,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_o_custkey from
(select DISTINCT ON (tid) tid, o_custkey, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_custkey) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,o_orderstatus,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_o_orderstatus from
(select DISTINCT ON (tid) tid, o_orderstatus, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_orderstatus) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,o_totalprice,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_o_totalprice from
(select DISTINCT ON (tid) tid, o_totalprice, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_totalprice) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,o_orderdate,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_o_orderdate from
(select DISTINCT ON (tid) tid, o_orderdate, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_orderdate) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,o_orderpriority,CASE WHEN c6.w1>1 THEN 0 ELSE 1 END AS u_o_orderpriority from
(select DISTINCT ON (tid) tid, o_orderpriority, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_orderpriority) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,o_clerk,CASE WHEN c7.w1>1 THEN 0 ELSE 1 END AS u_o_clerk from
(select DISTINCT ON (tid) tid, o_clerk, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_clerk) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,o_shippriority,CASE WHEN c8.w1>1 THEN 0 ELSE 1 END AS u_o_shippriority from
(select DISTINCT ON (tid) tid, o_shippriority, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_shippriority) c8) l8 on l1.tid=l8.tid
JOIN
(Select c9.tid,o_comment,CASE WHEN c9.w1>1 THEN 0 ELSE 1 END AS u_o_comment from
(select DISTINCT ON (tid) tid, o_comment, max(w1) OVER (PARTITION BY tid) as w1 from orders_o_comment) c9) l9 on l1.tid=l9.tid;