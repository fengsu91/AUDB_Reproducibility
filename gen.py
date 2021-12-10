import os
import math
import subprocess
#pip3 install pg8000
#import pg8000
import psycopg2
from config import config
import microtest.microgen as mcg
import time
import argparse

import sqlite3
import numpy as np
from sqlite3 import OperationalError
import random
import sys
import copy
import statistics

dir = None
#conn = None
#cur = None
conn2 = None
cur2 = None
pgport = "5432"

########################################
#########     some configs    ##########
########################################

multrep = 1 # for each timed query, how many repititions to do for the mean.

#s = [0.1, 1, 10]
s = [0.1, 1] # faster version with smaller datasize
x = [0.02, 0.05, 0.1, 0.3]
mcdbRep = 10
#test1 = ['s10_x2']
test1 = ['s10_x2','s10_x5','s10_x10','s10_x30'] # faster version with smaller datasize
#test1 = ['s100_x2','s100_x5','s100_x10','s100_x30'] # original version
test2 = ['s10_x2','s100_x2'] # faster version with smaller datasize
#test2 = ['s10_x2','s100_x2','s1000_x2'] # original version

########################################
#########     End configs     ##########
########################################

#psqlbin = '/Applications/Postgres.app/Contents/Versions/10/bin/psql -p5432 "uadb" -c '
gpromcom = [str("gprom"), "-host", "none", "-db", "/Users/sufeng/git/UADB_Reproducibility/dbs/incomp.db", "-port", "none", "-user", "none", "-passwd", "none", "-loglevel", "0", "-backend", "sqlite", "-Pexecutor", "sql", "-query"]
gprompg = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%(pgport), "-user", "postgres", "-loglevel", "0", "-backend", "postgres", "-Pexecutor", "sql", "-query"]
#gpromsd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%(pgport), "-user", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-Pexecutor", "sql", "-query"]
gpromsd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%(pgport), "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-Pexecutor", "sql", "-query"]

# pdbench
table_init_dir = 'table_init_sql'
pdbenchTables = ['customer','lineitem','nation','orders','part','partsupp','region','supplier']
queries = ['pdQuery/Q1.sql','pdQuery/Q2.sql','pdQuery/Q3.sql']
queries_mb = ['pdQuery/Q1_maybms.sql','pdQuery/Q2_maybms.sql','pdQuery/Q3_maybms.sql']
queries_uadb = ['pdQuery/Q1_uadb.sql','pdQuery/Q2_uadb.sql','pdQuery/Q3_uadb.sql']
queries_radb = ['pdQuery/Q1_radb.sql','pdQuery/Q2_radb.sql','pdQuery/Q3_radb.sql']
queries_cert = ['pdQuery/Q1_cert.sql','pdQuery/Q2_cert.sql','pdQuery/Q3_cert.sql']

def stdoutquery(query, fname, setion='postgresql'):
    conn = None
    cur = None
    query = "COPY ({0}) TO STDOUT".format(query)
    print(query)
    try:
        params = config.config(section=setion)
        conn = psycopg2.connect(**params)
        conn.set_session(autocommit=True)
    except Exception as error:
        print(error)
    cur = conn.cursor()
    #    print(query)
    try:
        with open(fname, 'a') as f:
            cur.copy_expert(query, f)
            conn.commit()
    except Exception as e:
        print("query error: %s"%e)
    #        pass
    cur.close()
    conn.close()
    
def pushQuery(query, setion='postgresql'):
    conn = None
    cur = None
    try:
        params = config.config(section=setion)
        conn = psycopg2.connect(**params)
        conn.set_session(autocommit=True)
    except Exception as error:
        print(error)
    cur = conn.cursor()
#    print(query)
    try:
        cur.execute(query)
        conn.commit()
    except Exception as e:
        print("query error: %s"%e)
#        pass
    cur.close()
    conn.close()
        
def runQuery(query, setion='postgresql'):
    conn = None
    cur = None
    try:
        # read connection parameters
        params = config.config(section=setion)
        # connect to the PostgreSQL server
        conn = psycopg2.connect(**params)
    except Exception as error:
        print(error)
    cur = conn.cursor()
#    print(query)
    try:
        cur.execute(query)
        conn.commit()
    except Exception as e:
        print("query error: %s"%e)
        pass
#   get outpu if any
    try:
        ret = cur.fetchall()
#        disconnect()
        return ret
    except Exception as e:
        print(e)
        pass
    except TypeError as e:
        print(e)
        pass
    cur.close()
    conn.close()
    
def timeQuery(query, setion='postgresql'):
    query = 'EXPLAIN ANALYSE create table dummy as %s'%query
    ret = runQuery(query, setion)
    runQuery("drop table IF EXISTS dummy;")
    rt = ret[-1][0].split()[-2]
    print("time Query: %s."%rt)
    return rt
    
def materializequery(query, tablename='dummy', setion='postgresql'):
    runQuery("drop table IF EXISTS %s;"%(tablename))
    query = 'create table %s as %s'%(tablename,query)
    tm = timeQuerySel(query, setion)
    print(tm)
    return

def timeQuerySel(query, setion='postgresql'):
    ret = runQuery('EXPLAIN ANALYSE %s'%query, setion)
    rt = ret[-1][0].split()[-2]
    print("time Query: %s."%rt)
    return rt
    
def timeQueryMult(query, setion='postgresql', rep = multrep):
    ret = []
    for i in range(rep):
        ret.append(timeQuerySel(query))
    ret = ret[int(rep/2):]
    retint = [float(i) for i in ret]
    return (', '.join(ret)), str(statistics.mean(retint))
    
def timeQueryMultInput(query, setion='postgresql', targ="radb", rep = multrep, x = 0.02, s = 1):
    ret = []
    rsec = "_"+str(int(x*100))+"_"+str(int(s*100)) +"_"+ targ
    print(rsec)
    query = query.replace("_"+targ, rsec)
    for i in range(rep):
        ret.append(timeQuerySel(query))
    ret = ret[int(rep/2):]
    retint = [float(i) for i in ret]
    return (', '.join(ret)), str(statistics.mean(retint))
    
def timeQueryFile(filename='pdQuery/Q3.sql', setion='postgresql', targ="radb", rep = 20, x = 0.02, s = 1):
    with open(filename) as fp:
        query = fp.read()
        return timeQueryMultInput(query, setion, targ, rep, x, s)
        
def timeQueryFileDef(filename='pdQuery/Q3.sql', setion='postgresql', rep = 20):
    with open(filename) as fp:
        query = fp.read()
        return timeQueryMult(query, setion, rep)
        
    
def sizeQuery(query, setion='postgresql'):
    ret = runQuery('select count(*) from (%s) xyz;'%query.split(";")[0], setion)
    return ret[-1][0]
    
def pdbenchGenOnX(sval = 1, imp=False):
    global dir
    global x
    genDir = list()
    subprocess.call(["make","clean"],cwd='%s/dbgen'%dir)
    subprocess.call(["make"],cwd='%s/dbgen'%dir)
#    call dbgen
    for xval in x:
        os.chdir('%s/dbgen'%dir)
        os.system('./dbgen -x %f -s %f'%(xval,sval))
        print('./dbgen -x %f -s %f'%(xval,sval))
        os.chdir(dir)
        dirname = 's%d_x%d'%(sval*100,xval*100)
        os.system('mkdir %s'%dirname)
        os.system('mv dbgen/*.tbl %s'%dirname)
        if imp:
            importPdbenchTables(dirname,xval,sval)
        genDir.append(dirname)
    global test1
    test1 = genDir
    
def pdbenchGenOnS(xval = 0.02, imp = False):
    global dir
    global s
    genDir = list()
    subprocess.call(["make","clean"],cwd='%s/dbgen'%dir)
    subprocess.call(["make"],cwd='%s/dbgen'%dir)
#    call dbgen
    for sval in s:
        os.chdir('%s/dbgen'%dir)
        os.system('./dbgen -x %f -s %f'%(xval,sval))
        print('./dbgen -x %f -s %f'%(xval,sval))
        os.chdir(dir)
        dirname = 's%d_x%d'%(sval*100,xval*100)
        os.system('mkdir %s'%dirname)
        os.system('mv dbgen/*.tbl %s'%dirname)
        if imp:
            importPdbenchTables(dirname,xval,sval)
        genDir.append(dirname)
    global test2
    test2 = genDir
            
def importPdbenchTables(datadir,x,s,affix=True):
#    global conn
#    pushQuery('set autocommit on;');
    with open("%s/cleanup.sql"%table_init_dir) as fp:
        line = fp.readline().split("\n")[0]
        while line:
            print(line);
            pushQuery(line)
            line = fp.readline()
        fp.close()
    for tbs in pdbenchTables:
        tname = tbs
        if(affix):
            tname = "%s_%d_%d"%(tbs,x*100,s*100)
        with open("%s/%s.sql"%(table_init_dir, tbs)) as fp:
            line = fp.readline().split("\n")[0]
            while line:
                tablename = line.split()[2]
                pushQuery(line);
                pushQuery("COPY %s FROM '%s/%s/%s.tbl' DELIMITER '|';"%(tablename,dir,datadir,tablename))
                print('importing [%s][%s]'%(datadir,tablename))
                line = fp.readline()
            fp.close()
        with open("%s/tlevel/%s.sql"%(table_init_dir, tbs)) as fp:
            query = fp.read()
            pushQuery('drop table if exists %s;'%(tbs))
            print('creating tlevel [%s][%s]'%(datadir,tbs))
            pushQuery('create table %s as %s'%(tbs,query))
            if tname != tbs:
                pushQuery('drop table if exists %s;'%(tname))
                print('creating tlevel [%s][%s]'%(datadir,tname))
                pushQuery('create table %s as select * from %s;'%(tname,tbs))
            fp.close()
        with open("%s/radb/%s.sql"%(table_init_dir, tbs)) as fp:
            query = fp.read()
            pushQuery('drop table if exists %s_radb;'%(tname))
            print('creating radb [%s][%s_radb]'%(datadir,tname))
            pushQuery('create table %s_radb as %s'%(tname,query))
            fp.close()
        with open("%s/uadb/%s.sql"%(table_init_dir, tbs)) as fp:
            query = fp.read()
            pushQuery('drop table if exists %s_uadb;'%(tname))
            print('creating uadb [%s][%s_uadb]'%(datadir,tname))
            pushQuery('create table %s_uadb as %s'%(tname,query))
            fp.close()
        with open("%s/bg/%s.sql"%(table_init_dir, tbs)) as fp:
            query = fp.read()
            pushQuery('drop table if exists %s_bg'%(tname))
            print('creating bg [%s][%s_bg]'%(datadir,tname))
            pushQuery('create table %s_bg as %s'%(tname,query))
            fp.close()
