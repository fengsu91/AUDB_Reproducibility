select n_name, sum(l_extendedprice * (1 - l_discount)) as revenue from
(((select r_regionkey from region_bg where r_name = 'ASIA') as r1 join (select n_name, n_nationkey, n_regionkey from nation_bg) as n1 on r_regionkey = n_regionkey) as j1 join 
(select * from (((((select o_orderkey, o_custkey from orders_bg where o_orderdate >= '1994-01-01' and o_orderdate < '1995-01-01') as o1
 join (select l_orderkey,l_suppkey,l_extendedprice,l_discount from lineitem_bg) as l1 on o_orderkey = l_orderkey) as j2
 join (select c_custkey, c_nationkey from customer_bg) as c1 on o_custkey = c_custkey) as j3
 join (select s_suppkey, s_nationkey from supplier_bg) as s1 on l_suppkey = s_suppkey))as j4 where c_nationkey = s_nationkey) as s1 on n_nationkey = s_nationkey) as j5
group by n_name;