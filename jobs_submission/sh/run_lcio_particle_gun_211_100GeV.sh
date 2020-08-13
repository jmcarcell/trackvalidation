source /cvmfs/clicdp.cern.ch/iLCSoft/builds/nightly/x86_64-slc6-gcc62-opt/init_ilcsoft.sh  

#python lcio_particle_gun.py nEvts pt pdg charge mass decayLength
python lcio_particle_gun.py 10000 100 211 -1 0.138 7.8e3