#    pushQuery('vacuum FULL analyse;')

def importtrio(datadir,x,s, aff = False):
    with open("%s/cleanup.sql"%table_init_dir) as fp:
        line = fp.readline().split("\n")[0]
        while line:
            print(line);
            pushQuery(line)
            line = fp.readline()
        fp.close()
    for tbs in pdbenchTables:
        tname = tbs
        if(aff):
            tname = "%s_%d_%d"%(tbs,x*100,s*100)
        with open("%s/%s.sql"%(table_init_dir, tbs)) as fp:
            line = fp.readline().split("\n")[0]
            while line:
                tablename = line.split()[2]
                pushQuery(line);
                pushQuery("COPY %s FROM '%s/%s/%s.tbl' DELIMITER '|';"%(tablename,dir,datadir,tablename))
                print('importing [%s][%s]'%(datadir,tablename))
                line = fp.readline()
            fp.close()
        with open("%s/colxdb/%s.sql"%(table_init_dir, tbs)) as fp:
            line = fp.readline().split("\n")[0]
            while line:
                rq = line
                if tname != tbs:
                    rq = rq.replace(tbs+"_", tname+"_")
                print(rq);
                pushQuery(rq)
                line = fp.readline()
            fp.close()
        with open("%s/colxdb/gen_%s.sql"%(table_init_dir, tbs)) as fp:
            line = fp.readline().split("\n")[0][:-1]
            while line:
                fname = "trio/"+tbs+".triql"
                print(fname)
                stdoutquery(line, fname)
                line = fp.readline().split("\n")[0][:-1]
            fp.close()
                
def testQueryPDbench():
    global mcdbRep

    qt = list()
    uadbt = list()
    radbt = list()
    certt = list()
    mbt = list()
    mcdbt = list()
    alltime = ""
    
    for qs in queries:
        with open(qs) as fp:
            q = fp.read()
            print("--------Testing "+qs+"----------\n")
#            print(q)
            allt, mt = timeQueryMult(q)
            qt.append(mt)
            alltime += (qs + ": " + allt + "\n")
            totaltime = 0
#            for i in range(0,mcdbRep):
#                totaltime += float(timeQuery(q))
#            mcdbt.append(str(totaltime))
            allt, mt = timeQueryMult(q, 'postgresql', mcdbRep)
            mcdbt.append(str(float(mt)*(mcdbRep)))
#            qts.append(sizeQuery(q))
    for qs2 in queries_uadb:
        with open(qs2) as fp:
            q = fp.read()
#            q = getAUDBQueryFromGProM(q)
            print("--------Testing "+qs2+"----------\n")
#            print(q)
            allt, mt = timeQueryMult(q)
            uadbt.append(mt)
            alltime += (qs2 + ": " + allt + "\n")
#            uadbts.append(sizeQuery(q))
    for qs4 in queries_radb:
        with open(qs4) as fp:
            q = fp.read()
#            q = getAUDBQueryFromGProM(q)
            print("--------Testing "+qs4+"----------\n")
#            print(q)
            allt, mt = timeQueryMult(q)
            radbt.append(mt)
            alltime += (qs4 + ": " + allt + "\n")
#            radbt.append(timeQuery(q))
#            uadbts.append(sizeQuery(q))
    for qs2 in queries_cert:
        with open(qs2) as fp:
            q = fp.read()
#            print(q)
            print("--------Testing "+qs2+"----------\n")
            allt, mt = timeQueryMult(q)
            certt.append(mt)
            alltime += (qs2 + ": " + allt + "\n")
#            uadbts.append(sizeQuery(q))
    for qs3 in queries_mb:
        with open(qs3) as fp:
            q = fp.read()
            print("--------Testing "+qs3+"----------\n")
#            print(q)
            allt, mt = timeQueryMult(q)
            mbt.append(mt)
            alltime += (qs3 + ": " + allt + "\n")
#            mbts.append(sizeQuery(q))
    return [qt,uadbt,radbt,certt,mbt,mcdbt], alltime

        
def plotPDbenchUncert(fn, maxval, yl = "Runtime (ms)"):
    global x
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.4",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin -3",
            "set bmargin -2",
            "set rmargin 0",
            "set lmargin 12",
            "set border 3 front linetype -1 linewidth 1.000",
            "set boxwidth 0.95 absolute",
            "set style fill solid 1.00 noborder",
            'set linetype 1 lw 1 lc rgb "#222222"',
            'set linetype 2 lw 1 lc rgb "#FF0000"',
            'set linetype 3 lw 1 lc rgb "#FFDD11"',
            'set linetype 4 lw 1 lc rgb "#0000FF"',
            'set linetype 5 lw 1 lc rgb "#55FF95"',
            'set linetype 6 lw 1 lc rgb "#55AAAA"',
            "set linetype cycle 4",
            "set grid nopolar",
            "set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics",
            "set grid layerdefault linetype 0 linewidth 3.000,  linetype 0 linewidth 1.000",
            "set key nobox autotitles columnhead Left reverse left",
            'set key font "Arial,26"',
            "set key width 1",
            "set key samplen 1",
            "set key spacing 1",
            "set key maxrows 2",
            "set key at -0.5, %d"%int(maxval),
            "set style histogram clustered gap 1 title  offset character 2, -0.25, 1",
            "set datafile missing '-'",
            "set style data histograms",

            "set xtics border in scale 0,0 nomirror   offset character 0.5, -0.5, 2 autojustify",
            'set xtics norangelimit font ",24"',
            "set xtics   ()",
            "set xrange [ -0.5 : %d]"%len(x),
                
            'set ylabel "%s"'%(yl),
            'set ylabel font "Arial,30"',
            "set ylabel offset character -3, 0, 0",

            "set logscale y",
            "set yrange [ 0.1 : %d ]"%int(maxval),
            "set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify",
            'set ytics font ",34"',

            'set xlabel font "Arial,34"',
            'set xlabel "Amount of Uncertainty"',
            "set xlabel  offset character 0, -1, 0  norotate",
                
            "plot '%s.csv' using 2 t col, '' using 3:xtic(1) t col, '' using 4 t col, '' using 5 t col, '' using 6 t col , '' using 7 t col"%fn
        ]))
        file.close()
        subprocess.call(["gnuplot", "%s.gp"%fn])
        subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
        #subprocess.call(["rm", "%s.gp"%fn])
        subprocess.call(["rm", "%s.ps"%fn])
        return "%s.pdf"%fn
        
def plotPDbenchScale(fn, maxval, yl = "Runtime (ms)"):
    global s
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.4",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin -3",
            "set bmargin -2",
            "set rmargin 0",
            "set lmargin 8",
            "set border 3 front linetype -1 linewidth 1.000",
            "set boxwidth 0.95 absolute",
            "set style fill   solid 1.00 noborder",
            'set linetype 1 lw 1 lc rgb "#222222"',
            'set linetype 2 lw 1 lc rgb "#FF0000"',
            'set linetype 3 lw 1 lc rgb "#FFDD11"',
            'set linetype 4 lw 1 lc rgb "#0000FF"',
            'set linetype 5 lw 1 lc rgb "#55FF95"',
            'set linetype 6 lw 1 lc rgb "#55AAAA"',
            "set linetype cycle 4",
            "set grid nopolar",
            "set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics",
            "set grid layerdefault linetype 0 linewidth 3.000,  linetype 0 linewidth 1.000",
            "set key nobox autotitles columnhead Left reverse left",
            'set key font "Arial,26"',
            "set key width 1",
            "set key samplen 1",
            "set key spacing 1",
            "set key maxrows 2",
            "set key at -0.5, %d"%int(maxval),
            "set style histogram clustered gap 1 title  offset character 2, -0.25, 1",
            "set datafile missing '-'",
            "set style data histograms",

            "set xtics border in scale 0,0 nomirror   offset character 0.5, -0.5, 2 autojustify",
            'set xtics norangelimit font ",24"',
            "set xtics   ()",
            "set xrange [ -0.5 : %d]"%len(s),
                    
            'set ylabel "%s"'%(yl),
            'set ylabel font "Arial,30"',
            "set ylabel  offset character -3, 0, 0",

            "set logscale y",
            "set yrange [ 0.1 : %d ]"%int(maxval),
            "set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify",
            'set ytics font ",34"',

            'set xlabel font "Arial,34"',
            'set xlabel "Data size"',
            "set xlabel  offset character 0, -1, 0  norotate",
                    
            "plot '%s.csv' using 2 t col, '' using 3:xtic(1) t col, '' using 4 t col, '' using 5 t col, '' using 6 t col , '' using 7 t col"%fn
        ]))
        file.close()
        subprocess.call(["gnuplot", "%s.gp"%fn])
        subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
        subprocess.call(["rm", "%s.gp"%fn])
        subprocess.call(["rm", "%s.ps"%fn])
        return "%s.pdf"%fn
        
def writetofile(fn, content):
    with open(fn,"w+") as f:
        f.write(content)
        f.close()
    
