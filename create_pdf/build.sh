PATHPLOTS="/home/ericabro/CLICstudies/2020/trackingvalidation_finalise/plots/clic_releases_validation/ILCSoft-2019-09-04/"
NEWFOLDER="plots/"
NEWFOLDER2="plots/"

#Copying files in the folder
sh copy_plots.sh $PATHPLOTS $NEWFOLDER

python personalise_pdf.py --filein "release_summary_template.tex" --fileout "release_summary.tex" --authorinstitute CERN --authorname "Erica Brondolin" --date 03-06-2020 --release "ILCSoft-2020-02-07" --path $NEWFOLDER --minHits_singlePart "3" --minHits_complex "4"
pdflatex release_summary.tex

python personalise_pdf.py --filein "release_comparison_template.tex" --fileout "release_comparison.tex" --authorinstitute CERN --authorname "Erica Brondolin" --date 03-06-2020 --release "ILCSoft-2019-09-04" --path $NEWFOLDER --minHits_singlePart "3" --minHits_complex "4" --release2 "ILCSoft-2020-02-07" --path2 $NEWFOLDER2
pdflatex release_comparison.tex

evince release_summary.pdf
evince release_comparison.pdf
