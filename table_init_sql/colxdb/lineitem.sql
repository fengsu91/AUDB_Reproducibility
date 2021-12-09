drop table if exists x_lineitem_l_orderkey;
drop table if exists x_lineitem_l_partkey;
drop table if exists x_lineitem_l_suppkey;
drop table if exists x_lineitem_l_linenumber;
drop table if exists x_lineitem_l_quantity;
drop table if exists x_lineitem_l_extendedprice;
drop table if exists x_lineitem_l_discount;
drop table if exists x_lineitem_l_tax;
drop table if exists x_lineitem_l_returnflag;
drop table if exists x_lineitem_l_linestatus;
drop table if exists x_lineitem_l_shipdate;
drop table if exists x_lineitem_l_commitdate;
drop table if exists x_lineitem_l_receiptdate;
drop table if exists x_lineitem_l_shipinstruct;
drop table if exists x_lineitem_l_shipmode;
drop table if exists x_lineitem_l_comment;
create table x_lineitem_l_orderkey as select tid, string_agg(''''||l_orderkey::character varying||'''', '|') as l_orderkey from lineitem_l_orderkey group by tid;
create table x_lineitem_l_partkey as select tid, string_agg(''''||l_partkey::character varying||'''', '|') as l_partkey from lineitem_l_partkey group by tid;
create table x_lineitem_l_suppkey as select tid, string_agg(''''||l_suppkey::character varying||'''', '|') as l_suppkey from lineitem_l_suppkey group by tid;
create table x_lineitem_l_linenumber as select tid, string_agg(''''||l_linenumber::character varying||'''', '|') as l_linenumber from lineitem_l_linenumber group by tid;
create table x_lineitem_l_quantity as select tid, string_agg(''''||l_quantity::character varying||'''', '|') as l_quantity from lineitem_l_quantity group by tid;
create table x_lineitem_l_extendedprice as select tid, string_agg(''''||l_extendedprice::character varying||'''', '|') as l_extendedprice from lineitem_l_extendedprice group by tid;
create table x_lineitem_l_discount as select tid, string_agg(''''||l_discount::character varying||'''', '|') as l_discount from lineitem_l_discount group by tid;
create table x_lineitem_l_tax as select tid, string_agg(''''||l_tax::character varying||'''', '|') as l_tax from lineitem_l_tax group by tid;
create table x_lineitem_l_returnflag as select tid, string_agg(''''||l_returnflag::character varying||'''', '|') as l_returnflag from lineitem_l_returnflag group by tid;
create table x_lineitem_l_linestatus as select tid, string_agg(''''||l_linestatus::character varying||'''', '|') as l_linestatus from lineitem_l_linestatus group by tid;
create table x_lineitem_l_shipdate as select tid, string_agg(''''||l_shipdate::character varying||'''', '|') as l_shipdate from lineitem_l_shipdate group by tid;
create table x_lineitem_l_commitdate as select tid, string_agg(''''||l_commitdate::character varying||'''', '|') as l_commitdate from lineitem_l_commitdate group by tid;
create table x_lineitem_l_receiptdate as select tid, string_agg(''''||l_receiptdate::character varying||'''', '|') as l_receiptdate from lineitem_l_receiptdate group by tid;
create table x_lineitem_l_shipinstruct as select tid, string_agg(''''||l_shipinstruct::character varying||'''', '|') as l_shipinstruct from lineitem_l_shipinstruct group by tid;
create table x_lineitem_l_shipmode as select tid, string_agg(''''||l_shipmode::character varying||'''', '|') as l_shipmode from lineitem_l_shipmode group by tid;
create table x_lineitem_l_comment as select tid, string_agg(''''||l_comment::character varying||'''', '|') as l_comment from lineitem_l_comment group by tid;