def test_pdbench_uncert():
    global test1
    global queries
    global x
    res = []
    flist = []
    alltimeout = ""
    for datadir in test1:
        print("--------Importing "+datadir+"----------\n")
        importPdbenchTables(datadir, 0, 0, False)
        print("--------Testing "+datadir+"----------\n")
        tlist, allt = testQueryPDbench()
        res.append(tlist)
        print("All res: "+allt)
        print("Mean: "+str(tlist))
        alltimeout += (datadir + "-----\n")
        alltimeout += allt
    #format and plot
    for i in range(0,len(queries)):
        writein = "Query\tDet\tUA-DB\tAU-DB\tLibkin\tMayBMS\tMCDB \n"
        toplimit = 0
        for k in range(0,len(res)):
            list = res[k]
            writein = writein + "%d%%\t"%(x[k]*100);
            for j in range(0,len(list)):
                writein = writein + list[j][i] + "\t"
                toplimit = max(toplimit, int(float(list[j][i])))
            writein = writein + "\n"
#        print(writein)
        writetofile("uncert_Q%d.csv"%i, writein)
        pdfname = plotPDbenchUncert("uncert_Q%d"%i, toplimit*10)
        flist.append(pdfname)
        flist.append("uncert_Q%d.csv"%i)
        
    subprocess.call(["mkdir", "results/pdbench_uncert"])
    for fn in flist:
        subprocess.call(["mv", "%s"%fn,"results/pdbench_uncert/%s"%fn])
    writetofile("uncert_raw.csv", alltimeout)
    subprocess.call(["mv", "uncert_raw.csv","results/pdbench_uncert/uncert_raw.csv"])
        
def test_pdbench_scale():
    global test2
    global queries
    global s
    res = []
    flist = []
    alltimeout = ""
    for datadir in test2:
        print("--------Importing "+datadir+"----------\n")
        importPdbenchTables(datadir, 0, 0, False)
        print("--------Testing "+datadir+"----------\n")
        tlist, allt = testQueryPDbench()
        res.append(tlist)
        print("All res: "+allt)
        print("Mean: "+str(tlist))
        alltimeout += (datadir + "-----\n")
        alltimeout += allt
        #format and plot
    for i in range(0,len(queries)):
        writein = "Query    Det    UA-DB    AU-DB    Libkin    MayBMS    MCDB \n"
        toplimit = 0
        for k in range(0,len(res)):
            list = res[k]
            writein = writein + "%dGB    "%(s[k]*100);
            for j in range(0,len(list)):
                writein = writein + list[j][i] + "    "
                toplimit = max(toplimit, int(float(list[j][i])))
            writein = writein + "\n"
        print(writein)

        writetofile("scale_Q%d.csv"%i, writein)
        pdfname = plotPDbenchScale("scale_Q%d"%i, toplimit*10)
        flist.append(pdfname)
        flist.append("scale_Q%d.csv"%i)
        
    subprocess.call(["mkdir", "results/pdbench_scale"])
    for fn in flist:
        subprocess.call(["mv", "%s"%fn,"results/pdbench_scale/%s"%fn])
    writetofile("scale_raw.csv", alltimeout)
    subprocess.call(["mv", "scale_raw.csv","results/pdbench_scale/scale_raw.csv"])
        
#db for incompletness test
db = 'dbs/incomp.db'
        #table
        #tb = 'pls'
tbs = ['graffiti','buffalo','busi','cont','crime','foodins','violation','permit','pls']
attrnums = [15,20,25,12,17,16,35,19,97]

def getSchema(tname, n):
    global db
    conn = sqlite3.connect(db)
    c = conn.cursor()
    res = c.execute("select * from %s limit 1;"%tname)
    names = [description[0] for description in res.description]
    return names[:n]

def runLiteQuery(q):
    global db
    conn = sqlite3.connect(db)
    c = conn.cursor()
    result = c.execute(q)
    conn.commit()
    row = result.fetchall()
    if len(row)>0:
        row = row[0]
    c.close()
    conn.close()
    return row
    
def timeLiteQuery(q):
    global db
    conn = sqlite3.connect(db)
    c = conn.cursor()
    start = time.time()
    c.execute(q)
    end = time.time()
    conn.commit()
    c.close()
    conn.close()
    return end-start
    
def runLiteQueryDB(dbn,q):
    conn = sqlite3.connect(dbn)
    c = conn.cursor()
    result = c.execute(q)
    conn.commit()
    row = result.fetchall()[0]
    c.close()
    conn.close()
    return row[0]

def onerun(attrs, n, tb):
    rd = random.sample(attrs, n)
    urd = ["U_"+itm for itm in rd]

    str = '","'.join(rd+urd)
    str = '"'+ str + '",U_R'

    wstrfn = " AND ".join(['"'+it+'"'+"='t'" for it in urd]) + " AND U_R_T='f'"
    wstrtp = " AND ".join(['"'+it+'"'+"='t'" for it in urd]) + " AND U_R_T='t'"
    wstrfp = "(" + " OR ".join(['"'+it+'"'+"='f'" for it in urd]) + ") AND U_R_T='f'"

    command = "select count(*) from (select %s from %s where %s);" % (str,tb,wstrfn)
    fn = runLiteQuery(command)
            
    command = "select count(*) from (select %s from %s where %s);" % (str,tb,wstrtp)
    tp = runLiteQuery(command)
            
    command = "select count(*) from (select %s from %s where %s);" % (str,tb,wstrfp)
    fp = runLiteQuery(command)
            
    return (tp,fn,fp),rd
    
        
def plotmicro(fn, maxx, maxy, xlab, miny = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "%s"'%(xlab),
            'set xtics font "Arial,28"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "#FFDD11"  lw 9',
            'set style line 2 lt 1 lc rgb "black" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Time (sec)" font "Arial,32"',
            'set ylabel offset character -1, 0, 0',
            'set ytics font "Arial,32"',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width -2',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            'plot "%s.csv" using 1:2 title "AUDB" with lines linestyle 1, "%s.csv" using 1:3 title "Det" with lines linestyle 2, "%s.csv" using 1:4 title "AUDB(8)" with lines linestyle 3, "%s.csv" using 1:5 title "AUDB(9)" with lines linestyle 4, "%s.csv" using 1:6 title "Conventional" with lines linestyle 5'%(fn,fn,fn,fn,fn)
        ]))
        file.close()
        subprocess.call(["gnuplot", "%s.gp"%fn])
        subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
        subprocess.call(["rm", "%s.gp"%fn])
        subprocess.call(["rm", "%s.ps"%fn])
        return "%s.pdf"%fn
        
        
def plotmicroRange(fn, maxx, maxy, xlab, miny = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "%s"'%(xlab),
            'set xtics font "Arial,30"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Time (ms)" font "Arial,32"',
            'set ylabel offset character -2, 0, 0',
            'set ytics font "Arial,32"',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width 0',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            "plot '%s.csv' using 1:2 title 'CT=4' with lines linestyle 1, '%s.csv' using 1:3 title 'CT=32' with lines linestyle 2, '%s.csv' using 1:4 title 'CT=256' with lines linestyle 3, '%s.csv' using 1:5 title 'CT=512' with lines linestyle 4"%(fn,fn,fn,fn)
        ]))
        file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
    
#def plotmicroAggCompressionall(fn, fn2, maxx, maxy1, maxy2, miny1 = 0, miny2=0):
#    with open("%s.gp"%fn, "w+") as file:
#        file.write("\n".join([
#            "set size ratio 0.55",
#            "set terminal postscript color enhanced",
#            "set output '%s.ps'"%fn,
#            "unset title",
#            "set tmargin 0",
#            "set bmargin 1",
#            "set rmargin 0",
#            "set lmargin -9",
#            "set border 3 front linetype -1 linewidth 1.500",
#            "set style fill solid 0.65 border -1",
#            'set xlabel font "Arial,35" offset 0,-2',
#            'set xlabel "Compression result size (CT)"',
#            'set xtics font "Arial,28" offset 0,-0.5',
#            "set for [i=1:4] linetype i dt i",
#            'set style line 1 lt 1 lc rgb "orange"  lw 9',
#            'set style line 2 lt 1 lc rgb "grey" lw 9',
#            'set style line 3 lt 1 lc rgb "green" lw 9',
#            'set style line 4 lt 1 lc rgb "blue" lw 9',
#            'set style line 5 lt 1 lc rgb "black" lw 9',
#            'set style line 6 lt 1 lc rgb "#110099" lw 9',
#            'set ylabel "Time (sec)" font "Arial,32"',
#            'set ylabel offset character -4, 0, 0',
#            'set y2label "Metric" font "Arial,32"',
#            'set y2label offset character 3, 0, 0',
#            'set ytics font "Arial,28"',
#            'set y2tics font "Arial,28" ',
#            'set ytics nomirror',
#            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
#            'set key font "Arial,30"',
#            'set key spacing 1',
#            'set key samplen 3',
#            'set key width -2',
#            'set grid nopolar',
#            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
#            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
#            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
#            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny1),str(maxy1)),
#            "set y2range [ %s : %s ] noreverse nowriteback"%(str(miny2),str(maxy2)),
#            'plot "%s.csv" using 2:xticlabels(1) title "Runtime" with lines linestyle 1, "%s.csv" using 2:xticlabels(1) title "Runtime" with lines linestyle 1'%(fn, fn2)
#    ]))
#    file.close()
#    subprocess.call(["gnuplot", "%s.gp"%fn])
#    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
#    subprocess.call(["rm", "%s.gp"%fn])
#    subprocess.call(["rm", "%s.ps"%fn])
#    return "%s.pdf"%fn

def plotmicroAggCompressionSQ(fn, fn2, maxx, maxy1, maxy2, miny1 = 0, miny2=0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.3",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 0",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'unset xtics',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Time(s)" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
            'set ytics 0,20,80 font "Arial,28"',
            'set ytics nomirror',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width -1',
            'set grid nopolar',
            'set multiplot layout 3,1',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny1),str(maxy1)),
            'plot "%s.csv" using 2:xticlabels(1) title "Runtime" with lines linestyle 1'%(fn),
            'set xtics font "Arial,28" offset 0,-0.5',
            'set xlabel font "Arial,35" offset 0,-2',
            'set xlabel "Compression result size (CT)"',
            'set ylabel "Range" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
            'unset ytics',
            'set ytics 0, 10000000, 55000000 font "Arial,28"',
            "set format y '%.s%c'",
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny2),str(maxy2)),
            'set key width -1',
            'plot "%s.csv" using 2:xticlabels(1) title "Mean Range" with lines linestyle 2'%(fn2),
            'unset multiplot'
        ]))
        file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
        
