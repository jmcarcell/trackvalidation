import sys
import os
import json

def main(argv):
  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #Test
  test = run_conf["Test Mode"]

  FILENAMESIN  = run_conf["Input files"]
  PATHFILESOUT = run_conf["Output folder"]
  FILENAMESOUT = run_conf["Plot specs"]
  
  for group_folder_name in PATHFILESOUT:
    if not os.path.exists(str(group_folder_name)):
      os.makedirs(str(group_folder_name))

  #check if input file exists already
  for group_file_name in FILENAMESIN:
    for filein in group_file_name:
      isthere = os.path.isfile(filein)
      if not isthere :
        print("> Input file (%s) does NOT exist."%(filein))
        sys.exit()
      else :
        if bool(test) : print("> Input file (%s) exist."%(filein))
    
  particle_list = ["muon","ele","pion","ttbar3TeV","ttbar380GeV"]

  for particle, group_file_name, path_out, histo_out, features in zip(run_conf["Histo types"], FILENAMESIN, PATHFILESOUT, FILENAMESOUT, run_conf["Group features"]):
    if particle not in particle_list:
      print("> %s not in the list of particles! Possible choices are: %s"%(particle,', '.join(particle_list)))
      continue

    #efficiency
    listFilein = ""
    for filein in group_file_name:
      listFilein += filein + " "

    histo_name = ""
    titleAxis = ""
    rangeXaxis = ""
    logXaxis = ""
    rangeYaxis = ""
    logYaxis = ""
    sub_histo = histo_out.split("|")
    for var in sub_histo:
      list_sub_histo = var.split(":")
      histo_name += list_sub_histo[0] + " "
      if list_sub_histo[1] == "x":
        rangeXaxis += list_sub_histo[3] + ":" + list_sub_histo[4] + " "
        logXaxis += list_sub_histo[5] + " "
      if list_sub_histo[6] == "y":
        rangeYaxis += list_sub_histo[8] + ":" + list_sub_histo[9] + " "
        logYaxis += list_sub_histo[10] + " "
      titleAxis += "\";" + list_sub_histo[2] + ";" + list_sub_histo[7] + "\" "

    command_plot = "python plotting.py --filesin %s --folderout %s --histonames %s --varAxes %s --sample %s --features %s"%(listFilein,path_out,histo_name,titleAxis,particle,features)
    command_plot += " --logXaxis %s --rangeXaxis %s "%(logXaxis,rangeXaxis)
    command_plot += " --logYaxis %s --rangeYaxis %s"%(logYaxis,rangeYaxis)
    if bool(test) : 
      command_plot += " --verbose "
      print(command_plot)
    os.system(command_plot)

if __name__ == "__main__":
  if len(sys.argv[1:]) == 0:
    print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
    sys.exit()

  main(sys.argv[1:])

