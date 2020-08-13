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
marlinVersion = sys.argv[3]+'_gcc62'
detectorModel =  sys.argv[4]
baseSteeringMarlin = 'local_files/clicReconstruction.xml'
nameSteeringMarlin = "local_files/clicReconstruction_final.xml"

nJobs = int(sys.argv[5]) #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py
nEvts = int(sys.argv[6]) #according to what was simulated with submit_allSingleParticles_ddsim_fixedPt.py
nameJobGroup = sys.argv[7]

templateOutRoot = "histograms"
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)
 
pathSLCIO = sys.argv[8]+'files_'+particle+'_'+gunPt+'GeV_fixedPt_ddsim'
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
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin

#####################################################################     
#job definition
i = 0

for f in listdir(pathSLCIO):
    #running all sub-jobs produced in the job
    currJob = f[f.find("particles_")+10:f.find('.slcio')]
    if int(currJob) > nJobs:
      continue
    i+=1

    outputFile = 'reco_'+f
    fsplitted = f.split('.')
    rootFile = 'reco_'+fsplitted[0]

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
    ma.setInputFile('LFN:'+subpathSLCIO+'/'+f)

    #set customised library
    if len(sys.argv) > 9:
      customisedLibrary = str(sys.argv[9])
      print('Using Marlin customised library: %s'%customisedLibrary)
      job.setInputSandbox(customisedLibrary)

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
        