def plotmicroAggCompression(fn, maxx, maxy1, miny1 = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-2',
            'set xlabel "Compression result size (CT)"',
            'set xtics font "Arial,28" offset 0,-0.5',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Time (sec)" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
#            'set y2label "Metric" font "Arial,32"',
#            'set y2label offset character 3, 0, 0',
            'set ytics font "Arial,28"',
#            'set y2tics font "Arial,28" ',
            'set ytics nomirror',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width -2',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny1),str(maxy1)),
#            "set y2range [ %s : %s ] noreverse nowriteback"%(str(miny2),str(maxy2)),
            'plot "%s.csv" using 2:xticlabels(1) title "Runtime" with lines linestyle 1'%(fn)
    ]))
    file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
def plotmicroAggCompressionMetric(fn, maxx, maxy1, miny1 = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 10",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-2',
            'set xlabel "Compression result size (CT)"',
            'set xtics font "Arial,28" offset 0,-0.5',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Max range" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
#            'set y2label "Metric" font "Arial,32"',
#            'set y2label offset character 3, 0, 0',
            'set ytics font "Arial,28"',
#            'set y2tics font "Arial,28" ',
            'set ytics nomirror',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width -2',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 0.00000 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny1),str(maxy1)),
            'plot "%s.csv" using 2:xticlabels(1) title "Quality" with lines linestyle 1'%(fn)
        ]))
    file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
def plotmicrojoin(fn, maxx, maxy, miny = 10):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "Data size"',
            'set xtics font "Arial,28"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "black"  lw 9',
            'set style line 2 lt 1 lc rgb "red" lw 9',
            'set style line 3 lt 1 lc rgb "blue" lw 9',
            'set style line 4 lt 1 lc rgb "green" lw 9',
            'set style line 5 lt 1 lc rgb "orange" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Time (ms)" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
            'set ytics font "Arial,28"',
            "set logscale y",
            'set key inside left top vertical Right noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 2',
            'set key width -2',
            "set key maxrows 2",
            "set format x '%.s%c'",
            'set key width 0',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 4500 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            'plot "%s.csv" using 1:2 title "Non-Op" with lines linestyle 1, "%s.csv" using 1:3 title "CT=4" with lines linestyle 2, "%s.csv" using 1:4 title "CT=32" with lines linestyle 3, "%s.csv" using 1:5 title "CT=256" with lines linestyle 4, "%s.csv" using 1:6 title "CT=1024" with lines linestyle 5'%(fn,fn,fn,fn,fn)
        ]))
    file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
def plotmicrojoinMetrics(fn, maxx, maxy, miny = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "Data size"',
            'set xtics font "Arial,28"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "black"  lw 9',
            'set style line 2 lt 1 lc rgb "red" lw 9',
            'set style line 3 lt 1 lc rgb "blue" lw 9',
            'set style line 4 lt 1 lc rgb "green" lw 9',
            'set style line 5 lt 1 lc rgb "orange" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Possbile size" font "Arial,32"',
            'set ylabel offset character -4, 0, 0',
            'set ytics font "Arial,28"',
            'set key inside left top vertical Right noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 2',
            'set key width -2',
            "set key maxrows 2",
            "set format x '%.s%c'",
            "set format y '%.s%c'",
            'set key width 0',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 4500 : %s ] noreverse nowriteback"%(str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            'plot "%s.csv" using 1:2 title "Non-Op" with lines linestyle 1, "%s.csv" using 1:3 title "CT=4" with lines linestyle 2, "%s.csv" using 1:4 title "CT=32" with lines linestyle 3, "%s.csv" using 1:5 title "CT=256" with lines linestyle 4, "%s.csv" using 1:6 title "CT=1024" with lines linestyle 5'%(fn,fn,fn,fn,fn)
        ]))
    file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn

def printtriotest(fn, maxx, maxy, miny = 0):
    subprocess.call(["mkdir", "results/trio"])
    dst = "results/trio/trio.csv"
    subprocess.call(["cp", "%s.csv"%(fn), dst])
    fn = "results/trio/trio"
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.4",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "# Aggregation Operators"',
            'set xtics 0,1,10 font "Arial,32" offset 0, -0.5',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 pointtype 4 lc rgb "#222222" lw 7',
            'set style line 2 lt 1 pointtype 8 lc rgb "purple" lw 7',
            'set style line 3 lt 1 pointtype 12 lc rgb "#FFDD11" lw 7',
            'set style line 4 lt 1 pointtype 17 lc rgb "orange" lw 7',
            'set style line 5 lt 1 pointtype 17 lc rgb "#55AAAA" lw 7',
#            'set linetype 1 lw 1 lc rgb "#222222"',
#            'set linetype 2 lw 1 lc rgb "#FF0000"',
#            'set linetype 3 lw 1 lc rgb "#FFDD11"',
#            'set linetype 4 lw 1 lc rgb "#0000FF"',
#            'set linetype 5 lw 1 lc rgb "#55FF95"',
#            'set linetype 6 lw 1 lc rgb "#55AAAA"',
            'set ylabel "Time (sec)" font "Arial,32"',
            'set ylabel  offset character -3, 0, 0',
            'set ytics font "Arial,32"',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,28"',
            'set key spacing 1',
            'set key samplen 3',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ 1.00000 : %s ] noreverse nowriteback"%(str(maxx+1)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            'plot "%s.csv" using 1:2 title "Det" with linespoints linestyle 1, "%s.csv" using 1:3 title "AUDB" with linespoints linestyle 3, "%s.csv" using 1:4 title "Trio" with linespoints linestyle 2, "%s.csv" using 1:5 title "Symb" with linespoints linestyle 4, "%s.csv" using 1:6 title "MCDB" with linespoints linestyle 5'%(fn,fn,fn,fn,fn)
    ]))
    file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
def plotmicroovergrouping(fn, minx, maxx, maxy, xlab, miny = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "%s"'%(xlab),
            'set xtics font "Arial,30"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "Percentage (%)" font "Arial,32"',
            'set ylabel offset character -2, 0, 0',
            'set ytics font "Arial,32"',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width 0',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ %s : %s ] noreverse nowriteback"%(str(minx),str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            "plot '%s.csv' using 1:2 title '2%% uncertainty' with lines linestyle 3, '%s.csv' using 1:3 title '3%% uncertainty' with lines linestyle 1, '%s.csv' using 1:4 title '5%% uncertainty' with lines linestyle 4"%(fn,fn,fn)
        ]))
        file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
    
def plotmicrooverrange(fn, minx, maxx, maxy, xlab, miny = 0):
    with open("%s.gp"%fn, "w+") as file:
        file.write("\n".join([
            "set size ratio 0.6",
            "set terminal postscript color enhanced",
            "set output '%s.ps'"%fn,
            "unset title",
            "set tmargin 0",
            "set bmargin 1",
            "set rmargin 0",
            "set lmargin -9",
            "set border 3 front linetype -1 linewidth 1.500",
            "set style fill solid 0.65 border -1",
            'set xlabel font "Arial,35" offset 0,-1',
            'set xlabel "%s"'%(xlab),
            'set xtics font "Arial,30"',
            "set for [i=1:4] linetype i dt i",
            'set style line 1 lt 1 lc rgb "orange"  lw 9',
            'set style line 2 lt 1 lc rgb "grey" lw 9',
            'set style line 3 lt 1 lc rgb "green" lw 9',
            'set style line 4 lt 1 lc rgb "blue" lw 9',
            'set style line 5 lt 1 lc rgb "black" lw 9',
            'set style line 6 lt 1 lc rgb "#110099" lw 9',
            'set ylabel "factor" font "Arial,32"',
            'set ylabel offset character -2, 0, 0',
            'set ytics font "Arial,32"',
            'set key inside left top vertical Left noreverse noenhanced autotitle nobox',
            'set key font "Arial,30"',
            'set key spacing 1',
            'set key samplen 3',
            'set key width 0',
            'set grid nopolar',
            'set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics',
            'set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 3.000',
            "set xrange [ %s : %s ] noreverse nowriteback"%(str(minx),str(maxx)),
            "set yrange [ %s : %s ] noreverse nowriteback"%(str(miny),str(maxy)),
            "plot '%s.csv' using 1:2 title '2%% uncertainty' with lines linestyle 3, '%s.csv' using 1:3 title '3%% uncertainty' with lines linestyle 1, '%s.csv' using 1:4 title '5%% uncertainty' with lines linestyle 4"%(fn,fn,fn)
        ]))
        file.close()
    subprocess.call(["gnuplot", "%s.gp"%fn])
    subprocess.call(["ps2pdf", "%s.ps"%fn, "%s.pdf"%fn])
    subprocess.call(["rm", "%s.gp"%fn])
    subprocess.call(["rm", "%s.ps"%fn])
    return "%s.pdf"%fn
        
