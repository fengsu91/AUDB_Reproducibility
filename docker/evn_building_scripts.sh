apt-get update
apt-get -y update
apt-get --no-install-recommends install -y ant
apt-get --no-install-recommends install -y autoconf
apt-get --no-install-recommends install -y automake
apt-get --no-install-recommends install -y autotools-dev
apt-get --no-install-recommends install -y bison
apt-get --no-install-recommends install -y build-essential:native
apt-get --no-install-recommends install -y bzip2
apt-get --no-install-recommends install -y curl
apt-get --no-install-recommends install -y dh-make
apt-get --no-install-recommends install -y fakeroot
apt-get --no-install-recommends install -y flex
apt-get --no-install-recommends install -y git
apt-get --no-install-recommends install -y graphviz
apt-get --no-install-recommends install -y libbz2-dev
apt-get --no-install-recommends install -y libcurl4-openssl-dev
apt-get --no-install-recommends install -y liblzma-dev
apt-get --no-install-recommends install -y libpq-dev
apt-get --no-install-recommends install -y libreadline6
apt-get --no-install-recommends install -y libreadline6-dev
apt-get --no-install-recommends install -y libsqlite3-dev
apt-get --no-install-recommends install -y libtool
apt-get --no-install-recommends install -y libzip-dev
apt-get --no-install-recommends install -y make
apt-get --no-install-recommends install -y man-db
apt-get --no-install-recommends install -y openssl
apt-get --no-install-recommends install -y pkg-config
apt-get --no-install-recommends install -y pandoc
apt-get --no-install-recommends install -y postgresql
apt-get --no-install-recommends install -y rman
apt-get --no-install-recommends install -y sshpass
apt-get --no-install-recommends install -y sqlite3
apt-get --no-install-recommends install -y sudo
apt-get --no-install-recommends install -y unzip
apt-get --no-install-recommends install -y vim
apt-get --no-install-recommends install -y zlib1g-dev
apt-get --no-install-recommends install -y wget
apt-get --no-install-recommends install -y ghostscript
apt-get --no-install-recommends install -y gnuplot
apt-get --no-install-recommends install -y python3
apt-get --no-install-recommends install -y python3-pip
apt-get --no-install-recommends install -y sudo
rm -rf /var/lib/apt/lists/*
pip3 install pg8000
pip3 install numpy
pip3 install psycopg2
pip3 install psycopg2-binary
git clone --single-branch --branch CPB https://github.com/IITDBGroup/gprom.git /gprom/src
cd /gprom/src
make -f Makefile.maintainer
./configure --disable-java --disable-monetdb --disable-oracle --with-libpq-headers=/usr/include/postgresql
make
make install
mkdir -p /postgresdata
chown -R postgres /postgresdata/
sudo -u postgres /usr/lib/postgresql/13/bin/initdb -D /postgresdata
cd /postgresdata
sed -i '113s/.*/shared_buffers = 10240MB/' postgresql.conf
sed -i '123s/.*/maintenance_work_mem = 10240MB/' postgresql.conf
git clone https://gitlab.odin.cse.buffalo.edu/bglavic/MayBMS_Mirror.git /maybms/src
cd /maybms/src/postgresql-8.3.3/
export CFLAGS='-O0'
./configure --prefix /maybms/install
make -j 32
make install
mkdir -p /maybms/data
chown -R postgres /maybms/data
sudo -u postgres /maybms/install/bin/initdb -D /maybms/data
cd /maybms/data
sed -i '107s/.*/shared_buffers = 10240MB/' postgresql.conf
sed -i '115s/.*/maintenance_work_mem = 10240MB/' postgresql.conf
git clone https://github.com/fengsu91/AUDB_Reproducibility /reproducibility_audb
cd /reproducibility_audb
python3 gen.py
