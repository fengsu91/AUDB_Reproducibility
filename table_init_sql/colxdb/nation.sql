drop table if exists x_nation_n_nationkey;
drop table if exists x_nation_n_name;
drop table if exists x_nation_n_regionkey;
drop table if exists x_nation_n_comment;
create table x_nation_n_nationkey as select tid, string_agg(''''||n_nationkey::character varying||'''', '|') as n_nationkey from nation_n_nationkey group by tid;
create table x_nation_n_name as select tid, string_agg(''''||n_name::character varying||'''', '|') as n_name from nation_n_name group by tid;
create table x_nation_n_regionkey as select tid, string_agg(''''||n_regionkey::character varying||'''', '|') as n_regionkey from nation_n_regionkey group by tid;
create table x_nation_n_comment as select tid, string_agg(''''||n_comment::character varying||'''', '|') as n_comment from nation_n_comment group by tid;