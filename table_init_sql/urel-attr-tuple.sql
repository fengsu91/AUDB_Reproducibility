create table nation_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4,
       U1.tid, n_nationkey, n_name, n_regionkey, n_comment
from nation_n_nationkey U1, nation_n_name U2, nation_n_regionkey U3, nation_n_comment U4
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and 
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1);

create table region_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3,
       U1.tid, r_regionkey, r_name, r_comment
from region_r_regionkey U1, region_r_name U2, region_r_comment U3
where U1.tid=U2.tid and U1.tid=U3.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1);

create table part_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U6.c1 as c6, U6.w1 as w6, U7.c1 as c7, U7.w1 as w7, U8.c1 as c8, U8.w1 as w8, U9.c1 as c9, U9.w1 as w9,
       U1.tid, p_partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment
from part_p_partkey U1, part_p_name U2, part_p_mfgr U3, part_p_brand U4, part_p_type U5, part_p_size U6, part_p_container U7, part_p_retailprice U8, part_p_comment U9
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and
      U1.tid=U6.tid and U1.tid=U7.tid and U1.tid=U8.tid and U1.tid=U9.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U1.C1<>U6.C1 or U1.W1=U6.W1) and (U1.C1<>U7.C1 or U1.W1=U7.W1) and (U1.C1<>U8.C1 or U1.W1=U8.W1) and (U1.C1<>U9.C1 or U1.W1=U9.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U2.C1<>U6.C1 or U2.W1=U6.W1) and (U2.C1<>U7.C1 or U2.W1=U7.W1) and (U2.C1<>U8.C1 or U2.W1=U8.W1) and (U2.C1<>U9.C1 or U2.W1=U9.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U3.C1<>U6.C1 or U3.W1=U6.W1) and (U3.C1<>U7.C1 or U3.W1=U7.W1) and (U3.C1<>U8.C1 or U3.W1=U8.W1) and (U3.C1<>U9.C1 or U3.W1=U9.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1) and (U4.C1<>U6.C1 or U4.W1=U6.W1) and (U4.C1<>U7.C1 or U4.W1=U7.W1) and (U4.C1<>U8.C1 or U4.W1=U8.W1) and (U4.C1<>U9.C1 or U4.W1=U9.W1) and (U5.C1<>U6.C1 or U5.W1=U6.W1) and (U5.C1<>U7.C1 or U5.W1=U7.W1) and (U5.C1<>U8.C1 or U5.W1=U8.W1) and (U5.C1<>U9.C1 or U5.W1=U9.W1) and (U6.C1<>U7.C1 or U6.W1=U7.W1) and (U6.C1<>U8.C1 or U6.W1=U8.W1) and (U6.C1<>U9.C1 or U6.W1=U9.W1) and (U7.C1<>U8.C1 or U7.W1=U8.W1) and (U7.C1<>U9.C1 or U7.W1=U9.W1) and (U8.C1<>U9.C1 or U8.W1=U9.W1);


create table supplier_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U6.c1 as c6, U6.w1 as w6, U7.c1 as c7, U7.w1 as w7,
       U1.tid, s_suppkey, s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment
from supp_s_suppkey U1, supp_s_name U2, supp_s_address U3, supp_s_nationkey U4, supp_s_phone U5, supp_s_acctbal U6, supp_s_comment U7
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and U1.tid=U6.tid and U1.tid=U7.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U1.C1<>U6.C1 or U1.W1=U6.W1) and (U1.C1<>U7.C1 or U1.W1=U7.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U2.C1<>U6.C1 or U2.W1=U6.W1) and (U2.C1<>U7.C1 or U2.W1=U7.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U3.C1<>U6.C1 or U3.W1=U6.W1) and (U3.C1<>U7.C1 or U3.W1=U7.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1) and (U4.C1<>U6.C1 or U4.W1=U6.W1) and (U4.C1<>U7.C1 or U4.W1=U7.W1) and (U5.C1<>U6.C1 or U5.W1=U6.W1) and (U5.C1<>U7.C1 or U5.W1=U7.W1) and (U6.C1<>U7.C1 or U6.W1=U7.W1);


create table partsupp_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U1.tid, ps_partkey, ps_suppkey, ps_availqty, ps_supplycost, ps_comment
from psupp_ps_partkey U1, psupp_ps_suppkey U2, psupp_ps_availqty U3, psupp_ps_supplycost U4, psupp_ps_comment U5
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1);


