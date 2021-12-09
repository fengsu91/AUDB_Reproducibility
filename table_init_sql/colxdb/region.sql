drop table if exists x_region_r_regionkey;
drop table if exists x_region_r_name;
drop table if exists x_region_r_comment;
create table x_region_r_regionkey as select tid, string_agg(''''||r_regionkey::character varying||'''', '|') as r_regionkey from region_r_regionkey group by tid;
create table x_region_r_name as select tid, string_agg(''''||r_name::character varying||'''', '|') as r_name from region_r_name group by tid;
create table x_region_r_comment as select tid, string_agg(''''||r_comment::character varying||'''', '|') as r_comment from region_r_comment group by tid;