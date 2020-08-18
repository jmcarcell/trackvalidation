# Production of the validation sample

In general, three different steps are needed to obtain the tracking validation ntuples:
- SIM: sample simulation
- REC: event reconstruction, full or only track reconstruction and fit
- VAL: tracking validator `$ILCSOFT/ClicPerformance/HEAD/Tracking/ClicEfficiencyCalculator` which produced comprehensive set of ntuples

In the specific case of the production of single particle at fixed pt, also the DDSIM step is required.

The package was tested with single particles (muon, electrons and pions) and complex events (ttbar without and with overlay).

Scripts and automatic submission were created in the context of the ILCDirac grid.
There is also the possibility of running the validation locally - the instructions are in "Not on the grid, local test".

## Setup environment

First set up the environment
```
sh setup.sh

#Set up Dirac environment
source /cvmfs/clicdp.cern.ch/DIRAC/bashrc
dirac-proxy-init
```

## Automatic script

Modify the config file `../cfg/run_script_job_submit.json` accordingly. 

These are the input parameters concerning the job submission:
- "Release date": release used to produce the sample
- "Detector model": detector model used to produce the sample 
- "Particles": list of particles to produce the sample
- "Variables": produce sample at specific theta, energy and pT (single particles only!) 
- "Input/type sample ttbar": number and type of sample used as input (ttbar only!). To search for it, you can use: 
```
#search ing in the path with meta data
dirac-ilc-find-in-FC -D /ilc/prod/clic/ Energy=3000 Datatype=SIM

#tells you everything about that prodID
dirac-ilc-get-info -p #ProdId
```
- "NJobs": number of jobs submitted
- "NEvents per job": number of events per jobs
- "Jobs output folder": name used for the jobs output folder in eos
- "Jobs customised lib": if not empty, it is used to set a specific customised Marlin library as input
- "Sample fixed pt SIM folder": folder that contains the SIM-DDSIM sample (used only to create sample with fixed pT)
- "Test Mode": if set to 1, more printout are added and only 2 events are submit with 3 events 

As a first step the vanilla xml files of the specific release are copied in `local_files`. 
Modify them similarly to the ones in `local_files/templates/`:
- `local_files/templates/clicReconstruction.xml` to run only the tracking step in the reconstruction
- `local_files/templates/clicReconstruction_onlyValidator.xml` to run only the tracking validator
- `local_files/templates/clicReconstruction_overlay.xml` to include the overlay in the reconstruction

Not all particles require all filed. In case these files are needed but not present in `local_files` folder, the automatic script will ask to include them.

After setting the config file correctly, run:
```
python script_submit.py ../cfg/run_script_job_submit.json
```

According to the particle type and the variables listed in the cfg file, the steps run on the grid are:
- In the case of single particle, the script runs automatically SIM-REC-VAL steps for 10, 30 and 89 deg in theta and 1, 10, 100 GeV in energy.
- In the case of single particle, the script must be called three time to run sim/ddsim/reco steps [SIM-DDSIM-REC-VAL] steps for 1, 10, 100 GeV in transverse momentum.
- In the case of ttbar sample, the automatic scripts run REC-VAL or only VAL steps depending on the configuration. In both cases the input slcio sample number is needed. If the name of the sample contains "ove", then the overlay will also be included in the reconstruction.

## Standalone scripts

- `submit_allSingleParticles_gps_theta.py` run SIM-REC-VAL steps of single particle at a certain theta angle [deg]
- `submit_allSingleParticles_gun_energy.py` run SIM-REC-VAL steps of single particle at a certain energy [GeV]
- `submit_ttbar_fromSIM.py` run REC-VAL steps for ttbar events (number of SIM sample in input is needed!)
- `submit_ttbar_fromREC.py` run VAL step for ttbar events (number of REC sample in input is needed!)

When running one of them standalone, you need to give the following arguments:
```
python submit_XXX.py sampleName energy/theta/sampleNum release detector nJobs nEv nameTag [path to customised Marlin library]
```

In case of fixed pt (and not energy) one step more is needed: SIM-DDSIM-RECO-VAL.
The following scripts can be used:
- `submit_allSingleParticles_slcio_fixedPt.py` produce SIM files (10'000 events are always created!)
- `submit_allSingleParticles_ddsim_fixedPt.py` produce DDSIM files ("NJobs" referres to the number of input SIM files. 1'000 events are always run per jobs. Therefore, the real number of jobs sumitted will be NJobs\*10!)
- `submit_allSingleParticles_reco_fixedPt.py` run REC-VAL (same as above)
In the first one, the `sh/lcio_particle_gun.py` and `sh/run_lcio_particle_gun_\*_\*GeV.sh` scripts are then called.

## Not on the grid, local test
Produce SIM files locally:
```
#Sourcing the ILCSoft release 20XX_XX_XX
/cvmfs/clicdp.cern.ch/iLCSoft/builds/20XX_XX_XX/x86_64-slc6-gcc62-opt/init_ilcsoft.sh

#Generating 10 events of 10 GeV muons with the CLICdet oX_vXX
ddsim --steeringFile $ILCSOFT/ClicPerformance/HEAD/clicConfig/clic_steer.py --compactFile $ILCSOFT/lcgeo/HEAD/CLIC/compact/CLIC_oX_vXX/CLIC_oX_vXX.xml --enableGun --gun.particle mu- --gun.energy 10*GeV --gun.distribution uniform --outputFile output.slcio --numberOfEvents 10

#Open event display with the CLICdet oX_vXX for double-checking
ced2go -d $ILCSOFT/ClicPerformance/HEAD/Visualisation/CLIC_oX_vXX_CED/CLIC_oX_vXX_CED.xml output.slcio
#-s 1 will show the correct surfaces as well
#-t $ILCSOFT/ClicPerformance/HEAD/Visualisation/CLIC_o3_v06_CED/ced2go-template-DD4.xml will provide the correct template
```

REC-VAL steps are run using `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml`.
The `SIM` slcio file must be given in input and the tracking is run using the `ConformalTracking` processor.
```
Marlin $ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml --global.LCIOInputFiles=fileInput.slcio
```
