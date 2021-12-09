select 'insert into nation_n_nationkey values ('||tid::character varying||',['||n_nationkey||']);' from x_nation_n_nationkey;
select 'insert into nation_n_name values ('||tid::character varying||',['||n_name||']);' from x_nation_n_name;
select 'insert into nation_n_regionkey values ('||tid::character varying||',['||n_regionkey||']);' from x_nation_n_regionkey;
select 'insert into nation_n_comment values ('||tid::character varying||',['||n_comment||']);' from x_nation_n_comment;