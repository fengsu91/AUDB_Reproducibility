# SIGMOD Reproducibility for "Efficient Uncertainty Tracking for Complex Queries with Attribute-level Bounds"

## A) Source code info

The **GProM** system is written in `C`. For the experiments we did use  [PostgreSQL](https://www.postgresql.org/) as a backend for storage. TODO
GProM is available at [https://github.com/IITDBGroup/gprom](https://github.com/IITDBGroup/gprom). For the experiments, please use the `CPB` branch. GProM acts as a client for a relational database. AU-DB creation and querying is available through an extension of SQL.

- Repository: https://github.com/IITDBGroup/gprom
- Programming Language: C, Python
- **Compiler Info:** gcc-11.0.0
- **Required libraries/packages:**
  - postgresql
  - gnuplot (5.2)
  - GPRoM dependencies
  - python3 (for automated testing scripts)
  - psycopg2
  - numpy


## B)  Datasets info

We used several open real world datasets in the experiments. See below for links. We cleaned these datasets by replacing null values with constants using missing value imputation as implemented in Mimir. In the output every row with at least one replaced value was marked as uncertain. The UA-DBs generated in this ways were used in the experiments. In addition we use two synthetic datasets. One if PDBench (TODO link), a probabilistic version of the TPC-H datasets and ...
- **Real world datasets:**

| Dataset              | Cols | Rows    | URL                                                                                                        |
|----------------------|------|---------|------------------------------------------------------------------------------------------------------------|
| Netflix              | 12   | >6k     | https://www.kaggle.com/shivamb/netflix-shows                                                               |
| Healthcare           | 15   | >171k   | https://data.medicare.gov/data/hospital-compare                                                            |
| Chicago Crime        | 22   | >1.4M   | https://data.cityofchicago.org/Public-Safety/Crimes-2001-to-present/ijzp-q8t2                              |

- **Data generators:**
  - `PDBench` is a probabilistic version of the TPC-H data generator, we used a fork (fixing some compilation bugs) hosted here: https://github.com/IITDBGroup/pdbench
  - For microbenchmark data, we generate the data in the python testing script.

## C) Hardware Info

All runtime experiments were executed on a server with the following specs:

| Element          | Description                                                                   |
|------------------|-------------------------------------------------------------------------------|
| CPU              | 2 x AMD Opteron(tm) Processor 4238, 3.3Ghz                                    |
| Caches (per CPU) | L1 (288KiB), L2 (6 MiB), L3 (6MiB)                                            |
| Memory           | 128GB (DDR3 1333MHz)                                                          |
| RAID Controller  | LSI Logic / Symbios Logic MegaRAID SAS 2108 [Liberator] (rev 05), 512MB cache |
| RAID Config      | 4 x 1TB, configured as RAID 5                                                 |
| Disks            | 4 x 1TB 7.2K RPM Near-Line SAS 6Gbps (DELL CONSTELLATION ES.3)                |


## D) Installation and Setup

### Docker installation of the whole experiment setups

A docker hub is prepared with all requirements installed,

To install docker, please refer to the docker website:

(A account is required to download/use docker)

https://www.docker.com/get-started

After docker is installed, you can pull the docker image with:
~~~shell
docker pull iitdbgroup/audbreproduce
~~~
The image entry point is set to /bin/bash, so to load the docker image in command line:
~~~shell
docker run -ti iitdbgroup/audbreproduce
~~~
(Some systems may need sudo for docker operations)

The main test script is under the default entry folder (/reproducibility_audb).
All test results is saved to /results folder. To get the results in local system, mount a local system folder to the docker container result folder when load the image:
~~~shell
docker run -ti -v /your/local/directory:/reproducibility_audb/results iitdbgroup/audbreproduce 
~~~
# Run Experiments

We provide a unified python script file (gen.py) to run all experiments.
To run all default experiments:
~~~shell
 python3 gen.py
~~~
The schript will record which tests are done so when reruning the script it will not repeat finshed tests.
To start from beginning of the test, use command -r:
~~~shell
 python3 gen.py -r
~~~
The script also allowing executing only one experiment
for specifying a single experiment/step using -s:
~~~shell
-s command values:

1 - pdbench data generation on all test cases.

2 - perform pdbench test varying amount of uncertainty.

3 - perform pdbench test varying data size.

4 - perform micro benchmarks.

5 - perform trio test.

6 - perform realQuery test.
~~~

For example, to run only pdbench test by varying data size and regenerate data using pdbench:
~~~shell
 python3 gen.py -r -s 3
~~~

## Get experiment results
All experiment results will be save to /result folder in the docker image. If the folder is mounted to a local folder, the files will be accessable through the local folder.

### step by step installations (NOT required if use docker)

#### Install GProM

Please follow instruction in the GProM GitHub repository to install the system:

~~~shell
git clone git@github.com:IITDBGroup/gprom ...
~~~

#### Install PDBench

Our python script will make and generate PDbench data when needed by the experiments.
The data will be saved into a folder with name s[sval]x[xval] where sval is the scale factor and xval is the uncertainy factor.

#### Install Postgres + load database ####

By default the python testing script will connect to database 'postgres' with port 5432. Connection can be configured in the /config/config.ini file. 

# Suggestions and Instructions for Alternative Experiments

## PDBench experiments with different parameter settings

Different parameter settings need to modify the 's' list and 'x' list in the gen.py file where 's' is the list of all scale factors will be tested and 'x' is the list of all uncertainty factors will be tested. For lowering the testing load we reduced the default scale factor of data size for each test so each test can Run faster. To use large data size same as the paper, modify the scale factor list and the corresponding folder name list and set pdbench gen function input to 1 instead of 0.1.

## Running ad hoc queries through GProM

You can use the gprom system included in the container to run queries with AU-DB semantics over the provided datasets.

### GProM UA-DB Syntax

To run a query with range annotations, the whole query should be wrapped in:

~~~sql
urange (
    ...
);
~~~

Unless instructed otherwise, GProM expects inputs to be AU-DBs. However, GProM can also interpret different types of uncertain data models and translate them into UA-DBs as part of queries. To inform GProM that an input table should be interpreted as a certain type of uncertain relation, you specify the type after the table access in the `FROM` clause. Currently, we support tuples-independent probabilistic databases (TIPS) and x-relations.

#### TIPs

A TIP stores for each row it's marginal probability. The existence of tuples in the database are assumed to be mutually independent of each other. That is, the set of possible worlds represented by a TIP database are all subsets of the TIP database. To use a TIP table in GProM, the table should have an additional attribute storing the tuple probabilities and the table access in the `FROM` clause should be followed by `IS TIP (prob)` where `prob` is the name of the attribute storing tuple probabilities. For instance, consider this table `R` (attribute `p` stores probabilities):

```sql
| name  | age | p  |
|-------|-----|----|
| Peter | 34 | 0.9 |
| Alice | 19 | 0.6 |
| Bob   | 23 | 1.0 |
```

An example query over this table

~~~sql
urange (
  SELECT * FROM R IS TIP(p);
);
~~~

#### X-DBs

An X-table consists of x-tuples which are sets of tuples called alternatives, each associated with a probability. X-tables are a specific type of block-independent probabilistic databases. The alternatives of an x-tuple are disjoint events while alternatives from different x-tuples are independent events. GProM expects an X-tables to have two additional attributes: one that stores probabilities for alternatives and one that stores a unique identifier for each x-tuple. For instance, consider the following table where we are uncertain about Peter's age, and Alice may or may not be in the table.

```sql
| name  | age | x-id |  p  |
|-------|-----|------|----|
| Peter | 34 | 1     | 0.4 |
| Peter | 35 | 1     | 0.3 |
| Peter | 36 | 1     | 0.3 |
| Alice | 19 | 2     | 0.6 |
| Bob   | 23 | 3     | 1.0 |
```

To use a X-table in GProM you have to specify the names of the attributes storing probabilities and x-tuple identifiers. For instance, for the table above:

~~~sql
urange (
  SELECT * FROM R IS XTABLE(x-id,p);
);
~~~
