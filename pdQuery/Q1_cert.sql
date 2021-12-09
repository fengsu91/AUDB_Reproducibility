SELECT o_orderkey,u_o_orderkey, o_orderdate,u_o_orderdate, o_shippriority,u_o_shippriority FROM customer c, orders o, lineitem l WHERE o_orderdate > '1995-03-15' AND u_o_orderdate > 0 AND l_shipdate < '1995-03-17' AND u_l_shipdate>0 AND c_mktsegment = 'BUILDING' AND u_c_mktsegment > 0 AND c_custkey = o_custkey AND o_orderkey = l_orderkey;