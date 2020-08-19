import sys
import os
import json

def check_file_exist(file_name):
  isthere = os.path.isfile(file_name)
  if not isthere :
    print("> Input xml file (%s) does NOT exist."%(file_name))
    print("> Run script_copyXML.py and adapt them as in local_files/templates/")
    sys.exit()
  return isthere

def main(argv):
  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #store variables from json
  nev_per_job = run_conf["NEvents per job"]
  n_jobs = run_conf["NJobs"]
  eos_output_folder = run_conf["Jobs output folder"]
  eos_custom_lib = run_conf["Jobs customised lib"]
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
  complex_events_list = ["ttbar", "ttbar_ove"]

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
          cmd_run_theta = "python submit_allSingleParticles_gps_theta.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, theta, run_conf["Release date"], run_conf["Detector model"], 
                          n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
          print(cmd_run_theta)
          os.system(cmd_run_theta)
      elif var == "energy":
        for energy in ["1","10","100"] :
          if bool(test) : print(">> Running simulation and reconstruction for %s on grid at energy = %s GeV"%(particle, energy))
          cmd_run_energy = "python submit_allSingleParticles_gun_energy.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, energy, run_conf["Release date"], run_conf["Detector model"], 
                           n_jobs, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
          print(cmd_run_energy)
          os.system(cmd_run_energy)
      elif var == "pT" or var == "pt":
        step = raw_input("> Which step would you like to run to produce %s at fixed pt ? [sim/ddsim/reco] "%particle) 
        for pt in ["1","10","100"] :
          if str(step) == 'sim':
            if bool(test) : print(">> Producing sim files for %s on grid at pt = %s GeV"%(particle, pt))
            #ERICA: in this step 10'000 events are always created!
            cmd_run_pt = "python submit_allSingleParticles_slcio_pt.py %s %s ILCSoft-%s %s %s %s %s "%(particle, pt, run_conf["Release date"], run_conf["Detector model"], 
                             n_jobs, nev_per_job, eos_output_folder)
            print(cmd_run_pt)
            os.system(cmd_run_pt)
          elif str(step) == 'ddsim':
            if bool(test) : print(">> Running simulation for %s on grid at pt = %s GeV"%(particle, pt))

            #ddsim production requires clic_steer file
            steer_file_name = "local_files/clic_steer.py"
            if check_file_exist(steer_file_name) and bool(test):
              print("> Input steer file already exist")

            #Sample fixed pt SIM folder is used to set the path to input SIM files
            #In this step n_jobs is the number of input SIM files taken from "Sample fixed pt SIM folder"
            #1000 events are always run (10'000 / 1'000 = 10 jobs per file)!
            cmd_run_pt = "python submit_allSingleParticles_ddsim_pt.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, pt, run_conf["Release date"], run_conf["Detector model"], 
                             n_jobs, "1000", eos_output_folder, run_conf["Sample fixed pt SIM folder"])
            print(cmd_run_pt)
            os.system(cmd_run_pt)

          elif str(step) == 'reco':
            if bool(test) : print(">> Running reconstruction for %s on grid at pt = %s GeV"%(particle, pt))

            #ddsim production requires clic_steer file
            xml_file_name = "local_files/clicReconstruction.xml"
            if check_file_exist(xml_file_name) and bool(test):
              print("> Input xml file already exist")

            #Sample fixed pt SIM folder is used to set the path to input DDSIM files
            #In this step n_jobs is the number of input SIM files taken from "Sample fixed pt SIM folder"
            #1000 events are always run (10'000 / 1'000 = 10 jobs per file)!
            cmd_run_pt = "python submit_allSingleParticles_reco_pt.py %s %s ILCSoft-%s %s %s %s %s %s %s"%(particle, pt, run_conf["Release date"], run_conf["Detector model"], 
                             n_jobs-1, "1000", eos_output_folder, run_conf["Sample fixed pt SIM folder"], "" if not eos_custom_lib else eos_custom_lib)
            print(cmd_run_pt)
            os.system(cmd_run_pt)
          else:
            print("> Step not included in the list!")
      else:
        print("> Variable not included in the list!")

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
                      n_jobs+1, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
      print(cmd_run_ttbar)
      os.system(cmd_run_ttbar)

    if run_conf["Type sample ttbar"] == "REC":
      #check (or create) the input xml file
      xml_file_name = "local_files/clicReconstruction_onlyValidator.xml"
      if check_file_exist(xml_file_name) and bool(test):
        print("> Input xml file already exist")

      if bool(test) : print(">> Running validation for %s on grid"%(particle))
      cmd_run_ttbar = "python submit_ttbar_fromREC.py %s %s ILCSoft-%s %s %s %s %s %s"%(particle, run_conf["Input sample ttbar"], run_conf["Release date"], run_conf["Detector model"], 
                      n_jobs+1, nev_per_job, eos_output_folder, "" if not eos_custom_lib else eos_custom_lib)
      print(cmd_run_ttbar)
      os.system(cmd_run_ttbar)

if __name__ == "__main__":
  if len(sys.argv[1:]) == 0:
    print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
    sys.exit()

  check_dirac = raw_input("Did you setup Dirac? [yes/no] ") 
  if str(check_dirac) == 'yes':
    main(sys.argv[1:])

