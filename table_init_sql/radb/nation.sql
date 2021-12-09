select l1.tid,n_nationkey,n_name,n_regionkey,n_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_n_nationkey,lb_n_nationkey,
ub_n_name,lb_n_name,
ub_n_regionkey,lb_n_regionkey,
ub_n_comment,lb_n_comment,
case when ub_n_nationkey=lb_n_nationkey and ub_n_name=lb_n_name and ub_n_regionkey=lb_n_regionkey and ub_n_comment=lb_n_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_n_nationkey as n_nationkey,ub_n_nationkey,lb_n_nationkey from
(select tid,max(n_nationkey) as ub_n_nationkey,min(n_nationkey) as lb_n_nationkey from nation_n_nationkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_n_name as n_name,ub_n_name,lb_n_name from
(select tid,max(n_name) as ub_n_name,min(n_name) as lb_n_name from nation_n_name group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_n_regionkey as n_regionkey,ub_n_regionkey,lb_n_regionkey from
(select tid,max(n_regionkey) as ub_n_regionkey,min(n_regionkey) as lb_n_regionkey from nation_n_regionkey group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_n_comment as n_comment,ub_n_comment,lb_n_comment from
(select tid,max(n_comment) as ub_n_comment,min(n_comment) as lb_n_comment from nation_n_comment group by tid) c4) l4 on l1.tid=l4.tid;