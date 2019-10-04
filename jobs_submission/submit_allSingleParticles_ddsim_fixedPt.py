#!/bin/python
import sys, getopt

#####################################################################     
#set general parameters 

#nJobs = 200
nEvts = int(sys.argv[1])
nEvtGen = 10000 #nEvents in the SLCIO files produced with submit_allSingleParticles_slcio_fixedPt.py
gunPt = sys.argv[2]
gunPdg = sys.argv[3]
nameTag = 'fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV'
nameJobGroup = 'efficiencies'
clicConfig = 'ILCSoft-2019-09-04'
ddsimVersion = 'ILCSoft-2019-09-04_gcc62'
detectorModel =  'CLIC_o3_v14'
baseSteeringDDSim = '/home/ericabro/CLICstudies/2019/LowPtStudy/ILCSoft_2019-09-04/CLICPerformance/clicConfig/steeringFiles/clic_steer.py'
templateOutFile = 'sim.slcio'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag
pathSLCIO = '/eos/experiment/clicdp/grid/ilc/user/e/ericabro/CLIC/2019/CLICo3v14/ILCSoft-2019-09-04/efficiencies/sim/files_fixedPt_'+gunPdg+'_'+gunPt+'GeV'

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
        ddsim.setInputFile('LFN:/ilc/user/e/ericabro/CLIC/2019/CLICo3v14/ILCSoft-2019-09-04/efficiencies/sim/files_fixedPt_'+gunPdg+'_'+gunPt+'GeV/'+f)
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
        
