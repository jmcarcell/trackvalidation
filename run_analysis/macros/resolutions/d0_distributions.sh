root -l -b <<EOF
.L d0_distributions.C
d0_distributions("/eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/resolutions/reco/merged_$1_$2$3.root","d0_resolution_$1.root","$1","$2","$3",$4)
EOF