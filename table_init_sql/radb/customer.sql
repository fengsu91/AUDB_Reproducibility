select l1.tid,c_custkey,c_name,c_address,c_nationkey,c_phone,c_acctbal,c_mktsegment,c_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_c_custkey,lb_c_custkey,
ub_c_name,lb_c_name,
ub_c_address,lb_c_address,
ub_c_nationkey,lb_c_nationkey,
ub_c_phone,lb_c_phone,
ub_c_acctbal,lb_c_acctbal,
ub_c_mktsegment,lb_c_mktsegment,
ub_c_comment,lb_c_comment,
case when ub_c_custkey=lb_c_custkey and ub_c_name=lb_c_name and ub_c_address=lb_c_address and ub_c_nationkey=lb_c_nationkey and ub_c_phone=lb_c_phone and ub_c_acctbal=lb_c_acctbal and ub_c_mktsegment=lb_c_mktsegment and ub_c_comment=lb_c_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r
from 
(Select c1.tid,ub_c_custkey as c_custkey,ub_c_custkey,lb_c_custkey from 
(select tid,max(c_custkey) as ub_c_custkey,min(c_custkey) as lb_c_custkey from cust_c_custkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_c_name as c_name,ub_c_name,lb_c_name from 
(select tid,max(c_name) as ub_c_name,min(c_name) as lb_c_name from cust_c_name group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_c_address as c_address,ub_c_address,lb_c_address from
(select tid,max(c_address) as ub_c_address,min(c_address) as lb_c_address from cust_c_address group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_c_nationkey as c_nationkey,ub_c_nationkey,lb_c_nationkey from
(select tid,max(c_nationkey) as ub_c_nationkey,min(c_nationkey) as lb_c_nationkey from cust_c_nationkey group by tid) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ub_c_phone as c_phone,ub_c_phone,lb_c_phone from
(select tid,max(c_phone) as ub_c_phone,min(c_phone) as lb_c_phone from cust_c_phone group by tid) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,ub_c_acctbal as c_acctbal,ub_c_acctbal,lb_c_acctbal from
(select tid,max(c_acctbal) as ub_c_acctbal,min(c_acctbal) as lb_c_acctbal from cust_c_acctbal group by tid) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,ub_c_mktsegment as c_mktsegment,ub_c_mktsegment,lb_c_mktsegment from
(select tid,max(c_mktsegment) as ub_c_mktsegment,min(c_mktsegment) as lb_c_mktsegment from cust_c_mktsegment group by tid) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,ub_c_comment as c_comment,ub_c_comment,lb_c_comment from
(select tid,max(c_comment) as ub_c_comment,min(c_comment) as lb_c_comment from cust_c_comment group by tid) c8) l8 on l1.tid=l8.tid;