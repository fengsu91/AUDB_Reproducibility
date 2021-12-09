create table partsupp_urel with oids as 
select U1.c1, U1.w1, U2.c1 as c2, U2.w1 as w2, U3.c1 as c3, U3.w1 as w3, U4.c1 as c4, U4.w1 as w4, U5.c1 as c5, U5.w1 as w5,
       U1.tid, ps_partkey, ps_suppkey, ps_availqty, ps_supplycost, ps_comment
from psupp_ps_partkey U1, psupp_ps_suppkey U2, psupp_ps_availqty U3, psupp_ps_supplycost U4, psupp_ps_comment U5
where U1.tid=U2.tid and U1.tid=U3.tid and U1.tid=U4.tid and U1.tid=U5.tid and
      (U1.C1<>U2.C1 or U1.W1=U2.W1) and (U1.C1<>U3.C1 or U1.W1=U3.W1) and (U1.C1<>U4.C1 or U1.W1=U4.W1) and (U1.C1<>U5.C1 or U1.W1=U5.W1) and (U2.C1<>U3.C1 or U2.W1=U3.W1) and (U2.C1<>U4.C1 or U2.W1=U4.W1) and (U2.C1<>U5.C1 or U2.W1=U5.W1) and (U3.C1<>U4.C1 or U3.W1=U4.W1) and (U3.C1<>U5.C1 or U3.W1=U5.W1) and (U4.C1<>U5.C1 or U4.W1=U5.W1);