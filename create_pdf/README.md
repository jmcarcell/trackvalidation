# Create summary pdf

In this part, the plots produced are included in a summary pdf.

## Setup environment

To run the scripts in this folder you will need LATEX.

# Automatic script

Modify the config file `../cfg/run_create_pdf.json` accordingly.

The inputs are:
- "Author name", "Author institute", "Date": used only as text in the pdf
- "Release date(2)": used only as text in the pdf 
- "Input folder(2)": contains plots used for the differerent releases
- "Comparison": if set to 1, it will produce also the comparison pdf
- "Min hits": validation plots end with \_minNhits#MINHITS#.eps
- "Test Mode": if set to 1, more printout are added

After setting the config file correctly, run:
```
python script_create.py ../cfg/cfg_file.json
```
The outputs are `release\_summary.pdf` and `release\_comparison.pdf`

For each release the pdf will search for the following folders:
- 'muons/'
- 'pions/'
- 'ttbar3TeV/'

And for each folder the following plots:
- 'eff\_vs\_VAR\_minNhits\*.eps'
- 'fake\_vs\_VAR\_minNhits\*.eps'
- 'dupl\_vs\_VAR\_minNhits\*.eps'
with VAR = theta, pt, phi 

## Standalone scripts

The pdf can also be created directly with a standalone python script.
Use the following command to read the input parameters needed:
```
python personalise_pdf.py --help
```
