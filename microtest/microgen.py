#!/usr/bin/python
import os
import math
import subprocess
import time

import numpy as np
import random
import sys
import copy
import statistics

def iscert(uncert):
    return random.randrange(100) > uncert*100

def tablegen(colnum, rolnum, rangeval, uncert, minval, maxval, aoff=0):
    dpy =""
    dpyr =""
    tbl = [[1 for x in range(colnum*3+3)] for y in range(rolnum)]
#    print(tbl)
    for i in range(rolnum):
        for j in range(colnum):
            val = random.randint(minval, maxval)
            ubval = val
            lbval = val
            if not iscert(uncert):
                ubval += rangeval
            tbl[i][j] = val
            tbl[i][colnum+j*2] = ubval
            tbl[i][colnum+j*2+1] = lbval
    for i in range(aoff, colnum+aoff):
        dpy+=('a'+str(i)+',')
        dpyr+=('ub_a'+str(i)+',')
        dpyr+=('lb_a'+str(i)+',')
    dpyr+='cet_r,'
    dpyr+='bst_r,'
    dpyr+='pos_r'
    dpy = dpy+dpyr
    print(dpy)
    nptbl = np.array(tbl, dtype='<i4')
    print(nptbl.shape)
    np.savetxt("micro.csv", nptbl, fmt='%d', header = dpy, delimiter=",", comments='')
    print("Generated micro instance of %d tuples, %d columns, %f uncertainty and range from %s to %s with max uncertain range %d."%(rolnum,colnum,uncert,minval,maxval, rangeval))
    return dpy
    
def tablegentidb(colnum, rolnum, rangeval, uncert, minval, maxval, tsize, aoff=0):
    dpy =""
    ct = 0
    tbl = []
#    print(tbl)
    for i in range(rolnum):
        lastt = []
        if not iscert(uncert):
            for k in range(tsize):
                tup = []
                for j in range(colnum):
                    if j==0:
                        val = i
                        tup.append(val)
                    else:
                        if len(lastt)<1:
                            val = random.randint(minval, maxval)
                            tup.append(val)
                        else:
                            val = random.randint(lastt[j], lastt[j]+rangeval)
                            tup.append(val)
                tbl.append(tup)
#                print(tup)
                ct+=1
                if len(lastt)<1:
                    lastt = tup
        else:
            tup = []
            for j in range(colnum):
                if j==0:
                    val = i
                else:
                    val = random.randint(minval, maxval)
                tup.append(val)
            tbl.append(tup)
            ct+=1
    for i in range(aoff, colnum+aoff):
        if i==0:
            dpy+=('id')
        else:
            dpy+=(',a'+str(i))
    print(dpy)
    nptbl = np.array(tbl, dtype='<i4')
#    print(nptbl)
    print(nptbl.shape)
    np.savetxt("microtidb.csv", nptbl, fmt='%d', header = dpy, delimiter=",", comments='')
    print("Generated micro TIDB of %d(%d) tuples, %d columns, %f uncertainty and range from %s to %s with max uncertain range %d."%(rolnum, ct ,colnum,uncert,minval,maxval, rangeval))
    return dpy
    
if __name__ == '__main__':
    colnum = 100
    rolnum = 100000

    minval = 1
    maxval = 10

    rangeval = 3 #uncertain attribute range
    uncert = 0.05 #uncertainty percentage
    tablegen(colnum, rolnum, rangeval, uncert, minval, maxval)
