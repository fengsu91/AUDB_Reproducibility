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

def tablegen(colnum, rolnum, rangeval, uncert, minval, maxval):
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
    for i in range(colnum):
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
    
if __name__ == '__main__':
    colnum = 100
    rolnum = 100000

    minval = 1
    maxval = 10

    rangeval = 3 #uncertain attribute range
    uncert = 0.05 #uncertainty percentage
    tablegen(colnum, rolnum, rangeval, uncert, minval, maxval)