#test result size and certain percentage
def test_pdbenchSize():
    pdbenchGenOnX(sval=0.1)
    global test1
    global queries
    global x
    res = []
    mbres = []
    ret = "uncert\tQ1\tQ2\tQ3\t"
    for k in range(0,len(test1)):
        datadir = test1[k]
        importPdbenchTables(datadir)
        ret = ret + "\n%d%%\t"%(x[k]*100)
        resl = []
        reslmb = []
        for i in range(0,len(queries_uadb)):
            with open(queries_uadb[i]) as fp:
                q = fp.read()
                resl.append(sizeQuery(q))
                qnew = q.split(';')[0]
                allres = runQuery("select count(*) from (%s) xx;"%qnew)[0]
                certres = runQuery("select count(*) from (%s) xx where u_r=1;"%qnew)[0]
                ret = ret + str(int(round(float(certres[0])/float(allres[0])))) + "%%(%d)\t"%certres[0]
            with open(queries_mb[i]) as fp:
                q = fp.read()
                reslmb.append(sizeQuery(q))
        res.append(resl)
        mbres.append(reslmb)
    result = "uncert\tQ1_uadb\tQ2_uadb\tQ3_uadb\tQ1_maybms\tQ2_maybms\tQ3_maybms\t"
    for i in range(0,len(res)):
        result = result + "\n%d%%\t"%int(x[i]*100)
        for j in range(0, len(res[0])):
            result = result + "%d\t"%res[i][j]
        for j in range(0, len(mbres[0])):
            result = result + "%d\t"%mbres[i][j]
    writetofile("size.csv",result)
    subprocess.call(["mkdir", "results/pdbench_stats"])
    subprocess.call(["mv", "size.csv","results/pdbench_stats/size.csv"])
    writetofile("results/pdbench_stats/uncert_percentage.csv",ret)

def getAUDBQueryFromGProM(query, gpromcomand = gpromsd):
    qy = copy.copy(gpromcomand)
    qy.append(query)
    rt = subprocess.check_output(qy)
    return rt.decode()

def test_realQ():
    queries = ['realQuery/Q1.txt','realQuery/Q2.txt','realQuery/Q3.txt','realQuery/Q4.txt','realQuery/Q5.txt']
    runLiteQuery("drop table if exists dummy;")
    global gpromcom
    global dir
    gpromcom[4] = dir+"/dbs/incomp.db"
    rep = 5
    ret = "\tQ1\tQ2\tQ3\tQ4\tQ5\nOverhead\t"
    retl2 = "\nError Rate\t"
    for qf in queries:
        print("Testing Real Query %s"%qf)
        query = copy.copy(gpromcom)
        with open(qf) as fp:
            contents = fp.read().split(';')
            q = contents[0] + ";"
            qsize = "select count(*) from (" + contents[0] + ");"
            qsizeu = contents[1] + ";"
#            print(q)
            query.append(q)
            x = subprocess.check_output(query)
            x = x.decode()
            t0 = 0
            t1 = 0
            for i in range (0,rep):
                t1 += timeLiteQuery("create table dummy as " + x)
                runLiteQuery("drop table if exists dummy;")
                t0 += timeLiteQuery("create table dummy as " + q)
                runLiteQuery("drop table if exists dummy;")
            sz1 = runLiteQuery(qsize)
            sz2 = runLiteQuery(qsizeu)
            rsz = float(sz2[0])/float(sz1[0])
            ret = ret + "%s%%\t"%str(((t1-t0)/t0)*100)
            retl2 = retl2 + "%f%%\t"%(rsz*100)
    subprocess.call(["mkdir", "results/realQuery"])
    writetofile("results/realQuery/realQuery.csv",ret+retl2)
    
def exittest():
#    print("Closing postgres servers")
#    os.system('sudo -u postgres /usr/lib/postgresql/10/bin/pg_ctl -D /home/perm/su/pgdata -m fast stop')
#    os.system('sudo -u postgres /maybms/install/bin/pg_ctl -D /maybms/data -m fast stop')
#    time.sleep(10)
    print("Test done")
    quit()
    
def importmicrotable(colnum, rolnum, rangeval, uncert, minval, maxval, tn="micro", aoff=0):
    tname = tn
    attrs = mcg.tablegen(colnum, rolnum, rangeval, uncert, minval, maxval, aoff)
    pushQuery('drop table if exists %s'%(tname))
    tableinitq = "create table %s ("%(tname)
    for an in list(attrs.split(",")) :
        tableinitq += "%s integer,"%(an)
    tableinitq = tableinitq[:-1]+");"
    print(tableinitq)
    pushQuery(tableinitq)
    tablecopy = "copy %s from '%s/micro.csv' DELIMITER ',' CSV HEADER;"%(tname, dir)
    print(tablecopy)
    pushQuery(tablecopy)
    return attrs
    
def importmicrotablefromtidb(colnum, rolnum, rangeval, uncert, minval, maxval, tsize, tn="micro", aoff=0):
    tname = tn
    tiname = tn+"_tidb"
    attrs = mcg.tablegentidb(colnum, rolnum, rangeval, uncert, minval, maxval,tsize,aoff)
    pushQuery('drop table if exists %s'%(tname))
    pushQuery('drop table if exists %s'%(tiname))
    titableinitq = "create table %s ("%(tiname)
    for an in list(attrs.split(",")):
        titableinitq += "%s integer,"%(an)
    titableinitq = titableinitq[:-1]+");"
    print(titableinitq)
    pushQuery(titableinitq)
    tablecopy = "copy %s from '%s/microtidb.csv' DELIMITER ',' CSV HEADER;"%(tiname, dir)
    print(tablecopy)
    pushQuery(tablecopy)
    autable = "create table " + tname + " as select"
    bdlist = ""
    print(attrs)
    for an in list(attrs.split(",")):
        if an=="id":
            autable += " id,"
        else:
            autable += " min(" + an + ") as " +an+","
            bdlist += " max(" + an + ") as ub_" +an+", min(" + an + ") as lb_" +an+","
    autable += (bdlist+"1 as cet_r, 1 as bst_r, 1 as pos_r from " + tiname + " group by id;")
    pushQuery(autable)
    return attrs
    
def microbenchmark():

    print("[TESTING MICROBENCHMARK]")
    
    resname = "micro_r"

#    colnum = 2
#    rolnum = 100000
#
#    minval = 1
#    maxval = 10000
#
#    rangeval = 5000 #uncertain attribute range
#    uncert = 0.1 #uncertainty percentage
#
#    interv = 2 #groupby increment
#
#    attrs = importmicrotable(colnum, rolnum, rangeval, uncert, minval, maxval)
#
#    return
    

    colnum = 100
    rolnum = 350000

    minval = 1
    maxval = 100

    rangeval = 5 #uncertain attribute range
    uncert = 0.05 #uncertainty percentage
    
    interv = 2 #groupby increment
    
    attrs = importmicrotable(colnum, rolnum, rangeval, uncert, minval, maxval)
    
    
    #########################################varying groupby########################################
#    print("[TESTING MICROBENCHMARK] - varying groupby")
#    queryn = "select sum(a%s) from micro group by "%(str(colnum-1))
#    queryh = "urange (select sum(a%s) from micro is radb group by "%(str(colnum-1))
#    groupby = ""
#    resnum = []
#    resorig = []
#    resres = []
#    maxy = 0
#    for i in range(0,colnum-1, 5):
#        if i==0:
#            groupby += "a"+str(i)
#        else :
#            groupby += ", a"+str(i)
#        if (i%interv)==0:
#            resnum.append(str(i))
#            querynorm = queryn + groupby + ";"
##            print(querynorm)
#            query = queryh + groupby + ");"
#            rewrite = getAUDBQueryFromGProM(query)
#            allt, mt = timeQueryMult(rewrite)
#            allt, mt2 = timeQueryMult(querynorm)
#            print(float(mt))
#            print(mt2)
#            if float(mt) > maxy:
#                maxy = float(mt)
#                print(int(maxy))
#            resres.append(str(mt))
#            resorig.append(str(mt2))
#    reswrite = ""
#    for i,num in enumerate(resnum):
#        reswrite += (num + "\t" + resres[i] + "\t" + resorig[i] + "\n")
#    print(reswrite)
#    subprocess.call(["mkdir", "results/microbench"])
#    writetofile("groupby.csv",reswrite)
#    plotmicro("groupby", colnum, int(maxy)+1, "# groupby Attributs")
#    subprocess.call(["mv", "groupby.csv","results/microbench/groupby.csv"])
#    subprocess.call(["mv", "groupby.pdf","results/microbench/groupby.pdf"])
#
#    #########################################varying aggregation########################################
#    print("[TESTING MICROBENCHMARK] - varying aggregation")
#    queryn = " from micro group by a%s"%(str(colnum-1))
#    queryh = " from micro is radb group by a%s"%(str(colnum-1))
#    aggf = ""
#    resnum = []
#    resorig = []
#    resres = []
#    maxy = 0
#    for i in range(0,colnum-1, 5):
#        if i==0:
#            aggf += "sum(a%s)"%(str(i))
#        else :
#            aggf += ", sum(a%s)"%(str(i))
#        if (i%interv)==0:
#            resnum.append(str(i))
#            querynorm = "select " + aggf + queryn + ";"
#            print(querynorm)
#            query = "urange (select " + aggf + queryh + ");"
#            print(query)
#            rewrite = getAUDBQueryFromGProM(query)
#            allt, mt = timeQueryMult(rewrite)
#            allt, mt2 = timeQueryMult(querynorm)
#            print(mt)
#            print(mt2)
#            if float(mt) > maxy:
#                maxy = float(mt)
#                print(int(maxy))
#            resres.append(str(mt))
#            resorig.append(str(mt2))
#    reswrite = ""
#    for i,num in enumerate(resnum):
#        reswrite += (num + "\t" + resres[i] + "\t" + resorig[i] + "\n")
#    print(reswrite)
#    writetofile("aggregation.csv",reswrite)
#    plotmicro("aggregation", colnum, int(maxy)+1, "# Aggregation functions")
#    subprocess.call(["mv", "aggregation.csv","results/microbench/aggregation.csv"])
#    subprocess.call(["mv", "aggregation.pdf","results/microbench/aggregation.pdf"])
#
#
#    ########################################varying range########################################
#    print("[TESTING MICROBENCHMARK] - varying range")
#    compfactor = [2,5,8,9]
#    minval = 1
#    maxval = 100000
#    interval = 5000
#    maxy = 0
#    uncert = 0.10
#    res = ""
#    query = "urange (select a0,sum(a1) from micro is radb group by a0);"
#    for i in range(minval, maxval, interval):
#        res += str(i/100000)
#        attrs = importmicrotable(2, rolnum, i, uncert, minval, maxval)
#        for cf in compfactor:
#            print("[microbench - varying range] range = %d, c_factor = %d"%(i, cf))
#            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_compression_rate", "%s"%(str(cf)), "-Pexecutor", "sql", "-query"]
#            rquery = getAUDBQueryFromGProM(query, gpromcmd)
#            allt, mt = timeQueryMult(rquery)
#            if float(mt) > maxy:
#                maxy = float(mt)
#            res += ("\t"+str(mt))
#        res += "\n"
#    print(res)
#    writetofile("range.csv",res)
#    #plotmicro("range", maxval, int(maxy)+1, "Uncertain attribute range")
#    subprocess.call(["mv", "range.csv","results/microbench/range.csv"])
#    #subprocess.call(["mv", "range.pdf","results/microbench/range.pdf"])
#    plotmicroRange("results/microbench/range",1, int(maxy)+1, "Attribute bound size / Domain size", -1)

    
        #######################################varying compression rate########################################
    print("[TESTING MICROBENCHMARK] - varying compression rate")
    maxiteration = 17
