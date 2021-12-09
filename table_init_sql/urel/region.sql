create table region_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3,
       U1.tid, r_regionkey, r_name, r_comment
from region_r_regionkey U1, region_r_name U2, region_r_comment U3
where U1.tid=U2.tid and U1.tid=U3.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1);