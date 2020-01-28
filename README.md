# Tracking Validation

Collect script and macros to validate tracking for new releases.

In the case `REC` files are already present, then you can just run the validation of the tracking on them.

The validation of the tracking is run using the module `ClicEfficiencyCalculator` in `$ILCSOFT/ClicPerformance/HEAD/Tracking`.
This can be run again including only this module in `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml` and running directly on `REC` files.

If you want to run this on the grid:
- Change the location of the modified clicReconstruction.xml file in the script `jobs_submission/submit_ttbar_onlyClicEffCalculator.py` 
- Set the sample number and the other variables accordingly
- Run it with ` python jobs_submission/submit_ttbar_onlyClicEffCalculator.py `. The script will create one job per `REC` file.

You can use the same script w/o and w/overlay sample.

Currently, the following macros can be used to produce efficiency, fakerate and duplicates summary:
- `macros/plot_duplicatesXXX.C`
- `macros/plot_efficiencyXXX.C`
- `macros/plot_fakerateXXX.C`

`_singleParticle` can be chosen in case of muons, electrons, and pions.

`ComplexEvents` can be chosen in case of ttbar with and without overlay.

---

In the case `REC` files are NOT already present, follow the procedure to simulate and reconstruct the validation sample:

## Simulation of the validation sample 

For basic validation, both muons and ttbar are used.

The SIM files for ttbar are usually produced centrally on the grid.

The SIM files for muons must be simulated with the script in `$ILCSoft/ClicPerformance/HEAD/clicConfig/clic_steer.py`

### Local
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
### On the Grid
Get the `ClicPerformance` repository locally. Copy the steer files in a new folder called `steeringFiles` in the `clicConfig`.
Modify the location in the scripts in `jobs_submission`
```
cp $ILCSoft/ClicPerformance/HEAD/clicConfig/clic_steer.py /local/path/CLICPerformance/clicConfig/steeringFiles/
cp $ILCSoft/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml /local/path/CLICPerformance/clicConfig/steeringFiles/
#Submit muon gun with theta fixed to 89 deg
python submit_muons_gps_theta.py 89
#Submit muon gun with energy fixed to 10 GeV
python submit_muons_gun_energy.py 10
```

In case of fixed pT (and not energy) the `lcio_particle_gun.py` is used:
```
#Produce slcio file from sh/lcio_particle_gun with sh/run_lcio_particle_gunXX.sh. It needs nEntries, pT, particleType as input
#Example: 10000 events of 1GeV muons
python submit_allSingleParticles_slcio_fixedPt.py 10000 1 11
#Produce ddsim from slcio. It needs nEntries per file, pT, particleType as input
python submit_allSingleParticles_ddsim_fixedPt.py 10000 1 11
#Produce validation files from ddsim. It needs nEntries per jobs, nJobs, pT, particleType as input
python submit_allSingleParticles_reco_fixedPt.py 100 200 1 11
``` 

Nota Bene: Remember to set up the Dirac environment!

## Reconstruction of the validation sample 

The `SIM` slcio file produced in the previous step is given as input in `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml` and the tracking is run using the `ConformalTracking` processor

```
Marlin $ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml --global.LCIOInputFiles fileInput.slcio
```


