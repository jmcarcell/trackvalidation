import sys
import os
import json

def main(argv):
  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #Test
  test = run_conf["Test Mode"]

  PATHFILESIN  = run_conf["Input folder"]
  PATHFILESOUT = run_conf["Output folder"]
  
  if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)
  
  FILENAMESIN  = run_conf["Ntuple names"]
  FILENAMESOUT = run_conf["Histo names"]

  #check if input file exists already
  for file_name in FILENAMESIN:
    filein = PATHFILESIN + file_name
    isthere = os.path.isfile(filein)
    if not isthere :
      print("> Input file (%s) does NOT exist."%(filein))
      print("> To create it, you can use:")
      print("`python script_merge.py %s` or `hadd -f %s %s/*`"%(run_config_file, file_name, PATHFILESIN))
      sys.exit()
    else :
      if bool(test) : print("> Input file (%s) exist."%(filein))
    
  #check if output file exists already
  for file_name in FILENAMESOUT:
    fileout = PATHFILESOUT + file_name
    isthere = os.path.isfile(fileout)
    if isthere :
      recreate = raw_input("%s already exists. Do you want to recreate the file [y/n]?"%fileout)
      if recreate is "y":
        os.system("rm %s"%fileout)

  particle_list = ["muons", "electrons", "pions", "ttbar3TeV", "ttbar3TeV_ove"]

  for particle, file_name, fileout in zip(run_conf["Ntuple types"],FILENAMESIN,FILENAMESOUT):
    if particle not in particle_list:
      print("> %s not in the list of particles! Possible choices are: %s"%(particle,', '.join(particle_list)))
      continue
    filein = PATHFILESIN + file_name
    fileout = PATHFILESOUT + fileout
    #single particles
    if particle == "muons" or particle == "electrons" or particle == "pions": 

      if "deg" not in file_name and "GeV" not in file_name :
        print("> Input file does not contain \'deg\' or \'GeV\', not sure what to plot!")

      #efficiency
      ANALYSIS_FLAGS = "--efficiency"
      if "deg" in file_name :
        LISTVAR_FIXED_THETA    = "pt:100:-2:3:True phi:180:-180:180:False"
        CUTS_FIXED_THETA       = "vertexR:0:0.0001:nHitsMC:3:1000 vertexR:0:0.0001:nHitsMC:3:1000"
        command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR_FIXED_THETA,CUTS_FIXED_THETA,ANALYSIS_FLAGS)
        if bool(test) : 
          command_analysis += " --verbose "
          print(command_analysis)
        os.system(command_analysis)

      if "GeV" in file_name :
        LISTVAR_FIXED_PT       = "theta:83:7:90:False phi:180:-180:180:False"
        CUTS_FIXED_PT          = "vertexR:0:0.0001:nHitsMC:3:1000 vertexR:0:0.0001:nHitsMC:3:1000"
        command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR_FIXED_PT,CUTS_FIXED_PT,ANALYSIS_FLAGS)
        if bool(test) : 
          command_analysis += " --verbose "
          print(command_analysis)
        os.system(command_analysis)

      #MC comparison, fakerate and duplicates
      ANALYSIS_FLAGS = "--compareRecoSim --fakerate --duplicates"
      if "deg" in file_name :
        CUTS_FIXED_THETA       = "nHits:3:1000 nHits:3:1000"
        command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR_FIXED_THETA,CUTS_FIXED_THETA,ANALYSIS_FLAGS)
        if bool(test) : 
          command_analysis += " --verbose "
          print(command_analysis)
        os.system(command_analysis)

      if "GeV" in file_name :
        CUTS_FIXED_PT          = "nHits:3:1000 nHits:3:1000"
        command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR_FIXED_PT,CUTS_FIXED_PT,ANALYSIS_FLAGS)
        if bool(test) : 
          command_analysis += " --verbose "
          print(command_analysis)
        os.system(command_analysis)

    #complex events
    elif particle == "ttbar3TeV" or particle == "ttbar3TeV_ove" : 
      #efficiency
      ANALYSIS_FLAGS = "--efficiency"

      LISTVAR = "theta:83:7:90:False pt:100:-1:3:True phi:180:-180:180:False"
      CUTS    = "pt:1:10000:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000 theta:10:170:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000 theta:10:170:pt:1:10000:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000"
      
      command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR,CUTS,ANALYSIS_FLAGS)
      if bool(test) : 
        command_analysis += " --verbose "
        print(command_analysis)
      os.system(command_analysis)

      #comparison and fakerate
      ANALYSIS_FLAGS = "--compareRecoSim --fakerate --duplicates"
      LISTVAR = "theta:83:7:90:False pt:100:-1:3:True phi:180:-180:180:False"
      CUTS    = "pt:1:10000:nHits:4:1000 theta:10:170:nHits:4:1000 theta:10:170:pt:1:10000:nHits:4:1000"
      
      command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s %s"%(filein,fileout,LISTVAR,CUTS,ANALYSIS_FLAGS)
      if bool(test) : 
        command_analysis += " --verbose "
        print(command_analysis)
      os.system(command_analysis)

if __name__ == "__main__":
  if len(sys.argv[1:]) == 0:
    print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
    sys.exit()

  main(sys.argv[1:])

