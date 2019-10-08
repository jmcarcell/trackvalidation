//----------------------------------------------------------------
//
// macro to plot tracking efficiency of complex events
// needs 2 input files (without, with background overlay) 
// input file example: /eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/merged_ttbar3TeV_NoOverlay_conformal.root 
// can plot Z->uds@500GeV, bbbar@3TeV and ttbar@3TeV,380GeV
// can plot vs pt, theta, phi, distance of closest MCparticle, vertex radius
// author: Emilia Leogrande
//
//----------------------------------------------------------------


#include "CLICdpStyle.C"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <string>

const double purityMin = 0.75;
const int thetaMin = 10;
const int thetaMax = 170;
const double ptMin = 1;
const double vertexRMax = 50;
const double distClosestMCPMin = 0.02;

TString path = "";
TString figuresFolder = "";

const char *treeName = "MyClicEfficiencyCalculator/simplifiedEfficiencyTree";
const char *signalBranch = "m_signal";
const char *ptBranch = "m_pt";
const char *thetaBranch = "m_theta";
const char *phiBranch = "m_phi";
const char *vertexRBranch = "m_vertexR";
const char *distClosestMCPBranch = "m_distClosestMCPart";
const char *purityBranch = "m_purity";
const char *reconstructedBranch = "m_reconstructed";
const char *nHitsBranch = "m_nHits";

std::string vsPt("pt");
std::string vsTheta("theta");
std::string vsPhi("phi");
std::string vsVertexR("vertexR");
std::string vsDistClosestMCP("dist");

void BinLogX(TH1D *);

