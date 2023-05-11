import sys
import os
import json

def create_link(FOLDERIN,FOLDEROUT):
    if os.path.exists(FOLDERIN):
        print("%s exists"%FOLDERIN)
        if os.path.exists(FOLDEROUT):
            recreate = input("%s already exists. Do you want to recreate the file [y/n]?"%FOLDEROUT)
            if recreate == "y":
                os.system("rm %s"%FOLDEROUT)
            else:
                return
    else:
        print("%s does NOT exist !"%FOLDERIN)
        sys.exit()
    os.system("ln -s %s %s"%(FOLDERIN,FOLDEROUT))

def main(argv):
    run_config_file = argv[0]
    with open(run_config_file) as json_run_file:
        run_conf = json.load(json_run_file)

    #Test
    test = run_conf["Test Mode"]

    SUBFOLDER = "tmp_input_folder"
    PATHPLOTS = run_conf["Input folder"]

    #Copying files in a symbolic link (problem can occur if using eps files in LaTeX)
    create_link(PATHPLOTS,SUBFOLDER)
    if bool(test) : print('%s was linked in %s'%(PATHPLOTS,SUBFOLDER))

    RELEASE   = run_conf["Release date"]
    AUTHOR    = run_conf["Author name"]
    INSTITUTE = run_conf["Author institute"]
    DATE      = run_conf["Date"]

    MIN_HITS_SINGLE  = run_conf["Min hits (single)"]
    MIN_HITS_COMPLEX = run_conf["Min hits (complex)"]

    command_create = "python personalise_pdf.py --filein \"release_summary_template.tex\" --fileout \"release_summary.tex\" "
    command_create += " --authorinstitute \"%s\" --authorname \"%s\" --date %s --release %s --path %s --minHits_singlePart %s --minHits_complex %s"%(INSTITUTE,AUTHOR,DATE,RELEASE,SUBFOLDER,MIN_HITS_SINGLE,MIN_HITS_COMPLEX)
    if bool(test) : print(command_create)
    os.system(command_create)
    os.system("pdflatex release_summary.tex")

    SUBFOLDER2 = "tmp_input_folder2"
    PATHPLOTS2 = run_conf["Input folder2"]

    #Copying files in a symbolic link (problem can occur if using eps files in LaTeX)
    create_link(PATHPLOTS2,SUBFOLDER2)
    if bool(test) : print('%s was linked in %s'%(PATHPLOTS2,SUBFOLDER2))

    if not run_conf["Comparison"]:
        return

    RELEASE2  = run_conf["Release date2"]

    command_create = "python personalise_pdf.py --filein \"release_comparison_template.tex\" --fileout \"release_comparison.tex\" "
    command_create += " --authorinstitute \"%s\" --authorname \"%s\" --date %s --release %s --path %s --minHits_singlePart %s --minHits_complex %s"%(INSTITUTE,AUTHOR,DATE,RELEASE,SUBFOLDER,MIN_HITS_SINGLE,MIN_HITS_COMPLEX)
    command_create += " --release2 %s --path2 %s"%(RELEASE2,SUBFOLDER2)
    if bool(test) : print(command_create)
    os.system(command_create)
    os.system("pdflatex release_comparison.tex")

if __name__ == "__main__":
    if len(sys.argv[1:]) == 0:
        print("Please always give the run_config file as argument, e.g. \n python script.py ../cfg/run_info_local.json")
        sys.exit()

    main(sys.argv[1:])
