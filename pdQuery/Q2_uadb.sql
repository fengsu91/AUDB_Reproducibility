SELECT l_extendedprice, u_r FROM lineitem_uadb l WHERE l_shipdate > '1994-01-01' AND l_shipdate < '1996-01-01' AND l_discount > 0.05 AND l_discount < 0.08 AND l_quantity < 24;