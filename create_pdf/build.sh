PATHPLOTS="/home/ericabro/CLICstudies/2020/trackingvalidation_finalise/plots/clic_releases_validation/ILCSoft-2019-09-04/"
NEWFOLDER="plots/"

#Copying files in the folder
sh copy_plots.sh $PATHPLOTS $NEWFOLDER

python personalise_pdf.py --authorinstitute CERN --authorname "Erica Brondolin" --date 03-06-2020 --release "ILCSoft-2020-02-07" --path $NEWFOLDER --minHits_singlePart "3" --minHits_complex "4"
pdflatex release_summary.tex
evince release_summary.pdf
