#!/bin/python
import sys, getopt

#####################################################################     
#set general parameters 

nJobs = int(sys.argv[2]) #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py 
nEvts = int(sys.argv[1]) #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py 
gunPt = sys.argv[3]
gunPdg = sys.argv[4]
nameTag = 'reco_fixedPt_'+gunPdg+'_'+gunPt+'GeV'
nameJobGroup = 'efficiencies'
clicConfig = 'ILCSoft-2019-09-04'
marlinVersion = 'ILCSoft-2019-09-04_gcc62'
detectorModel =  'CLIC_o3_v14'
baseSteeringMarlin = 'local/path/CLICPerformance/clicConfig/steeringFiles/clicReconstruction.xml'
nameSteeringMarlin = "local/path/CLICPerformance/clicConfig/clicReconstruction_final.xml"
templateOutRoot = "histograms"
#templateOutputRec = 'Output_REC'
#templateOutputDst = 'Output_DST'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
#valid for muons
path = '/eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/sim/files_fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV'
#valid for electrons and pions
#path = '/eos/experiment/clicdp/grid/ilc/user/e/ericabro/CLIC/2019/CLICo3v14/ILCSoft-2019-09-04/efficiencies/sim/files_fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV'

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
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin

#####################################################################     
#job definition
i = 0

for f in listdir(path):
    outputFile = 'reco_'+f
    fsplitted = f.split('.')
    rootFile = 'reco_'+fsplitted[0]

    if i > nJobs:
        break
    i += 1

    with open(baseSteeringMarlin) as fmarlin:
        open(nameSteeringMarlin,"w").write(fmarlin.read().replace(templateOutRoot,rootFile))
                                      
    job = UserJob()
    job.setJobGroup(nameJobGroup)
    job.setCPUTime(86400)
    job.setName(nameTag)
    job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
    job.setInputSandbox([nameSteeringMarlin])
    job.setOutputSandbox([ "*.log"]) 
    job.setOutputData([rootFile+'.root'],nameDir,"CERN-DST-EOS")   
    #job.setSplitEvents(nEvts,nJobs) 

#####################################################################     
#marlin

    ma = Marlin()

    ma.setVersion(marlinVersion)
    ma.setDetectorModel(detectorModel)
    ma.setInputFile('LFN:/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/sim/files_fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV/'+f)
    #ma.setInputFile('LFN:/ilc/user/e/ericabro/CLIC/2019/CLICo3v14/ILCSoft-2019-09-04/efficiencies/sim/files_fixedPt_ddsim_'+gunPdg+'_'+gunPt+'GeV/'+f)
    ma.setNumberOfEvents(nEvts)
    ma.setSteeringFile(nameSteeringMarlin)
    res = job.append(ma)

    if not res['OK']:
        print res['Message']
        sys.exit(2)

#####################################################################     
#submit
    
    job.dontPromptMe()
    print job.submit(dirac)
        
