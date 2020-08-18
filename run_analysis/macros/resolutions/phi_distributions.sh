root -l -b <<EOF
.L phi_distributions.C
phi_distributions("/eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/resolutions/reco/merged_$1_$2$3.root","phi_resolution_$1.root","$1","$2","$3")
EOF