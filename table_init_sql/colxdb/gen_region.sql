select 'insert into region_r_regionkey values ('||tid::character varying||',['||r_regionkey||']);' from x_region_r_regionkey;
select 'insert into region_r_name values ('||tid::character varying||',['||r_name||']);' from x_region_r_name;
select 'insert into region_r_comment values ('||tid::character varying||',['||r_comment||']);' from x_region_r_comment;