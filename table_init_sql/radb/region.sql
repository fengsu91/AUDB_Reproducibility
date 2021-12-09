select l1.tid,r_regionkey,r_name,r_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_r_regionkey,lb_r_regionkey,
ub_r_name,lb_r_name,
ub_r_comment,lb_r_comment,
case when ub_r_regionkey=lb_r_regionkey and ub_r_name=lb_r_name and ub_r_regionkey=lb_r_regionkey then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_r_regionkey as r_regionkey,ub_r_regionkey,lb_r_regionkey from
(select tid,max(r_regionkey) as ub_r_regionkey,min(r_regionkey) as lb_r_regionkey from region_r_regionkey group by tid) c1 ) l1
JOIN
(Select c2.tid,ub_r_name as r_name,ub_r_name,lb_r_name from
(select tid,max(r_name) as ub_r_name,min(r_name) as lb_r_name from region_r_name group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_r_comment as r_comment,ub_r_comment,lb_r_comment from
(select tid,max(r_comment) as ub_r_comment,min(r_comment) as lb_r_comment from region_r_comment group by tid) c3) l3 on l1.tid=l3.tid;