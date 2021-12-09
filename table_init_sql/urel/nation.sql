create table nation_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4,
       U1.tid, n_nationkey, n_name, n_regionkey, n_comment
from nation_n_nationkey U1, nation_n_name U2, nation_n_regionkey U3, nation_n_comment U4
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and 
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1);