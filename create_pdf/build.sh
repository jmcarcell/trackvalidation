SUBFOLDER="plots/"
PATHPLOTS="/home/ericabro/CLICstudies/2020/trackingvalidation_finalise/"$SUBFOLDER

#Copying files in a symbolic link (problem can occur if using eps files in LaTeX)
sh copy_plots.sh $PATHPLOTS $SUBFOLDER

RELEASE="ILCSoft-2019-09-04"
RELEASEFOLDER=$SUBFOLDER"/clic_releases_validation/"$RELEASE

python personalise_pdf.py --filein "release_summary_template.tex" --fileout "release_summary.tex" --authorinstitute CERN --authorname "Erica Brondolin" --date 03-06-2020 --release $RELEASE --path $RELEASEFOLDER --minHits_singlePart "3" --minHits_complex "4"
pdflatex release_summary.tex
evince release_summary.pdf

RELEASE2="ILCSoft-2019-09-04"
RELEASEFOLDER2="plots/clic_releases_validation/"$RELEASE2

python personalise_pdf.py --filein "release_comparison_template.tex" --fileout "release_comparison.tex" --authorinstitute CERN --authorname "Erica Brondolin" --date 03-06-2020 --release $RELEASE --path $RELEASEFOLDER --minHits_singlePart "3" --minHits_complex "4" --release2 $RELEASE2 --path2 $RELEASEFOLDER2
pdflatex release_comparison.tex
evince release_comparison.pdf
