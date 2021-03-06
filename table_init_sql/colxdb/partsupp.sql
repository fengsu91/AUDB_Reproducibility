drop table if exists x_psupp_ps_partkey;
drop table if exists x_psupp_ps_suppkey;
drop table if exists x_psupp_ps_availqty;
drop table if exists x_psupp_ps_supplycost;
drop table if exists x_psupp_ps_comment;
create table x_psupp_ps_partkey as select tid, string_agg(''''||ps_partkey::character varying||'''', '|') as ps_partkey from psupp_ps_partkey group by tid;
create table x_psupp_ps_suppkey as select tid, string_agg(''''||ps_suppkey::character varying||'''', '|') as ps_suppkey from psupp_ps_suppkey group by tid;
create table x_psupp_ps_availqty as select tid, string_agg(''''||ps_availqty::character varying||'''', '|') as ps_availqty from psupp_ps_availqty group by tid;
create table x_psupp_ps_supplycost as select tid, string_agg(''''||ps_supplycost::character varying||'''', '|') as ps_supplycost from psupp_ps_supplycost group by tid;
create table x_psupp_ps_comment as select tid, string_agg(''''||ps_comment::character varying||'''', '|') as ps_comment from psupp_ps_comment group by tid; 