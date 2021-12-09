select l1.tid,ps_partkey,ps_suppkey,ps_availqty,ps_supplycost,ps_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_ps_partkey,lb_ps_partkey,
ub_ps_suppkey,lb_ps_suppkey,
ub_ps_availqty,lb_ps_availqty,
ub_ps_supplycost,lb_ps_supplycost,
ub_ps_comment,lb_ps_comment,
case when ub_ps_partkey=lb_ps_partkey and ub_ps_suppkey=lb_ps_suppkey and ub_ps_availqty=lb_ps_availqty and ub_ps_supplycost=lb_ps_supplycost and ub_ps_comment=lb_ps_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_ps_partkey as ps_partkey,ub_ps_partkey,lb_ps_partkey from
(select tid,max(ps_partkey) as ub_ps_partkey,min(ps_partkey) as lb_ps_partkey from psupp_ps_partkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_ps_suppkey as ps_suppkey,ub_ps_suppkey,lb_ps_suppkey from
(select tid,max(ps_suppkey) as ub_ps_suppkey,min(ps_suppkey) as lb_ps_suppkey from psupp_ps_suppkey group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_ps_availqty as ps_availqty,ub_ps_availqty,lb_ps_availqty from
(select tid,max(ps_availqty) as ub_ps_availqty,min(ps_availqty) as lb_ps_availqty from psupp_ps_availqty group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_ps_supplycost as ps_supplycost,ub_ps_supplycost,lb_ps_supplycost from
(select tid,max(ps_supplycost) as ub_ps_supplycost,min(ps_supplycost) as lb_ps_supplycost from psupp_ps_supplycost group by tid) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ub_ps_comment as ps_comment,ub_ps_comment,lb_ps_comment from
(select tid,max(ps_comment) as ub_ps_comment,min(ps_comment) as lb_ps_comment from psupp_ps_comment group by tid) c5) l5 on l1.tid=l5.tid;