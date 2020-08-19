# Tracking Validation

Collect script and macros to validate [Conformal Tracking](https://github.com/iLCSoft/ConformalTracking) for new [ILCSoft releases](https://gitlab.cern.ch/CLICdp/SoftwareConfigurations/iLCSoft/-/releases).

To run all packages, Python 2.7 is needed.

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

