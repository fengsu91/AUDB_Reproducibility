select l1.tid,c_custkey,c_name,c_address,c_nationkey,c_phone,c_acctbal,c_mktsegment,c_comment,u_c_custkey,u_c_name,u_c_address,u_c_nationkey,u_c_phone,u_c_acctbal,u_c_mktsegment,u_c_comment from 
(Select c1.tid,c_custkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_c_custkey from 
(select DISTINCT ON (tid) tid, c_custkey, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_custkey) c1) l1
JOIN
(Select c2.tid,c_name,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_c_name from 
(select DISTINCT ON (tid) tid, c_name, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_name) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,c_address,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_c_address from
(select DISTINCT ON (tid) tid, c_address, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_address) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,c_nationkey,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_c_nationkey from
(select DISTINCT ON (tid) tid, c_nationkey, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_nationkey) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,c_phone,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_c_phone from
(select DISTINCT ON (tid) tid, c_phone, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_phone) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,c_acctbal,CASE WHEN c6.w1>1 THEN 0 ELSE 1 END AS u_c_acctbal from
(select DISTINCT ON (tid) tid, c_acctbal, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_acctbal) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,c_mktsegment,CASE WHEN c7.w1>1 THEN 0 ELSE 1 END AS u_c_mktsegment from
(select DISTINCT ON (tid) tid, c_mktsegment, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_mktsegment) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,c_comment,CASE WHEN c8.w1>1 THEN 0 ELSE 1 END AS u_c_comment from
(select DISTINCT ON (tid) tid, c_comment, max(w1) OVER (PARTITION BY tid) as w1 from cust_c_comment) c8) l8 on l1.tid=l8.tid;