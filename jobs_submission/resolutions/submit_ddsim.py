#!/bin/python
import sys, getopt

#####################################################################     
#set general parameters 

nJobs = 10
nEvts = int(sys.argv[4])
gunTheta = sys.argv[3]
gunPt = sys.argv[2]
gunPdg = sys.argv[1]
nameTag = 'fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV_'+gunTheta+'deg'
nameJobGroup = 'resolutions'
clicConfig = 'ILCSoft-2019-02-20'
ddsimVersion = 'ILCSoft-2019-02-20_gcc62'
detectorModel =  'CLIC_o3_v14'
baseSteeringDDSim = '/afs/cern.ch/user/e/eleogran/Desktop/CLIC/2019/2019-02-20/steeringFiles/clic_steer.py'
templateOutFile = 'sim.slcio'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag
path = '/eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/'+nameJobGroup+'/sim/files_fixedPt_'+gunPdg+'_'+gunPt+'GeV_'+gunTheta+'deg'

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
i = 0

for f in listdir(path):
    print(f)
    outputFile = 'ddsim_'+f
    if i > nJobs:
        break
    i += 1

    job = UserJob()
    job.setJobGroup(nameJobGroup)
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
    ddsim.setInputFile('LFN:/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/'+nameJobGroup+'/sim/files_fixedPt_'+gunPdg+'_'+gunPt+'GeV_'+gunTheta+'deg/'+f)
    ddsim.setOutputFile(outputFile)
    ddsim.setNumberOfEvents(nEvts)
    ddsim.setSteeringFile(baseSteeringDDSim)
    res = job.append(ddsim)

    if not res['OK']:
        print res['Message']
        sys.exit(2)

#####################################################################     
#submit
    
    job.dontPromptMe()
    print job.submit(dirac)
        
