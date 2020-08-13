#!/bin/python
import sys, getopt

#####################################################################

#parameters
particle = sys.argv[1]
gunPt = sys.argv[2]
nameTag = particle+'_'+gunPt+'GeV_fixedPt'
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
baseSteeringGA = 'sh/lcio_particle_gun.py'

nJobs = int(sys.argv[5])
nEvts = int(sys.argv[6])
nameJobGroup = sys.argv[7]
outputFile = 'mcparticles.slcio'
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/sim/files_'+nameTag
print('Output files can be found in %s'%nameDir)
 
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
ga.setScript('sh/run_lcio_particle_gun_'+gunPdg+'_'+gunPt+'GeV.sh')
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

