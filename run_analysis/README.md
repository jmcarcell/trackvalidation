# Analysis of the ntuple and production of the local root histo

In this part, the ntuple produced by the validator are analysed in terms of efficiency, fakerate and duplicates.
To obtain the track parameter resolution as a function of theta and pt, use the scripts in `macros/resolutions`.

The package was tested with single particles (muon, electrons and pions) and complex events (ttbar without and with overlay).

## Setup environment

To run the scripts in this folder you will need ROOT and Python 2.7.

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

## Merging the ntuples

To merge all ntuple, you can use
```
python script_merge.py ../cfg/run_script_analysis.json
```

To transfer them to the grid you have to set up the Dirac environment:
```
#Set up Dirac environment
source /cvmfs/clicdp.cern.ch/DIRAC/bashrc
dirac-proxy-init
```

and then use the specific Dirac command:
```
dirac-dms-add-file /ilc/user/i/initial/some/path/file.root file.root CERN-DST-EOS
```
