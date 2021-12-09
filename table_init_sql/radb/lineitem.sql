select l1.tid,l_orderkey,l_partkey,l_suppkey,l_linenumber,l_quantity,l_extendedprice,l_discount,l_tax,l_returnflag,l_linestatus,l_shipdate,l_commitdate,l_receiptdate,l_shipinstruct,l_shipmode,l_comment, 
l1.tid as ub_tid,l1.tid as lb_tid,
ub_l_orderkey,lb_l_orderkey,
ub_l_partkey,lb_l_partkey,
ub_l_suppkey,lb_l_suppkey,
ub_l_linenumber,lb_l_linenumber,
ub_l_quantity,lb_l_quantity,
ub_l_extendedprice,lb_l_extendedprice,
ub_l_discount,lb_l_discount,
ub_l_tax,lb_l_tax,
ub_l_returnflag,lb_l_returnflag,
ub_l_linestatus,lb_l_linestatus,
ub_l_shipdate,lb_l_shipdate,
ub_l_commitdate,lb_l_commitdate,
ub_l_receiptdate,lb_l_receiptdate,
ub_l_shipinstruct,lb_l_shipinstruct,
ub_l_shipmode,lb_l_shipmode,
ub_l_comment,lb_l_comment,
case when ub_l_orderkey=lb_l_orderkey and ub_l_partkey=lb_l_partkey and ub_l_suppkey=lb_l_suppkey and ub_l_linenumber=lb_l_linenumber and ub_l_quantity=lb_l_quantity and ub_l_extendedprice=lb_l_extendedprice and ub_l_discount=lb_l_discount and ub_l_tax=lb_l_tax and ub_l_returnflag=lb_l_returnflag and ub_l_linestatus=lb_l_linestatus and ub_l_shipdate=lb_l_shipdate and ub_l_commitdate=lb_l_commitdate and ub_l_receiptdate=lb_l_receiptdate and ub_l_shipinstruct=lb_l_shipinstruct and ub_l_shipmode=lb_l_shipmode and ub_l_comment=lb_l_comment then 1 else 0 end as cet_r, 
1 as bst_r,
1 as pos_r
from
(Select c1.tid,ub_l_orderkey as l_orderkey,ub_l_orderkey,lb_l_orderkey from
(select tid,max(l_orderkey) as ub_l_orderkey,min(l_orderkey) as lb_l_orderkey from lineitem_l_orderkey group by tid) c1) l1
JOIN
(Select c2.tid,ub_l_partkey as l_partkey,ub_l_partkey,lb_l_partkey from
(select tid,max(l_partkey) as ub_l_partkey,min(l_partkey) as lb_l_partkey from lineitem_l_partkey group by tid) c2) l2 on l1.tid=l2.tid 
JOIN
(Select c3.tid,ub_l_suppkey as l_suppkey,ub_l_suppkey,lb_l_suppkey from
(select tid,max(l_suppkey) as ub_l_suppkey,min(l_suppkey) as lb_l_suppkey from lineitem_l_suppkey group by tid) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,ub_l_linenumber as l_linenumber,ub_l_linenumber,lb_l_linenumber from
(select tid,max(l_linenumber) as ub_l_linenumber,min(l_linenumber) as lb_l_linenumber from lineitem_l_linenumber group by tid) c4) l4 on l1.tid=l4.tid 
JOIN
(Select c5.tid,ub_l_quantity as l_quantity,ub_l_quantity,lb_l_quantity from
(select tid,max(l_quantity) as ub_l_quantity,min(l_quantity) as lb_l_quantity from lineitem_l_quantity group by tid) c5) l5 on l1.tid=l5.tid 
JOIN
(Select c6.tid,ub_l_extendedprice as l_extendedprice,ub_l_extendedprice,lb_l_extendedprice from
(select tid,max(l_extendedprice) as ub_l_extendedprice,min(l_extendedprice) as lb_l_extendedprice from lineitem_l_extendedprice group by tid) c6) l6 on l1.tid=l6.tid 
JOIN
(Select c7.tid,ub_l_discount as l_discount,ub_l_discount,lb_l_discount from
(select tid,max(l_discount) as ub_l_discount,min(l_discount) as lb_l_discount from lineitem_l_discount group by tid) c7) l7 on l1.tid=l7.tid 
JOIN
(Select c8.tid,ub_l_tax as l_tax,ub_l_tax,lb_l_tax from
(select tid,max(l_tax) as ub_l_tax,min(l_tax) as lb_l_tax from lineitem_l_tax group by tid) c8) l8 on l1.tid=l8.tid 
JOIN
(Select c9.tid,ub_l_returnflag as l_returnflag,ub_l_returnflag,lb_l_returnflag from
(select tid,max(l_returnflag) as ub_l_returnflag,min(l_returnflag) as lb_l_returnflag from lineitem_l_returnflag group by tid) c9) l9 on l1.tid=l9.tid 
JOIN
(Select c10.tid,ub_l_linestatus as l_linestatus,ub_l_linestatus,lb_l_linestatus from
(select tid,max(l_linestatus) as ub_l_linestatus,min(l_linestatus) as lb_l_linestatus from lineitem_l_linestatus group by tid) c10) l10 on l1.tid=l10.tid 
JOIN
(Select c11.tid,ub_l_shipdate as l_shipdate,ub_l_shipdate,lb_l_shipdate from
(select tid,max(l_shipdate) as ub_l_shipdate,min(l_shipdate) as lb_l_shipdate from lineitem_l_shipdate group by tid) c11) l11 on l1.tid=l11.tid 
JOIN
(Select c12.tid,ub_l_commitdate as l_commitdate,ub_l_commitdate,lb_l_commitdate from
(select tid,max(l_commitdate) as ub_l_commitdate,min(l_commitdate) as lb_l_commitdate from lineitem_l_commitdate group by tid) c12) l12 on l1.tid=l12.tid 
JOIN
(Select c13.tid,ub_l_receiptdate as l_receiptdate,ub_l_receiptdate,lb_l_receiptdate from
(select tid,max(l_receiptdate) as ub_l_receiptdate,min(l_receiptdate) as lb_l_receiptdate from lineitem_l_receiptdate group by tid) c13) l13 on l1.tid=l13.tid 
JOIN
(Select c14.tid,ub_l_shipinstruct as l_shipinstruct,ub_l_shipinstruct,lb_l_shipinstruct from
(select tid,max(l_shipinstruct) as ub_l_shipinstruct,min(l_shipinstruct) as lb_l_shipinstruct from lineitem_l_shipinstruct group by tid) c14) l14 on l1.tid=l14.tid 
JOIN
(Select c15.tid,ub_l_shipmode as l_shipmode,ub_l_shipmode,lb_l_shipmode from
(select tid,max(l_shipmode) as ub_l_shipmode,min(l_shipmode) as lb_l_shipmode from lineitem_l_shipmode group by tid) c15) l15 on l1.tid=l15.tid 
JOIN
(Select c16.tid,ub_l_comment as l_comment,ub_l_comment,lb_l_comment from
(select tid,max(l_comment) as ub_l_comment,min(l_comment) as lb_l_comment from lineitem_l_comment group by tid) c16) l16 on l1.tid=l16.tid;