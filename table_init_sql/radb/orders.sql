select l1.tid,o_orderkey,o_custkey,o_orderstatus,o_totalprice,o_orderdate,o_orderpriority,o_clerk,o_shippriority,o_comment,
l1.tid as ub_tid,l1.tid as lb_tid,
ub_o_orderkey,lb_o_orderkey,
ub_o_custkey,lb_o_custkey,
ub_o_orderstatus,lb_o_orderstatus,
ub_o_totalprice,lb_o_totalprice,
ub_o_orderdate,lb_o_orderdate,
ub_o_orderpriority,lb_o_orderpriority,
ub_o_clerk,lb_o_clerk,
ub_o_shippriority,lb_o_shippriority,
ub_o_comment,lb_o_comment,
case when ub_o_orderkey=lb_o_orderkey and ub_o_custkey=lb_o_custkey and ub_o_orderstatus=lb_o_orderstatus and ub_o_totalprice=lb_o_totalprice and ub_o_orderdate=lb_o_orderdate and ub_o_orderpriority=lb_o_orderpriority and ub_o_clerk=lb_o_clerk and ub_o_shippriority=lb_o_shippriority and ub_o_comment=lb_o_comment then 1 else 0 end as cet_r,
1 as bst_r,
1 as pos_r 
from
(Select c1.tid,ub_o_orderkey as o_orderkey,ub_o_orderkey,lb_o_orderkey from
(select tid,max(o_orderkey) as ub_o_orderkey,min(o_orderkey) as lb_o_orderkey from orders_o_orderkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_o_custkey as o_custkey,ub_o_custkey,lb_o_custkey from
(select tid,max(o_custkey) as ub_o_custkey,min(o_custkey) as lb_o_custkey from orders_o_custkey group by tid) c2) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,ub_o_orderstatus as o_orderstatus,ub_o_orderstatus,lb_o_orderstatus from
(select tid,max(o_orderstatus) as ub_o_orderstatus,min(o_orderstatus) as lb_o_orderstatus from orders_o_orderstatus group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_o_totalprice as o_totalprice,ub_o_totalprice,lb_o_totalprice from
(select tid,max(o_totalprice) as ub_o_totalprice,min(o_totalprice) as lb_o_totalprice from orders_o_totalprice group by tid) c4) l4 on l1.tid=l4.tid
JOIN
(Select c5.tid,ub_o_orderdate as o_orderdate,ub_o_orderdate,lb_o_orderdate from
(select tid,max(o_orderdate) as ub_o_orderdate,min(o_orderdate) as lb_o_orderdate from orders_o_orderdate group by tid) c5) l5 on l1.tid=l5.tid
JOIN
(Select c6.tid,ub_o_orderpriority as o_orderpriority,ub_o_orderpriority,lb_o_orderpriority from
(select tid,max(o_orderpriority) as ub_o_orderpriority,min(o_orderpriority) as lb_o_orderpriority from orders_o_orderpriority group by tid) c6) l6 on l1.tid=l6.tid
JOIN
(Select c7.tid,ub_o_clerk as o_clerk,ub_o_clerk,lb_o_clerk from
(select tid,max(o_clerk) as ub_o_clerk,min(o_clerk) as lb_o_clerk from orders_o_clerk group by tid) c7) l7 on l1.tid=l7.tid
JOIN
(Select c8.tid,ub_o_shippriority as o_shippriority,ub_o_shippriority,lb_o_shippriority from
(select tid,max(o_shippriority) as ub_o_shippriority,min(o_shippriority) as lb_o_shippriority from orders_o_shippriority group by tid) c8) l8 on l1.tid=l8.tid
JOIN
(Select c9.tid,ub_o_comment as o_comment,ub_o_comment,lb_o_comment from
(select tid,max(o_comment) as ub_o_comment,min(o_comment) as lb_o_comment from orders_o_comment group by tid) c9) l9 on l1.tid=l9.tid;