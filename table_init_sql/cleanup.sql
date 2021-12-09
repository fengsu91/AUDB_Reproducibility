drop table if exists cust_c_custkey;
drop table if exists cust_c_name;
drop table if exists cust_c_address;
drop table if exists cust_c_nationkey;
drop table if exists cust_c_phone;
drop table if exists cust_c_acctbal;
drop table if exists cust_c_mktsegment;
drop table if exists cust_c_comment;
drop table if exists lineitem_l_orderkey;
drop table if exists lineitem_l_partkey;
drop table if exists lineitem_l_suppkey;
drop table if exists lineitem_l_linenumber;
drop table if exists lineitem_l_quantity;
drop table if exists lineitem_l_extendedprice;
drop table if exists lineitem_l_discount;
drop table if exists lineitem_l_tax;
drop table if exists lineitem_l_returnflag;
drop table if exists lineitem_l_linestatus;
drop table if exists lineitem_l_shipdate;
drop table if exists lineitem_l_commitdate;
drop table if exists lineitem_l_receiptdate;
drop table if exists lineitem_l_shipinstruct;
drop table if exists lineitem_l_shipmode;
drop table if exists lineitem_l_comment;
drop table if exists nation_n_nationkey;
drop table if exists nation_n_name;
drop table if exists nation_n_regionkey;
drop table if exists nation_n_comment;
drop table if exists orders_o_orderkey;
drop table if exists orders_o_custkey;
drop table if exists orders_o_orderstatus;
drop table if exists orders_o_totalprice;
drop table if exists orders_o_orderdate;
drop table if exists orders_o_orderpriority;
drop table if exists orders_o_clerk;
drop table if exists orders_o_shippriority;
drop table if exists orders_o_comment;
drop table if exists part_p_partkey;
drop table if exists part_p_name;
drop table if exists part_p_mfgr;
drop table if exists part_p_brand;
drop table if exists part_p_type;
drop table if exists part_p_size;
drop table if exists part_p_container;
drop table if exists part_p_retailprice;
drop table if exists part_p_comment;
drop table if exists psupp_ps_partkey;
drop table if exists psupp_ps_suppkey;
drop table if exists psupp_ps_availqty;
drop table if exists psupp_ps_supplycost;
drop table if exists psupp_ps_comment;
drop table if exists region_r_regionkey;
drop table if exists region_r_name;
drop table if exists region_r_comment;
drop table if exists supp_s_suppkey;
drop table if exists supp_s_name;
drop table if exists supp_s_address;
drop table if exists supp_s_nationkey;
drop table if exists supp_s_phone;
drop table if exists supp_s_acctbal;
drop table if exists supp_s_comment;
drop table if exists customer;
drop table if exists lineitem;
drop table if exists nation;
drop table if exists orders;
drop table if exists part;
drop table if exists partsupp;
drop table if exists region;
drop table if exists supplier;
drop table if exists customer_uadb;
drop table if exists lineitem_uadb;
drop table if exists nation_uadb;
drop table if exists orders_uadb;
drop table if exists part_uadb;
drop table if exists partsupp_uadb;
drop table if exists region_uadb;
drop table if exists supplier_uadb;
drop table if exists customer_bg;
drop table if exists lineitem_bg;
drop table if exists nation_bg;
drop table if exists orders_bg;
drop table if exists part_bg;
drop table if exists partsupp_bg;
drop table if exists region_bg;
drop table if exists supplier_bg;
drop table if exists customer_radb;
drop table if exists lineitem_radb;
drop table if exists nation_radb;
drop table if exists orders_radb;
drop table if exists part_radb;
drop table if exists partsupp_radb;
drop table if exists region_radb;
drop table if exists supplier_radb;
drop table if exists dummy;