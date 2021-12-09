select l1.tid,r_regionkey,r_name,r_comment,
u_r_regionkey,u_r_name,u_r_comment from
(Select c1.tid,r_regionkey,CASE WHEN o1.w1>1 THEN 0 ELSE 1 END AS u_r_regionkey from
(select tid,r_regionkey from region_r_regionkey where w1=1) c1 join (select min(w1) as w1,tid from region_r_regionkey group by tid) o1 on c1.tid=o1.tid) l1
JOIN
(Select c2.tid,r_name,CASE WHEN o2.w1>1 THEN 0 ELSE 1 END AS u_r_name from
(select tid,r_name from region_r_name where w1=1) c2 join (select min(w1) as w1,tid from region_r_name group by tid) o2 on c2.tid=o2.tid) l2 on l1.tid=l2.tid
JOIN
(Select c3.tid,r_comment,CASE WHEN o3.w1>1 THEN 0 ELSE 1 END AS u_r_comment from
(select tid,r_comment from region_r_comment where w1=1) c3 join (select min(w1) as w1,tid from region_r_comment group by tid) o3 on c3.tid=o3.tid) l3 on l1.tid=l3.tid;