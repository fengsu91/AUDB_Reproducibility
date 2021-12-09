create table w(cid bigint, lwid bigint, aid serial);
      
insert into w
select c1,w1 from nation_n_nationkey
union select c1,w1 from nation_n_name
union select c1,w1 from nation_n_regionkey
union select c1,w1 from nation_n_comment
      --
union select c1,w1 from region_r_regionkey
union select c1,w1 from region_r_name
union select c1,w1 from region_r_comment
      --
union select c1,w1 from part_p_partkey
union select c1,w1 from part_p_name
union select c1,w1 from part_p_mfgr
union select c1,w1 from part_p_brand
union select c1,w1 from part_p_type
union select c1,w1 from part_p_size
union select c1,w1 from part_p_container
union select c1,w1 from part_p_retailprice
union select c1,w1 from part_p_comment
      --
union select c1,w1 from supp_s_suppkey
union select c1,w1 from supp_s_name
union select c1,w1 from supp_s_address
union select c1,w1 from supp_s_nationkey
union select c1,w1 from supp_s_phone
union select c1,w1 from supp_s_acctbal
union select c1,w1 from supp_s_comment
      --
union select c1,w1 from psupp_ps_partkey
union select c1,w1 from psupp_ps_suppkey
union select c1,w1 from psupp_ps_availqty
union select c1,w1 from psupp_ps_supplycost
union select c1,w1 from psupp_ps_comment
      --
union select c1,w1 from cust_c_custkey
union select c1,w1 from cust_c_name
union select c1,w1 from cust_c_address
union select c1,w1 from cust_c_nationkey
union select c1,w1 from cust_c_phone
union select c1,w1 from cust_c_acctbal
union select c1,w1 from cust_c_mktsegment
union select c1,w1 from cust_c_comment
      --
union select c1,w1 from orders_o_orderkey
union select c1,w1 from orders_o_custkey
union select c1,w1 from orders_o_orderstatus
union select c1,w1 from orders_o_totalprice
union select c1,w1 from orders_o_orderdate
union select c1,w1 from orders_o_orderpriority
union select c1,w1 from orders_o_clerk
union select c1,w1 from orders_o_shippriority
union select c1,w1 from orders_o_comment
      --
union select c1,w1 from lineitem_l_orderkey
union select c1,w1 from lineitem_l_partkey
union select c1,w1 from lineitem_l_suppkey
union select c1,w1 from lineitem_l_linenumber
union select c1,w1 from lineitem_l_quantity
union select c1,w1 from lineitem_l_extendedprice
union select c1,w1 from lineitem_l_discount
union select c1,w1 from lineitem_l_tax
union select c1,w1 from lineitem_l_returnflag
union select c1,w1 from lineitem_l_linestatus
union select c1,w1 from lineitem_l_shipdate
union select c1,w1 from lineitem_l_commitdate
union select c1,w1 from lineitem_l_receiptdate
union select c1,w1 from lineitem_l_shipinstruct
union select c1,w1 from lineitem_l_shipmode
union select c1,w1 from lineitem_l_comment;