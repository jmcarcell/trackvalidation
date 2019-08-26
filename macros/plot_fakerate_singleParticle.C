//----------------------------------------------------------------
////
//// macro to plot tracking fake rate of single particles vs theta, phi and pT
//// needs 3 input files (pt = 1, 10, 100 GeV) 
//// input file example: /eos/experiment/clicdp/grid/ilc/user/e/eleogran/CLIC/2019/CLICo3v14/ILCSoft-2019-02-20/efficiencies/reco/merged_muons_1GeV.root 
//// can plot muons, pions and electrons
//// author: Erica Brondolin
////
////----------------------------------------------------------------
//
#include "CLICdpStyle.C"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <string>

void BinLogX(TH1D *);
void BinLogX(TH2D* h);

const double purityMin = 0.75;

TString path = "";
TString figuresFolder = "../figures/";

TString treeName1 = "MyClicEfficiencyCalculator/puritytree";
TString treeName2 = "MyClicEfficiencyCalculator/perfTree";

void plot_fakerate_singleParticle(TString file1 = "merged_muons_1GeV.root", TString file2 = "merged_muons_10GeV.root", TString file3 = "merged_muons_100GeV.root", int minNhits = 3, bool debug = false){

  CLICdpStyle();
  TString inputFileName1 = path + file1;
  TString inputFileName2 = path + file2;
  TString inputFileName3 = path + file3;

  if( debug ){
    std::cout << "Input files: " << std::endl;
    std::cout << inputFileName1 << std::endl;
    std::cout << inputFileName2 << std::endl;
    std::cout << inputFileName3 << std::endl;
  }

  // input files and trees

  TFile *f_1 = TFile::Open(inputFileName1,"READ");
  if(!f_1) return;
  TFile *f_2 = TFile::Open(inputFileName2,"READ");
  if(!f_2) return;
  TFile *f_3 = TFile::Open(inputFileName3,"READ");
  if(!f_3) return;

  TTree *t_pur_1;
  f_1->GetObject(treeName1,t_pur_1);
  TTree *t_pur_2;
  f_2->GetObject(treeName1,t_pur_2);
  TTree *t_pur_3;
  f_3->GetObject(treeName1,t_pur_3);

  TTree *t_per_1;
  f_1->GetObject(treeName2,t_per_1);
  TTree *t_per_2;
  f_2->GetObject(treeName2,t_per_2);
  TTree *t_per_3;
  f_3->GetObject(treeName2,t_per_3);

  // check events number in trees

  if(t_pur_1->GetEntries() != t_per_1->GetEntries()){
    std::cerr << "Different number of entries in " << inputFileName1 << std::endl;
  }
  if(t_pur_2->GetEntries() != t_per_2->GetEntries()){
    std::cerr << "Different number of entries in " << inputFileName2 << std::endl;
  }
  if(t_pur_3->GetEntries() != t_per_3->GetEntries()){
    std::cerr << "Different number of entries in " << inputFileName3 << std::endl;
  }

  // var initialization
  
  vector<double> *trk_purity_1 = 0;
  vector<double> *trk_pt_1 = 0;
  vector<double> *trk_theta_1 = 0;
  vector<double> *trk_phi_1 = 0;
  vector<double> *trk_nhits_1 = 0;
  vector<double> *mc_vtxR_1 = 0;

  TBranch *b_trk_purity_1 = 0;
  t_pur_1->SetBranchAddress("trk_purity",&trk_purity_1,&b_trk_purity_1);
  TBranch *b_trk_pt_1 = 0;
  t_per_1->SetBranchAddress("recoPt",&trk_pt_1,&b_trk_pt_1);
  TBranch *b_trk_theta_1 = 0;
  t_per_1->SetBranchAddress("recoTheta",&trk_theta_1,&b_trk_theta_1);
  TBranch *b_trk_phi_1 = 0;
  t_per_1->SetBranchAddress("recoPhi",&trk_phi_1,&b_trk_phi_1);
  TBranch *b_trk_nhits_1 = 0;
  t_per_1->SetBranchAddress("recoNhits",&trk_nhits_1,&b_trk_nhits_1);
  TBranch *b_mc_vtxR_1 = 0;
  t_per_1->SetBranchAddress("trueVertexR",&mc_vtxR_1,&b_mc_vtxR_1);

  vector<double> *trk_purity_2 = 0;
  vector<double> *trk_pt_2 = 0;
  vector<double> *trk_theta_2 = 0;
  vector<double> *trk_phi_2 = 0;
  vector<double> *trk_nhits_2 = 0;
  vector<double> *mc_vtxR_2 = 0;

  TBranch *b_trk_purity_2 = 0;
  t_pur_2->SetBranchAddress("trk_purity",&trk_purity_2,&b_trk_purity_2);
  TBranch *b_trk_pt_2 = 0;
  t_per_2->SetBranchAddress("recoPt",&trk_pt_2,&b_trk_pt_2);
  TBranch *b_trk_theta_2 = 0;
  t_per_2->SetBranchAddress("recoTheta",&trk_theta_2,&b_trk_theta_2);
  TBranch *b_trk_phi_2 = 0;
  t_per_2->SetBranchAddress("recoPhi",&trk_phi_2,&b_trk_phi_2);
  TBranch *b_trk_nhits_2 = 0;
  t_per_2->SetBranchAddress("recoNhits",&trk_nhits_2,&b_trk_nhits_2);
  TBranch *b_mc_vtxR_2 = 0;
  t_per_2->SetBranchAddress("trueVertexR",&mc_vtxR_2,&b_mc_vtxR_2);

  vector<double> *trk_purity_3 = 0;
  vector<double> *trk_pt_3 = 0;
  vector<double> *trk_theta_3 = 0;
  vector<double> *trk_phi_3 = 0;
  vector<double> *trk_nhits_3 = 0;
  vector<double> *mc_vtxR_3 = 0;

  TBranch *b_trk_purity_3 = 0;
  t_pur_3->SetBranchAddress("trk_purity",&trk_purity_3,&b_trk_purity_3);
  TBranch *b_trk_pt_3 = 0;
  t_per_3->SetBranchAddress("recoPt",&trk_pt_3,&b_trk_pt_3);
  TBranch *b_trk_theta_3 = 0;
  t_per_3->SetBranchAddress("recoTheta",&trk_theta_3,&b_trk_theta_3);
  TBranch *b_trk_phi_3 = 0;
  t_per_3->SetBranchAddress("recoPhi",&trk_phi_3,&b_trk_phi_3);
  TBranch *b_trk_nhits_3 = 0;
  t_per_3->SetBranchAddress("recoNhits",&trk_nhits_3,&b_trk_nhits_3);
  TBranch *b_mc_vtxR_3 = 0;
  t_per_3->SetBranchAddress("trueVertexR",&mc_vtxR_3,&b_mc_vtxR_3);

  // vsPt or vsTheta/Phi ?

  bool vsPt = false, vsThetaPhi = false;

  if( inputFileName1.Contains("deg") && inputFileName2.Contains("deg") && inputFileName3.Contains("deg") )
    vsPt = true;
  else if( inputFileName1.Contains("GeV") && inputFileName2.Contains("GeV") && inputFileName3.Contains("GeV") )
    vsThetaPhi = true;

  if(vsPt){

    // file 1: 10deg

    Int_t nentries_1 = t_per_1->GetEntries();

    TH1D *h_fake_pt_1 = new TH1D("h_fake_pt_1","h_fake_pt_1",100,-2,3);
    h_fake_pt_1->Sumw2();
    BinLogX(h_fake_pt_1);
    TH1D *h_reconstructed_pt_1 = new TH1D("h_reconstructed_pt_1","h_reconstructed_pt_1",100,-2,3);
    h_reconstructed_pt_1->Sumw2();
    BinLogX(h_reconstructed_pt_1);
    TH2D *h_fake_2D_1 = new TH2D("h_fake_2D_1","h_fake_2D_1",100,-2,3,25,-0.5,24.5);
    BinLogX(h_fake_2D_1);

    for(Int_t i=0; i < nentries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_pt_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);
      b_mc_vtxR_1->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_1->size(); j++){ //loop over tracks
       if(mc_vtxR_1->at(j) == 0 && trk_nhits_1->at(j) > minNhits) {
          h_reconstructed_pt_1->Fill(trk_pt_1->at(j));
          if(trk_purity_1->at(j) < purityMin) {
            h_fake_pt_1->Fill(trk_pt_1->at(j));
            h_fake_2D_1->Fill(trk_pt_1->at(j), trk_nhits_1->at(j));
            //std::cout << trk_nhits_1->at(j) << std::endl;
          }
        } else {
          //std::cout << "10 deg --> Displaced vertex: " << mc_vtxR_1->at(j) << std::endl;
        }
      }

    }

   std::cout << "Entries fake: " << h_fake_pt_1->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_pt_1->GetEntries() << std::endl;

    TEfficiency *h_efficiency_pt_1 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_pt_1,*h_reconstructed_pt_1)){
      h_efficiency_pt_1 = new TEfficiency(*h_fake_pt_1,*h_reconstructed_pt_1);
    }

    // file2: 30 deg

    Int_t nentries_2 = t_per_2->GetEntries();

    TH1D *h_fake_pt_2 = new TH1D("h_fake_pt_2","h_fake_pt_2",100,-2,3);
    h_fake_pt_2->Sumw2();
    BinLogX(h_fake_pt_2);
    TH1D *h_reconstructed_pt_2 = new TH1D("h_reconstructed_pt_2","h_reconstructed_pt_2",100,-2,3);
    h_reconstructed_pt_2->Sumw2();
    BinLogX(h_reconstructed_pt_2);

    for(Int_t i=0; i < nentries_2; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_pt_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);
      b_mc_vtxR_2->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_2->size(); j++){ //loop over tracks
        if(mc_vtxR_2->at(j) == 0 && trk_nhits_2->at(j) > minNhits ) {

          h_reconstructed_pt_2->Fill(trk_pt_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_pt_2->Fill(trk_pt_2->at(j));
          }

        } else {
          //std::cout << "30 deg --> Displaced vertex: " << mc_vtxR_1->at(j) << std::endl;
        }
      }
    }

   std::cout << "Entries fake: " << h_fake_pt_2->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_pt_2->GetEntries() << std::endl;

    TEfficiency *h_efficiency_pt_2 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_pt_2,*h_reconstructed_pt_2)){
      h_efficiency_pt_2 = new TEfficiency(*h_fake_pt_2,*h_reconstructed_pt_2);
    }

    // file 3: 89 deg

    Int_t nentries_3 = t_per_3->GetEntries();

    TH1D *h_fake_pt_3 = new TH1D("h_fake_pt_3","h_fake_pt_3",100,-2,3);
    h_fake_pt_3->Sumw2();
    BinLogX(h_fake_pt_3);
    TH1D *h_reconstructed_pt_3 = new TH1D("h_reconstructed_pt_3","h_reconstructed_pt_3",100,-2,3);
    h_reconstructed_pt_3->Sumw2();
    BinLogX(h_reconstructed_pt_3);

    for(Int_t i=0; i < nentries_3; i++){

      Long64_t pur_entry = t_pur_3->LoadTree(i);
      b_trk_purity_3->GetEntry(pur_entry);

      Long64_t per_entry = t_per_3->LoadTree(i);
      b_trk_pt_3->GetEntry(per_entry);
      b_trk_nhits_3->GetEntry(per_entry);
      b_mc_vtxR_3->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_3->size(); j++){ //loop over tracks
        if(mc_vtxR_3->at(j) == 0 && trk_nhits_3->at(j) > minNhits) {

          h_reconstructed_pt_3->Fill(trk_pt_3->at(j));
          if(trk_purity_3->at(j) < purityMin) {
            h_fake_pt_3->Fill(trk_pt_3->at(j));
          }
      
        } else {
          //std::cout << "89 deg --> Displaced vertex: " << mc_vtxR_3->at(j) << std::endl;
        }
      }
    }

   std::cout << "Entries fake: " << h_fake_pt_3->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_pt_3->GetEntries() << std::endl;

    TEfficiency *h_efficiency_pt_3 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_pt_3,*h_reconstructed_pt_3)){
      h_efficiency_pt_3 = new TEfficiency(*h_fake_pt_3,*h_reconstructed_pt_3);
    }

    TCanvas *c = new TCanvas();
    c->SetLogx();  
    c->SetGrid();
    c->SetLogy();
    h_efficiency_pt_3->SetTitle(";Reconstructed track p_{T} [GeV];Fake rate");
    h_efficiency_pt_3->SetLineColor(kBlue);
    h_efficiency_pt_3->SetMarkerColor(kBlue);
    h_efficiency_pt_3->SetMarkerStyle(20);
    h_efficiency_pt_3->Draw();
    h_efficiency_pt_2->SetLineColor(kRed);
    h_efficiency_pt_2->SetMarkerColor(kRed);
    h_efficiency_pt_2->SetMarkerStyle(21);
    h_efficiency_pt_2->Draw("same");
    h_efficiency_pt_1->SetLineColor(kBlack);
    h_efficiency_pt_1->SetMarkerColor(kBlack);
    h_efficiency_pt_1->SetMarkerStyle(22);
    h_efficiency_pt_1->Draw("same");
    gPad->Update();
    auto graph_3 = h_efficiency_pt_3->GetPaintedGraph();
    graph_3->GetXaxis()->SetTitleOffset(1.2);
    TAxis *axis = graph_3->GetXaxis();
    axis->SetLimits(0.1,250);
    //graph_3->GetXaxis()->SetRangeUser(0.1,250);
    graph_3->SetMinimum(1e-5); //0.925 for muons
    graph_3->SetMaximum(1.);
    auto graph_2 = h_efficiency_pt_2->GetPaintedGraph();
    auto graph_1 = h_efficiency_pt_1->GetPaintedGraph();

    gPad->Update();
    
    TLatex *text = new TLatex();
    text->SetTextSize(0.035);
    text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

    auto legend0 = new TLegend(0.659148,0.713018,0.873434,0.862426);
    legend0->SetTextSize(0.03);
    if( inputFileName1.Contains("muon") ) legend0->SetHeader("Single #mu^{-}");
    else if( inputFileName1.Contains("ele") ) legend0->SetHeader("Single e^{-}");
    else if( inputFileName1.Contains("pion") ) legend0->SetHeader("Single #pi^{-}");
    legend0->AddEntry(h_efficiency_pt_1,"#theta = 10#circ","ep");
    legend0->AddEntry(h_efficiency_pt_2,"#theta = 30#circ","ep");
    legend0->AddEntry(h_efficiency_pt_3,"#theta = 89#circ","ep");
    legend0->Draw();
  
    if( inputFileName1.Contains("muon") )
      c->SaveAs(Form((figuresFolder + "/muons/fake_vs_pT_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("ele") )
      c->SaveAs(Form((figuresFolder + "/electrons/fake_vs_pT_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("pion") )
      c->SaveAs(Form((figuresFolder + "/pions/fake_vs_pT_minNhits%i.eps"),minNhits));

    // auto legend0 = new TLegend(0.659148,0.713018,0.873434,0.862426);
    // legend0->SetTextSize(0.03);
    // if( inputFileName1.Contains("muon") ) legend0->SetHeader("Single #mu^{-}");
    // else if( inputFileName1.Contains("electron") ) legend0->SetHeader("Single e^{-}");
    // else if( inputFileName1.Contains("pion") ) legend0->SetHeader("Single #pi^{-}");
    // legend0->AddEntry(h_efficiency_pt_1,"#theta = 10#circ","ep");
    // legend0->AddEntry(h_efficiency_pt_2,"#theta = 30#circ","ep");
    // legend0->AddEntry(h_efficiency_pt_3,"#theta = 89#circ","ep");
    // legend0->Draw();
  }

  if(vsThetaPhi){
    
    // file1: 1 gev

    Int_t nentries_1 = t_per_1->GetEntries();

    TH1D *h_fake_theta_1 = new TH1D("h_fake_theta_1","h_fake_theta_1",83,7,90);
    h_fake_theta_1->Sumw2();
    TH1D *h_reconstructed_theta_1 = new TH1D("h_reconstructed_theta_1","h_reconstructed_theta_1",83,7,90);
    h_reconstructed_theta_1->Sumw2();
    TH1D *h_fake_phi_1 = new TH1D("h_fake_phi_1","h_fake_phi_1",180,-180,180);
    h_fake_phi_1->Sumw2();
    TH1D *h_reconstructed_phi_1 = new TH1D("h_reconstructed_phi_1","h_reconstructed_phi_1",180,-180,180);
    h_reconstructed_phi_1->Sumw2();

    for(Int_t i=0; i < nentries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_theta_1->GetEntry(per_entry);
      b_trk_phi_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);
      b_mc_vtxR_1->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_1->size(); j++){ //loop over tracks
        if(mc_vtxR_1->at(j) == 0 && trk_nhits_1->at(j) > minNhits) {

          h_reconstructed_theta_1->Fill(trk_theta_1->at(j));
          h_reconstructed_phi_1->Fill(trk_phi_1->at(j));
          if(trk_purity_1->at(j) < purityMin) {
            h_fake_theta_1->Fill(trk_theta_1->at(j));
            h_fake_phi_1->Fill(trk_phi_1->at(j));
          }
        } else {
          //std::cout << "1 GeV --> Displaced vertex: " << mc_vtxR_1->at(j) << std::endl;
        }
      }

    }

   std::cout << "Entries fake: " << h_fake_theta_1->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_theta_1->GetEntries() << std::endl;

    TEfficiency *h_efficiency_theta_1 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_theta_1,*h_reconstructed_theta_1)){
      h_efficiency_theta_1 = new TEfficiency(*h_fake_theta_1,*h_reconstructed_theta_1);
    }

    TEfficiency *h_efficiency_phi_1 = 0;  
    if(TEfficiency::CheckConsistency(*h_fake_phi_1,*h_reconstructed_phi_1)){
      h_efficiency_phi_1 = new TEfficiency(*h_fake_phi_1,*h_reconstructed_phi_1);
    }

    // 10 gev

    Int_t nentries_2 = t_per_2->GetEntries();

    TH1D *h_fake_theta_2 = new TH1D("h_fake_theta_2","h_fake_theta_2",83,7,90);
    h_fake_theta_2->Sumw2();
    TH1D *h_reconstructed_theta_2 = new TH1D("h_reconstructed_theta_2","h_reconstructed_theta_2",83,7,90);
    h_reconstructed_theta_2->Sumw2();
    TH1D *h_fake_phi_2 = new TH1D("h_fake_phi_2","h_fake_phi_2",180,-180,180);
    h_fake_phi_2->Sumw2();
    TH1D *h_reconstructed_phi_2 = new TH1D("h_reconstructed_phi_2","h_reconstructed_phi_2",180,-180,180);
    h_reconstructed_phi_2->Sumw2();

    for(Int_t i=0; i < nentries_2; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_theta_2->GetEntry(per_entry);
      b_trk_phi_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);
      b_mc_vtxR_2->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_2->size(); j++){ //loop over tracks
        if(mc_vtxR_2->at(j) == 0 && trk_nhits_2->at(j) > minNhits) {

          h_reconstructed_theta_2->Fill(trk_theta_2->at(j));
          h_reconstructed_phi_2->Fill(trk_phi_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_theta_2->Fill(trk_theta_2->at(j));
            h_fake_phi_2->Fill(trk_phi_2->at(j));
          }
        } else {
          //std::cout << "10 GeV --> Displaced vertex: " << mc_vtxR_2->at(j) << std::endl;
        }
      }

    }

   std::cout << "Entries fake: " << h_fake_theta_2->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_theta_2->GetEntries() << std::endl;

    TEfficiency *h_efficiency_theta_2 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_theta_2,*h_reconstructed_theta_2)){
      h_efficiency_theta_2 = new TEfficiency(*h_fake_theta_2,*h_reconstructed_theta_2);
    }

    TEfficiency *h_efficiency_phi_2 = 0;  
    if(TEfficiency::CheckConsistency(*h_fake_phi_2,*h_reconstructed_phi_2)){
      h_efficiency_phi_2 = new TEfficiency(*h_fake_phi_2,*h_reconstructed_phi_2);
    }

    // 100 gev

    Int_t nentries_3 = t_per_3->GetEntries();

    TH1D *h_fake_theta_3 = new TH1D("h_fake_theta_3","h_fake_theta_3",83,7,90);
    h_fake_theta_3->Sumw2();
    TH1D *h_reconstructed_theta_3 = new TH1D("h_reconstructed_theta_3","h_reconstructed_theta_3",83,7,90);
    h_reconstructed_theta_3->Sumw2();
    TH1D *h_fake_phi_3 = new TH1D("h_fake_phi_3","h_fake_phi_3",180,-180,180);
    h_fake_phi_3->Sumw2();
    TH1D *h_reconstructed_phi_3 = new TH1D("h_reconstructed_phi_3","h_reconstructed_phi_3",180,-180,180);
    h_reconstructed_phi_3->Sumw2();

    for(Int_t i=0; i < nentries_3; i++){

      Long64_t pur_entry = t_pur_3->LoadTree(i);
      b_trk_purity_3->GetEntry(pur_entry);

      Long64_t per_entry = t_per_3->LoadTree(i);
      b_trk_theta_3->GetEntry(per_entry);
      b_trk_phi_3->GetEntry(per_entry);
      b_trk_nhits_3->GetEntry(per_entry);
      b_mc_vtxR_3->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_3->size(); j++){ //loop over tracks
        if(mc_vtxR_3->at(j) == 0 && trk_nhits_3->at(j) > minNhits) {

          h_reconstructed_theta_3->Fill(trk_theta_3->at(j));
          h_reconstructed_phi_3->Fill(trk_phi_3->at(j));
          if(trk_purity_3->at(j) < purityMin) {
            h_fake_theta_3->Fill(trk_theta_3->at(j));
            h_fake_phi_3->Fill(trk_phi_3->at(j));
          }
        } else {
          //std::cout << "100 GeV --> Displaced vertex: " << mc_vtxR_3->at(j) << std::endl;
        }
      }

    }

   std::cout << "Entries fake: " << h_fake_theta_3->GetEntries() << std::endl;
   std::cout << "Entries reconstructed: " << h_reconstructed_theta_3->GetEntries() << std::endl;

    TEfficiency *h_efficiency_theta_3 = 0;
    if(TEfficiency::CheckConsistency(*h_fake_theta_3,*h_reconstructed_theta_3)){
      h_efficiency_theta_3 = new TEfficiency(*h_fake_theta_3,*h_reconstructed_theta_3);
    }

    TEfficiency *h_efficiency_phi_3 = 0;  
    if(TEfficiency::CheckConsistency(*h_fake_phi_3,*h_reconstructed_phi_3)){
      h_efficiency_phi_3 = new TEfficiency(*h_fake_phi_3,*h_reconstructed_phi_3);
    }

    TCanvas *c = new TCanvas();
    c->SetGrid();
    c->SetLogy();
    h_efficiency_theta_3->SetTitle(";Reconstructed track #theta [#circ];Fake rate");
    h_efficiency_theta_3->SetLineColor(kBlue);
    h_efficiency_theta_3->SetMarkerColor(kBlue);
    h_efficiency_theta_3->SetMarkerStyle(20);
    h_efficiency_theta_3->Draw();
    h_efficiency_theta_2->SetLineColor(kRed);
    h_efficiency_theta_2->SetMarkerColor(kRed);
    h_efficiency_theta_2->SetMarkerStyle(21);
    h_efficiency_theta_2->Draw("same");
    h_efficiency_theta_1->SetLineColor(kBlack);
    h_efficiency_theta_1->SetMarkerColor(kBlack);
    h_efficiency_theta_1->SetMarkerStyle(22);
    h_efficiency_theta_1->Draw("same");
    gPad->Update();
    auto graph_3 = h_efficiency_theta_3->GetPaintedGraph();
    graph_3->GetXaxis()->SetTitleOffset(1.2);
    graph_3->SetMinimum(1e-5);
    graph_3->SetMaximum(1.);
    graph_3->GetXaxis()->SetLimits(7,90);
    auto graph_2 = h_efficiency_theta_2->GetPaintedGraph();
    auto graph_1 = h_efficiency_theta_1->GetPaintedGraph();
    gPad->Update();

    TLatex *text = new TLatex();
    text->SetTextSize(0.035);
    text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

    auto legend0 = new TLegend(0.657895,0.715976,0.87218,0.865385);
    legend0->SetTextSize(0.03);
    if( inputFileName1.Contains("muon") ) legend0->SetHeader("Single #mu^{-}");
    else if( inputFileName1.Contains("ele") ) legend0->SetHeader("Single e^{-}");
    else if( inputFileName1.Contains("pion") ) legend0->SetHeader("Single #pi^{-}");
    legend0->AddEntry(h_efficiency_theta_1,"p_{T} = 1 GeV","ep");
    legend0->AddEntry(h_efficiency_theta_2,"p_{T} = 10 GeV","ep");
    legend0->AddEntry(h_efficiency_theta_3,"p_{T} = 100 GeV","ep");
    legend0->Draw();
 
    if( inputFileName1.Contains("muon") )
      c->SaveAs(Form((figuresFolder + "/muons/fake_vs_theta_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("ele") )
      c->SaveAs(Form((figuresFolder + "/electrons/fake_vs_theta_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("pion") )
      c->SaveAs(Form((figuresFolder + "/pions/fake_vs_theta_minNhits%i.eps"),minNhits));

    TCanvas *c1 = new TCanvas();
    c1->SetGrid();
    c1->SetLogy();
    h_efficiency_phi_3->SetTitle(";Reconstructed track #phi [#circ];Fake rate");
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
    graph_3_1->SetMinimum(1e-5);
    graph_3_1->SetMaximum(1.);
    graph_3_1->GetXaxis()->SetLimits(-180,180);
    auto graph_2_1 = h_efficiency_phi_2->GetPaintedGraph();
    auto graph_1_1 = h_efficiency_phi_1->GetPaintedGraph();

    gPad->Update();
    TLatex *text1 = new TLatex();
    text1->SetTextSize(0.035);
    text1->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

    auto legend01 = new TLegend(0.636591,0.309172,0.850877,0.45858);
    legend01->SetTextSize(0.03);
    if( inputFileName1.Contains("muon") ) legend01->SetHeader("Single #mu^{-}");
    else if( inputFileName1.Contains("ele") ) legend01->SetHeader("Single e^{-}");
    else if( inputFileName1.Contains("pion") ) legend01->SetHeader("Single #pi^{-}");
    legend01->AddEntry(h_efficiency_phi_1,"p_{T} = 1 GeV","ep");
    legend01->AddEntry(h_efficiency_phi_2,"p_{T} = 10 GeV","ep");
    legend01->AddEntry(h_efficiency_phi_3,"p_{T} = 100 GeV","ep");
    legend01->Draw();
 
    if( inputFileName1.Contains("muon") )
      c->SaveAs(Form((figuresFolder + "/muons/fake_vs_phi_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("ele") )
      c->SaveAs(Form((figuresFolder + "/electrons/fake_vs_phi_minNhits%i.eps"),minNhits));
    else if( inputFileName1.Contains("pion") )
      c->SaveAs(Form((figuresFolder + "/pions/fake_vs_phi_minNhits%i.eps"),minNhits));
  }


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
void BinLogX(TH2D* h){
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
