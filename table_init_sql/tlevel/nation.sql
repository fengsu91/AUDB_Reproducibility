select l1.tid,n_nationkey,n_name,n_regionkey,n_comment,u_n_nationkey,u_n_name,u_n_regionkey,u_n_comment from
(Select c1.tid,n_nationkey,CASE WHEN o1.w1>1 THEN 0 ELSE 1 END AS u_n_nationkey from
(select tid,n_nationkey from nation_n_nationkey where w1=1) c1 join (select min(w1) as w1,tid from nation_n_nationkey group by tid) o1 on c1.tid=o1.tid) l1
JOIN
(Select c2.tid,n_name,CASE WHEN o2.w1>1 THEN 0 ELSE 1 END AS u_n_name from
(select tid,case when tid=7 then 'GERMANY' else n_name end from nation_n_name where w1=1) c2 join (select min(w1) as w1,tid from nation_n_name group by tid) o2 on c2.tid=o2.tid) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,n_regionkey,CASE WHEN o3.w1>1 THEN 0 ELSE 1 END AS u_n_regionkey from
(select tid,n_regionkey from nation_n_regionkey where w1=1) c3 join (select min(w1) as w1,tid from nation_n_regionkey group by tid) o3 on c3.tid=o3.tid) l3 on l1.tid=l3.tid
JOIN
(Select c4.tid,n_comment,CASE WHEN o4.w1>1 THEN 0 ELSE 1 END AS u_n_comment from
(select tid,n_comment from nation_n_comment where w1=1) c4 join (select min(w1) as w1,tid from nation_n_comment group by tid) o4 on c4.tid=o4.tid) l4 on l1.tid=l4.tid;