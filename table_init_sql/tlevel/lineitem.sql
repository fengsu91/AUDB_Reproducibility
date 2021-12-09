select l1.tid,l_orderkey,l_partkey,l_suppkey,l_linenumber,l_quantity,l_extendedprice,l_discount,l_tax,l_returnflag,l_linestatus,l_shipdate,l_commitdate,l_receiptdate,l_shipinstruct,l_shipmode,l_comment, 
u_l_orderkey,u_l_partkey,u_l_suppkey,u_l_linenumber,u_l_quantity,u_l_extendedprice,u_l_discount,u_l_tax,u_l_returnflag,u_l_linestatus,u_l_shipdate,u_l_commitdate,u_l_receiptdate,u_l_shipinstruct,u_l_shipmode,u_l_comment from
(Select c1.tid,l_orderkey,CASE WHEN c1.w1>1 THEN 0 ELSE 1 END AS u_l_orderkey from
(select DISTINCT ON (tid) tid, l_orderkey, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_orderkey) c1) l1
JOIN
(Select c2.tid,l_partkey,CASE WHEN c2.w1>1 THEN 0 ELSE 1 END AS u_l_partkey from
(select DISTINCT ON (tid) tid, l_partkey, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_partkey) c2) l2 on l1.tid=l2.tid 
JOIN
(Select c3.tid,l_suppkey,CASE WHEN c3.w1>1 THEN 0 ELSE 1 END AS u_l_suppkey from
(select DISTINCT ON (tid) tid, l_suppkey, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_suppkey) c3) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,l_linenumber,CASE WHEN c4.w1>1 THEN 0 ELSE 1 END AS u_l_linenumber from
(select DISTINCT ON (tid) tid, l_linenumber, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_linenumber) c4) l4 on l1.tid=l4.tid 
JOIN
(Select c5.tid,l_quantity,CASE WHEN c5.w1>1 THEN 0 ELSE 1 END AS u_l_quantity from
(select DISTINCT ON (tid) tid, l_quantity, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_quantity) c5) l5 on l1.tid=l5.tid 
JOIN
(Select c6.tid,l_extendedprice,CASE WHEN c6.w1>1 THEN 0 ELSE 1 END AS u_l_extendedprice from
(select DISTINCT ON (tid) tid, l_extendedprice, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_extendedprice) c6) l6 on l1.tid=l6.tid 
JOIN
(Select c7.tid,l_discount,CASE WHEN c7.w1>1 THEN 0 ELSE 1 END AS u_l_discount from
(select DISTINCT ON (tid) tid, l_discount, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_discount) c7) l7 on l1.tid=l7.tid 
JOIN
(Select c8.tid,l_tax,CASE WHEN c8.w1>1 THEN 0 ELSE 1 END AS u_l_tax from
(select DISTINCT ON (tid) tid, l_tax, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_tax) c8) l8 on l1.tid=l8.tid 
JOIN
(Select c9.tid,l_returnflag,CASE WHEN c9.w1>1 THEN 0 ELSE 1 END AS u_l_returnflag from
(select DISTINCT ON (tid) tid, l_returnflag, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_returnflag) c9) l9 on l1.tid=l9.tid 
JOIN
(Select c10.tid,l_linestatus,CASE WHEN c10.w1>1 THEN 0 ELSE 1 END AS u_l_linestatus from
(select DISTINCT ON (tid) tid, l_linestatus, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_linestatus) c10) l10 on l1.tid=l10.tid 
JOIN
(Select c11.tid,l_shipdate,CASE WHEN c11.w1>1 THEN 0 ELSE 1 END AS u_l_shipdate from
(select DISTINCT ON (tid) tid, l_shipdate, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_shipdate) c11) l11 on l1.tid=l11.tid 
JOIN
(Select c12.tid,l_commitdate,CASE WHEN c12.w1>1 THEN 0 ELSE 1 END AS u_l_commitdate from
(select DISTINCT ON (tid) tid, l_commitdate, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_commitdate) c12) l12 on l1.tid=l12.tid 
JOIN
(Select c13.tid,l_receiptdate,CASE WHEN c13.w1>1 THEN 0 ELSE 1 END AS u_l_receiptdate from
(select DISTINCT ON (tid) tid, l_receiptdate, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_receiptdate) c13) l13 on l1.tid=l13.tid 
JOIN
(Select c14.tid,l_shipinstruct,CASE WHEN c14.w1>1 THEN 0 ELSE 1 END AS u_l_shipinstruct from
(select DISTINCT ON (tid) tid, l_shipinstruct, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_shipinstruct) c14) l14 on l1.tid=l14.tid 
JOIN
(Select c15.tid,l_shipmode,CASE WHEN c15.w1>1 THEN 0 ELSE 1 END AS u_l_shipmode from
(select DISTINCT ON (tid) tid, l_shipmode, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_shipmode) c15) l15 on l1.tid=l15.tid 
JOIN
(Select c16.tid,l_comment,CASE WHEN c16.w1>1 THEN 0 ELSE 1 END AS u_l_comment from
(select DISTINCT ON (tid) tid, l_comment, max(w1) OVER (PARTITION BY tid) as w1 from lineitem_l_comment) c16) l16 on l1.tid=l16.tid;