########################################
# Derived from uadb-reproducibility using same environment
########################################
FROM iitdbgroup/uadbreproduce
MAINTAINER Boris Glavic <bglavic@iit.edu>
########################################
RUN git clone https://github.com/fengsu91/AUDB_Reproducibility /reproducibility_audb
########################################
RUN rm -r /reproducibility
ADD ./data.zip /reproducibility_audb/data.zip
RUN cd /reproducibility_audb \
     && unzip data.zip \
     && rm data.zip
########################################
# Setup workdin
########################################
WORKDIR /reproducibility_audb
########################################
# Per default start the gprom command line interface
########################################
#ENTRYPOINT ["gprom"]
#ENTRYPOINT ["/reproducibility_audb/gen.py"]
ENTRYPOINT ["/bin/bash"]
########################################
# Per default show help message
########################################
#CMD [""]
