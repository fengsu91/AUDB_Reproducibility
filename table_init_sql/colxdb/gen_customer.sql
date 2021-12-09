select 'insert into cust_c_custkey values ('||tid::character varying||',['||c_custkey||']);' from x_cust_c_custkey;
select 'insert into cust_c_name values ('||tid::character varying||',['||c_name||']);' from x_cust_c_name;
select 'insert into cust_c_address values ('||tid::character varying||',['||c_address||']);' from x_cust_c_address;
select 'insert into cust_c_nationkey values ('||tid::character varying||',['||c_nationkey||']);' from x_cust_c_nationkey;
select 'insert into cust_c_phone values ('||tid::character varying||',['||c_phone||']);' from x_cust_c_phone;
select 'insert into cust_c_acctbal values ('||tid::character varying||',['||c_acctbal||']);' from x_cust_c_acctbal;
select 'insert into cust_c_mktsegment values ('||tid::character varying||',['||c_mktsegment||']);' from x_cust_c_mktsegment;
select 'insert into cust_c_comment values ('||tid::character varying||',['||c_comment||']);' from x_cust_c_comment;