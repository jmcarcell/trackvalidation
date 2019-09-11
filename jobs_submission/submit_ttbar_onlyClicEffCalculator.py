import sys, os, subprocess
from string import Template

from DIRAC.Core.Base import Script
Script.parseCommandLine()
from ILCDIRAC.Interfaces.API.DiracILC import  DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import *
from ILCDIRAC.Interfaces.API.NewInterface.Applications import *

sampleNum = 14207
maxNumberEvents = 1000
nameJobGroup = 'efficiencies'
nameTag = 'ttbar'
clicConfig = 'ILCSoft-2019-09-04'
marlinVersion = 'ILCSoft-2019-09-04_gcc62'
detectorModel =  'CLIC_o3_v14'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
baseSteeringMarlin = 'local/path/CLICPerformance/clicConfig/steeringFiles/clicReconstruction_onlyClicEffCalculator.xml'
nameSteeringMarlin = "local/path/CLICPerformance/clicConfig/clicReconstruction_onlyClicEffCalculator_final.xml"
templateOutRoot = "histograms"

for input_ind in range(1,maxNumberEvents,1):

  rootFile = nameTag + '_' + str(input_ind)
  with open(baseSteeringMarlin) as f:
    open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))

  inputFolder_ind = input_ind / 1000
  print ( inputFolder_ind )

  dirac = DiracILC(False)
  job = UserJob()
  job.setJobGroup(nameJobGroup)
  job.setName(nameTag)
  job.setBannedSites(['LCG.IN2P3-CC.fr','OSG.UConn.us','LCG.Cracow.pl','OSG.MIT.us','LCG.Glasgow.uk','OSG.CIT.us','OSG.BNL.us','LCG.Brunel.uk','LCG.QMUL.uk'])
  #set customised library
  #job.setInputSandbox("LFN:/ilc/user/e/ericabro/libCLICPerformance-iLCSoft_2019-04-01_LLP.tar.gz")
  job.setOutputSandbox ( [ "*.log"] )
  job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")
  
  ma = Marlin()
  ma.setVersion(marlinVersion)
  #ma.setNbEvts(2)

  ma.setInputFile("LFN:/ilc/prod/clic/3tev/tt/CLIC_o3_v14/REC/000%s/00%s/tt_rec_%s_%s.slcio"%(str(sampleNum),str(inputFolder_ind),str(sampleNum),str(input_ind)))
  ma.setDetectorModel(detectorModel)
  ma.setSteeringFile(nameSteeringMarlin)
  #ma.setNumberOfEvents(3)
  
  res=job.append(ma)
  if not res['OK']:
     print res['Message']
     exit()

  job.dontPromptMe()
  print job.submit(dirac)
      
