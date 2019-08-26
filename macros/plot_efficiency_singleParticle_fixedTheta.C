//----------------------------------------------------------------
//
// macro to plot tracking efficiency of single particles vs pT
// needs 3 input files (theta = 10, 30, 89 deg) 
// input file example: /eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/merged_Pow1_muons_10deg.root 
// can plot muons, pions and electrons
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

void BinLogX(TH1D *);

const char *path = "";
TString figuresFolder = "../figures/";

void plot_efficiency_singleParticle_fixedTheta(TString file1 = "merged_Pow1_muons_10deg.root", TString file2 = "merged_Pow1_muons_30deg.root", TString file3 = "merged_Pow1_muons_89deg.root", int minNhits = 3){

  CLICdpStyle();

  char inputFileName1[200];
  strcpy(inputFileName1,path);
  strcat(inputFileName1,file1);
  char inputFileName2[200];
  strcpy(inputFileName2,path);
  strcat(inputFileName2,file2);
  char inputFileName3[200];
  strcpy(inputFileName3,path);
  strcat(inputFileName3,file3);

  // input files and trees

  TFile *f_1 = TFile::Open(inputFileName1,"READ");
  if(!f_1) return;
  TFile *f_2 = TFile::Open(inputFileName2,"READ");
  if(!f_2) return;
  TFile *f_3 = TFile::Open(inputFileName3,"READ");
  if(!f_3) return;

  TTree *t_1;
  f_1->GetObject("MyClicEfficiencyCalculator/simplifiedEfficiencyTree",t_1);
  TTree *t_2;
  f_2->GetObject("MyClicEfficiencyCalculator/simplifiedEfficiencyTree",t_2);
  TTree *t_3;
  f_3->GetObject("MyClicEfficiencyCalculator/simplifiedEfficiencyTree",t_3);

  // var initialization
  
  double m_pt_1 = 0;
  bool m_reconstructed_1 = 0;
  double m_vertexR_1 = 0;
  int m_nHits_1 = 0;

  t_1->SetBranchAddress("m_pt",&m_pt_1);
  t_1->SetBranchAddress("m_reconstructed",&m_reconstructed_1);
  t_1->SetBranchAddress("m_vertexR",&m_vertexR_1);
  t_1->SetBranchAddress("m_nHits",&m_nHits_1);

  double m_pt_2 = 0;
  bool m_reconstructed_2 = 0;
  double m_vertexR_2 = 0;
  int m_nHits_2 = 0;

  t_2->SetBranchAddress("m_pt",&m_pt_2);
  t_2->SetBranchAddress("m_reconstructed",&m_reconstructed_2);
  t_2->SetBranchAddress("m_vertexR",&m_vertexR_2);
  t_2->SetBranchAddress("m_nHits",&m_nHits_2);

  double m_pt_3 = 0;
  bool m_reconstructed_3 = 0;
  double m_vertexR_3 = 0;
  int m_nHits_3 = 0;

  t_3->SetBranchAddress("m_pt",&m_pt_3);
  t_3->SetBranchAddress("m_reconstructed",&m_reconstructed_3);
  t_3->SetBranchAddress("m_vertexR",&m_vertexR_3);
  t_3->SetBranchAddress("m_nHits",&m_nHits_3);

  //----file1: 10deg

  Int_t nentries_1 = t_1->GetEntries();

  TH1D *h_reconstructable_pt_1 = new TH1D("h_reconstructable_pt_1","h_reconstructable_pt_1",100,-2,3);
  h_reconstructable_pt_1->Sumw2();
  BinLogX(h_reconstructable_pt_1);
  TH1D *h_reconstructed_pt_1 = new TH1D("h_reconstructed_pt_1","h_reconstructed_pt_1",100,-2,3);
  h_reconstructed_pt_1->Sumw2();
  BinLogX(h_reconstructed_pt_1);

  for(Int_t i=0; i < nentries_1; i++){

    t_1->GetEntry(i);
    
    if(m_vertexR_1 == 0) { // only prompt MCparticle (with electrons and pions there may be secondary particles from material interaction)
      h_reconstructable_pt_1->Fill(m_pt_1);
      if(m_nHits_1 >= minNhits && m_reconstructed_1) {
	h_reconstructed_pt_1->Fill(m_pt_1);
      }
    }
  }

  TEfficiency *h_efficiency_pt_1 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_pt_1,*h_reconstructable_pt_1)){
    h_efficiency_pt_1 = new TEfficiency(*h_reconstructed_pt_1,*h_reconstructable_pt_1);
  }

  //----file2: 30 deg

  Int_t nentries_2 = t_2->GetEntries();

  TH1D *h_reconstructable_pt_2 = new TH1D("h_reconstructable_pt_2","h_reconstructable_pt_2",100,-2,3);
  h_reconstructable_pt_2->Sumw2();
  BinLogX(h_reconstructable_pt_2);
  TH1D *h_reconstructed_pt_2 = new TH1D("h_reconstructed_pt_2","h_reconstructed_pt_2",100,-2,3);
  h_reconstructed_pt_2->Sumw2();
  BinLogX(h_reconstructed_pt_2);

  for(Int_t i=0; i < nentries_2; i++){

    t_2->GetEntry(i);
    
    if(m_vertexR_2 == 0) {
      h_reconstructable_pt_2->Fill(m_pt_2);
      if(m_nHits_2 >= minNhits && m_reconstructed_2) {
	h_reconstructed_pt_2->Fill(m_pt_2);
      }
    }
  }

  TEfficiency *h_efficiency_pt_2 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_pt_2,*h_reconstructable_pt_2)){
    h_efficiency_pt_2 = new TEfficiency(*h_reconstructed_pt_2,*h_reconstructable_pt_2);
  }

  //----file3: 89 deg

  Int_t nentries_3 = t_3->GetEntries();

  TH1D *h_reconstructable_pt_3 = new TH1D("h_reconstructable_pt_3","h_reconstructable_pt_3",100,-2,3);
  h_reconstructable_pt_3->Sumw2();
  BinLogX(h_reconstructable_pt_3);
  TH1D *h_reconstructed_pt_3 = new TH1D("h_reconstructed_pt_3","h_reconstructed_pt_3",100,-2,3);
  h_reconstructed_pt_3->Sumw2();
  BinLogX(h_reconstructed_pt_3);

  for(Int_t i=0; i < nentries_3; i++){

    t_3->GetEntry(i);

    if(m_vertexR_3 == 0) {
      h_reconstructable_pt_3->Fill(m_pt_3);
      if(m_nHits_3 >= minNhits && m_reconstructed_3) {
	h_reconstructed_pt_3->Fill(m_pt_3);
      }
    }
  }

  TEfficiency *h_efficiency_pt_3 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_pt_3,*h_reconstructable_pt_3)){
    h_efficiency_pt_3 = new TEfficiency(*h_reconstructed_pt_3,*h_reconstructable_pt_3);
  }
  
  TCanvas *c = new TCanvas();
  c->SetLogx(); 
  c->SetGrid();
  h_efficiency_pt_3->SetTitle(";p_{T} [GeV];Tracking efficiency");
  h_efficiency_pt_3->SetLineColor(kBlue);
  h_efficiency_pt_3->SetMarkerColor(kBlue);
  h_efficiency_pt_3->SetMarkerStyle(20);
  h_efficiency_pt_3->SetMarkerSize(1.2);
  h_efficiency_pt_3->Draw();
  h_efficiency_pt_2->SetLineColor(kRed);
  h_efficiency_pt_2->SetMarkerColor(kRed);
  h_efficiency_pt_2->SetMarkerStyle(21);
  h_efficiency_pt_2->SetMarkerSize(0.9);
  h_efficiency_pt_2->Draw("same");
  h_efficiency_pt_1->SetLineColor(kBlack);
  h_efficiency_pt_1->SetMarkerColor(kBlack);
  h_efficiency_pt_1->SetMarkerStyle(22);
  h_efficiency_pt_1->SetMarkerSize(1.2);
  h_efficiency_pt_1->Draw("same");
  gPad->Update();
  auto graph_3 = h_efficiency_pt_3->GetPaintedGraph();
  graph_3->GetXaxis()->SetTitleOffset(1.2);
  TAxis *axis = graph_3->GetXaxis();
  axis->SetLimits(0.1,250);
  graph_3->SetMinimum(0.955); 
  graph_3->SetMaximum(1.01);
  auto graph_2 = h_efficiency_pt_2->GetPaintedGraph();
  auto graph_1 = h_efficiency_pt_1->GetPaintedGraph();

  gPad->Update();

  std::string inputFile1(file1,50);
  std::string muon("muon");
  std::string electron("ele");
  std::string pion("pion");
    
  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

  auto legend0 = new TLegend(0.635338,0.310651,0.849624,0.483728);
  legend0->SetTextSize(0.03);
  if( inputFile1.find(muon) != -1 ) legend0->SetHeader("Single #mu^{-}");
  else if( inputFile1.find(electron) != -1 ) legend0->SetHeader("Single e^{-}");
  else if( inputFile1.find(pion) != -1 ) legend0->SetHeader("Single #pi^{-}");
  legend0->AddEntry(h_efficiency_pt_1,"#theta = 10#circ (forward)","ep");
  legend0->AddEntry(h_efficiency_pt_2,"#theta = 30#circ (transition)","ep");
  legend0->AddEntry(h_efficiency_pt_3,"#theta = 89#circ (barrel)","ep");
  legend0->Draw();
  
  if( inputFile1.find(muon) != -1 )
    c->SaveAs(Form((figuresFolder + "/muons/eff_vs_pT_minNhits%i.eps"),minNhits));
  else if( inputFile1.find(electron) != -1 )
    c->SaveAs(Form((figuresFolder + "/electrons/eff_vs_pT_minNhits%i.eps"),minNhits));
  else if( inputFile1.find(pion) != -1 )
    c->SaveAs(Form((figuresFolder + "/pions/eff_vs_pT_minNhits%i.eps"),minNhits));

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
