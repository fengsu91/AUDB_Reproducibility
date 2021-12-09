SELECT U1.n_name as n_name, U2.n_name as n_name2, least(u1.u_r, u2.u_r) as u_r from 
(
(SELECT n_name, o_orderkey, least(least(c.u_r,o.u_r),n.u_r) as u_r FROM customer_uadb c, orders_uadb o, nation_uadb n WHERE c_nationkey = n_nationkey AND c_custkey = o_custkey AND n_name = 'CHINA') u1 
JOIN 
(SELECT n_name, o_orderkey, least(least(least(s.u_r,o.u_r),l.u_r),n.u_r) as u_r FROM supplier_uadb s, orders_uadb o, lineitem_uadb l, nation_uadb n WHERE s_nationkey = n_nationkey AND o_orderkey = l_orderkey AND s_suppkey = l_suppkey AND n_name = 'GERMANY') u2 ON u1.o_orderkey=u2.o_orderkey);