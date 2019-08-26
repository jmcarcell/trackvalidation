# Tracking Validation

Collect script and macros to validate tracking for new releases.

## Simulation of the validation sample 

For basic validation, both muons and ttbar are used.

The SIM files for ttbar are usually produced centrally on the grid

The SIM files for muons must be simulated with the script in `$ILCSoft/ClicPerformance/HEAD/clicConfig/clic_steer.py`

```
#Sourcing the ILCSoft release 20XX_XX_XX
/cvmfs/clicdp.cern.ch/iLCSoft/builds/20XX_XX_XX/x86_64-slc6-gcc62-opt/init_ilcsoft.sh

#Generating 10 events of 10 GeV muons with the CLICdet oX_vXX
ddsim --steeringFile $ILCSOFT/ClicPerformance/HEAD/clicConfig/clic_steer.py --compactFile $ILCSOFT/lcgeo/HEAD/CLIC/compact/CLIC_oX_vXX/CLIC_oX_vXX.xml --enableGun --gun.particle mu- --gun.energy 10*GeV --gun.distribution uniform --outputFile output.slcio --numberOfEvents 10

#Open event display with the CLICdet oX_vXX for double-checking 
ced2go -d $ILCSOFT/ClicPerformance/HEAD/Visualisation/CLIC_oX_vXX_CED/CLIC_oX_vXX_CED.xml output.slcio
```

## Reconstruction of the validation sample 

The `SIM` slcio file produced in the previous step is given as input in `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml` and the tracking is run using the `ConformalTracking` processor

```
Marlin $ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml --global.LCIOInputFiles fileInput.slcio
```

## Validation 

The validation of the tracking is run using the module `ClicEfficiencyCalculator` in `$ILCSOFT/ClicPerformance/HEAD/Tracking`.
This can be run again with `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml`.

