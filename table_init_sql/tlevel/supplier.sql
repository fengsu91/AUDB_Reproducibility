select l1.tid,s_suppkey,s_name,s_address,s_nationkey,s_phone,s_acctbal,s_comment,
u_s_suppkey,u_s_name,u_s_address,u_s_nationkey,u_s_phone,u_s_acctbal,u_s_comment from
(Select c1.tid,s_suppkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_s_suppkey from
(select DISTINCT ON (tid) tid, s_suppkey, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_suppkey) c1) l1
JOIN
(Select c2.tid,s_name,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_s_name from
(select DISTINCT ON (tid) tid, s_name, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_name) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,s_address,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_s_address from
(select DISTINCT ON (tid) tid, s_address, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_address) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,s_nationkey,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_s_nationkey from
(select DISTINCT ON (tid) tid, s_nationkey, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_nationkey) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,s_phone,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_s_phone from
(select DISTINCT ON (tid) tid, s_phone, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_phone) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,s_acctbal,CASE WHEN c6.w1>1 THEN 0 ELSE 1 END AS u_s_acctbal from
(select DISTINCT ON (tid) tid, s_acctbal, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_acctbal) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,s_comment,CASE WHEN c7.w1>1 THEN 0 ELSE 1 END AS u_s_comment from
(select DISTINCT ON (tid) tid, s_comment, max(w1) OVER (PARTITION BY tid) as w1 from supp_s_comment) c7) l7 on l1.tid=l7.tid;