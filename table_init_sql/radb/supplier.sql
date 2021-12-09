select l1.tid,s_suppkey,s_name,s_address,s_nationkey,s_phone,s_acctbal,s_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_s_suppkey,lb_s_suppkey,
ub_s_name,lb_s_name,
ub_s_address,lb_s_address,
ub_s_nationkey,lb_s_nationkey,
ub_s_phone,lb_s_phone,
ub_s_acctbal,lb_s_acctbal,
ub_s_comment,lb_s_comment,
case when ub_s_suppkey=lb_s_suppkey and ub_s_name=lb_s_name and ub_s_address=lb_s_address and ub_s_nationkey=lb_s_nationkey and ub_s_phone=lb_s_phone and ub_s_acctbal=lb_s_acctbal and ub_s_comment=lb_s_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_s_suppkey as s_suppkey,ub_s_suppkey,lb_s_suppkey from
(select tid,max(s_suppkey) as ub_s_suppkey,min(s_suppkey) as lb_s_suppkey from supp_s_suppkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_s_name as s_name,ub_s_name,lb_s_name from
(select tid,max(s_name) as ub_s_name,min(s_name) as lb_s_name from supp_s_name group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_s_address as s_address,ub_s_address,lb_s_address from
(select tid,max(s_address) as ub_s_address,min(s_address) as lb_s_address from supp_s_address group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_s_nationkey as s_nationkey,ub_s_nationkey,lb_s_nationkey from
(select tid,max(s_nationkey) as ub_s_nationkey,min(s_nationkey) as lb_s_nationkey from supp_s_nationkey group by tid) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ub_s_phone as s_phone,ub_s_phone,lb_s_phone from
(select tid,max(s_phone) as ub_s_phone,min(s_phone) as lb_s_phone from supp_s_phone group by tid) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,ub_s_acctbal as s_acctbal,ub_s_acctbal,lb_s_acctbal from
(select tid,max(s_acctbal) as ub_s_acctbal,min(s_acctbal) as lb_s_acctbal from supp_s_acctbal group by tid) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,ub_s_comment as s_comment,ub_s_comment,lb_s_comment from
(select tid,max(s_comment) as ub_s_comment,min(s_comment) as lb_s_comment from supp_s_comment group by tid) c7) l7 on l1.tid=l7.tid;