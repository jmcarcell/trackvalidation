import os
import json
import sys

def main(argv):
  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #set up environment
  set_root = raw_input("> Is ROOT set up correctly [y/n]?")
  if set_root is "n":
    print("> To setup root paste the following command in the terminal:")
    command_source = "source /cvmfs/clicdp.cern.ch/iLCSoft/builds/%s/x86_64-slc6-gcc62-opt/init_ilcsoft.sh"%run_conf["Release date"]
    print(command_source)

  PATH = run_conf["Input folder"]

  SINGLE_FOLD = ['files_ttbar', 'files_ttbar_ove',
                 'files_muons_10deg', 'files_muons_30deg', 'files_muons_89deg',
                 'files_muons_1GeV', 'files_muons_10GeV', 'files_muons_100GeV',
                 'files_muons_1GeV_fixedPt', 'files_muons_10GeV_fixedPt', 'files_muons_100GeV_fixedPt',
                 'files_electrons_10deg', 'files_electrons_30deg', 'files_electrons_89deg',
                 'files_electrons_1GeV', 'files_electrons_10GeV', 'files_electrons_100GeV',
                 'files_electrons_1GeV_fixedPt', 'files_electrons_10GeV_fixedPt', 'files_electrons_100GeV_fixedPt',
                 'files_pions_10deg', 'files_pions_30deg', 'files_pions_89deg',
                 'files_pions_1GeV', 'files_pions_10GeV', 'files_pions_100GeV',
                 'files_pions_1GeV_fixedPt', 'files_pions_10GeV_fixedPt', 'files_pions_100GeV_fixedPt']
  FILE_OUTPUT = ['merged_ttbar3TeV_NoOverlay_conformal.root', 'merged_ttbar3TeV_Overlay_conformal.root',
                 'merged_Pow1_muons_10deg.root','merged_Pow1_muons_30deg.root','merged_Pow1_muons_89deg.root',
                 'merged_muons_energy_1GeV.root','merged_muons_energy_10GeV.root','merged_muons_energy_100GeV.root',
                 'merged_reco_muons_pt_1GeV.root', 'merged_reco_muons_pt_10GeV.root', 'merged_reco_muons_pt_100GeV.root',
                 'merged_Pow1_electrons_10deg.root','merged_Pow1_electrons_30deg.root','merged_Pow1_electrons_89deg.root',
                 'merged_electrons_energy_1GeV.root','merged_electrons_energy_10GeV.root','merged_electrons_energy_100GeV.root',
                 'merged_reco_electrons_pt_1GeV.root', 'merged_reco_electrons_pt_10GeV.root', 'merged_reco_electrons_pt_100GeV.root',
                 'merged_Pow1_pions_10deg.root','merged_Pow1_pions_30deg.root','merged_Pow1_pions_89deg.root',
                 'merged_pions_energy_1GeV.root','merged_pions_energy_10GeV.root','merged_pions_energy_100GeV.root',
                 'merged_reco_pions_pt_1GeV.root', 'merged_reco_pions_pt_10GeV.root', 'merged_reco_pions_pt_100GeV.root']

  for single_fold,file_out in zip(SINGLE_FOLD,FILE_OUTPUT):
    folder_in = PATH+single_fold
    if os.path.exists(folder_in):
      print("%s exists"%folder_in)
    else:
      print("%s DOES NOT exists"%folder_in)
      continue
  
    if os.path.isfile(PATH+file_out):
      print("%s already exists."%(PATH+file_out))
      print("")
      continue
  
    print("> This folder contains: ")
    command_files = "ls -1 "+folder_in+" | wc -l"
    os.system(command_files)
  
    merging = raw_input("> Do you want to merge them [y/n]?")
    if merging is "y":
      log_out = file_out.replace(".root", ".log")
      #It can run on background but does not produce 'local' jobs
      command_merging = "hadd -f %s %s/* >& %s &"%(file_out,folder_in,log_out)
      print(command_merging)
      os.system(command_merging)
      print("> Outputfile can be found in %s and log in %s"%(file_out,log_out))
      print("> To copy the file on the grid:"
      print("> dirac-dms-add-file /ilc/user/i/initial/some/path/file.root file.root CERN-DST-EOS")
    else:
      print("> No merging was performed.")
    print("")

if __name__ == "__main__":
  if len(sys.argv[1:]) == 0:
    print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
    sys.exit()

  main(sys.argv[1:])