#
    minval = 1
    maxval = 10000
    rolnum = 10000
    uncert = 0.02

    rangeval = 20 #uncertain attribute range

    res = ""
    maxy = 0
    mres = ""

    attrs = importmicrotable(2, rolnum, rangeval, uncert, minval, maxval)

    resname = "micro_r"

    for i in range(maxiteration):
        gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_compression_rate", "%s"%(str(i+1)), "-Pexecutor", "sql", "-query"]
        query = "urange (select a0,sum(a1) as s1 from micro is radb group by a0);"
        rquery = getAUDBQueryFromGProM(query, gpromcmd)
        allt, mt = timeQueryMult(rquery)
        materializequery(rquery, resname)
        metrics = "%s\t"%(str(i+1)) + str(getmetric(resname)) + "\n"
        print(metrics)
        if float(mt) > maxy:
            maxy = float(mt)
        res += (str(i+1) + "\t" + str(mt) + "\n")
        mres += metrics
    print(res)
    print(mres)
    writetofile("compress.csv",res)
    writetofile("compress_metrics.csv",mres)
    writetofile("compress.csv",res)
    writetofile("compress_metrics.csv",mres)
    plotmicro("compress", maxiteration, int(maxy)+1, "Compress factor")
    subprocess.call(["mv", "compress.csv","results/microbench/compress.csv"])
    subprocess.call(["mv", "compress_metrics.csv","results/microbench/compress_metrics.csv"])
#    subprocess.call(["mv", "compress.pdf","results/microbench/compress.pdf"])
    plotmicroAggCompressionSQ("results/microbench/compress", "results/microbench/compress_metrics", 16, 80, 55000000, -5, -3000000)

    
    #######################################Join with different optimizations########################################
    print("[TESTING MICROBENCHMARK] - join optimizations")
    colnum = 2
    rolnum = 3000
    maxrl = 5000



    minval = 1
    maxval = 1000
    rangeval = 50

    maxy = 0

    compfactor = [2,3,4,5]
    resname = "micro_r"
    res = ""
    mres = ""

    rangeval = 15 #uncertain attribute range
    uncert = 0.03 #uncertainty percentage
    for i in range(rolnum, maxrl+1, 1000):
        attrs = importmicrotable(colnum, i, rangeval, uncert, minval, maxval)
        gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-Pexecutor", "sql", "-range_optimize_join", "FALSE", "-query"]
        query = "urange (select a0, a1, aa0, aa1 from micro is radb join (select a0 as aa0, a1 as aa1 from micro is radb) x on a0 = aa0);"
        rquery = getAUDBQueryFromGProM(query, gpromcmd)
        materializequery(rquery, resname)
        metrics = getmetric(resname)
        print(metrics[3])
        allt, mt = timeQueryMult(rquery)
        if float(mt) > maxy:
            maxy = float(mt)
        res += (str(i) + "\t" + str(mt))
        mres += (str(i) + "\t" + str(metrics[3]))
        for cf in compfactor:
            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_compression_rate", "%s"%(str(cf)), "-Pexecutor", "sql", "-query"]
            query = "urange (select a0, a1, aa0, aa1 from micro is radb join (select a0 as aa0, a1 as aa1 from micro is radb) x on a0 = aa0);"
            rquery = getAUDBQueryFromGProM(query, gpromcmd)
            materializequery(rquery, resname)
            metrics = getmetric(resname)
            print(metrics[3])
            allt, mt = timeQueryMult(rquery)
            if float(mt) > maxy:
                maxy = float(mt)
            res += ("\t" + str(mt))
            mres += ("\t" + str(metrics[3]))
        res += "\n"
        mres += "\n"
    print(res)
    print(mres)
    writetofile("join.csv",res)
    writetofile("join_metrics.csv",mres)
    plotmicro("join", maxrl, int(maxy)+1, "Data size")
    subprocess.call(["mv", "join.csv","results/microbench/join.csv"])
    subprocess.call(["mv", "join_metrics.csv","results/microbench/join_metrics.csv"])
    subprocess.call(["mv", "join.pdf","results/microbench/join.pdf"])

    #######################################varying number of joins########################################
    print("[TESTING MICROBENCHMARK] - # joins")
    
    rep = 4

    colnum = 2
    rolnum = 1000

    minval = 1
    maxval = 1000

    rangeval = 50 #uncertain attribute range
    uncert = 0.1 #uncertainty percentage

    cf = 8

    for i in range(rep+1):
        attrs = importmicrotable(colnum, rolnum, rangeval, uncert, minval, maxval, "t"+str(i), i*2)

    for currep in range(1, rep+1):
            query = ""
            for i in range(currep+1):
                tn = "t"+str(i) + " is radb"
                an = "a"+str(i*2)
                if i==0:
                    query = tn
                else:
                    query = "("+query+" join " + tn + " on " + "a"+str(i*2-1) + " = " + "a"+str(i*2) + ")"
            query = "urange (select * from " + query + ");"
            print(query)
            print("[microbench - varying number of joins] join# = %d, c_factor = %d"%(currep, cf))
#            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_optimize_join", "FALSE", "-Pexecutor", "sql", "-query"]
            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_compression_rate", "%s"%(str(cf)), "-Pexecutor", "sql", "-query"]
            rquery = getAUDBQueryFromGProM(query, gpromcmd)
#            print(rquery)
            allt, mt = timeQueryMult(rquery)
            size = sizeQuery(rquery)
            print(mt, size)
    
    
    
    ########################################varying attribute range measure overgrouping########################################
    print("[TESTING MICROBENCHMARK] - overgrouping")

    colnum = 3
    rolnum = 1000

    minval = 1
    maxval = 1000

    rangeval = 80 #uncertain attribute range
#    uncert = 0.03 #uncertainty percentage

    uncert = [0.02,0.03,0.05]

    cf = 1

    rep = 5

#    gp = "select a1 as gp, max(ub_a1) as ub_gp, min(lb_a1) as lb_gp from micro group by a1;"

    qtidb = "select count(t2.id) from (select distinct a1 as a1 from micro) t1, micro_tidb t2 where t1.a1 = t2.a1;" # and t1.id != t2.id

    qaudb = "select count(t2.id) from (select a1 as a1, max(ub_a1) as ub_a1, min(lb_a1) as lb_a1 from micro group by a1) t1, micro t2 where t1.ub_a1 >= t2.lb_a1 and t2.ub_a1 >= t1.lb_a1;" #and t1.id != t2.id

    allres = []
    resr = []

    for ut in uncert:
        res = []
        resr = []
        for i in range(20, rangeval, 1):
            rec = []
            for j in range(0,rep):
                attrs = importmicrotablefromtidb(colnum, rolnum, i, ut, minval, maxval, 19)
                ret = runQuery(qtidb)
                tn = float(ret[-1][0])
                ret = runQuery(qaudb)
                an = float(ret[-1][0])
                pct = (an-tn)/(tn)*100
                rec.append(pct)
            mpct = statistics.mean(rec)
            print(str(mpct)+"%")
            res.append(mpct)
            resr.append((float(i)/maxval)*100)
#            resr.append(i)
#        print(res)
        allres.append(res)
    print(allres)
    print(len(resr))
    reswrite = ""
    for i, num in enumerate(resr):
        reswrite += (str(num))
        for j in range(len(uncert)):
            reswrite += "\t" + str(allres[j][i])
        reswrite += "\n"
    print(reswrite)
    subprocess.call(["mkdir", "results/microbench"])
    writetofile("overgrouping.csv",reswrite)
    plotmicroovergrouping("overgrouping", 1 , max(resr)*1.1, max(sum(allres, []))*1.1, "Max relative uncertain range (%)")
    subprocess.call(["mv", "overgrouping.csv","results/microbench/overgrouping.csv"])
    subprocess.call(["mv", "overgrouping.pdf","results/microbench/overgrouping.pdf"])

############################################Verying range measure output range#######################################################
    print("[TESTING MICROBENCHMARK] - output range")
    colnum = 3
    rolnum = 1000

    minval = 1
    maxval = 700

    rangeval = 70 #uncertain attribute range
#    uncert = 0.03 #uncertainty percentage
    uncert = [0.02,0.03,0.05]

    cf = 1

    rep = 5

#    uctid = "(select m1.id, m1.a1, m1.a2, m2.ua from micro_tidb m1 join (select id, case when count(a2)>1 then 1 else 0 end as ua from micro_tidb group by id) m2 on m1.id=m2.id)"

    tidblb = "select t1.a1, sum(case when t2.ua>0 then 0 else t2.a2 end) as lb_sum from (select distinct a1 as a1 from micro) t1, (select m1.id, m1.a1, m1.a2, m2.ua from micro_tidb m1 join (select id, case when count(a2)>1 then 1 else 0 end as ua from micro_tidb group by id) m2 on m1.id=m2.id) t2 where t1.a1 = t2.a1 group by t1.a1"

    tidbub = "select g.a1, sum(g.a2) as ub_sum from (select t1.a1, t2.id, max(t2.a2) as a2 from (select distinct a1 as a1 from micro) t1, micro_tidb t2 where t1.a1 = t2.a1 group by t2.id, t1.a1) g group by g.a1"

    qtidb = "select t1.a1, t2.ub_sum as ub_sum, t1.lb_sum as lb_sum from (%s) t1 join (%s) t2 on t1.a1=t2.a1"%(tidblb,tidbub)

