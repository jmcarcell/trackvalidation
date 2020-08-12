import sys, os, subprocess
from string import Template

from DIRAC.Core.Base import Script
Script.parseCommandLine()
from ILCDIRAC.Interfaces.API.DiracILC import  DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import *
from ILCDIRAC.Interfaces.API.NewInterface.Applications import *

particle = sys.argv[1]
sampleNum = sys.argv[2]
nameTag = particle

clicConfig = sys.argv[3]
marlinVersion = sys.argv[3]+'_gcc62'
detectorModel =  sys.argv[4]
baseSteeringMarlin = 'local_files/clicReconstruction_onlyValidator.xml'
nameSteeringMarlin = 'local_files/clicReconstruction_final.xml'

nJobs = int(sys.argv[5])
nEvts = int(sys.argv[6])
nameJobGroup = sys.argv[7]
templateOutRoot = "histograms"
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)

for input_ind in range(1,nJobs,1):

  rootFile = nameTag+'_' + str(input_ind)
  with open(baseSteeringMarlin) as f:
    open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))

  inputFolder_ind = input_ind / 1000
  print ( inputFolder_ind )

  dirac = DiracILC(False)
  job = UserJob()
  job.setJobGroup(nameJobGroup)
  job.setName(nameTag)
  job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk','LCG.QMUL.uk'])
  job.setOutputSandbox ( [ "*.log"] )
  job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")
  #set customised library
  if len(sys.argv) > 8:
    customisedLibrary = str(sys.argv[8])
    print('Using Marlin customised library: %s'%customisedLibrary)
    job.setInputSandbox(customisedLibrary)

  
  ma = Marlin()
  ma.setVersion(marlinVersion)

  ma.setInputFile("LFN:/ilc/prod/clic/3tev/tt/CLIC_o3_v14/REC/000%s/00%s/tt_rec_%s_%s.slcio"%(str(sampleNum),str(inputFolder_ind),str(sampleNum),str(input_ind)))
  ma.setDetectorModel(detectorModel)
  ma.setSteeringFile(nameSteeringMarlin)
  ma.setNumberOfEvents(nEvts)
  
  res=job.append(ma)
  if not res['OK']:
     print res['Message']
     exit()

  job.dontPromptMe()
  print job.submit(dirac)
      
