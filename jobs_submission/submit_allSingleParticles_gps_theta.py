#!/bin/python

#set environment          
import os
import sys, getopt

#####################################################################

#Parsing parameters
#from DIRAC import exit as DIRACExit
from DIRAC.Core.Base import Script
#from DIRAC import S_OK, S_ERROR
from class_parse import *
#class Params(object):
#
#  def __init__(self):
#    self.raw = False
#    self.pingsToDo = 1
#
#  def setRawResult(self, value):
#    self.raw = True
#    return S_OK()
#
#  def setNumOfPingsToDo(self, value):
#    print("hello")
#    try:
#      self.pingsToDo = max(1, int(value))
#    except ValueError:
#      return S_ERROR("Number of pings to do has to be a number")
#    return S_OK()

# Instantiate the params class
cliParams = Params()

# Register accepted switches and their callbacks
Script.registerSwitch("r", "showRaw", "show raw result from the query", cliParams.setRawResult)
Script.registerSwitch("p:", "numPings=", "Number of pings to do (by default 1)", cliParams.setNumOfPingsToDo)
print("hello")

# Parse the command line and initialize DIRAC
Script.parseCommandLine(ignoreErrors=False)
print(cliParams.raw)
print(cliParams.pingsToDo)

#parameters
particle = sys.argv[1]
gunTheta = sys.argv[2]
macFile = "gps_"+gunTheta+"deg_Pow1_clic.mac"
macFilePath = "/ilc/user/e/eleogran/mac/"+particle+"/"+macFile
nameTag = particle+'_'+gunTheta+'deg'

clicConfig = sys.argv[3]
ddsimVersion = sys.argv[3]+'_gcc62' 
marlinVersion = sys.argv[3]+'_gcc62' 
detectorModel =  sys.argv[4]
baseSteeringDDSim = 'local_files/clic_steer.py'
baseSteeringMarlin = 'local_files/clicReconstruction.xml'
nameSteeringMarlin = 'local_files/clicReconstruction_final.xml'

nJobs = int(sys.argv[5])
nEvts = int(sys.argv[6])
nameJobGroup = sys.argv[7]
templateOutRoot = "histograms"
rootFile = particle+'_'+gunTheta+'deg'
outputFile = particle+'_'+gunTheta+'deg.slcio'
nameDir = 'CLIC/'+detectorModel+'/'+clicConfig+'/'+nameJobGroup+'/files_'+nameTag
print('Output files can be found in %s'%nameDir)

#####################################################################

with open(baseSteeringMarlin) as f:
    open(nameSteeringMarlin,"w").write(f.read().replace(templateOutRoot,rootFile))
 
######################################################################      
#from ILCDIRAC.Interfaces.API.DiracILC import DiracILC #job receiver class   
#dirac = DiracILC(False)      
#from ILCDIRAC.Interfaces.API.NewInterface.UserJob import UserJob
#from DIRAC.Resources.Catalog.FileCatalogClient import FileCatalogClient 
#from ILCDIRAC.Interfaces.API.NewInterface.Applications import DDSim
#from ILCDIRAC.Interfaces.API.NewInterface.Applications import Marlin
#   
######################################################################      
#
##job definition   
#
#job = UserJob() #use UserJob unless recommended differently      
#job.setJobGroup(nameJobGroup)
#job.setCPUTime(86400)
#job.setName(nameTag)
#job.setBannedSites(['LCG.UKI-LT2-IC-HEP.uk','LCG.KEK.jp','LCG.IN2P3-CC.fr','LCG.Tau.il','Weizmann.il','LCG.Weizmann.il','OSG.MIT.us','OSG.FNAL_FERMIGRID.us','OSG.GridUNESP_CENTRAL.br','OSG.SPRACE.br'])
#job.setInputSandbox([nameSteeringMarlin,'LFN:'+macFilePath])
#
##set customised library
#if len(sys.argv) > 8:
#  customisedLibrary = str(sys.argv[8])
#  print('Using Marlin customised library: %s'%customisedLibrary)
#  job.setInputSandbox(customisedLibrary)
#
#job.setOutputSandbox(["*.log"]) 
#job.setOutputData([rootFile+".root"],nameDir,"CERN-DST-EOS")   
#job.setSplitEvents(nEvts,nJobs)
#   
######################################################################      
#
##ddsim
#
#ddsim = DDSim()
#ddsim.setVersion(ddsimVersion)
#ddsim.setDetectorModel(detectorModel)
#ddsim.setOutputFile(outputFile)
#ddsim.setExtraCLIArguments(" --enableG4GPS --runType \"run\" --macroFile %s" % macFile)
#ddsim.setSteeringFile(baseSteeringDDSim)
#ddsim.setNumberOfEvents(nEvts)
#res = job.append(ddsim)
#
#if not res['OK']:
#    print res['Message']
#    sys.exit(2)
#
######################################################################
#
##marlin
#ma = Marlin()
#ma.setVersion(marlinVersion)
#ma.setNbEvts(nEvts)
#ma.setDetectorModel(detectorModel)
#ma.getInputFromApp(ddsim)
#ma.setProcessorsToUse([])
#ma.setSteeringFile(nameSteeringMarlin)
#res = job.append(ma) 
#
#if not res['OK']:
#    print res['Message']
#    sys.exit(2)
#
######################################################################      
##submit          
#
#job.dontPromptMe()
#print job.submit(dirac)
#