void plot_efficiencyComplexEvents(TString file1 = "merged_ttbar3TeV_NoOverlay_conformal.root", TString file2 = "merged_ttbar3TeV_Overlay_conformal.root", string vsWhat = "pt", int minNhits = 4){

  CLICdpStyle();

  char inputFileName1[200];
  strcpy(inputFileName1,path);
  strcat(inputFileName1,file1);

  char inputFileName2[200];
  strcpy(inputFileName2,path);
  strcat(inputFileName2,file2);

  TFile *f_1 = TFile::Open(inputFileName1,"READ");
  if(!f_1) return;

  TTree *t_1;
  f_1->GetObject(treeName,t_1);

  TFile *f_2 = TFile::Open(inputFileName2,"READ");
  if(!f_2) return;
 
  TTree *t_2;
  f_2->GetObject(treeName,t_2);
  
  bool m_signal_1 = 0;
  double m_pt_1 = 0;
  double m_theta_1 = 0;
  double m_phi_1 = 0;
  double m_vertexR_1 = 0;
  double m_distClosestMCPart_1 = 0;
  double m_purity_1 = 0;
  bool m_reconstructed_1 = 0;
  int m_nHits_1 = 0;

  t_1->SetBranchAddress(signalBranch,&m_signal_1);
  t_1->SetBranchAddress(ptBranch,&m_pt_1);
  t_1->SetBranchAddress(thetaBranch,&m_theta_1);
  t_1->SetBranchAddress(phiBranch,&m_phi_1);
  t_1->SetBranchAddress(vertexRBranch,&m_vertexR_1);
  t_1->SetBranchAddress(distClosestMCPBranch,&m_distClosestMCPart_1);
  t_1->SetBranchAddress(purityBranch,&m_purity_1);
  t_1->SetBranchAddress(reconstructedBranch,&m_reconstructed_1);
  t_1->SetBranchAddress(nHitsBranch,&m_nHits_1);

  bool m_signal_2 = 0;
  double m_pt_2 = 0;
  double m_theta_2 = 0;
  double m_phi_2 = 0;
  double m_vertexR_2 = 0;
  double m_distClosestMCPart_2 = 0;
  double m_purity_2 = 0;
  bool m_reconstructed_2 = 0;
  int m_nHits_2 = 0;

  t_2->SetBranchAddress(signalBranch,&m_signal_2);
  t_2->SetBranchAddress(ptBranch,&m_pt_2);
  t_2->SetBranchAddress(thetaBranch,&m_theta_2);
  t_2->SetBranchAddress(phiBranch,&m_phi_2);
  t_2->SetBranchAddress(vertexRBranch,&m_vertexR_2);
  t_2->SetBranchAddress(distClosestMCPBranch,&m_distClosestMCPart_2);
  t_2->SetBranchAddress(purityBranch,&m_purity_2);
  t_2->SetBranchAddress(reconstructedBranch,&m_reconstructed_2);
  t_2->SetBranchAddress(nHitsBranch,&m_nHits_2);

  TH1D *h_reconstructable_1;
  TH1D *h_reconstructed_1;

  Int_t nEntries_1 = t_1->GetEntries();

  TH1D *h_reconstructable_2;
  TH1D *h_reconstructed_2;

  Int_t nEntries_2 = t_2->GetEntries();

  //----vs pt

  if(vsWhat.find(vsPt) != -1){
    h_reconstructable_1 = new TH1D("h_reconstructable_1","h_reconstructable_1",100,-1,3);
    h_reconstructable_1->Sumw2();
    BinLogX(h_reconstructable_1);
    h_reconstructed_1 = new TH1D("h_reconstructed_1","h_reconstructed_1",100,-1,3);
    h_reconstructed_1->Sumw2();
    BinLogX(h_reconstructed_1);
    h_reconstructable_2 = new TH1D("h_reconstructable_2","h_reconstructable_2",100,-1,3);
    h_reconstructable_2->Sumw2();
    BinLogX(h_reconstructable_2);
    h_reconstructed_2 = new TH1D("h_reconstructed_2","h_reconstructed_2",100,-1,3);
    h_reconstructed_2->Sumw2();
    BinLogX(h_reconstructed_2);

    for(Int_t i=0; i<nEntries_1; i++){
      t_1->GetEntry(i);
      if(m_signal_1 && m_purity_1 >= purityMin && m_theta_1 > thetaMin && m_theta_1 < thetaMax 
         && m_vertexR_1 < vertexRMax && m_distClosestMCPart_1 > distClosestMCPMin){
	h_reconstructable_1->Fill(m_pt_1);
	if(m_nHits_1 >= minNhits && m_reconstructed_1) h_reconstructed_1->Fill(m_pt_1);
      }
    }
    std::cout << "Entries reconstructed: " << h_reconstructed_1->GetEntries() << std::endl;
    std::cout << "Entries simulated: " << h_reconstructable_1->GetEntries() << std::endl;

    for(Int_t i=0; i<nEntries_2; i++){
      t_2->GetEntry(i);
      if(m_signal_2 && m_purity_2 >= purityMin && m_theta_2 > thetaMin && m_theta_2 < thetaMax 
         && m_vertexR_2 < vertexRMax && m_distClosestMCPart_2 > distClosestMCPMin){
	h_reconstructable_2->Fill(m_pt_2);
	if(m_nHits_2 >= minNhits && m_reconstructed_2) h_reconstructed_2->Fill(m_pt_2);
      }
    }
    std::cout << "Entries reconstructed: " << h_reconstructed_2->GetEntries() << std::endl;
    std::cout << "Entries simulated: " << h_reconstructable_2->GetEntries() << std::endl;

  }

  //----83 7heta

  else if(vsWhat.find(vsTheta) != -1){
    h_reconstructable_1 = new TH1D("h_reconstructable_1","h_reconstructable_1",83,7,90);
    h_reconstructable_1->Sumw2();
    h_reconstructed_1 = new TH1D("h_reconstructed_1","h_reconstructed_1",83,7,90);
    h_reconstructed_1->Sumw2();
    h_reconstructable_2 = new TH1D("h_reconstructable_2","h_reconstructable_2",83,7,90);
    h_reconstructable_2->Sumw2();
    h_reconstructed_2 = new TH1D("h_reconstructed_2","h_reconstructed_2",83,7,90);
    h_reconstructed_2->Sumw2();

    for(Int_t i=0; i<nEntries_1; i++){
      t_1->GetEntry(i);
      if(m_signal_1 && m_purity_1 >= purityMin && m_pt_1 > ptMin && m_vertexR_1 < vertexRMax && m_distClosestMCPart_1 > distClosestMCPMin){
	h_reconstructable_1->Fill(m_theta_1);
	if(m_nHits_1 >= minNhits && m_reconstructed_1) h_reconstructed_1->Fill(m_theta_1);
      }
    }
    std::cout << "Entries reconstructed: " << h_reconstructed_1->GetEntries() << std::endl;
    std::cout << "Entries simulated: " << h_reconstructable_1->GetEntries() << std::endl;

    for(Int_t i=0; i<nEntries_2; i++){
      t_2->GetEntry(i);
      if(m_signal_2 && m_purity_2 >= purityMin && m_pt_2 > ptMin && m_vertexR_2 < vertexRMax && m_distClosestMCPart_2 > distClosestMCPMin){
	h_reconstructable_2->Fill(m_theta_2);
	if(m_nHits_2 >= minNhits && m_reconstructed_2) h_reconstructed_2->Fill(m_theta_2);
      }
    }
    std::cout << "Entries reconstructed: " << h_reconstructed_2->GetEntries() << std::endl;
    std::cout << "Entries simulated: " << h_reconstructable_2->GetEntries() << std::endl;

  }

  //----vs phi

  else if(vsWhat.find(vsPhi) != -1){
    h_reconstructable_1 = new TH1D("h_reconstructable_1","h_reconstructable_1",180,-180,180);
    h_reconstructable_1->Sumw2();
    h_reconstructed_1 = new TH1D("h_reconstructed_1","h_reconstructed_1",180,-180,180);
    h_reconstructed_1->Sumw2();
    h_reconstructable_2 = new TH1D("h_reconstructable_2","h_reconstructable_2",180,-180,180);
    h_reconstructable_2->Sumw2();
    h_reconstructed_2 = new TH1D("h_reconstructed_2","h_reconstructed_2",180,-180,180);
    h_reconstructed_2->Sumw2();

    for(Int_t i=0; i<nEntries_1; i++){
      t_1->GetEntry(i);
      if(m_signal_1 && m_purity_1 >= purityMin && m_pt_1 > ptMin && m_theta_1 > thetaMin && m_theta_1 < thetaMax && m_vertexR_1 < vertexRMax && m_distClosestMCPart_1 > distClosestMCPMin){
	h_reconstructable_1->Fill(m_phi_1);
	if(m_nHits_1 >= minNhits && m_reconstructed_1) h_reconstructed_1->Fill(m_phi_1);
      }
    }
    for(Int_t i=0; i<nEntries_2; i++){
      t_2->GetEntry(i);
      if(m_signal_2 && m_purity_2 >= purityMin && m_pt_2 > ptMin && m_theta_2 > thetaMin && m_theta_2 < thetaMax && m_vertexR_2 < vertexRMax && m_distClosestMCPart_2 > distClosestMCPMin){
	h_reconstructable_2->Fill(m_phi_2);
	if(m_nHits_2 >= minNhits && m_reconstructed_2) h_reconstructed_2->Fill(m_phi_2);
      }
    }

  }

  //----vs vertexR

  else if(vsWhat.find(vsVertexR) != -1){
    h_reconstructable_1 = new TH1D("h_reconstructable_1","h_reconstructable_1",60,0,600);
    h_reconstructable_1->Sumw2();
    h_reconstructed_1 = new TH1D("h_reconstructed_1","h_reconstructed_1",60,0,600);
    h_reconstructed_1->Sumw2();
    h_reconstructable_2 = new TH1D("h_reconstructable_2","h_reconstructable_2",60,0,600);
    h_reconstructable_2->Sumw2();
    h_reconstructed_2 = new TH1D("h_reconstructed_2","h_reconstructed_2",60,0,600);
    h_reconstructed_2->Sumw2();

    for(Int_t i=0; i<nEntries_1; i++){
      t_1->GetEntry(i);
      if(m_signal_1 && m_purity_1 >= purityMin && m_pt_1 > ptMin && m_theta_1 > thetaMin && m_theta_1 < thetaMax && m_distClosestMCPart_1 > distClosestMCPMin){
	h_reconstructable_1->Fill(m_vertexR_1);
	if(m_nHits_1 >= minNhits && m_reconstructed_1) h_reconstructed_1->Fill(m_vertexR_1);
      }
    }
    for(Int_t i=0; i<nEntries_2; i++){
      t_2->GetEntry(i);
      if(m_signal_2 && m_purity_2 >= purityMin && m_pt_2 > ptMin && m_theta_2 > thetaMin && m_theta_2 < thetaMax && m_distClosestMCPart_2 > distClosestMCPMin){
	h_reconstructable_2->Fill(m_vertexR_2);
	if(m_nHits_2 >= minNhits && m_reconstructed_2) h_reconstructed_2->Fill(m_vertexR_2);
      }
    }
  }

  //----vs distClosestMCPart

  else if(vsWhat.find(vsDistClosestMCP) != -1){
    h_reconstructable_1 = new TH1D("h_reconstructable_1","h_reconstructable_1",100,0,1);
    h_reconstructable_1->Sumw2();
    h_reconstructed_1 = new TH1D("h_reconstructed_1","h_reconstructed_1",100,0,1);
    h_reconstructed_1->Sumw2();
    h_reconstructable_2 = new TH1D("h_reconstructable_2","h_reconstructable_2",100,0,1);
    h_reconstructable_2->Sumw2();
    h_reconstructed_2 = new TH1D("h_reconstructed_2","h_reconstructed_2",100,0,1);
    h_reconstructed_2->Sumw2();

    for(Int_t i=0; i<nEntries_1; i++){
      t_1->GetEntry(i);
      if(m_signal_1 && m_purity_1 >= purityMin && m_pt_1 > ptMin && m_theta_1 > thetaMin && m_theta_1 < thetaMax && m_vertexR_1 < vertexRMax){
	h_reconstructable_1->Fill(m_distClosestMCPart_1);
	if(m_nHits_1 >= minNhits && m_reconstructed_1) h_reconstructed_1->Fill(m_distClosestMCPart_1);
      }
    }
    for(Int_t i=0; i<nEntries_2; i++){
      t_2->GetEntry(i);
      if(m_signal_2 && m_purity_2 >= purityMin && m_pt_2 > ptMin && m_theta_2 > thetaMin && m_theta_2 < thetaMax && m_vertexR_2 < vertexRMax){
	h_reconstructable_2->Fill(m_distClosestMCPart_2);
	if(m_nHits_2 >= minNhits && m_reconstructed_2) h_reconstructed_2->Fill(m_distClosestMCPart_2);
      }
    }
  }

  TEfficiency *h_efficiency_1 = 0;
  if(TEfficiency::CheckConsistency(*h_reconstructed_1,*h_reconstructable_1)){
    h_efficiency_1 = new TEfficiency(*h_reconstructed_1,*h_reconstructable_1);
  }
   
  TEfficiency *h_efficiency_2 = 0;
  if(TEfficiency::CheckConsistency(*h_reconstructed_2,*h_reconstructable_2)){
    h_efficiency_2 = new TEfficiency(*h_reconstructed_2,*h_reconstructable_2);
  }
 
  TCanvas *c = new TCanvas();
  c->SetGrid();
  if(vsWhat.find(vsPt)!=-1) c->SetLogx();

  if(vsWhat.find(vsPt)!=-1) h_efficiency_1->SetTitle(";p_{T} [GeV];Tracking efficiency");
  else if(vsWhat.find(vsTheta)!=-1) h_efficiency_1->SetTitle(";#theta [#circ];Tracking efficiency");
  else if(vsWhat.find(vsPhi)!=-1) h_efficiency_1->SetTitle(";#phi [#circ];Tracking efficiency");
  else if(vsWhat.find(vsVertexR)!=-1) h_efficiency_1->SetTitle(";vertex R [mm];Tracking efficiency");
  else if(vsWhat.find(vsDistClosestMCP)!=-1) h_efficiency_1->SetTitle(";#Delta_{MC} [rad];Tracking efficiency");
  h_efficiency_1->SetLineColor(kBlue);
  h_efficiency_1->SetMarkerColor(kBlue);
  h_efficiency_1->SetMarkerStyle(20);
  h_efficiency_1->Draw();
  h_efficiency_2->SetLineColor(kRed);
  h_efficiency_2->SetMarkerColor(kRed);
  h_efficiency_2->SetMarkerStyle(21);
  h_efficiency_2->Draw("same");
  gPad->Update();
  auto graph = h_efficiency_1->GetPaintedGraph();
  graph->GetXaxis()->SetTitleOffset(1.2);
  graph->GetXaxis()->SetRangeUser(0.09,250);
  if(vsWhat.find(vsDistClosestMCP)!=-1) graph->GetXaxis()->SetRangeUser(0.,0.46);
  if(vsWhat.find(vsVertexR)!=-1) graph->GetXaxis()->SetRangeUser(0.,650);
  graph->SetMinimum(0.7);
  graph->SetMaximum(1.05);
  if(vsWhat.find(vsVertexR)!=-1){
    graph->SetMinimum(0);
    graph->SetMaximum(1.25);
  }
  if(vsWhat.find(vsDistClosestMCP)!=-1){
    graph->SetMinimum(0.96); 
    graph->SetMaximum(1.005);
  }
  if(vsWhat.find(vsTheta)!=-1){
    graph->SetMinimum(0.9);
    graph->SetMaximum(1.01);
    graph->GetXaxis()->SetRangeUser(7,90);
  }
  if(vsWhat.find(vsPhi)!=-1){
    graph->SetMinimum(0.9);
    graph->SetMaximum(1.01);
    graph->GetXaxis()->SetRangeUser(-180,180);
  }

  std::string inputFile1(file1,50);
  std::string Zuds("Zuds");
  std::string ttbar("ttbar");
  std::string bbbar("bbbar");

  std::string TeV("TeV");
  std::string GeV("GeV");

  auto legend0 = new TLegend(0.244361,0.245562,0.888471,0.445266);
  legend0->SetTextSize(0.035);
  if(vsWhat.find(vsTheta)!=-1){ legend0 = new TLegend(0.258145,0.245562,0.882206,0.445266); legend0->SetTextSize(0.035);}
  if(vsWhat.find(vsVertexR)!=-1) {legend0 = new TLegend(0.299499,0.664201,0.884712,0.862426); legend0->SetTextSize(0.035);}
  else if(vsWhat.find(vsDistClosestMCP)!=-1) {legend0 = new TLegend(0.196742,0.244083,0.809524,0.443787); legend0->SetTextSize(0.035);}
  if(inputFile1.find(Zuds)!=-1) legend0->SetHeader("Z#rightarrowq#bar{q} (q = u,d,s), m_{Z} = 500 GeV");
  else if(inputFile1.find(bbbar)!=-1) legend0->SetHeader("b#bar{b}, E_{CM} =  3 TeV");
  else if(inputFile1.find(ttbar)!=-1){
    if(inputFile1.find(TeV)!=-1) {
      if(vsWhat.find(vsPt)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 3 TeV}{%i#circ < #theta < %i#circ, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", thetaMin, thetaMax, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsTheta)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 3 TeV}{p_{T} > %.0f GeV, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", ptMin, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsPhi)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 3 TeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", ptMin, thetaMin, thetaMax, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsVertexR)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 3 TeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, #Delta_{MC} > %.2f rad}", ptMin, thetaMin, thetaMax, distClosestMCPMin));
      else if(vsWhat.find(vsDistClosestMCP)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 3 TeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, vertex R < %.0f mm}", ptMin, thetaMin, thetaMax, vertexRMax));
    }
    else if(inputFile1.find(GeV)!=-1){
      if(vsWhat.find(vsPt)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 380 GeV}{%i#circ < #theta < %i#circ, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", thetaMin, thetaMax, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsTheta)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 380 GeV}{p_{T} > %.0f GeV, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", ptMin, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsPhi)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 380 GeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, vertex R < %.0f mm, #Delta_{MC} > %.2f rad}", ptMin, thetaMin, thetaMax, vertexRMax, distClosestMCPMin));
      else if(vsWhat.find(vsVertexR)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 380 GeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, #Delta_{MC} > %.2f rad}", ptMin, thetaMin, thetaMax, distClosestMCPMin));
      else if(vsWhat.find(vsDistClosestMCP)!=-1) legend0->SetHeader(Form("#splitline{t#bar{t}, E_{CM} = 380 GeV}{p_{T} > %.0f GeV, %i#circ < #theta < %i#circ, vertex R < %.0f mm}", ptMin, thetaMin, thetaMax, vertexRMax));
    }
  }

  legend0->AddEntry(h_efficiency_1,"No background","ep");
  if(inputFile1.find(TeV)!=-1) legend0->AddEntry(h_efficiency_2,"3 TeV #gamma#gamma#rightarrow hadrons background","ep");
  else if(inputFile1.find(GeV)!=-1) legend0->AddEntry(h_efficiency_2,"380 GeV #gamma#gamma#rightarrow hadrons background","ep");

  legend0->Draw();

  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

  TString path_to_fig = figuresFolder;
  
  if(inputFile1.find(Zuds)!=-1){
    path_to_fig += "/Zuds500GeV";
  }
  else if(inputFile1.find(bbbar)!=-1){
    path_to_fig += "/bbbar3TeV";
  }
  else if(inputFile1.find(ttbar)!=-1){
    if(inputFile1.find(TeV)!=-1){
      path_to_fig += "/ttbar3TeV";
    }
    else if(inputFile1.find(GeV)!=-1){
      path_to_fig += "/ttbar380GeV";
    }
  }

  if(vsWhat.find(vsPt)!=-1) c->SaveAs(path_to_fig+Form("/eff_vs_pt_minNhits%i.eps",minNhits));
  else if(vsWhat.find(vsTheta)!=-1) c->SaveAs(path_to_fig+Form("/eff_vs_theta_minNhits%i.eps",minNhits));
  else if(vsWhat.find(vsPhi)!=-1) c->SaveAs(path_to_fig+Form("/eff_vs_phi_minNhits%i.eps",minNhits));
  else if(vsWhat.find(vsVertexR)!=-1) c->SaveAs(path_to_fig+Form("/eff_vs_vertexR_minNhits%i.eps",minNhits));
  else if(vsWhat.find(vsDistClosestMCP)!=-1) c->SaveAs(path_to_fig+Form("/eff_vs_dist_minNhits%i.eps",minNhits));    

}

void BinLogX(TH1D* h){
  TAxis *axis = h->GetXaxis();
  int bins = axis->GetNbins();

  Axis_t from = axis->GetXmin();
  Axis_t to   = axis->GetXmax();
  Axis_t width = (to - from)/bins;
  Axis_t *new_bins = new Axis_t[bins+1];

  for(int i = 0; i <= bins; i++){
    new_bins[i] = TMath::Power(10, from + i*width);
  }
  axis->Set(bins,new_bins);
}
