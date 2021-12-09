urange (select l_orderkey, sum(l_extendedprice*(1-l_discount)) as revenue, o_orderdate, o_shippriority from (((select c_custkey from customer_radb is radb where c_mktsegment = 'AUTOMOBILE') c1 join (select o_custkey, o_orderkey, o_orderdate, o_shippriority from orders_radb is radb where o_orderdate < '1995-03-15') o1 on c_custkey = o_custkey) j1 join (select l_orderkey, l_extendedprice, l_discount from lineitem_radb is radb where l_shipdate > '1995-03-15') l1 on o_orderkey = l_orderkey) j2 group by l_orderkey, o_orderdate, o_shippriority);