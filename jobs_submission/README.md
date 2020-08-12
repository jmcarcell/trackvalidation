# Production of the validation sample

In general, three different steps are needed to obtain the tracking validation ntuples:
- SIM: sample simulation
- REC: event reconstruction, full or only track reconstruction and fit
- VAL: tracking validator `$ILCSOFT/ClicPerformance/HEAD/Tracking/ClicEfficiencyCalculator` which produced comprehensive set of ntuples

The package was tested with single particles (muon, electrons and pions) and complex events (ttbar without and with overlay).

Scripts and automatic submission were created in the context of the ILCDirac grid.
There is also the possibility of running the validation locally - the instructions are in Local.

## Setup environment

First set up the environment
```
sh setup.sh

#Set up Dirac environment
source /cvmfs/clicdp.cern.ch/DIRAC/bashrc
dirac-proxy-init
```

## Automatic script

Modify the config file `../cfg/run_submit_jobs.json` accordingly. 
These are the input parameters concerning the job submission:
- "Release date": release used to produce the sample
- "Detector model": detector model used to produce the sample 
- "Particles": list of particles to produce the sample
- "Variables": produce sample at specific theta, energy and pT (single particles only!) 
- "Input/type sample ttbar": number and type of sample used as input (ttbar only!)
- "NJobs": number of jobs submitted
- "NEvents per job": number of events per jobs
- "Jobs_output_folder": name used for the jobs output folder in eos
- "Jobs_customised_lib": if not empty, it is used to set a specific customised Marlin library as input

After set the config file correctly, one can run:
```
python submit.py ../cfg/cfg_file.json
```
According to the particle type and the variables listed in the cfg file, the steps needed to obtain the final ntuples will be run on the grid.

In the case of single particle, the automatic scripts runs SIM-REC-VAL steps for 10,30 and 89 deg in theta
and 1,10,100 GeV in energy.

In the case of ttbar sample, the automatic scripts run REC-VAL or only VAL steps depending on the configuration. 
In both cases the input slcio sample number is needed.
If the name of the sample contains "ove", then the overlay will also be included in the reconstruction.

## Standalone scripts

- `submit_gps_theta.py` run SIM-REC-VAL steps of single particle at a certain theta angle [deg]
- `submit_gun_energy.py` run SIM-REC-VAL steps of single particle at a certain energy [GeV]
- `submit_ttbar_fromSIM.py` run REC-VAL steps for ttbar events (number of SIM sample in input is needed!)
- `submit_ttbar_fromREC.py` run VAL step for ttbar events (number of REC sample in input is needed!)

When running one of them standalone, you need to give the following arguments:
```
python submit_XXX.py sampleName energy/theta/sampleNum release detector nJobs nEv nameTag [path to customised Marlin library]
```

## Local
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
