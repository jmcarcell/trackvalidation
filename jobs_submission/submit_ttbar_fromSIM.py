import sys, os, subprocess
from string import Template

#Parsing parameters
from DIRAC.Core.Base import Script
from class_parse import *

# Instantiate the params class
cliParams = Params()

# Register accepted switches and their callbacks
Script.registerSwitch("p:", "particle=", "particle type", cliParams.setParticle)
Script.registerSwitch("r:", "clicRelease=", "ILCSoft release", cliParams.setRelease)
Script.registerSwitch("D:", "detector=", "Detector configuration", cliParams.setDetector)
Script.registerSwitch("j:", "njobs=", "Number of jobs", cliParams.setNJobs)
Script.registerSwitch("e:", "nev=", "Number of events per job", cliParams.setNEvents)
Script.registerSwitch("g:", "group=", "Group name", cliParams.setGroup)
Script.registerSwitch("l:", "library=", "Library file in LFN", cliParams.setLibrary)
Script.registerSwitch("f:", "simFold=", "SIM sample prodID", cliParams.setSIMFolder)

# Parse the command line and initialize DIRAC
Script.parseCommandLine(ignoreErrors=False)

from DIRAC.Core.Base import Script
Script.parseCommandLine()
from ILCDIRAC.Interfaces.API.DiracILC import  DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import *
from ILCDIRAC.Interfaces.API.NewInterface.Applications import *

particle = cliParams.particle
sampleNum = cliParams.sim_folder
nameTag = particle

clicConfig = cliParams.release
marlinVersion = clicConfig+'_gcc62'
detectorModel =  cliParams.detector
baseSteeringMarlin = 'local_files/clicReconstruction.xml'
nameSteeringMarlin = 'local_files/clicReconstruction_final.xml'

nJobs = cliParams.njobs
nEvts = cliParams.nev
nameJobGroup = cliParams.group
templateOutRoot = "histograms"
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)

for input_ind in range(1,nJobs,1):

    rootFile = nameTag+'_' + str(input_ind)
    with open(baseSteeringMarlin) as f:
        open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))

    inputFolder_ind = input_ind / 1000
    print ( inputFolder_ind )

    dirac = DiracILC(False)
    job = UserJob()
    job.setJobGroup(nameJobGroup)
    job.setName(nameTag)
    job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk','LCG.QMUL.uk'])
    job.setOutputSandbox ( [ "*.log"] )
    job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")
    #set customised library
    if cliParams.lib is not "":
        print('Using Marlin customised library: %s'%cliParams.lib)
        job.setInputSandbox(cliParams.lib)

    if "ove" in particle :
        over = OverlayInput()
        #Overlay.NBunchtrain in clicReconstruction.xml
        over.setBXOverlay(30)
        #Overlay3TeV.NumberBackground in clicReconstruction.xml
        over.setGGToHadInt(3.2)
        over.setNumberOfSignalEventsPerJob( 100 )
        over.setBackgroundType("gghad")
        over.setDetectorModel(detectorModel)
        over.setEnergy("3000")
        over.setMachine("clic_opt")

    ma = Marlin()
    ma.setVersion(marlinVersion)

    ma.setInputFile("LFN:/ilc/prod/clic/3tev/tt/CLIC_o3_v14/SIM/000%s/00%s/tt_sim_%s_%s.slcio"%(sampleNum,str(inputFolder_ind),sampleNum,str(input_ind)))
    ma.setDetectorModel(detectorModel)
    ma.setSteeringFile(nameSteeringMarlin)
    ma.setNumberOfEvents(nEvts)

    if "ove" in particle :
        ma.setExtraCLIArguments( " --Config.Overlay=3TeV " )
        res=job.append(over)
    else:
        res=job.append(ma)
    if not res['OK']:
        print res['Message']
        exit()
    if "ove" in particle :
        res=job.append(ma)

    job.dontPromptMe()
    print job.submit(dirac)
