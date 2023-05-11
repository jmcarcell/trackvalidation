#!/bin/python
import sys, getopt

#####################################################################

#parameters
nJobs = 10
nEvts = sys.argv[1]
gunPt = sys.argv[2]
gunPdg = sys.argv[3]
gunTheta = sys.argv[4]
nameTag = 'fixedPt_'+gunPdg+'_'+gunPt+'GeV_'+gunTheta+'deg'
nameJobGroup = 'resolutions'
clicConfig = 'ILCSoft-2019-02-20'
ddsimVersion = 'ILCSoft-2019-02-20_gcc62'
detectorModel =  'CLIC_o3_v14'
baseSteeringGA = '/afs/cern.ch/user/e/eleogran/Desktop/CLIC/2019/2019-02-20/resolutions/lcio_particle_gun.py'
outputFile = 'mcparticles.slcio' #'fixedPt_'+gunPt+'GeV.slcio'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag

#####################################################################
#set environment
import os
import sys

from DIRAC.Core.Base import Script #dirac enviroment
Script.parseCommandLine() #dirac enviroment

from ILCDIRAC.Interfaces.API.DiracILC import DiracILC #job receiver class
dirac = DiracILC(False)
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient
from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim
from ILCDIRAC.Interfaces.API.NewInterface.Applications import GenericApplication

#####################################################################

#job definition

job = UserJob() #use UserJob unless recommended differently
job.setJobGroup(nameJobGroup)
job.setCPUTime(86400)
job.setName(nameTag)
job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
job.setInputSandbox([baseSteeringGA])
job.setOutputSandbox(["*.log"])
job.setOutputData([outputFile],nameDir,"CERN-DST-EOS")
job.setSplitEvents(nEvts,nJobs)

#####################################################################

#generic application

ga = GenericApplication()
ga.setScript('run_lcio_particle_gun_'+gunPdg+'_'+gunPt+'GeV_'+gunTheta+'deg.sh')
#ga.setArguments()
ga.setDependency({"ddsim":ddsimVersion})
ga.setOutputFile(outputFile)
res = job.append(ga)

if not res['OK']:
    print res['Message']
    sys.exit(2)

#####################################################################
#submit

job.dontPromptMe()
print job.submit(dirac)