create table customer_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U6.c1 as c6, U6.w1 as w6, U7.c1 as c7, U7.w1 as w7, U8.c1 as c8, U8.w1 as w8,
       U1.tid, c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment
from cust_c_custkey U1, cust_c_name U2, cust_c_address U3, cust_c_nationkey U4, cust_c_phone U5, cust_c_acctbal U6, cust_c_mktsegment U7, cust_c_comment U8
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and
      U1.tid=U6.tid and U1.tid=U7.tid and U1.tid=U8.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U1.C1<>U6.C1 or U1.W1=U6.W1) and (U1.C1<>U7.C1 or U1.W1=U7.W1) and (U1.C1<>U8.C1 or U1.W1=U8.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U2.C1<>U6.C1 or U2.W1=U6.W1) and (U2.C1<>U7.C1 or U2.W1=U7.W1) and (U2.C1<>U8.C1 or U2.W1=U8.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U3.C1<>U6.C1 or U3.W1=U6.W1) and (U3.C1<>U7.C1 or U3.W1=U7.W1) and (U3.C1<>U8.C1 or U3.W1=U8.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1) and (U4.C1<>U6.C1 or U4.W1=U6.W1) and (U4.C1<>U7.C1 or U4.W1=U7.W1) and (U4.C1<>U8.C1 or U4.W1=U8.W1) and (U5.C1<>U6.C1 or U5.W1=U6.W1) and (U5.C1<>U7.C1 or U5.W1=U7.W1) and (U5.C1<>U8.C1 or U5.W1=U8.W1) and (U6.C1<>U7.C1 or U6.W1=U7.W1) and (U6.C1<>U8.C1 or U6.W1=U8.W1) and (U7.C1<>U8.C1 or U7.W1=U8.W1);


create table orders_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U6.c1 as c6, U6.w1 as w6, U7.c1 as c7, U7.w1 as w7, U8.c1 as c8, U8.w1 as w8, U9.c1 as c9, U9.w1 as w9,
       U1.tid, o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment
from orders_o_orderkey U1, orders_o_custkey U2, orders_o_orderstatus U3, orders_o_totalprice U4, orders_o_orderdate U5,
     orders_o_orderpriority U6, orders_o_clerk U7, orders_o_shippriority U8, orders_o_comment U9
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and
      U1.tid=U6.tid and U1.tid=U7.tid and U1.tid=U8.tid and U1.tid=U9.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U1.C1<>U6.C1 or U1.W1=U6.W1) and (U1.C1<>U7.C1 or U1.W1=U7.W1) and (U1.C1<>U8.C1 or U1.W1=U8.W1) and (U1.C1<>U9.C1 or U1.W1=U9.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U2.C1<>U6.C1 or U2.W1=U6.W1) and (U2.C1<>U7.C1 or U2.W1=U7.W1) and (U2.C1<>U8.C1 or U2.W1=U8.W1) and (U2.C1<>U9.C1 or U2.W1=U9.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U3.C1<>U6.C1 or U3.W1=U6.W1) and (U3.C1<>U7.C1 or U3.W1=U7.W1) and (U3.C1<>U8.C1 or U3.W1=U8.W1) and (U3.C1<>U9.C1 or U3.W1=U9.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1) and (U4.C1<>U6.C1 or U4.W1=U6.W1) and (U4.C1<>U7.C1 or U4.W1=U7.W1) and (U4.C1<>U8.C1 or U4.W1=U8.W1) and (U4.C1<>U9.C1 or U4.W1=U9.W1) and (U5.C1<>U6.C1 or U5.W1=U6.W1) and (U5.C1<>U7.C1 or U5.W1=U7.W1) and (U5.C1<>U8.C1 or U5.W1=U8.W1) and (U5.C1<>U9.C1 or U5.W1=U9.W1) and (U6.C1<>U7.C1 or U6.W1=U7.W1) and (U6.C1<>U8.C1 or U6.W1=U8.W1) and (U6.C1<>U9.C1 or U6.W1=U9.W1) and (U7.C1<>U8.C1 or U7.W1=U8.W1) and (U7.C1<>U9.C1 or U7.W1=U9.W1) and (U8.C1<>U9.C1 or U8.W1=U9.W1);

create table lineitem_urel with oids as 
select U1.C1, U1.W1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U6.c1 as c6, U6.w1 as w6, U7.c1 as c7, U7.w1 as w7, U8.c1 as c8, U8.w1 as w8, U9.c1 as c9, U9.w1 as w9,
       U10.c1 as c10, U10.w1 as w10, U11.c1 as c11, U11.w1 as w11, U12.c1 as c12, U12.w1 as w12, U13.c1 as c13, U13.w1 as w13,
       U14.c1 as c14, U14.w1 as w14, U15.c1 as c15, U15.w1 as W15, U16.C1 as c16, U16.w1 as w16,
       U1.tid, l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus,
       l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment
