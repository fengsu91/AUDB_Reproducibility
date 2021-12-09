select l1.tid,ps_partkey,ps_suppkey,ps_availqty,ps_supplycost,ps_comment,
u_ps_partkey,u_ps_suppkey,u_ps_availqty,u_ps_supplycost,u_ps_comment from
(Select c1.tid,ps_partkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_ps_partkey from
(select DISTINCT ON (tid) tid, ps_partkey, max(w1) OVER (PARTITION BY tid) as w1 from psupp_ps_partkey) c1) l1
JOIN
(Select c2.tid,ps_suppkey,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_ps_suppkey from
(select DISTINCT ON (tid) tid, ps_suppkey, max(w1) OVER (PARTITION BY tid) as w1 from psupp_ps_suppkey) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ps_availqty,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_ps_availqty from
(select DISTINCT ON (tid) tid, ps_availqty, max(w1) OVER (PARTITION BY tid) as w1 from psupp_ps_availqty) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ps_supplycost,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_ps_supplycost from
(select DISTINCT ON (tid) tid, ps_supplycost, max(w1) OVER (PARTITION BY tid) as w1 from psupp_ps_supplycost) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ps_comment,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_ps_comment from
(select DISTINCT ON (tid) tid, ps_comment, max(w1) OVER (PARTITION BY tid) as w1 from psupp_ps_comment) c5) l5 on l1.tid=l5.tid;