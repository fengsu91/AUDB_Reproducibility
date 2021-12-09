select o_orderkey,o_orderdate,o_shippriority from
(select o_orderkey,o_orderdate,o_shippriority from 
(select c_custkey from customer_uadb where c_mktsegment = 'BUILDING') a1
JOIN
(select o_orderkey,o_orderdate,o_shippriority,o_custkey from orders where o_orderdate > '1995-03-15') a2 ON c_custkey = o_custkey) b1
JOIN
(select l_orderkey,l_shipdate from lineitem where l_shipdate < '1995-03-17') b2
ON o_orderkey = l_orderkey;