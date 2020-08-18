# Produce analysis plot

In this part, the root files produced by the `run_analysis` are plotted.

The package was tested with single particles (muon, electrons and pions) and complex events (ttbar without and with overlay).

## Setup environment

To run the scripts in this folder you will need ROOT.

# Automatic script

Modify the config file `../cfg/run_script_plotting.json` accordingly.

These are the input parameters concerning the plotting:
- "Input files": input files with the entire path. They are divided in groups which one wants to compare.
- "Group feature": list of features used for each input file as label:color:markerStyle 
- "Histo types": it is needed to set the header in the legend for each plot. It can be "muon,ele,pion,ttbar3TeV,ttbar380GeV".
- "Output folder": folder to store output plots
- "Plot specs": specific of the plot one want to produce as nameHisto:x:titleX:minX:maxX:logX:y:titleY:minY:maxY:logY
- "Test Mode": if set to 1, more printout are added

After setting the config file correctly, run:
```
python script_plotting.py ../cfg/run_script_plotting.json
```
The output plots will be in EPS and PNG format.

## Standalone scripts

The analysis can also be run standalone.
Use the following command to read the input parameters needed:
```
python plotting.py --help
```
The output plots will be in EPS and PNG format.

It is possible to use to other standalone scripts:
- `script_plot_muons.py` specific for muons
- `script_plot_ttbar.py` specific for ttbar
