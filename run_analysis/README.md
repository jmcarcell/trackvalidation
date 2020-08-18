# Analysis of the ntuple and production of the local root histo

In this part, the ntuple produced by the validator are analysed in terms of efficiency, fakerate, ...

The package was tested with single particles (muon, electrons and pions) and complex events (ttbar without and with overlay).

## Setup environment

To run the scripts in this folder you will need ROOT.

# Automatic script

Modify the config file `../cfg/run_script_analysis.json` accordingly.

These are the input parameters concerning the analysis:
- "Particles": list of particles to produce the sample
- "Ntuple folder": folder which contains the tracking validator ntuples
- For each file one want to analyse, one should specify "Ntuple types", "Ntuple names" and "Histo names" accordingly
- "Test Mode": if set to 1, more printout are added

After setting the config file correctly, run:
```
python script_analysis.py ../cfg/run_script_analysis.json
```

## Standalone scripts

The analysis can also be run standalone.
Use the following command to read the input parameters needed:
```
python analysis.py --help
```