from lineitem_l_orderkey U1, lineitem_l_partkey U2, lineitem_l_suppkey U3, lineitem_l_linenumber U4, lineitem_l_quantity U5, lineitem_l_extendedprice U6,
     lineitem_l_discount U7, lineitem_l_tax U8, lineitem_l_returnflag U9, lineitem_l_linestatus U10, lineitem_l_shipdate U11, lineitem_l_commitdate U12, lineitem_l_receiptdate U13,
     lineitem_l_shipinstruct U14, lineitem_l_shipmode U15, lineitem_l_comment U16
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and U1.tid=U6.tid and U1.tid=U7.tid and
      U1.tid=U8.tid and U1.tid=U9.tid and U1.tid=U10.tid and U1.tid=U11.tid and U1.tid=U12.tid and U1.tid=U13.tid and
      U1.tid=U14.tid and U1.tid=U15.tid and U1.tid=U16.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U1.C1<>U6.C1 or U1.W1=U6.W1) and (U1.C1<>U7.C1 or U1.W1=U7.W1) and (U1.C1<>U8.C1 or U1.W1=U8.W1) and (U1.C1<>U9.C1 or U1.W1=U9.W1) and (U1.C1<>U10.C1 or U1.W1=U10.W1) and (U1.C1<>U11.C1 or U1.W1=U11.W1) and (U1.C1<>U12.C1 or U1.W1=U12.W1) and (U1.C1<>U13.C1 or U1.W1=U13.W1) and (U1.C1<>U14.C1 or U1.W1=U14.W1) and (U1.C1<>U15.C1 or U1.W1=U15.W1) and (U1.C1<>U16.C1 or U1.W1=U16.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U2.C1<>U6.C1 or U2.W1=U6.W1) and (U2.C1<>U7.C1 or U2.W1=U7.W1) and (U2.C1<>U8.C1 or U2.W1=U8.W1) and (U2.C1<>U9.C1 or U2.W1=U9.W1) and (U2.C1<>U10.C1 or U2.W1=U10.W1) and (U2.C1<>U11.C1 or U2.W1=U11.W1) and (U2.C1<>U12.C1 or U2.W1=U12.W1) and (U2.C1<>U13.C1 or U2.W1=U13.W1) and (U2.C1<>U14.C1 or U2.W1=U14.W1) and (U2.C1<>U15.C1 or U2.W1=U15.W1) and (U2.C1<>U16.C1 or U2.W1=U16.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U3.C1<>U6.C1 or U3.W1=U6.W1) and (U3.C1<>U7.C1 or U3.W1=U7.W1) and (U3.C1<>U8.C1 or U3.W1=U8.W1) and (U3.C1<>U9.C1 or U3.W1=U9.W1) and (U3.C1<>U10.C1 or U3.W1=U10.W1) and (U3.C1<>U11.C1 or U3.W1=U11.W1) and (U3.C1<>U12.C1 or U3.W1=U12.W1) and (U3.C1<>U13.C1 or U3.W1=U13.W1) and (U3.C1<>U14.C1 or U3.W1=U14.W1) and (U3.C1<>U15.C1 or U3.W1=U15.W1) and (U3.C1<>U16.C1 or U3.W1=U16.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1) and (U4.C1<>U6.C1 or U4.W1=U6.W1) and (U4.C1<>U7.C1 or U4.W1=U7.W1) and (U4.C1<>U8.C1 or U4.W1=U8.W1) and (U4.C1<>U9.C1 or U4.W1=U9.W1) and (U4.C1<>U10.C1 or U4.W1=U10.W1) and (U4.C1<>U11.C1 or U4.W1=U11.W1) and (U4.C1<>U12.C1 or U4.W1=U12.W1) and (U4.C1<>U13.C1 or U4.W1=U13.W1) and (U4.C1<>U14.C1 or U4.W1=U14.W1) and (U4.C1<>U15.C1 or U4.W1=U15.W1) and (U4.C1<>U16.C1 or U4.W1=U16.W1) and (U5.C1<>U6.C1 or U5.W1=U6.W1) and (U5.C1<>U7.C1 or U5.W1=U7.W1) and (U5.C1<>U8.C1 or U5.W1=U8.W1) and (U5.C1<>U9.C1 or U5.W1=U9.W1) and (U5.C1<>U10.C1 or U5.W1=U10.W1) and (U5.C1<>U11.C1 or U5.W1=U11.W1) and (U5.C1<>U12.C1 or U5.W1=U12.W1) and (U5.C1<>U13.C1 or U5.W1=U13.W1) and (U5.C1<>U14.C1 or U5.W1=U14.W1) and (U5.C1<>U15.C1 or U5.W1=U15.W1) and (U5.C1<>U16.C1 or U5.W1=U16.W1) and (U6.C1<>U7.C1 or U6.W1=U7.W1) and (U6.C1<>U8.C1 or U6.W1=U8.W1) and (U6.C1<>U9.C1 or U6.W1=U9.W1) and (U6.C1<>U10.C1 or U6.W1=U10.W1) and (U6.C1<>U11.C1 or U6.W1=U11.W1) and (U6.C1<>U12.C1 or U6.W1=U12.W1) and (U6.C1<>U13.C1 or U6.W1=U13.W1) and (U6.C1<>U14.C1 or U6.W1=U14.W1) and (U6.C1<>U15.C1 or U6.W1=U15.W1) and (U6.C1<>U16.C1 or U6.W1=U16.W1) and (U7.C1<>U8.C1 or U7.W1=U8.W1) and (U7.C1<>U9.C1 or U7.W1=U9.W1) and (U7.C1<>U10.C1 or U7.W1=U10.W1) and (U7.C1<>U11.C1 or U7.W1=U11.W1) and (U7.C1<>U12.C1 or U7.W1=U12.W1) and (U7.C1<>U13.C1 or U7.W1=U13.W1) and (U7.C1<>U14.C1 or U7.W1=U14.W1) and (U7.C1<>U15.C1 or U7.W1=U15.W1) and (U7.C1<>U16.C1 or U7.W1=U16.W1) and (U8.C1<>U9.C1 or U8.W1=U9.W1) and (U8.C1<>U10.C1 or U8.W1=U10.W1) and (U8.C1<>U11.C1 or U8.W1=U11.W1) and (U8.C1<>U12.C1 or U8.W1=U12.W1) and (U8.C1<>U13.C1 or U8.W1=U13.W1) and (U8.C1<>U14.C1 or U8.W1=U14.W1) and (U8.C1<>U15.C1 or U8.W1=U15.W1) and (U8.C1<>U16.C1 or U8.W1=U16.W1) and (U9.C1<>U10.C1 or U9.W1=U10.W1) and (U9.C1<>U11.C1 or U9.W1=U11.W1) and (U9.C1<>U12.C1 or U9.W1=U12.W1) and (U9.C1<>U13.C1 or U9.W1=U13.W1) and (U9.C1<>U14.C1 or U9.W1=U14.W1) and (U9.C1<>U15.C1 or U9.W1=U15.W1) and (U9.C1<>U16.C1 or U9.W1=U16.W1) and (U10.C1<>U11.C1 or U10.W1=U11.W1) and (U10.C1<>U12.C1 or U10.W1=U12.W1) and (U10.C1<>U13.C1 or U10.W1=U13.W1) and (U10.C1<>U14.C1 or U10.W1=U14.W1) and (U10.C1<>U15.C1 or U10.W1=U15.W1) and (U10.C1<>U16.C1 or U10.W1=U16.W1) and (U11.C1<>U12.C1 or U11.W1=U12.W1) and (U11.C1<>U13.C1 or U11.W1=U13.W1) and (U11.C1<>U14.C1 or U11.W1=U14.W1) and (U11.C1<>U15.C1 or U11.W1=U15.W1) and (U11.C1<>U16.C1 or U11.W1=U16.W1) and (U12.C1<>U13.C1 or U12.W1=U13.W1) and (U12.C1<>U14.C1 or U12.W1=U14.W1) and (U12.C1<>U15.C1 or U12.W1=U15.W1) and (U12.C1<>U16.C1 or U12.W1=U16.W1) and (U13.C1<>U14.C1 or U13.W1=U14.W1) and (U13.C1<>U15.C1 or U13.W1=U15.W1) and (U13.C1<>U16.C1 or U13.W1=U16.W1) and (U14.C1<>U15.C1 or U14.W1=U15.W1) and (U14.C1<>U16.C1 or U14.W1=U16.W1) and (U15.C1<>U16.C1 or U15.W1=U16.W1);
      
      
      -- create table W
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