select l1.tid,p_partkey,p_name,p_mfgr,p_brand,p_type,p_size,p_container,p_retailprice,p_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_p_partkey,lb_p_partkey,
ub_p_name,lb_p_name,
ub_p_mfgr,lb_p_mfgr,
ub_p_brand,lb_p_brand,
ub_p_type,lb_p_type,
ub_p_size,lb_p_size,
ub_p_container,lb_p_container,
ub_p_retailprice,lb_p_retailprice,
ub_p_comment,lb_p_comment,
case when ub_p_partkey=lb_p_partkey and ub_p_name=lb_p_name and ub_p_mfgr=lb_p_mfgr and ub_p_brand=lb_p_brand and ub_p_type=lb_p_type and ub_p_size=lb_p_size and ub_p_container=lb_p_container and ub_p_retailprice=lb_p_retailprice and ub_p_comment=lb_p_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_p_partkey as p_partkey,ub_p_partkey,lb_p_partkey from
(select tid,max(p_partkey) as ub_p_partkey,min(p_partkey) as lb_p_partkey from part_p_partkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_p_name as p_name,ub_p_name,lb_p_name from
(select tid,max(p_name) as ub_p_name,min(p_name) as lb_p_name from part_p_name group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_p_mfgr as p_mfgr,ub_p_mfgr,lb_p_mfgr from
(select tid,max(p_mfgr) as ub_p_mfgr,min(p_mfgr) as lb_p_mfgr from part_p_mfgr group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_p_brand as p_brand,ub_p_brand,lb_p_brand from
(select tid,max(p_brand) as ub_p_brand,min(p_brand) as lb_p_brand from part_p_brand group by tid) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ub_p_type as p_type,ub_p_type,lb_p_type from
(select tid,max(p_type) as ub_p_type,min(p_type) as lb_p_type from part_p_type group by tid) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,ub_p_size as p_size,ub_p_size,lb_p_size from
(select tid,max(p_size) as ub_p_size,min(p_size) as lb_p_size from part_p_size group by tid) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,ub_p_container as p_container,ub_p_container,lb_p_container from
(select tid,max(p_container) as ub_p_container,min(p_container) as lb_p_container from part_p_container group by tid) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,ub_p_retailprice as p_retailprice,ub_p_retailprice,lb_p_retailprice from
(select tid,max(p_retailprice) as ub_p_retailprice,min(p_retailprice) as lb_p_retailprice from part_p_retailprice group by tid) c8) l8 on l1.tid=l8.tid
JOIN
(Select c9.tid,ub_p_comment as p_comment,ub_p_comment,lb_p_comment from
(select tid,max(p_comment) as ub_p_comment,min(p_comment) as lb_p_comment from part_p_comment group by tid) c9) l9 on l1.tid=l9.tid;