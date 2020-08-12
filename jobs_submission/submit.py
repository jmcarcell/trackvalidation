import sys
import os
import json

def check_file_exist(file_name):
  isthere = os.path.isfile(file_name)
  if not isthere :
    print("> Input xml file (%s) does NOT exist."%(file_name))
    check_xml = raw_input("> Do you want to copy the xml locally? [yes/no] ") 
    if str(check_xml) == 'yes':
      cmd_cp = "cp -r $ILCSOFT/ClicPerformance/HEAD/clicConfig/clic* local_files/"
      os.system(cmd_cp)
      print(cmd_cp)
      print(">> Adapt the files to run only tracking or only validation! Example in local_files/templates/")
      sys.exit()
  return isthere

def main(argv):
  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #Test
  print(run_conf["Particles"])
  print(run_conf["Variables"])
  print(run_conf["Jobs_customised_lib"])
  print(run_conf["Test Mode"])
  print(run_conf["NEvents per job"])
  print(run_conf["NJobs"])
  print(run_conf["Type sample ttbar"])

  #store variables from json
  nev_per_job = run_conf["NEvents per job"]
  n_jobs = run_conf["NJobs"]
  eos_output_folder = run_conf["Jobs_output_folder"]
  eos_custom_lib = run_conf["Jobs_customised_lib"]
  test = run_conf["Test Mode"]

  #adapt to test mode
  if bool(test) :
    print("> Running in test mode, only 2 events are submit with 3 events")
    nev_per_job = 3
    n_jobs = 2

  #set up environment
  cmd_source_ILCSoft = "source_ILCSoft %s"%(run_conf["Release date"])
  os.system(cmd_source_ILCSoft)
  if bool(test) : print("Source ILCSoft release with: %s"%cmd_source_ILCSoft)

  single_particle_list = ["muons", "electrons", "pions"]
  complex_events_list = ["ttbar"]

  #running sim-reco-val for single particles
  for particle in run_conf["Particles"]:
    if particle not in single_particle_list:
      print("%s not in the list of single particles! Possible choices are: %s"%(particle,', '.join(single_particle_list)))
      continue

    #all sumbit scripts for single particle use the same xml file
    xml_file_name = "local_files/clicReconstruction.xml"
    if check_file_exist(xml_file_name) and bool(test):
      print("> Input xml file already exist")

    for var in run_conf["Variables"]:
      if var == "theta":
        for theta in ["10","30","89"] :
          if bool(test) : print(">> Running simulation and reconstruction for %s on grid at theta = %s deg"%(particle, theta))
          cmd_run_theta = "python submit_gps_theta.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, theta, run_conf["Release date"], run_conf["Detector model"], 
                          n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
          print(cmd_run_theta)
          #os.system(cmd_run_theta)
      elif var == "energy":
        for energy in ["1","10","100"] :
          if bool(test) : print(">> Running simulation and reconstruction for %s on grid at energy = %s GeV"%(particle, energy))
          cmd_run_energy = "python submit_gun_energy.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, energy, run_conf["Release date"], run_conf["Detector model"], 
                           n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
          print(cmd_run_energy)
          #os.system(cmd_run_energy)

  #running reco-val for complex events
  for particle in run_conf["Particles"]:
    if particle not in complex_events_list:
      print("%s not in the list of complex events! Possible choices are: %s"%(particle,', '.join(complex_events_list)))
      continue

    if run_conf["Type sample ttbar"] == "SIM":
      #check (or create) the input xml file
      xml_file_name = "local_files/clicReconstruction.xml"
      if check_file_exist(xml_file_name) and bool(test):
        print("> Input xml file already exist")

      if bool(test) : print(">> Running reconstruction and validation for %s on grid"%(particle))
      cmd_run_ttbar = "python submit_ttbar_fromSIM.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, run_conf["Input sample ttbar"], run_conf["Release date"], run_conf["Detector model"], 
                      n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
      print(cmd_run_ttbar)
      #os.system(cmd_run_ttbar)

    if run_conf["Type sample ttbar"] == "REC":
      #check (or create) the input xml file
      xml_file_name = "local_files/clicReconstruction_onlyValidator.xml"
      if check_file_exist(xml_file_name) and bool(test):
        print("> Input xml file already exist")

      if bool(test) : print(">> Running validation for %s on grid"%(particle))
      cmd_run_ttbar = "python submit_ttbar_fromREC.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, run_conf["Input sample ttbar"], run_conf["Release date"], run_conf["Detector model"], 
                      n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
      print(cmd_run_ttbar)
      #os.system(cmd_run_ttbar)

if __name__ == "__main__":
  if len(sys.argv[1:]) == 0:
    print("Please always give the run_config file as argument, e.g. \n python main.py ../cfg/run_info_local.json")
    sys.exit()

  check_dirac = raw_input("Did you setup Dirac? [yes/no] ") 
  if str(check_dirac) == 'yes':
    main(sys.argv[1:])

