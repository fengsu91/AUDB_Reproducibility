select 'insert into lineitem_l_orderkey values ('||tid::character varying||',['||l_orderkey||']);' from x_lineitem_l_orderkey;
select 'insert into lineitem_l_partkey values ('||tid::character varying||',['||l_partkey||']);' from x_lineitem_l_partkey;
select 'insert into lineitem_l_suppkey values ('||tid::character varying||',['||l_suppkey||']);' from x_lineitem_l_suppkey;
select 'insert into lineitem_l_linenumber values ('||tid::character varying||',['||l_linenumber||']);' from x_lineitem_l_linenumber;
select 'insert into lineitem_l_quantity values ('||tid::character varying||',['||l_quantity||']);' from x_lineitem_l_quantity;
select 'insert into lineitem_l_extendedprice values ('||tid::character varying||',['||l_extendedprice||']);' from x_lineitem_l_extendedprice;
select 'insert into lineitem_l_discount values ('||tid::character varying||',['||l_discount||']);' from x_lineitem_l_discount;
select 'insert into lineitem_l_tax values ('||tid::character varying||',['||l_tax||']);' from x_lineitem_l_tax;
select 'insert into lineitem_l_returnflag values ('||tid::character varying||',['||l_returnflag||']);' from x_lineitem_l_returnflag;
select 'insert into lineitem_l_linestatus values ('||tid::character varying||',['||l_linestatus||']);' from x_lineitem_l_linestatus;
select 'insert into lineitem_l_shipdate values ('||tid::character varying||',['||l_shipdate||']);' from x_lineitem_l_shipdate;
select 'insert into lineitem_l_commitdate values ('||tid::character varying||',['||l_commitdate||']);' from x_lineitem_l_commitdate;
select 'insert into lineitem_l_receiptdate values ('||tid::character varying||',['||l_receiptdate||']);' from x_lineitem_l_receiptdate;
select 'insert into lineitem_l_shipinstruct values ('||tid::character varying||',['||l_shipinstruct||']);' from x_lineitem_l_shipinstruct;
select 'insert into lineitem_l_shipmode values ('||tid::character varying||',['||l_shipmode||']);' from x_lineitem_l_shipmode;
select 'insert into lineitem_l_comment values ('||tid::character varying||',['||l_comment||']);' from x_lineitem_l_comment;