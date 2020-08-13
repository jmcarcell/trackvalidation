#!/bin/python
import sys, getopt

#####################################################################

#parameters 
particle = sys.argv[1]
gunPt = sys.argv[2]
nameTag = particle+'_'+gunPt+'GeV_fixedPt_ddsim'
if 'muon' in particle :
  gunPdg = "13"
elif 'ele' in particle:
  gunPdg = "11"
elif 'pion' in particle:
  gunPdg = "211"
else:
  print('ERROR in submit_allSingleParticles_slcio_fixedPt.py >> Particle not in the list!')

clicConfig = sys.argv[3]
ddsimVersion = sys.argv[3]+'_gcc62'
detectorModel =  sys.argv[4]
baseSteeringDDSim = 'local_files/clic_steer.py'

nJobs = int(sys.argv[5])
nEvts = int(sys.argv[6])
nameJobGroup = sys.argv[7]
nEvtGen = 10000 #nEvents in the SLCIO files produced with submit_allSingleParticles_slcio_fixedPt.py
templateOutFile = 'sim.slcio'
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag
print('Output files can be found in %s'%nameDir)
 
pathSLCIO = sys.argv[8]+'files_'+particle+'_'+gunPt+'GeV_fixedPt'
subpathSLCIO = pathSLCIO[pathSLCIO.find("/ilc/"):]

#####################################################################     
#set environment 
import os
from os import listdir

from DIRAC.Core.Base import Script
Script.parseCommandLine()

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC
dirac = DiracILC(False)
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient
from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim

#####################################################################     
#job definition

for f in listdir(pathSLCIO):
    #print("Input file: %s" % f)
    for i in range(0,nEvtGen/nEvts):
        #print("Index chunk of events to process: %s" % i)
        outputFile = 'ddsim_j'+str(i)+'_'+f
        #print("Output file: %s" % outputFile)
        skipEvents = i*nEvts
        #if fileCounter > nJobs:
            #break

        job = UserJob()
        job.setJobGroup(nameTag)
        job.setCPUTime(86400)
        job.setName(nameTag)
        job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
        job.setInputSandbox([baseSteeringDDSim])
        job.setOutputSandbox(["*.log"]) 
        job.setOutputData([outputFile],nameDir,"CERN-DST-EOS") 
    #job.setSplitEvents(nEvts,nJobs) 

#####################################################################     
#ddsim

        ddsim = DDSim()

        ddsim.setVersion(ddsimVersion)
        ddsim.setDetectorModel(detectorModel)
        ddsim.setInputFile('LFN:'+subpathSLCIO+'/'+f)
        ddsim.setOutputFile(outputFile)
        ddsim.setNumberOfEvents(nEvts) 
        ddsim.setSteeringFile(baseSteeringDDSim)
        ddsim.setExtraCLIArguments("--skipNEvents %(evts)s" % {'evts' : skipEvents})
        #print("Skip %s events" % skipEvents)
        res = job.append(ddsim)

        if not res['OK']:
            print res['Message']
            sys.exit(2)

#####################################################################     
#submit
    
        job.dontPromptMe()
        print job.submit(dirac)
        
