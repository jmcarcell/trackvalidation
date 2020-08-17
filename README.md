# Tracking Validation

Collect script and macros to validate [Conformal Tracking](https://github.com/iLCSoft/ConformalTracking) for new [ILCSoft releases](https://gitlab.cern.ch/CLICdp/SoftwareConfigurations/iLCSoft/-/releases).

## Package structure 

The validation is performed with four different steps:
1. Production of the validation sample
2. Analysis of the ntuple and production of the local root histo
3. Produce analysis plot (including comparison among samples)
4. Create summary pdf (and comparison with other releases)

The correspondent folder for each step is:
1. `job_submission`
2. `run_analysis`
3. `produce_plots`
4. `create_pdf`

Each folder contains the README to run the step.

Additional folders:
- `plots` contains the final plots for each release validated
- `cfg` contains configuration files which can be used across folders

## Old text from here:

Currently, the following macros can be used to produce efficiency, fakerate and duplicates summary in case of muons, electrons, and pions:
- `macros/plot_duplicates_singleParticle.C`
- `macros/plot_efficiency_singleParticle.C`
- `macros/plot_fakerate_singleParticle.C`

In case of more complex events, such as ttbar or bbar, the macros are called:
- `macros/plot_duplicatesComplexEvents.C`
- `macros/plot_efficiencyComplexEvents.C`
- `macros/plot_fakerateComplexEvents.C`


