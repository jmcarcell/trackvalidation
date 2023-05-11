import sys, os, subprocess
from string import Template

from DIRAC.Core.Base import Script
Script.parseCommandLine()
from ILCDIRAC.Interfaces.API.DiracILC import  DiracILC
from ILCDIRAC.Interfaces.API.NewInterface.UserJob import *
from ILCDIRAC.Interfaces.API.NewInterface.Applications import *

sampleNum = 13837
nameJobGroup = 'efficiencies'
nameTag = 'ttbar_overlay'
clicConfig = 'ILCSoft-2019-07-09'
marlinVersion = 'ILCSoft-2019-07-09_gcc62'
detectorModel =  'CLIC_o3_v14'
nameDir = 'CLIC/2019/CLICo3v14/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
baseSteeringMarlin = '/home/ericabro/CLICstudies/2019/CPUtime_zcut/iLCSoft_2019-07-09/Reference/clicConfig/steeringFiles/clicReconstruction_overlay.xml'
nameSteeringMarlin = "clicReconstruction_final.xml"
templateOutRoot = "histograms"

#for input_ind in range(1,2,1):
for input_ind in range(1,4153,1):

    rootFile = nameTag + "_" + str(input_ind)
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

    over = OverlayInput()
    #Overlay.NBunchtrain in clicReconstruction.xml
    over.setBXOverlay(30)
    #Overlay3TeV.NumberBackground in clicReconstruction.xml
    over.setGGToHadInt(3.2)
    over.setNumberOfSignalEventsPerJob( 100 )
    over.setBackgroundType("gghad")
    over.setDetectorModel(detectorModel)
    over.setEnergy("3000")
    over.setMachine("clic_opt")

    ma = Marlin()
    ma.setVersion(marlinVersion)
    #ma.setNbEvts(2)

    ma.setInputFile("LFN:/ilc/prod/clic/3tev/tt/CLIC_o3_v14/SIM/000%s/00%s/tt_sim_%s_%s.slcio"%(str(sampleNum),str(inputFolder_ind),str(sampleNum),str(input_ind)))
    ma.setDetectorModel(detectorModel)
    ma.setSteeringFile(nameSteeringMarlin)
    #ma.setNumberOfEvents(3)

    res=job.append(over)
    if not res['OK']:
        print res['Message']
        exit()
    res=job.append(ma)

    job.dontPromptMe()
    print job.submit(dirac)