#    qtidb = "select t1.a1, sum(t2.a2) as ub_sum, sum(case when t2.ua>0 then 0 else t2.a2 end) as lb_sum from (select distinct a1 as a1 from micro) t1, (select m1.id, m1.a1, m1.a2, m2.ua from micro_tidb m1 join (select id, case when count(a2)>1 then 1 else 0 end as ua from micro_tidb group by id) m2 on m1.id=m2.id) t2 where t1.a1 = t2.a1 group by t1.a1"

    qaudb = "select t1.a1, t1.ub_a1, t1.lb_a1, sum(t2.ub_a2) as ub_sum, sum(case when t2.ub_a1 > t2.lb_a1 or t1.ub_a1 > t1.lb_a1 then 0 else t2.lb_a2 end) as lb_sum from (select a1 as a1, max(ub_a1) as ub_a1, min(lb_a1) as lb_a1 from micro group by a1) t1, micro t2 where t1.ub_a1 >= t2.lb_a1 and t2.ub_a1 >= t1.lb_a1 group by t1.a1, t1.ub_a1, t1.lb_a1"

    join = "select t1.a1, t1.ub_sum as ub1, t1.lb_sum as lb1, t2.ub_sum as ub2, t2.lb_sum as lb2, t1.ub_sum-t1.lb_sum as dif1, t2.ub_sum-t2.lb_sum as dif2 from (%s) t1, (%s) t2 where t1.a1<=t2.ub_a1 and t1.a1>=t2.lb_a1"%(qtidb,qaudb)

#    mena = "select * from (%s) x where "

    dif = "select avg((dif2-dif1)/700) as over from (%s) x"%(join)

#    print(dif)

#    qaudb = "select a1, count(*) as ct, sum(case when ub_a2 < 0 then ub_a2*cet_r else ub_a1*pos_r end) as ub_sum, sum(case when lb_a2 > 0 then lb_a2*cet_r else lb_a2*pos_r end) as lb_sum, abs(sum(case when ub_a2 < 0 then ub_a2*cet_r else ub_a2*pos_r end)-sum(case when lb_a2 > 0 then lb_a2*cet_r else lb_a2*pos_r end)) as dif from micro group by a1;"

    allres = []
    resr = []
#
    for ut in uncert:
        res = []
        resr = []
        for i in range(10, rangeval, 1):
            rec = []
            for j in range(0,rep):
                attrs = importmicrotablefromtidb(colnum, rolnum, i, ut, minval, maxval, 10)
                ret = runQuery(dif)
                diff = float(ret[-1][0])
                rec.append(diff)
            mpct = statistics.mean(rec)
#            print(str(mpct)+"%")
            res.append(mpct)
            resr.append((float(i)/maxval)*100)
#        print(res)
        allres.append(res)
    print(allres)
    print(len(resr))
    reswrite = ""
    for i, num in enumerate(resr):
        reswrite += (str(num))
        for j in range(len(uncert)):
            reswrite += "\t" + str(allres[j][i])
        reswrite += "\n"
    print(reswrite)
    subprocess.call(["mkdir", "results/microbench"])
    writetofile("rangeoverhead.csv",reswrite)
    plotmicrooverrange("rangeoverhead", 0, max(resr)*1.1, max(sum(allres, []))*1.1, "Max relative uncertain range (%)")
    subprocess.call(["mv", "rangeoverhead.csv","results/microbench/rangeoverhead.csv"])
    subprocess.call(["mv", "rangeoverhead.pdf","results/microbench/rangeoverhead.pdf"])
    
        
def getmetric(tbn, fig = False):
    query = "select column_name from INFORMATION_SCHEMA.COLUMNS where table_name ='%s'"%(tbn)
    ret = runQuery(query);
    schema = [i[0] for i in ret[:int((len(ret)-3)/3)]]
    query = "select data_type from INFORMATION_SCHEMA.COLUMNS where table_name ='%s'"%(tbn)
    ret = runQuery(query);
    datatype = [i[0] for i in ret[:int((len(ret)-3)/3)]]
    ns = ""
    rs = ""
    ars = ""
    for i,c in enumerate(schema):
        if ns == "":
            ns = c
            if datatype[i] == "integer" or datatype[i] == "float" or datatype[i] == "numeric" or datatype[i] == "bigint":
                rs = "abs(ub_%s::numeric-lb_%s::numeric) as r_%s"%(c,c,c)
                ars = "abs(max(ub_%s::numeric) over ()-min(lb_%s::numeric) over ()) as ar_%s"%(c,c,c)
            else:
                rs = "abs(stringdist(ub_%s::varchar,lb_%s::varchar)) as r_%s"%(c,c,c)
                ars = "abs(stringdist(max(ub_%s::varchar) over (), min(lb_%s::varchar) over ())) as ar_%s"%(c,c,c)
        else:
            ns += ", %s"%(c)
            if datatype[i] == "integer" or datatype[i] == "float" or datatype[i] == "numeric" or datatype[i] == "bigint":
                rs += ", abs(ub_%s::numeric-lb_%s::numeric) as r_%s"%(c,c,c)
                ars += ", abs(max(ub_%s::numeric) over ()-min(lb_%s::numeric) over ()) as ar_%s"%(c,c,c)
            else:
                rs += ", abs(stringdist(ub_%s::varchar,lb_%s::varchar)) as r_%s"%(c,c,c)
                ars += ", abs(stringdist(max(ub_%s::varchar) over (), min(lb_%s::varchar) over ())) as ar_%s"%(c,c,c)
    meansq = "select %s, %s, %s, cet_r, bst_r, pos_r from %s"%(ns,rs,ars,tbn)
    rmetricexpr = ""
    for c in schema:
        if rmetricexpr == "":
            rmetricexpr = "(r_%s/ar_%s"%(c,c)
        else:
            rmetricexpr += "+r_%s/ar_%s"%(c,c)
    rmetricexpr += ")/%d as column_metric"%(len(schema))
    amq = "select %s, cet_r, bst_r, pos_r from (%s) x "%(rmetricexpr, meansq)
    if not fig:
#        fq = "select sum(column_metric*cet_r) as sval, sum(cet_r) from (%s) z;"%(amq)
        fq = "select cval/cct as c_metric, cct, pval/pct as p_metric, pct from (select sum(column_metric*cet_r) as cval, sum(column_metric*pos_r) as pval, sum(cet_r) as cct, sum(pos_r) as pct from (%s) z) x;"%(amq)
#    fq = "select sum(cet_r) filter (where column_metric < 0.25) as Q1, sum(cet_r) filter (where column_metric >= 0.25 and column_metric < 0.5) as Q2, sum(cet_r) filter (where column_metric >= 0.5 and column_metric < 0.75) as Q3, sum(cet_r) filter (where column_metric >= 0.75) as Q4, sum(pos_r) filter (where column_metric < 0.25) as Q1_P, sum(pos_r) filter (where column_metric >= 0.25 and column_metric < 0.5) as Q2_P, sum(pos_r) filter (where column_metric >= 0.5 and column_metric < 0.75) as Q3_P, sum(pos_r) filter (where column_metric >= 0.75) as Q4_P from (%s) z;"%(amq)
        res = runQuery(fq)[0]
#        print(fq)
        ret = res
#    print(ret)
        return ret
    return
    
def xdbtoradb(tbn):
    query = "select column_name from INFORMATION_SCHEMA.COLUMNS where table_name ='%s'"%(tbn)
    ret = runQuery(query);
    schema = [i[0] for i in ret]
    print(schema)
    query = "select id, "
    bds = ""
    for cl in schema:
        if cl != 'id':
            query += "%s, "%(cl)
            bds += "max(%s) OVER (PARTITION BY id) as ub_%s,min(%s) OVER (PARTITION BY id) as lb_%s,"%(cl,cl,cl,cl)
    query += "%s 1 as cet_r,1 as bst_r,1 as pos_r from %s"%(bds, tbn)
    query = "create table %s_r as select DISTINCT ON (id) * FROM (%s) x;"%(tbn, query)
    print(query)
    pushQuery("drop table if exists %s_r;"%(tbn))
    pushQuery("drop table if exists %s_radb;"%(tbn))
    pushQuery(query)
    pushQuery("ALTER TABLE %s_r DROP COLUMN id;"%(tbn))
    pushQuery("create table %s_radb as select * from %s_r;"%(tbn,tbn))
    pushQuery("drop table if exists %s_r;"%(tbn))
    #save xdb inserts
#    tup = ""
#    for cl in schema:
#        if tup=="":
#            tup = "'('''||%s||''''"%(cl)
#        else:
#            tup += ("||','''||%s||''''"%(cl))
#    tup += "||')'"
#    query = "select 'insert into %s values ['||STRING_AGG(%s,'|')||'];' from %s group by id"%(tbn,tup,tbn)
#    print(query)
#    stdoutquery(query, "trio/%s.triql"%(tbn))

def getschema(tbn):
    query = "select column_name from INFORMATION_SCHEMA.COLUMNS where table_name ='%s'"%(tbn)
    ret = runQuery(query);
    return [i[0] for i in ret]

def getMCDB(tbn, outname):
    sch = getschema(tbn)
    sch.remove('id')
    str = ','.join(sch)
    print(sch)
    query = "select distinct on (id) id, %s from %s order by id, random()"%(str, tbn)
    materializequery(query, outname)
    print(query)
    
    

# ==================================================================
# ==================================================================
# ==================================================================
# ==================================================================
# ============================= MAIN ===============================
# ==================================================================
# ==================================================================
# ==================================================================
# ==================================================================

if __name__ == '__main__':
    dir = os.path.dirname(os.path.abspath(__file__))
    print(dir)
    subprocess.call(["mkdir", "results"])
    
