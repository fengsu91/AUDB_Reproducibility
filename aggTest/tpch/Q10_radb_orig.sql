urange (select c_custkey, c_name, sum(l_extendedprice * (1 - l_discount)) as revenue, c_acctbal, n_name, c_address, c_phone, c_comment from ((((select c_custkey,c_name,c_acctbal,c_address, c_phone, c_comment,c_nationkey from customer_radb is radb) join (select o_custkey,o_orderkey from orders_radb is radb where o_orderdate >= '1993-09-01' and o_orderdate < '1993-12-01') on c_custkey = o_custkey) join (select l_extendedprice,l_discount,l_orderkey from lineitem_radb is radb where l_returnflag = 'R') on o_orderkey = l_orderkey) join (select n_name,n_nationkey from nation_radb is radb) on c_nationkey = n_nationkey) group by c_custkey, c_name, c_acctbal, c_phone, n_name, c_address, c_comment);