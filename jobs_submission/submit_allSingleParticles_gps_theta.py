#!/bin/python

#set environment          
import os
import sys, getopt

#####################################################################

#Parsing parameters
from DIRAC.Core.Base import Script
from class_parse import *

# Instantiate the params class
cliParams = Params()

# Register accepted switches and their callbacks
Script.registerSwitch("p:", "particle=", "particle type", cliParams.setParticle)
Script.registerSwitch("v:", "theta=", "Fixed theta value", cliParams.setTheta)
Script.registerSwitch("r:", "clicRelease=", "ILCSoft release", cliParams.setRelease)
Script.registerSwitch("D:", "detector=", "Detector configuration", cliParams.setDetector)
Script.registerSwitch("j:", "njobs=", "Number of jobs", cliParams.setNJobs)
Script.registerSwitch("e:", "nev=", "Number of events per job", cliParams.setNEvents)
Script.registerSwitch("g:", "group=", "Group name", cliParams.setGroup)
Script.registerSwitch("l:", "library=", "Library file in LFN", cliParams.setLibrary)

# Parse the command line and initialize DIRAC
Script.parseCommandLine(ignoreErrors=False)

#parameters
particle = cliParams.particle
gunTheta = cliParams.theta
single_particle_list = ["muons", "electrons", "pions"]
if particle not in single_particle_list:
  print("%s not in the list of single particles! Possible choices are: %s"%(particle,', '.join(single_particle_list)))
  sys.exit()
macFile = "gps_"+gunTheta+"deg_Pow1_clic.mac"
macFilePath = "/ilc/user/e/eleogran/mac/"+particle+"/"+macFile
nameTag = particle+'_'+gunTheta+'deg'

clicConfig = cliParams.release
ddsimVersion = clicConfig+'_gcc62' 
marlinVersion = clicConfig+'_gcc62' 
detectorModel =  cliParams.detector
baseSteeringDDSim = 'local_files/clic_steer.py'
baseSteeringMarlin = 'local_files/clicReconstruction.xml'
nameSteeringMarlin = 'local_files/clicReconstruction_final.xml'

nJobs = cliParams.njobs
nEvts = cliParams.nev
nameJobGroup = cliParams.group
templateOutRoot = "histograms"
rootFile = particle+'_'+gunTheta+'deg'
outputFile = particle+'_'+gunTheta+'deg.slcio'
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)

#####################################################################

#changing output name
with open(baseSteeringMarlin) as f:
    open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))
 
#####################################################################

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC #job receiver class   
dirac = DiracILC(False)      
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient 
from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin
   
#####################################################################      

#job definition   

job = UserJob() #use UserJob unless recommended differently      
job.setJobGroup(nameJobGroup)
job.setCPUTime(86400)
job.setName(nameTag)
job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
job.setInputSandbox([nameSteeringMarlin,'LFN:'+macFilePath])

#set customised library
if cliParams.lib is not "":
  print('Using Marlin customised library: %s'%cliParams.lib)
  job.setInputSandbox(cliParams.lib)

job.setOutputSandbox(["*.log"]) 
job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")   
job.setSplitEvents(nEvts,nJobs)
   
####################################################################      

#ddsim

ddsim = DDSim()
ddsim.setVersion(ddsimVersion)
ddsim.setDetectorModel(detectorModel)
ddsim.setOutputFile(outputFile)
ddsim.setExtraCLIArguments(" --enableG4GPS --runType \"run\" --macroFile %s" % macFile)
ddsim.setSteeringFile(baseSteeringDDSim)
ddsim.setNumberOfEvents(nEvts)
res = job.append(ddsim)

if not res['OK']:
    print res['Message']
    sys.exit(2)

####################################################################

#marlin
ma = Marlin()
ma.setVersion(marlinVersion)
ma.setNbEvts(nEvts)
ma.setDetectorModel(detectorModel)
ma.getInputFromApp(ddsim)
ma.setProcessorsToUse([])
ma.setSteeringFile(nameSteeringMarlin)
res = job.append(ma) 

if not res['OK']:
    print res['Message']
    sys.exit(2)

#####################################################################      

#submit          
job.dontPromptMe()
print job.submit(dirac)

