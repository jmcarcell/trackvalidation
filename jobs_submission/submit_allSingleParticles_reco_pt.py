#!/bin/python

#set environment
import os
from os import listdir
import sys, getopt

#####################################################################

#Parsing parameters
from DIRAC.Core.Base import Script
from class_parse import *

# Instantiate the params class
cliParams = Params()

# Register accepted switches and their callbacks
Script.registerSwitch("p:", "particle=", "particle type", cliParams.setParticle)
Script.registerSwitch("v:", "pt=", "Fixed pt value", cliParams.setPt)
Script.registerSwitch("r:", "clicRelease=", "ILCSoft release", cliParams.setRelease)
Script.registerSwitch("D:", "detector=", "Detector configuration", cliParams.setDetector)
Script.registerSwitch("j:", "njobs=", "Number of jobs", cliParams.setNJobs)
Script.registerSwitch("e:", "nev=", "Number of events per job", cliParams.setNEvents)
Script.registerSwitch("g:", "group=", "Group name", cliParams.setGroup)
Script.registerSwitch("l:", "library=", "Library file in LFN", cliParams.setLibrary)
Script.registerSwitch("f:", "simFold=", "SIM folder used as input", cliParams.setSIMFolder)
Script.registerSwitch("", "local", "If set, run job locally for testing", cliParams.setLocal)

# Parse the command line and initialize DIRAC
Script.parseCommandLine(ignoreErrors=False)

#parameters
particle = cliParams.particle
gunPt = cliParams.pt
nameTag = particle+'_'+gunPt+'GeV_fixedPt'
if 'muon' in particle :
    gunPdg = "13"
elif 'ele' in particle:
    gunPdg = "11"
elif 'pion' in particle:
    gunPdg = "211"
else:
    print('ERROR in submit_allSingleParticles_slcio_fixedPt.py >> Particle not in the list!')

clicConfig = cliParams.release
marlinVersion = clicConfig+'_gcc62'
detectorModel =  cliParams.detector
baseSteeringMarlin = os.path.join(os.getcwd(), 'local_files/templates/clicReconstruction.xml')
nameSteeringMarlin = "local_files/clicReconstruction_final.xml"

nJobs = cliParams.njobs #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py
nEvts = cliParams.nev #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py
nameJobGroup = cliParams.group

templateOutRoot = "histograms"
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)

pathSLCIO = cliParams.sim_folder+'files_'+particle+'_'+gunPt+'GeV_fixedPt_ddsim'
subpathSLCIO = pathSLCIO[pathSLCIO.find("/ilc/"):]

#####################################################################

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
dirac = DiracILC(False)
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin

#####################################################################
#job definition
i = 0

listOfFiles = listdir(pathSLCIO)
listOfFiles = [f for f in listOfFiles if int(f[f.find("particles_")+10:f.find('.slcio')]) <= nJobs]
inputFiles = [os.path.join(subpathSLCIO, f) for f in listOfFiles]

rootFiles = ['reco_' + f.split('.', 1)[0] for f in listOfFiles]
outputFiles = [f + '.root' for f in rootFiles]

job = UserJob()
job.setJobGroup(nameJobGroup)
job.setCPUTime(86400)
job.setName(nameTag)
job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
job.setInputSandbox([baseSteeringMarlin])
job.setOutputSandbox(["*.log"])
job.setSplitOutputData(outputFiles, nameDir, "CERN-DST-EOS")
job.setSplitDoNotAlterOutputFilename()
job.setSplitInputData(lfns=inputFiles, numberOfFilesPerJob=1)
job.setSplitParameter('rootFile', rootFiles)

#####################################################################
#marlin

ma = Marlin()

ma.setVersion(marlinVersion)
ma.setDetectorModel(detectorModel)
ma.setExtraCLIArguments('--MyAIDAProcessor.FileName=%(rootFile)s')
#set customised library
if cliParams.lib is not "":
    print('Using Marlin customised library: %s'%cliParams.lib)
    job.setInputSandbox(cliParams.lib)

ma.setNumberOfEvents(nEvts)
ma.setSteeringFile(os.path.basename(baseSteeringMarlin))
res = job.append(ma)

if not res['OK']:
    print res['Message']
    sys.exit(2)

#####################################################################
#submit

job.dontPromptMe()
print job.submit(dirac, mode='local' if cliParams.local else 'wms')
