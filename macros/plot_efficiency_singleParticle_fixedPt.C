//----------------------------------------------------------------
//
// macro to plot tracking efficiency of single particles vs theta and phi
// needs 3 input files (pt = 1, 10, 100 GeV) 
// input file example: /eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/reco/merged_muons_1GeV.root 
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

TString path = "";
TString figuresFolder = "../figures/";

void plot_efficiency_singleParticle_fixedPt(TString file1 = "merged_muons_1GeV.root", TString file2 = "merged_muons_10GeV.root", TString file3 = "merged_muons_100GeV.root", int minNhits = 3){

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
  if(t_1 == NULL) f_1->GetObject("simplifiedEfficiencyTree",t_1);
  TTree *t_2;
  f_2->GetObject("MyClicEfficiencyCalculator/simplifiedEfficiencyTree",t_2);
  if(t_2 == NULL) f_2->GetObject("simplifiedEfficiencyTree",t_2);
  TTree *t_3;
  f_3->GetObject("MyClicEfficiencyCalculator/simplifiedEfficiencyTree",t_3);
  if(t_3 == NULL) f_3->GetObject("simplifiedEfficiencyTree",t_3);

  // var initialization
  
  double m_theta_1 = 0;
  double m_phi_1 = 0;
  bool m_reconstructed_1 = 0;
  double m_vertexR_1 = 0;
  int m_nHits_1 = 0;

  t_1->SetBranchAddress("m_theta",&m_theta_1);
  t_1->SetBranchAddress("m_phi",&m_phi_1);
  t_1->SetBranchAddress("m_reconstructed",&m_reconstructed_1);
  t_1->SetBranchAddress("m_vertexR",&m_vertexR_1);
  t_1->SetBranchAddress("m_nHits",&m_nHits_1);

  double m_theta_2 = 0;
  double m_phi_2 = 0;
  bool m_reconstructed_2 = 0;
  double m_vertexR_2 = 0;
  int m_nHits_2 = 0;

  t_2->SetBranchAddress("m_theta",&m_theta_2);
  t_2->SetBranchAddress("m_phi",&m_phi_2);
  t_2->SetBranchAddress("m_reconstructed",&m_reconstructed_2);
  t_2->SetBranchAddress("m_vertexR",&m_vertexR_2);
  t_2->SetBranchAddress("m_nHits",&m_nHits_2);

  double m_theta_3 = 0;
  double m_phi_3 = 0;
  bool m_reconstructed_3 = 0;
  double m_vertexR_3 = 0;
  int m_nHits_3 = 0;

  t_3->SetBranchAddress("m_theta",&m_theta_3);
  t_3->SetBranchAddress("m_phi",&m_phi_3);
  t_3->SetBranchAddress("m_reconstructed",&m_reconstructed_3);
  t_3->SetBranchAddress("m_vertexR",&m_vertexR_3);
  t_3->SetBranchAddress("m_nHits",&m_nHits_3);

  //----file1: 1 GeV

  Int_t nentries_1 = t_1->GetEntries();

  TH1D *h_reconstructable_theta_1 = new TH1D("h_reconstructable_theta_1","h_reconstructable_theta_1",83,7,90);
  h_reconstructable_theta_1->Sumw2();
  TH1D *h_reconstructed_theta_1 = new TH1D("h_reconstructed_theta_1","h_reconstructed_theta_1",83,7,90);
  h_reconstructed_theta_1->Sumw2();
  TH1D *h_reconstructable_phi_1 = new TH1D("h_reconstructable_phi_1","h_reconstructable_phi_1",180,-180,180);
  h_reconstructable_phi_1->Sumw2();
  TH1D *h_reconstructed_phi_1 = new TH1D("h_reconstructed_phi_1","h_reconstructed_phi_1",180,-180,180);
  h_reconstructed_phi_1->Sumw2();

  for(Int_t i=0; i < nentries_1; i++){

    t_1->GetEntry(i);
  
    if(m_vertexR_1 == 0) { 
      h_reconstructable_theta_1->Fill(m_theta_1);
      h_reconstructable_phi_1->Fill(m_phi_1);
      if(m_reconstructed_1 && m_nHits_1 >= minNhits) {
	h_reconstructed_theta_1->Fill(m_theta_1);
	h_reconstructed_phi_1->Fill(m_phi_1);
      }
    }
  }

  TEfficiency *h_efficiency_theta_1 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_theta_1,*h_reconstructable_theta_1)){
    h_efficiency_theta_1 = new TEfficiency(*h_reconstructed_theta_1,*h_reconstructable_theta_1);
  }

  TEfficiency *h_efficiency_phi_1 = 0;
    
  if(TEfficiency::CheckConsistency(*h_reconstructed_phi_1,*h_reconstructable_phi_1)){
    h_efficiency_phi_1 = new TEfficiency(*h_reconstructed_phi_1,*h_reconstructable_phi_1);
  }

  //----file1: 10 GeV

  Int_t nentries_2 = t_2->GetEntries();

  TH1D *h_reconstructable_theta_2 = new TH1D("h_reconstructable_theta_2","h_reconstructable_theta_2",83,7,90);
  h_reconstructable_theta_2->Sumw2();
  TH1D *h_reconstructed_theta_2 = new TH1D("h_reconstructed_theta_2","h_reconstructed_theta_2",83,7,90);
  h_reconstructed_theta_2->Sumw2();
  TH1D *h_reconstructable_phi_2 = new TH1D("h_reconstructable_phi_2","h_reconstructable_phi_2",180,-180,180);
  h_reconstructable_phi_2->Sumw2();
  TH1D *h_reconstructed_phi_2 = new TH1D("h_reconstructed_phi_2","h_reconstructed_phi_2",180,-180,180);
  h_reconstructed_phi_2->Sumw2();

  for(Int_t i=0; i < nentries_2; i++){

    t_2->GetEntry(i);

    if(m_vertexR_2 == 0) {  
      h_reconstructable_theta_2->Fill(m_theta_2);
      h_reconstructable_phi_2->Fill(m_phi_2);
      if(m_reconstructed_2 && m_nHits_2 >= minNhits) {
	h_reconstructed_theta_2->Fill(m_theta_2);
	h_reconstructed_phi_2->Fill(m_phi_2);
      }
    }
  }

  TEfficiency *h_efficiency_theta_2 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_theta_2,*h_reconstructable_theta_2)){
    h_efficiency_theta_2 = new TEfficiency(*h_reconstructed_theta_2,*h_reconstructable_theta_2);
  }

  TEfficiency *h_efficiency_phi_2 = 0;
    
  if(TEfficiency::CheckConsistency(*h_reconstructed_phi_2,*h_reconstructable_phi_2)){
    h_efficiency_phi_2 = new TEfficiency(*h_reconstructed_phi_2,*h_reconstructable_phi_2);
  }

  //----file1: 100 GeV

  Int_t nentries_3 = t_3->GetEntries();

  TH1D *h_reconstructable_theta_3 = new TH1D("h_reconstructable_theta_3","h_reconstructable_theta_3",83,7,90);
  h_reconstructable_theta_3->Sumw2();
  TH1D *h_reconstructed_theta_3 = new TH1D("h_reconstructed_theta_3","h_reconstructed_theta_3",83,7,90);
  h_reconstructed_theta_3->Sumw2();
  TH1D *h_reconstructable_phi_3 = new TH1D("h_reconstructable_phi_3","h_reconstructable_phi_3",180,-180,180);
  h_reconstructable_phi_3->Sumw2();
  TH1D *h_reconstructed_phi_3 = new TH1D("h_reconstructed_phi_3","h_reconstructed_phi_3",180,-180,180);
  h_reconstructed_phi_3->Sumw2();

  for(Int_t i=0; i < nentries_3; i++){

    t_3->GetEntry(i);

    if(m_vertexR_3 == 0) { 
      h_reconstructable_theta_3->Fill(m_theta_3);
      h_reconstructable_phi_3->Fill(m_phi_3);
      if(m_reconstructed_3 && m_nHits_3 >= minNhits) {
	h_reconstructed_theta_3->Fill(m_theta_3);
	h_reconstructed_phi_3->Fill(m_phi_3);
      }
    }
  }

  TEfficiency *h_efficiency_theta_3 = 0;

  if(TEfficiency::CheckConsistency(*h_reconstructed_theta_3,*h_reconstructable_theta_3)){
    h_efficiency_theta_3 = new TEfficiency(*h_reconstructed_theta_3,*h_reconstructable_theta_3);
  }

  TEfficiency *h_efficiency_phi_3 = 0;
    
  if(TEfficiency::CheckConsistency(*h_reconstructed_phi_3,*h_reconstructable_phi_3)){
    h_efficiency_phi_3 = new TEfficiency(*h_reconstructed_phi_3,*h_reconstructable_phi_3);
  }

  TCanvas *c = new TCanvas();
  c->SetGrid();
  h_efficiency_theta_3->SetTitle(";#theta [#circ];Tracking efficiency");
  h_efficiency_theta_3->SetLineColor(kBlue);
  h_efficiency_theta_3->SetMarkerColor(kBlue);
  h_efficiency_theta_3->SetMarkerStyle(20);
  h_efficiency_theta_3->SetMarkerSize(1.2);
  h_efficiency_theta_3->Draw();
  h_efficiency_theta_2->SetLineColor(kRed);
  h_efficiency_theta_2->SetMarkerColor(kRed);
  h_efficiency_theta_2->SetMarkerStyle(21);
  h_efficiency_theta_2->SetMarkerSize(0.9);
  h_efficiency_theta_2->Draw("same");
  h_efficiency_theta_1->SetLineColor(kBlack);
  h_efficiency_theta_1->SetMarkerColor(kBlack);
  h_efficiency_theta_1->SetMarkerStyle(22);
  h_efficiency_theta_1->SetMarkerSize(1.2);
  h_efficiency_theta_1->Draw("same");
  gPad->Update();
  auto graph_3 = h_efficiency_theta_3->GetPaintedGraph();
  graph_3->GetXaxis()->SetTitleOffset(1.2);
  graph_3->SetMinimum(0.955);
  graph_3->SetMaximum(1.01);
  graph_3->GetXaxis()->SetLimits(7,90);
  auto graph_2 = h_efficiency_theta_2->GetPaintedGraph();
  auto graph_1 = h_efficiency_theta_1->GetPaintedGraph();

  gPad->Update();

  std::string inputFile1(file1,50);
  std::string muon("muon");
  std::string electron("ele");
  std::string pion("pion");

  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

  auto legend0 = new TLegend(0.636591,0.309172,0.850877,0.45858);
  legend0->SetTextSize(0.03);
  if( inputFile1.find(muon) != -1 ) legend0->SetHeader("Single #mu^{-}");
  else if( inputFile1.find(electron) != -1 ) legend0->SetHeader("Single e^{-}");
  else if( inputFile1.find(pion) != -1 ) legend0->SetHeader("Single #pi^{-}");
  legend0->AddEntry(h_efficiency_theta_1,"p_{T} = 1 GeV","ep");
  legend0->AddEntry(h_efficiency_theta_2,"p_{T} = 10 GeV","ep");
  legend0->AddEntry(h_efficiency_theta_3,"p_{T} = 100 GeV","ep");
  legend0->Draw();
 
  if( inputFile1.find(muon) != -1 )
    c->SaveAs(Form(figuresFolder + "/muons/eff_vs_theta_minNhits%i.eps",minNhits));
  else if( inputFile1.find(electron) != -1 )
    c->SaveAs(Form(figuresFolder + "/electrons/eff_vs_theta_minNhits%i.eps",minNhits));
  else if( inputFile1.find(pion) != -1 )
    c->SaveAs(Form(figuresFolder + "/pions/eff_vs_theta_minNhits%i.eps",minNhits));


  TCanvas *c1 = new TCanvas();
  c1->SetGrid();
  h_efficiency_phi_3->SetTitle(";#phi [#circ];Tracking efficiency");
  h_efficiency_phi_3->SetLineColor(kBlue);
  h_efficiency_phi_3->SetMarkerColor(kBlue);
  h_efficiency_phi_3->SetMarkerStyle(26);
  h_efficiency_phi_3->Draw();
  h_efficiency_phi_2->SetLineColor(kRed);
  h_efficiency_phi_2->SetMarkerColor(kRed);
  h_efficiency_phi_2->SetMarkerStyle(25);
  h_efficiency_phi_2->Draw("same");
  h_efficiency_phi_1->SetLineColor(kBlack);
  h_efficiency_phi_1->SetMarkerColor(kBlack);
  h_efficiency_phi_1->SetMarkerStyle(24);
  h_efficiency_phi_1->Draw("same");
  gPad->Update();
  auto graph_3_1 = h_efficiency_phi_3->GetPaintedGraph();
  graph_3_1->GetXaxis()->SetTitleOffset(1.2);
  graph_3_1->SetMinimum(0.955);
  graph_3_1->SetMaximum(1.01);
  graph_3_1->GetXaxis()->SetLimits(-180,180);
  auto graph_2_1 = h_efficiency_phi_2->GetPaintedGraph();
  auto graph_1_1 = h_efficiency_phi_1->GetPaintedGraph();

  gPad->Update();
  TLatex *text1 = new TLatex();
  text1->SetTextSize(0.035);
  text1->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

  auto legend01 = new TLegend(0.636591,0.309172,0.850877,0.45858);
  legend01->SetTextSize(0.04);
  if( inputFile1.find(muon) != -1 ) legend01->SetHeader("Single #mu^{-}");
  else if( inputFile1.find(electron) != -1 ) legend01->SetHeader("Single e^{-}");
  else if( inputFile1.find(pion) != -1 ) legend01->SetHeader("Single #pi^{-}");
  legend01->AddEntry(h_efficiency_phi_1,"p_{T} = 1 GeV","ep");
  legend01->AddEntry(h_efficiency_phi_2,"p_{T} = 10 GeV","ep");
  legend01->AddEntry(h_efficiency_phi_3,"p_{T} = 100 GeV","ep");
  legend01->Draw();
   
  if( inputFile1.find(muon) != -1 )
    c->SaveAs(Form(figuresFolder + "/muons/eff_vs_phi_minNhits%i.eps",minNhits));
  else if( inputFile1.find(electron) != -1 )
    c->SaveAs(Form(figuresFolder + "/electrons/eff_vs_phi_minNhits%i.eps",minNhits));
  else if( inputFile1.find(pion) != -1 )
    c->SaveAs(Form(figuresFolder + "/pions/eff_vs_theta_phi%i.eps",minNhits));

}
