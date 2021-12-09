########################################
# Derived from gprom travis build image which supports all backends
########################################
FROM iitdbgroup/gprom_travis
MAINTAINER Boris Glavic <bglavic@iit.edu>
########################################
# Install extra packages for experiments
########################################
RUN apt-get -y update && apt-get install -y \
    ghostscript \
    gnuplot \
    python3 \
    postgresql \
    python3-pip \
    sudo \
	&& rm -rf /var/lib/apt/lists/*
########################################
# Install python packages
########################################
RUN pip3 install pg8000 \
    && pip3 install numpy \
    && pip3 install psycopg2
########################################
# Clone GProM github repository
########################################
RUN git clone --single-branch --branch CPB https://github.com/IITDBGroup/gprom.git /gprom/src
########################################
# Build GProM
########################################
RUN cd /gprom/src \
	&& make -f Makefile.maintainer \
	&& ./configure --with-oci-headers=/usr/local/oracle/include/oracle/12.2/client64/ --with-oci-lib=/usr/local/oracle/lib/oracle/12.2/client64/lib/ --disable-monetdb  \
	&& make \
    && make install
########################################
# Install MayBMS
########################################
RUN git clone https://gitlab.odin.cse.buffalo.edu/bglavic/MayBMS_Mirror.git /maybms/src
RUN cd /maybms/src/postgresql-8.3.3/ \
    && export CFLAGS='-O0' \
    && ./configure --prefix /maybms/install \
    && make -j 32 \
    && make install
########################################
# Create database cluster for MayBMS
########################################
RUN mkdir -p /maybms/data \
    && chown -R postgres /maybms/data \
    && sudo -u postgres /maybms/install/bin/initdb -D /maybms/data
########################################
# Create database cluster for MayBMS
########################################
RUN mkdir -p /postgresdata \
    && chown -R postgres /postgresdata/ \
    && sudo -u postgres /usr/lib/postgresql/9.5/bin/initdb -D /postgresdata
########################################
# Clone reproducibility repro
########################################
RUN git clone https://github.com/IITDBGroup/UADB_Reproducibility.git /reproducibility
########################################
# Add sqlite DB for experiments
########################################
RUN mkdir -p /reproducibility/dbs
ADD ./dbs.zip /reproducibility/dbs/dbs.zip
RUN cd /reproducibility/dbs \
     && unzip dbs.zip \
     && rm dbs.zip
########################################
# Setup workdin
########################################
WORKDIR /reproducibility
########################################
# Per default start the gprom command line interface
########################################
#ENTRYPOINT ["gprom"]
#ENTRYPOINT ["/reproducibility/gen.py"]
ENTRYPOINT ["/bin/bash"]
########################################
# Per default show help message
########################################
#CMD [""]
