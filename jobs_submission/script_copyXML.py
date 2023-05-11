import sys
import json
import os

def main(argv):
    run_config_file = argv[0]
    with open(run_config_file) as json_run_file:
        run_conf = json.load(json_run_file)

    #set up environment
    print("> To copy the xml files locally, paste the following command in the terminal:")
    command_source = "source /cvmfs/clicdp.cern.ch/iLCSoft/builds/%s/x86_64-slc6-gcc62-opt/init_ilcsoft.sh"%run_conf["Release date"]
    print(command_source)
    #os.system(command_source)
    #print(">> Marlin library:")
    #os.system("echo $MARLIN_DLL")

    cmd_cp = "cp -r $ILCSOFT/ClicPerformance/HEAD/clicConfig/clic* local_files/"
    print(cmd_cp)
    #os.system(cmd_cp)

    print("> Remember to adapt them to run only tracking or only validation!")
    print("> Examples can be found in local_files/templates/")

if __name__ == "__main__":
    if len(sys.argv[1:]) == 0:
        print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
        sys.exit()

    main(sys.argv[1:])
