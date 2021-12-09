########################################
# Derived from uadb-reproducibility using same environment
########################################
FROM iitdbgroup/uadbreproduce
MAINTAINER Boris Glavic <bglavic@iit.edu>
########################################
RUN git clone https://github.com/fengsu91/AUDB_Reproducibility /reproducibility_audb
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
