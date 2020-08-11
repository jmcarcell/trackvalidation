import sys
import os
import json

def main(argv):
  if len(argv) == 0:
    print("Please always give the run_config file as argument, e.g. \n python main.py ../cfg/run_info_local.json")
    sys.exit()

  run_config_file = argv[0]
  with open(run_config_file) as json_run_file:
    run_conf = json.load(json_run_file)

  #Test
  print(run_conf["Particles"])
  print(run_conf["Variables"])

  #set up environment
  cmd_source_ILCSoft = "source_ILCSoft %s"%(run_conf["Release date"])
  os.system(cmd_source_ILCSoft)
  print(cmd_source_ILCSoft)

  check_xml = raw_input("Do you want to copy the xml locally? [yes/no] ") 
  if str(check_xml) == 'yes':
    cmd_cp = "cp -r $ILCSOFT/ClicPerformance/HEAD/clicConfig/clic* local_files/"
    os.system(cmd_cp)
    print(cmd_cp)
    print(">> Adapt the files to run only tracking! Example in local_files/clicReconstruction_template.xml")
    sys.exit()

  particle_list = ["muons", "electrons", "pions"]

  for particle in run_conf["Particles"]:
    print(particle)
    if particle not in particle_list:
      print("%s not in the list of particles! Possible choices are: %s"%(particle,', '.join(particle_list)))
      sys.exit()
    for var in run_conf["Variables"]:
      print(var)
      if var == "theta":
        for theta in ["10","30","89"] :
          print(">> Running simulation and reconstruction for %s on grid at theta = %s deg"%(particle, theta))
          cmd_run_theta = "python submit_gps_theta.py %s %s ILCSoft-%s %s %s %s %s"%(particle, theta, run_conf["Release date"], run_conf["Detector model"], 
                          run_conf["NJobs"], run_conf["NEvents_per_job"], "prova")
          print(cmd_run_theta)
          os.system(cmd_run_theta)
      elif var == "energy":
        for energy in ["1","10","100"] :
          print(">> Running simulation and reconstruction for %s on grid at energy = %s GeV"%(particle, energy))
          cmd_run_energy = "python submit_gun_energy.py %s %s ILCSoft-%s %s %s %s %s"%(particle, energy, run_conf["Release date"], run_conf["Detector model"], 
                           run_conf["NJobs"], run_conf["NEvents_per_job"], "prova")
          print(cmd_run_energy)
          os.system(cmd_run_energy)


if __name__ == "__main__":
  check_dirac = raw_input("Did you setup Dirac? [yes/no] ") 
  if str(check_dirac) == 'yes':
    main(sys.argv[1:])

