#!/bin/python
import sys, getopt

#####################################################################

#parameters
nJobs = 200
nEvts = 10000
gunEnergy = sys.argv[1] 
nameTag = 'muons_'+gunEnergy+'GeV'
nameJobGroup = 'efficiencies'
clicConfig = 'ILCSoft-2019-09-04'
ddsimVersion = 'ILCSoft-2019-09-04_gcc62' 
marlinVersion = 'ILCSoft-2019-09-04_gcc62' 
detectorModel =  'CLIC_o3_v14'
baseSteeringDDSim = 'local/path/CLICPerformance/clicConfig/steeringFiles/clic_steer.py'
baseSteeringMarlin = 'local/path/CLICPerformance/clicConfig/steeringFiles/clicReconstruction.xml'
nameSteeringMarlin = "local/path/CLICPerformance/clicConfig/clicReconstruction_final.xml"
templateOutRoot = "histograms"
rootFile = 'muons_'+gunEnergy+'GeV'
outputFile = 'muons_'+gunEnergy+'GeV.slcio'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag

#####################################################################

with open(baseSteeringMarlin) as f:
    open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))
 
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
from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin
   
#####################################################################      
 
#job definition   

job = UserJob() #use UserJob unless recommended differently      
job.setJobGroup(nameJobGroup)
job.setCPUTime(86400)
job.setName(nameTag)
job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
job.setInputSandbox([nameSteeringMarlin])
#job.setInputSandbox("LFN:/ilc/user/e/ericabro/libConformalTracking.tar.gz")
job.setOutputSandbox(["*.log"]) 
job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")   
job.setSplitEvents(nEvts,nJobs)
   
#####################################################################      

#ddsim

ddsim = DDSim()
ddsim.setVersion(ddsimVersion)
ddsim.setDetectorModel(detectorModel)
ddsim.setOutputFile(outputFile)
ddsim.setExtraCLIArguments(" --enableGun --gun.particle mu- --gun.energy %(energy)s*GeV --gun.distribution uniform" % {'energy' : gunEnergy})
ddsim.setSteeringFile(baseSteeringDDSim)
ddsim.setNumberOfEvents(nEvts)
res = job.append(ddsim)

if not res['OK']:
    print res['Message']
    sys.exit(2)

#####################################################################

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

