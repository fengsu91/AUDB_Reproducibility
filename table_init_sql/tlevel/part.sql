select l1.tid,p_partkey,p_name,p_mfgr,p_brand,p_type,p_size,p_container,p_retailprice,p_comment,
u_p_partkey,u_p_name,u_p_mfgr,u_p_brand,u_p_type,u_p_size,u_p_container,u_p_retailprice,u_p_comment from
(Select c1.tid,p_partkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_p_partkey from
(select DISTINCT ON (tid) tid, p_partkey, max(w1) OVER (PARTITION BY tid) as w1 from part_p_partkey) c1) l1
JOIN
(Select c2.tid,p_name,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_p_name from
(select DISTINCT ON (tid) tid, p_name, max(w1) OVER (PARTITION BY tid) as w1 from part_p_name) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,p_mfgr,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_p_mfgr from
(select DISTINCT ON (tid) tid, p_mfgr, max(w1) OVER (PARTITION BY tid) as w1 from part_p_mfgr) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,p_brand,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_p_brand from
(select DISTINCT ON (tid) tid, p_brand, max(w1) OVER (PARTITION BY tid) as w1 from part_p_brand) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,p_type,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_p_type from
(select DISTINCT ON (tid) tid, p_type, max(w1) OVER (PARTITION BY tid) as w1 from part_p_type) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,p_size,CASE WHEN c6.w1>1 THEN 0 ELSE 1 END AS u_p_size from
(select DISTINCT ON (tid) tid, p_size, max(w1) OVER (PARTITION BY tid) as w1 from part_p_size) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,p_container,CASE WHEN c7.w1>1 THEN 0 ELSE 1 END AS u_p_container from
(select DISTINCT ON (tid) tid, p_container, max(w1) OVER (PARTITION BY tid) as w1 from part_p_container) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,p_retailprice,CASE WHEN c8.w1>1 THEN 0 ELSE 1 END AS u_p_retailprice from
(select DISTINCT ON (tid) tid, p_retailprice, max(w1) OVER (PARTITION BY tid) as w1 from part_p_retailprice) c8) l8 on l1.tid=l8.tid
JOIN
(Select c9.tid,p_comment,CASE WHEN c9.w1>1 THEN 0 ELSE 1 END AS u_p_comment from
(select DISTINCT ON (tid) tid, p_comment, max(w1) OVER (PARTITION BY tid) as w1 from part_p_comment) c9) l9 on l1.tid=l9.tid;