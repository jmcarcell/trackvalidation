import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from ROOT import gROOT, TFile, TCanvas, TGraph, TLegend, gPad, TLatex
import json
import argparse
#STILL TO DO:
#- phi -180, 180 range doesnt work 

gROOT.Macro("CLICdpStyle.C") 
parser = argparse.ArgumentParser(description='Produce root files with tracking analysis plots')
parser.add_argument('--filesin', nargs='+', help='List of root file used as input')
parser.add_argument('--features', nargs='+', help='List of features used for this input file as label:color:markerStyle (ex. "p_{T} = 1 GeV:1:20:False")')
parser.add_argument('--folderout', help='Name of output folder')
parser.add_argument('--histonames', nargs='+', help='Name of histos to plot (ex. eff_vs_theta eff_vs_phi)')
parser.add_argument('--varAxes', nargs='+', help='Name of axes in histos (ex. ";#theta [#circ];Tracking efficiency" ";#phi [#circ];Tracking efficiency")')
parser.add_argument('--logXaxis', nargs='+', help='Bool to set log of X axes in histos')
parser.add_argument('--logYaxis', nargs='+', help='Bool to set log of Y axes in histos')
parser.add_argument('--rangeYaxis', nargs='+', help='Range Y axis (ex. min:max)', default="0.9:1.0 0.9:1.0")
parser.add_argument('--rangeXaxis', nargs='+', help='Range X axis (ex. min:max)', default="7:90 -180:180")
parser.add_argument('--sample', help="Data sample", type=str, choices=["muon", "ele", "pion", "ttbar3TeV", "ttbar380GeV"])
parser.add_argument('-v', '--verbose', help="increase output verbosity", action="store_true")
args = parser.parse_args()

VERBOSE = args.verbose
if VERBOSE : print(args)

INPUTFILES = args.filesin
OUTPUTFOLDER = args.folderout

LABELS = []
COLORS = []
MARKERS = []
for feature in args.features:
  feat = feature.split(":")
  #print(feat)
  LABELS.append(feat[0])
  COLORS.append(int(feat[1]))
  MARKERS.append(int(feat[2]))

SAMPLE = args.sample
HISTONAMES = args.histonames
AXISTITLE = args.varAxes
AXISXLOG  = args.logXaxis
AXISYLOG  = args.logYaxis

MINYAXIS = []
MAXYAXIS = []
for rangeY in args.rangeYaxis :
  axis = rangeY.split(":")
  #print(axis)
  MINYAXIS.append(float(axis[0]))
  MAXYAXIS.append(float(axis[1]))

MINXAXIS = []
MAXXAXIS = []
for rangeX in args.rangeXaxis :
  axis = rangeX.split(":")
  #print(axis)
  MINXAXIS.append(float(axis[0]))
  MAXXAXIS.append(float(axis[1]))

def main():
  for i_histo in range(0,len(HISTONAMES)) :
    if VERBOSE : 
      print("> Plotting %s histogram:"%HISTONAMES[i_histo])
      print("  Axis titles = %s"%(AXISTITLE[i_histo]))
      print("  X axis: min = %.2f, max = %.2f, isLog = %s"%(MINXAXIS[i_histo],MAXXAXIS[i_histo],AXISXLOG[i_histo]))
      print("  Y axis: min = %.2f, max = %.2f, isLog = %s"%(MINYAXIS[i_histo],MAXYAXIS[i_histo],AXISYLOG[i_histo]))
    c = TCanvas("c","c")
    gROOT.SetBatch(True);

    c.SetGrid()
    if json.loads(AXISXLOG[i_histo].lower()):
      c.SetLogx()
    if json.loads(AXISYLOG[i_histo].lower()):
      c.SetLogy()

    leg = TLegend(0.63,0.30,0.85,0.45)
    leg.SetTextSize(0.03)
    if SAMPLE == "muon" :
      if "fake" in HISTONAMES[i_histo] : leg = TLegend(0.63,0.60,0.85,0.75)
      leg.SetHeader("Single #mu^{-}")
    elif SAMPLE == "ele" :
      leg.SetHeader("Single e^{-}")
    elif SAMPLE == "pion" :
      leg.SetHeader("Single #pi^{-}")
    elif SAMPLE == "ttbar3TeV" :
      leg = TLegend(0.23,0.20,0.85,0.35)
      leg.SetHeader("t#bar{t}, E_{CM} = 3 TeV")
    elif SAMPLE == "ttbar380GeV" :
      leg.SetHeader("t#bar{t}, E_{CM} = 380 GeV")

    if VERBOSE : print("  Input files used:")
    for i_gr in range(0,len(INPUTFILES)):
      if VERBOSE : print("  %s"%(INPUTFILES[i_gr]))
      inputTFile = TFile(INPUTFILES[i_gr])
      graph = inputTFile.Get(HISTONAMES[i_histo])
      graph.SetTitle(AXISTITLE[i_histo])
      graph.SetMarkerStyle(MARKERS[i_gr])
      graph.SetMarkerSize(1.3)
      graph.SetMarkerColor(COLORS[i_gr])
      graph.SetLineColor(COLORS[i_gr])

      if i_gr == 0 : 
        graph.Draw()
        gPad.Update()
        graph_copy = graph.GetPaintedGraph()
        graph_copy.GetXaxis().SetTitleOffset(1.2)
        graph_copy.SetMinimum(MINYAXIS[i_histo])
        graph_copy.SetMaximum(MAXYAXIS[i_histo])
        graph_copy.GetXaxis().SetRangeUser(MINXAXIS[i_histo],MAXXAXIS[i_histo]);
      else : 
        graph.Draw("same")

      leg.AddEntry(graph, LABELS[i_gr], "ep")
      leg.Draw()
    
    gPad.Update()
    text = TLatex();
    text.SetTextSize(0.035);
    text.DrawTextNDC(0.175, 0.939349, "CLICdp work in progress");
    
    c.Draw()
    nameOutputPlot = OUTPUTFOLDER+HISTONAMES[i_histo]
    c.SaveAs(nameOutputPlot+".eps","eps")
    c.SaveAs(nameOutputPlot+".png","png")

if __name__ == "__main__":
    main()
