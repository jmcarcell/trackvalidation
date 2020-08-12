# Tracking Validation

Collect script and macros to validate [Conformal Tracking](https://github.com/iLCSoft/ConformalTracking) for new releases.

## Package structure 

The validation is performed with four different steps:
1. Production of the validation sample
2. Analysis of the ntuple and production of the local root histo
3. Perform analysis plot (including comparison among samples)
4. Create summary pdf (and comparison with other releases)

The correspondent folders for each steps are:
1. `job_submission`
2. `produce_histos`
3. `produce_plots`
4. `create_pdf`
Each folder contains the README to run the step.

Additional folders:
- `plots` contains the final plots produced
- `cfg` contains configuration files

## Old text from here:

For basic validation, both muons and ttbar are used.

The tracking validation is run using the module `$ILCSOFT/ClicPerformance/HEAD/Tracking/ClicEfficiencyCalculator` on `REC` files.
This can be run again including only this module in `$ILCSOFT/ClicPerformance/HEAD/clicConfig/clicReconstruction.xml`.

Currently, the following macros can be used to produce efficiency, fakerate and duplicates summary in case of muons, electrons, and pions:
- `macros/plot_duplicates_singleParticle.C`
- `macros/plot_efficiency_singleParticle.C`
- `macros/plot_fakerate_singleParticle.C`

In case of more complex events, such as ttbar or bbar, the macros are called:
- `macros/plot_duplicatesComplexEvents.C`
- `macros/plot_efficiencyComplexEvents.C`
- `macros/plot_fakerateComplexEvents.C`

---

## Simulation of the validation sample 

### On the Grid

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




