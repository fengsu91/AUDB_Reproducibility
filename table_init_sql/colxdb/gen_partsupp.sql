select 'insert into psupp_ps_partkey values ('||tid::character varying||',['||ps_partkey||']);' from x_psupp_ps_partkey;
select 'insert into psupp_ps_availqty values ('||tid::character varying||',['||ps_availqty||']);' from x_psupp_ps_availqty;
select 'insert into psupp_ps_supplycost values ('||tid::character varying||',['||ps_supplycost||']);' from x_psupp_ps_supplycost;
select 'insert into psupp_ps_comment values ('||tid::character varying||',['||ps_comment||']);' from x_psupp_ps_comment;