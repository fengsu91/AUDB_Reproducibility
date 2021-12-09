select 'insert into supp_s_suppkey values ('||tid::character varying||',['||s_suppkey||']);' from x_supp_s_suppkey;
select 'insert into supp_s_name values ('||tid::character varying||',['||s_name||']);' from x_supp_s_name;
select 'insert into supp_s_address values ('||tid::character varying||',['||s_address||']);' from x_supp_s_address;
select 'insert into supp_s_nationkey values ('||tid::character varying||',['||s_nationkey||']);' from x_supp_s_nationkey;
select 'insert into supp_s_phone values ('||tid::character varying||',['||s_phone||']);' from x_supp_s_phone;
select 'insert into supp_s_acctbal values ('||tid::character varying||',['||s_acctbal||']);' from x_supp_s_acctbal;
select 'insert into supp_s_comment values ('||tid::character varying||',['||s_comment||']);' from x_supp_s_comment;