#    start postgres server
    print("start server")
    os.system('sudo -u postgres /usr/lib/postgresql/9.5/bin/pg_ctl -D /postgresdata stop')
    os.system('sudo -u postgres /maybms/install/bin/pg_ctl -D /maybms/data stop')
    os.system('sudo -u postgres /usr/lib/postgresql/9.5/bin/pg_ctl -o "-p 5432" -D /postgresdata start')
    os.system('sudo -u postgres /maybms/install/bin/pg_ctl -o "-p 5433" -D /maybms/data start')
#
    
#    time.sleep(10)
    print("server started")
    
    
    #parse arguments
    helpmsg  = "RADB testing main script. By default the script will run all default test and if interrupted continue running from last experiment without repeating previous experiments."
    
    parser = argparse.ArgumentParser(description=helpmsg)
    parser.add_argument("-r", "--redo", help="Start the script from begining discrd all progress.", action="store_true")
    parser.add_argument("-s", "--step", help="Specify a single step to execute.")
    parser.add_argument("-g", "--gen", help="Generate and load all data.", action="store_true")
    parser.add_argument("-f", "--filesql", help="Time a specific sql from file. [filename, x, s, affix].")
    parser.add_argument("-t", "--trio", help="Generate trio [tablename].")
    parser.add_argument("-p", "--plot", help="Plot all graphs.", action="store_true")
    parser.add_argument("-m", "--metrics", help="Show metrics of given table. [table name]")
    parser.add_argument("-mq", "--metricquery", help="Show metrics of given table. [file name]")
    parser.add_argument("-gs", "--samplegen", help="generate a sample instance.", action="store_true")
    parser.add_argument("-gm", "--samplemetric", help="generate a sample instance.", action="store_true")
    
    args = parser.parse_args()
    singlestep = -1
    if args.step:
        singlestep = int(args.step)
    if args.redo:
        config.stepsetconfig(curstep=1)
    if args.samplegen:
        genDir = list()
        subprocess.call(["make","clean"],cwd='%s/dbgen'%dir)
        subprocess.call(["make"],cwd='%s/dbgen'%dir)
        #    call dbgen
        os.chdir('%s/dbgen'%dir)
        os.system('./dbgen -x 0.02 -s 0.1')
        os.chdir(dir)
        dirname = 's%d_x%d'%(0.1*100,0.02*100)
        os.system('mkdir %s'%dirname)
        os.system('mv dbgen/*.tbl %s'%dirname)
        importPdbenchTables(dirname,0.02,0.1)
        exittest()
    if args.samplemetric:
        importmicrotable(2, 5000, 20, 0.05, 1, 100)
        exittest()
    if args.plot:
        print("[Plotting only]")
#        plotPDbenchUncert("results/uncert/pdbench_uncert", 100,"Runtime / Det-runtime")
#        plotPDbenchScale("results/scale/pdbench_scale", 100,"Runtime / Det-runtime")
#        plotPDbenchUncert("results/uncert/uncert_Q0", 999999)
#        plotPDbenchUncert("results/uncert/uncert_Q1", 999999)
#        plotPDbenchUncert("results/uncert/uncert_Q2", 999999)
#        plotPDbenchScale("results/scale/scale_Q0", 999999)
#        plotPDbenchScale("results/scale/scale_Q1", 999999)
#        plotPDbenchScale("results/scale/scale_Q2", 999999)
#        plotmicroAggCompression("results/microbench/compress",16, 80, -5)
#        plotmicroAggCompressionMetric("results/microbench/compress_metrics",16, 55000000, -1000000)
#        plotmicro("results/microbench/groupby",92, 7, "# groupby attributts")
#        plotmicro("results/microbench/aggregation",92, 1.2, "# aggregation functions")
#        plotmicroRange("results/microbench/range",1, 35000, "Attribute bound size / Domain size", -1)
#        plotmicrojoin("results/microbench/join", 21000, 1000000)
#        plotmicrojoinMetrics("results/microbench/join_metrics", 21000, 260000000, -10000000)
#        plotmicroAggCompressionSQ("results/microbench/compress", "results/microbench/compress_metrics", 16, 80, 55000000, -5, -3000000)
        exittest()
        
    if args.gen: #only generate and load datasets with nameing of tbn_x_s_representation
        print("[Generating data only]")
        pdbenchGenOnX(0.1,True)
        pdbenchGenOnS(0.02,True)
        exittest()
        
    if args.metrics:
        tnstr = str(args.metrics)
        print("[Caculating metrics for table %s]"%(tnstr))
        rt = getmetric(tnstr)
        print(rt)
        exittest()
        
    if args.metricquery:
        tnstr = str(args.metricquery)
        mtname = tnstr.split('/')[-1].split('.')[0]+"_r"
        acturalname = mtname+"a"
        print(mtname, acturalname)
        print("[Caculating metrics for query %s]"%(tnstr))
        with open(tnstr) as fp:
            query = fp.read()
#            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_optimize_agg", "FALSE","-range_optimize_join", "FALSE", "-Pexecutor", "sql", "-query"]
#            rquery = getAUDBQueryFromGProM(query, gpromcmd)
#            materializequery(rquery, acturalname)
            gpromcmd = [str("gprom"), "-host", "127.0.0.1", "-db", "postgres", "-port", "%s"%pgport, "-user", "postgres", "-passwd", "postgres", "-loglevel", "0", "-backend", "postgres", "-Omerge_unsafe_proj", "TRUE", "-Oremove_unnecessary_columns", "FALSE", "-Oselection_move_around", "FALSE", "-heuristic_opt", "TRUE", "-Cschema_consistency", "FALSE", "-range_compression_rate", "14", "-Pexecutor", "sql", "-query"]
            rquery = getAUDBQueryFromGProM(query, gpromcmd)
            materializequery(rquery, mtname)
#            rt1 = runQuery("select sum(cet_r) from %s;"%(acturalname))
#            print(rt1)
            rt2 = runQuery("select sum(cet_r) from %s;"%(mtname))
            print(rt2)
#            rt3 = runQuery("select sum(bst_r) from %s;"%(acturalname))
#            print(rt3)
            rt4 = runQuery("select sum(pos_r) from %s;"%(mtname))
            print(rt4)
#            getmetric(mtname)
#            at,mt = timeQueryMult(rquery)
#            print(mt)
        exittest()
        
    if args.filesql:
        fnstr = str(args.filesql)
        flist = fnstr.split(",")
        if len(flist) == 1:
            allt,mt = timeQueryFileDef(flist[0])
            print("time single query: \n %s"%(flist[0]))
            print(mt)
        elif len(flist) == 4:
            allt,mt = timeQueryFile(flist[0], targ = flist[3], x = float(flist[1]), s = float(flist[2]))
            print("[time single file]: \n%s"%(flist[0]))
            print(mt)
        else:
            print("[Time single file] Argument wrong.")
        exittest()
    if args.trio:
        tn = str(args.trio)
        xdbtoradb(tn)
#            importtrio(datadir, s, x)
        exittest()
        
    curs = config.stepconfig()
    
    if (curs==1 and (singlestep == 2 or singlestep == -1)) or singlestep == 1:
        pdbenchGenOnX(0.1)#gen pdbench uncert
        pdbenchGenOnS(0.02)#gen pdbench scale.
        curs += 1
        config.stepsetconfig(curs)
        if singlestep==1:
            exittest()
    else:
        print("By passing pdbench gen")

#    with open('pdQuery/Q2.sql') as fp:
#        q = fp.read()
#        print(q)
#        print("conventional: %d"%(timeQuery(q)))
#        totaltime = 0
#        for i in range(0,mcdbRep):
#            totaltime += float(timeQuery(q))
#        print("mcdb: %f"%(totaltime))
#    for xval in x:
#        with open('pdQuery/Q2_uadb.sql') as fp:
#            q = fp.read().replace("_uadb", "_%d_100_uadb"%(xval*100))
##            print(q)
#            print("uadb[%d]: %s"%(xval*100,timeQuery(q)))
#        with open('pdQuery/Q2_radb.sql') as fp:
#            q = fp.read().replace("_radb", "_%d_100_radb"%(xval*100))
##            print(q)
#            print("radb[%d]: %s"%(xval*100,timeQuery(q)))
#    with open('pdQuery/Q2_cert.sql') as fp:
#        q = fp.read().replace("_cert", "_2_100_cert")
##        print(q)
#        print("cert: %s"%(timeQuery(q)))
    
    
#    importPdbenchTables(test1[0])
        
    if curs == 2 and singlestep == -1 or singlestep == 2:
        pdbenchGenOnX(0.1)
        test_pdbench_uncert()
        if(singlestep == -1):
            curs += 1
        else:
            exittest()
        config.stepsetconfig(curs)
    else:
        print("By passing PDbench uncert test")
#
    if curs == 3 and singlestep == -1 or singlestep == 3:
        pdbenchGenOnS()
        test_pdbench_scale()
        if(singlestep == -1):
            curs += 1
        else:
            exittest()
        config.stepsetconfig(curs)
    else:
        print("By Passing PDbench scale test")
        
    if curs ==4 and singlestep == -1 or singlestep == 4:
        microbenchmark()
        if(singlestep == -1):
            curs += 1
        else:
            exittest()
        config.stepsetconfig(curs)
    else:
           print("By Passing microbench test")
        
    if curs ==5 and singlestep == -1 or singlestep == 5:
        printtriotest("table_init_sql/trio/trio/trio", 10, 18, -0.5)
        if(singlestep == -1):
            curs += 1
        else:
            exittest()
        config.stepsetconfig(curs)
    else:
           print("By Passing trio result")
           
    if curs ==6 and singlestep == -1 or singlestep == 6:
        for i in range (10):
            getMCDB("real_tax","MCDB_%d"%(i+1))
        if(singlestep == -1):
            curs += 1
        else:
            exittest()
        config.stepsetconfig(curs)
    else:
        print("By Passing real query result")
    exittest()
#    subprocess.call(["/usr/lib/postgresql/9.5/bin/pg_ctl", "-D", "/postgresdata", "stop"])
    
