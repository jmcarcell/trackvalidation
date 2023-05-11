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
Script.registerSwitch("f:", "simFold=", "SIM folder used as input", cliParams.setSIMFolder)
Script.registerSwitch("", "local", "If set, run job locally for testing", cliParams.setLocal)

# Parse the command line and initialize DIRAC
Script.parseCommandLine(ignoreErrors=False)

#parameters
particle = cliParams.particle
gunPt = cliParams.pt
nameTag = particle+'_'+gunPt+'GeV_fixedPt_ddsim'
if 'muon' in particle :
    gunPdg = "13"
elif 'ele' in particle:
    gunPdg = "11"
elif 'pion' in particle:
    gunPdg = "211"
else:
    print('ERROR in submit_allSingleParticles_slcio_fixedPt.py >> Particle not in the list!')

clicConfig = cliParams.release
ddsimVersion = clicConfig+'_gcc62'
detectorModel =  cliParams.detector
baseSteeringDDSim = os.path.join(os.getcwd(), 'local_files/clic_steer.py')

nJobs = cliParams.njobs
nEvts = cliParams.nev
nameJobGroup = cliParams.group
nEvtGen = 10000 #nEvents in the SLCIO files produced with submit_allSingleParticles_slcio_fixedPt.py
templateOutFile = 'sim.slcio'
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag
print('Output files can be found in %s'%nameDir)

pathSLCIO = cliParams.sim_folder+'files_'+particle+'_'+gunPt+'GeV_fixedPt'
subpathSLCIO = pathSLCIO[pathSLCIO.find("/ilc/"):]

#####################################################################

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
dirac = DiracILC(False)
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient
from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim

#####################################################################
#job definition

listOfFiles = listdir(pathSLCIO)

inputFiles = [os.path.join(subpathSLCIO, f) for f in listOfFiles]

jobsPerFile = nEvtGen / nEvts
outputTmp = ['ddsim_j_%d_' + f for f in listOfFiles]
outputFiles = []
for outputFile in outputTmp:
    for i in range(jobsPerFile):
        outputFiles.append(outputFile % i)

#print("Input file: %s" % f)
#for i in range(0,nEvtGen/nEvts):
#print("Index chunk of events to process: %s" % i)

#print("Output file: %s" % outputFile)
#if fileCounter > nJobs:
        #break

job = UserJob()
job.setJobGroup(nameTag)
job.setCPUTime(86400)
job.setName(nameTag + '_%n')
job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
job.setInputSandbox([baseSteeringDDSim])
job.setOutputSandbox(["*.log"])
job.setSplitDoNotAlterOutputFilename()
job.setSplitFilesAcrossJobs(lfns=inputFiles, eventsPerFile=nEvtGen, eventsPerJob=nEvts)
job.setSplitOutputData(outputFiles, nameDir, "CERN-DST-EOS")
job.setSplitParameter('outputFile', outputFiles)

#####################################################################
#ddsim

ddsim = DDSim()

ddsim.setVersion(ddsimVersion)
ddsim.setDetectorModel(detectorModel)
ddsim.setOutputFile('%(outputFile)s')
ddsim.setNumberOfEvents(nEvts)
ddsim.setSteeringFile(baseSteeringDDSim)
res = job.append(ddsim)

if not res['OK']:
    print res['Message']
    sys.exit(2)

#####################################################################
#submit

job.dontPromptMe()
print job.submit(dirac, mode='local' if cliParams.local else 'wms')
