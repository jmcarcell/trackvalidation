//----------------------------------------------------------------
////
//// macro to plot tracking fakerate of complex events
//// needs 2 input files (without, with background overlay) 
//// input file example: /eos/experiment/clicdp/grid/ilc/user/e/ericabro/CLIC/2019/CLICo3v14/ILCSoft-2019-07-09/efficiencies/merged_ttbar3TeV_NoOverlay_conformal.root 
//// can plot Z->uds@500GeV, bbbar@3TeV and ttbar@3TeV,380GeV
//// can plot vs pt, theta, phi, vertex radius
//// author: Erica Brondolin
////
////----------------------------------------------------------------
//
#include "../CLICdpStyle.C"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <string>

const double purityMin = 0.75;
const double thetaMin = 10;
const double thetaMax = 170;
const double ptMin = 1;

TString path = "";
TString figuresFolder = "../figures/";

TString treeName1 = "MyClicEfficiencyCalculator/puritytree";
TString treeName2 = "MyClicEfficiencyCalculator/perfTree";

TString vsPt("pt");
TString vsTheta("theta");
TString vsPhi("phi");
TString vsNHits("nhits");

void BinLogX(TH1D *);
void BinLogX(TH2D *);
void BinLogY(TH2D *);

void plot_fakesComplexEvents(TString file1 = "merged_ttbar3TeV_NoOverlay_conformal.root", TString file2 = "merged_ttbar3TeV_Overlay_conformal.root", TString vsWhat = "pt", int minNhits = 4, bool debug = true){

  CLICdpStyle();
  TString inputFileName1 = path + file1;
  TString inputFileName2 = path + file2;

  if( debug ){
    std::cout << "Input files: " << std::endl;
    std::cout << inputFileName1 << std::endl;
    std::cout << inputFileName2 << std::endl;
  }

  // input files and trees

  TFile *f_1 = TFile::Open(inputFileName1,"READ");
  if(!f_1) return;
  TFile *f_2 = TFile::Open(inputFileName2,"READ");
  if(!f_2) return;

  TTree *t_pur_1;
  f_1->GetObject(treeName1,t_pur_1);
  TTree *t_pur_2;
  f_2->GetObject(treeName1,t_pur_2);

  TTree *t_per_1;
  f_1->GetObject(treeName2,t_per_1);
  TTree *t_per_2;
  f_2->GetObject(treeName2,t_per_2);

  // check events number in trees

  if(t_pur_1->GetEntries() != t_per_1->GetEntries()){
    std::cerr << "Different number of entries in " << inputFileName1 << std::endl;
  }
  if(t_pur_2->GetEntries() != t_per_2->GetEntries()){
    std::cerr << "Different number of entries in " << inputFileName2 << std::endl;
  }

  // var initialization  
  vector<double> *trk_purity_1 = 0;
  vector<double> *trk_pt_1 = 0;
  vector<double> *trk_theta_1 = 0;
  vector<double> *trk_phi_1 = 0;  
  vector<double> *trk_nhits_1 = 0;
  vector<double> *mc_simPt_1 = 0;
  vector<double> *mc_simTheta_1 = 0;

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
  TBranch *b_mc_simPt_1 = 0;
  t_per_1->SetBranchAddress("truePt",&mc_simPt_1,&b_mc_simPt_1);
  TBranch *b_mc_simTheta_1 = 0;
  t_per_1->SetBranchAddress("trueTheta",&mc_simTheta_1,&b_mc_simTheta_1);

  vector<double> *trk_purity_2 = 0;
  vector<double> *trk_pt_2 = 0;
  vector<double> *trk_theta_2 = 0;
  vector<double> *trk_phi_2 = 0;
  vector<double> *trk_nhits_2 = 0;

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

  TH1D *h_total_1;
  TH1D *h_fake_1;

  Int_t nEntries_1 = t_pur_1->GetEntries();

  TH1D *h_total_2;
  TH1D *h_fake_2;

  Int_t nEntries_2 = t_pur_2->GetEntries();

  std::cout << t_pur_1->GetEntries() << std::endl;
  std::cout << t_per_1->GetEntries() << std::endl;
  std::cout << t_pur_2->GetEntries() << std::endl;
  std::cout << t_per_2->GetEntries() << std::endl;

  for(Int_t i=0; i < 10; i++){
    Long64_t pur_entry = t_pur_1->LoadTree(i);
    b_trk_purity_1->GetEntry(pur_entry);

    Long64_t per_entry = t_per_1->LoadTree(i);
    b_trk_pt_1->GetEntry(per_entry);
    b_trk_theta_1->GetEntry(per_entry);
    b_trk_nhits_1->GetEntry(per_entry);
    b_mc_simPt_1->GetEntry(per_entry);
    b_mc_simTheta_1->GetEntry(per_entry);
 
    for(UInt_t j = 0; j < trk_purity_1->size(); j++){
    } 
  }


 
  //vs pt
  if(vsWhat.Contains(vsPt)){

    h_total_1 = new TH1D("h_total_1","h_total_1",100,-1,3);
    h_total_1->Sumw2();
    BinLogX(h_total_1);
    h_fake_1 = new TH1D("h_fake_1","h_fake_1",100,-1,3);
    h_fake_1->Sumw2();
    BinLogX(h_fake_1);
    h_total_2 = new TH1D("h_total_2","h_total_2",100,-1,3);
    h_total_2->Sumw2();
    BinLogX(h_total_2);
    h_fake_2 = new TH1D("h_fake_2","h_fake_2",100,-1,3);
    h_fake_2->Sumw2();
    BinLogX(h_fake_2);

	//for(Int_t i=0; i < 1; i++){
	for(Int_t i=0; i < nEntries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_pt_1->GetEntry(per_entry);
      b_trk_theta_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);
      b_mc_simPt_1->GetEntry(per_entry);
      b_mc_simTheta_1->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_1->size(); j++){ //loop over tracks
      	if(trk_theta_1->at(j) > thetaMin && trk_theta_1->at(j) < thetaMax && trk_nhits_1->at(j) >= minNhits){
          h_total_1->Fill(trk_pt_1->at(j));
          if(trk_purity_1->at(j) < purityMin) {
            h_fake_1->Fill(trk_pt_1->at(j));            
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }
    std::cout << "Entries fake: " << h_fake_1->GetEntries() << std::endl;
    std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;

    //for(Int_t i=0; i < 1; i++){
    for(Int_t i=0; i < nEntries_2; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_pt_2->GetEntry(per_entry);
      b_trk_theta_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_2->size(); j++){ //loop over tracks
      	if(trk_theta_2->at(j) > thetaMin && trk_theta_2->at(j) < thetaMax && trk_nhits_2->at(j) >= minNhits){
          h_total_2->Fill(trk_pt_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_2->Fill(trk_pt_2->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }

    std::cout << "Entries fake: " << h_fake_2->GetEntries() << std::endl;
    std::cout << "Entries reconstructed: " << h_total_2->GetEntries() << std::endl;

  }
  
  //vs theta
  else if(vsWhat.Contains(vsTheta)){

    h_total_1 = new TH1D("h_total_1","h_total_1",83,7,90);
    h_total_1->Sumw2();
    h_fake_1 = new TH1D("h_fake_1","h_fake_1",83,7,90);
    h_fake_1->Sumw2();
    h_total_2 = new TH1D("h_total_2","h_total_2",83,7,90);
    h_total_2->Sumw2();
    h_fake_2 = new TH1D("h_fake_2","h_fake_2",83,7,90);
    h_fake_2->Sumw2();

    //for(Int_t i=0; i < 10; i++){
	for(Int_t i=0; i < nEntries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_pt_1->GetEntry(per_entry);
      b_trk_theta_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);
      b_mc_simPt_1->GetEntry(per_entry);
      b_mc_simTheta_1->GetEntry(per_entry);


      for(UInt_t j = 0; j < trk_theta_1->size(); j++){ //loop over tracks
      	if(trk_pt_1->at(j) > ptMin && trk_nhits_1->at(j) >= minNhits){
          h_total_1->Fill(trk_theta_1->at(j));
          if(trk_purity_1->at(j) <purityMin) {
            h_fake_1->Fill(trk_theta_1->at(j));
//      if(j < 5)
  //          std::cout << "is a fake!" << std::endl;
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }
   	//std::cout << "Entries fake: " << h_fake_1->GetEntries() << std::endl;
   	//std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;

    }
   	std::cout << "Entries fake: " << h_fake_1->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;

    for(Int_t i=0; i < nEntries_2; i++){
    //for(Int_t i=0; i < 10; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_pt_2->GetEntry(per_entry);
      b_trk_theta_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);

      
      for(UInt_t j = 0; j < trk_theta_2->size(); j++){ //loop over tracks
      	if(trk_pt_2->at(j) > ptMin && trk_nhits_2->at(j) >= minNhits){
          h_total_2->Fill(trk_theta_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_2->Fill(trk_theta_2->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }
    //  std::cout << "Entries fake: " << h_fake_2->GetEntries() << std::endl;
   //   std::cout << "Entries reconstructed: " << h_total_2->GetEntries() << std::endl;

    }

   	std::cout << "Entries fake: " << h_fake_2->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_2->GetEntries() << std::endl;

  }
  
  //vs phi
  else if(vsWhat.Contains(vsPhi)){

    h_total_1 = new TH1D("h_total_1","h_total_1",360,-180,180);
    h_total_1->Sumw2();
    h_fake_1 = new TH1D("h_fake_1","h_fake_1",360,-180,180);
    h_fake_1->Sumw2();
    h_total_2 = new TH1D("h_total_2","h_total_2",360,-180,180);
    h_total_2->Sumw2();
    h_fake_2 = new TH1D("h_fake_2","h_fake_2",360,-180,180);
    h_fake_2->Sumw2();
  	std:: cout << "No overlay"<< std::endl;

    for(Int_t i=0; i < nEntries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_pt_1->GetEntry(per_entry);
      b_trk_theta_1->GetEntry(per_entry);
      b_trk_phi_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_1->size(); j++){ //loop over tracks
      	if(trk_pt_1->at(j) > ptMin && trk_theta_1->at(j) > thetaMin && trk_theta_1->at(j) < thetaMax && trk_nhits_1->at(j) >= minNhits){
		  // std:: cout << trk_nhits_1->at(j) << ", ";
          h_total_1->Fill(trk_phi_1->at(j));
          if(trk_purity_1->at(j) < purityMin) {
            h_fake_1->Fill(trk_phi_1->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }
    std::cout << "Entries fake: " << h_fake_1->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;
  	std:: cout << "With overlay"<< std::endl;

    for(Int_t i=0; i < nEntries_2; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_pt_2->GetEntry(per_entry);
      b_trk_theta_2->GetEntry(per_entry);
      b_trk_phi_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_2->size(); j++){ //loop over tracks
      	

      	if(trk_pt_2->at(j) > ptMin && trk_theta_2->at(j) > thetaMin && trk_theta_2->at(j) < thetaMax && trk_nhits_2->at(j) >= minNhits){
			if(trk_phi_2->at(j) > -1 && trk_phi_2->at(j) < 1){
	      		std::cout << trk_nhits_2->at(j) << std::endl;
		  	}
          h_total_2->Fill(trk_phi_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_2->Fill(trk_phi_2->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }
    std::cout << "Entries fake: " << h_fake_2->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_2->GetEntries() << std::endl;

  }

  //vs nHits
  else if(vsWhat.Contains(vsNHits)){
    h_total_1 = new TH1D("h_total_1","h_total_1",25,-0.5,24.5);
    h_total_1->Sumw2();
    h_fake_1 = new TH1D("h_fake_1","h_fake_1",25,-0.5,24.5);
    h_fake_1->Sumw2();
    h_total_2 = new TH1D("h_total_2","h_total_2",25,-0.5,24.5);
    h_total_2->Sumw2();
    h_fake_2 = new TH1D("h_fake_2","h_fake_2",25,-0.5,24.5);
    h_fake_2->Sumw2();

    for(Int_t i=0; i < nEntries_1; i++){

      Long64_t pur_entry = t_pur_1->LoadTree(i);
      b_trk_purity_1->GetEntry(pur_entry);

      Long64_t per_entry = t_per_1->LoadTree(i);
      b_trk_pt_1->GetEntry(per_entry);
      b_trk_theta_1->GetEntry(per_entry);
      b_trk_phi_1->GetEntry(per_entry);
      b_trk_nhits_1->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_1->size(); j++){ //loop over tracks
      	if(trk_pt_1->at(j) > ptMin && trk_theta_1->at(j) > thetaMin && trk_theta_1->at(j) < thetaMax){
          h_total_1->Fill(trk_nhits_1->at(j));
          if(trk_purity_1->at(j) < purityMin) {
            h_fake_1->Fill(trk_nhits_1->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }
    std::cout << "Entries fake: " << h_fake_1->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;

    for(Int_t i=0; i < nEntries_2; i++){

      Long64_t pur_entry = t_pur_2->LoadTree(i);
      b_trk_purity_2->GetEntry(pur_entry);

      Long64_t per_entry = t_per_2->LoadTree(i);
      b_trk_pt_2->GetEntry(per_entry);
      b_trk_theta_2->GetEntry(per_entry);
      b_trk_phi_2->GetEntry(per_entry);
      b_trk_nhits_2->GetEntry(per_entry);

      for(UInt_t j = 0; j < trk_purity_2->size(); j++){ //loop over tracks
      	if(trk_pt_2->at(j) > ptMin && trk_theta_2->at(j) > thetaMin && trk_theta_2->at(j) < thetaMax){
          h_total_2->Fill(trk_nhits_2->at(j));
          if(trk_purity_2->at(j) < purityMin) {
            h_fake_2->Fill(trk_nhits_2->at(j));
          }
        } else {
          //std::cout << "Not reconstructuble" << std::endl;
        }
      }

    }
    std::cout << "Entries fake: " << h_fake_2->GetEntries() << std::endl;
   	std::cout << "Entries reconstructed: " << h_total_2->GetEntries() << std::endl;

  }

  TEfficiency *h_efficiency_1 = 0;
  if(TEfficiency::CheckConsistency(*h_fake_1,*h_total_1)){
    h_efficiency_1 = new TEfficiency(*h_fake_1,*h_total_1);
  }
   
  TEfficiency *h_efficiency_2 = 0;
  if(TEfficiency::CheckConsistency(*h_fake_2,*h_total_2)){
    h_efficiency_2 = new TEfficiency(*h_fake_2,*h_total_2);
  }
 
  TCanvas *c = new TCanvas();
  c->SetGrid();
  c->SetLogy();
  if(vsWhat.Contains(vsPt)) c->SetLogx();
  if(vsWhat.Contains(vsPt)) h_efficiency_1->SetTitle(";Reconstructed track p_{T} [GeV];Fake rate");
  else if(vsWhat.Contains(vsTheta)) h_efficiency_1->SetTitle(";Reconstructed track #theta [#circ];Fake rate");
  else if(vsWhat.Contains(vsPhi)) h_efficiency_1->SetTitle(";Reconstructed track #phi [#circ];Fake rate");
  else if(vsWhat.Contains(vsNHits)) h_efficiency_1->SetTitle(";Reconstructed track n_{Hits};Fake rate");
  h_efficiency_1->SetLineColor(kBlue);//kRed
  h_efficiency_1->SetMarkerColor(kBlue);
  h_efficiency_1->SetMarkerStyle(20);//20
  h_efficiency_1->Draw("ap");
  h_efficiency_2->SetLineColor(kRed);
  h_efficiency_2->SetMarkerColor(kRed);
  h_efficiency_2->SetMarkerStyle(21);
  h_efficiency_2->Draw("samep");
  gPad->Update();
  auto graph = h_efficiency_1->GetPaintedGraph();
  graph->SetMinimum(1e-4);
  graph->SetMaximum(0.9);
  if(vsWhat.Contains(vsNHits)) graph->SetMaximum(1.0);
  graph->GetXaxis()->SetTitleOffset(1.2);
  graph->GetXaxis()->SetRangeUser(0.09,250);
  if(vsWhat.Contains(vsTheta)) graph->GetXaxis()->SetRangeUser(7,90);
  if(vsWhat.Contains(vsPhi)) graph->GetXaxis()->SetRangeUser(-180,180);

  std::string Zuds("Zuds");
  std::string ttbar("ttbar");
  std::string bbbar("bbbar");

  auto legend0 = new TLegend(0.318296,0.690828,0.893058,0.890533);
  legend0->SetTextSize(0.035);
  if(vsWhat.Contains(vsPhi)) {legend0 = new TLegend(0.254386, 0.688427, 0.659148, 0.890208); legend0->SetTextSize(0.04);}

  if(inputFileName1.Contains(Zuds)) legend0->SetHeader("Z#rightarrowq#bar{q} (q = u,d,s), m_{Z} = 500 GeV");
  else if(inputFileName1.Contains(bbbar)) legend0->SetHeader("b#bar{b}, E_{CM} =  3 TeV");
  else if(inputFileName1.Contains(ttbar) ){
    if(inputFileName1.Contains("TeV") ) {
      if(vsWhat.Contains(vsPt) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 3 TeV, %.0f#circ < #theta < %.0f#circ", thetaMin, thetaMax));
      else if(vsWhat.Contains(vsTheta) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 3 TeV, p_{T} > %.0f GeV", ptMin));
      else if(vsWhat.Contains(vsPhi) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 3 TeV, p_{T} > %.0f GeV, %.0f#circ < #theta < %.0f#circ", ptMin, thetaMin, thetaMax));
    }
    else if(inputFileName1.Contains("GeV") ){
      if(vsWhat.Contains(vsPt) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 380 GeV, %.0f#circ < #theta < %.0f#circ", thetaMin, thetaMax));
      else if(vsWhat.Contains(vsTheta) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 380 GeV, p_{T} > %.0f GeV", ptMin));
      else if(vsWhat.Contains(vsPhi) ) legend0->SetHeader(Form("t#bar{t}, E_{CM} = 380 GeV, p_{T} > %.0f GeV, %.0f#circ < #theta < %.0f#circ", ptMin, thetaMin, thetaMax));
    }
  }

  legend0->AddEntry(h_efficiency_1,"No background","ep");
  legend0->AddEntry(h_efficiency_2,"3 TeV #gamma#gamma#rightarrow hadrons background","ep");
  legend0->Draw();

  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.175, 0.939349, "CLICdp"); // work in progress");

  TString path_to_fig = figuresFolder;

  if(inputFileName1.Contains(Zuds)){
    path_to_fig = path_to_fig + "/Zuds500GeV";
  }
  else if(inputFileName1.Contains(ttbar)){
    path_to_fig = path_to_fig + "/ttbar3TeV";
  }
  if(inputFileName1.Contains(bbbar)){
    path_to_fig = path_to_fig + "/bbbar3TeV";
  }

  if(vsWhat.Contains(vsPt) )  c->SaveAs(path_to_fig+Form("/fake_vs_pt_minNhits%i.eps",minNhits));
  else if(vsWhat.Contains(vsTheta) )  c->SaveAs(path_to_fig+Form("/fake_vs_theta_minNhits%i.eps",minNhits));
  else if(vsWhat.Contains(vsPhi) ) c->SaveAs(path_to_fig + Form("/fake_vs_phi_minNhits%i.eps",minNhits));

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
void BinLogY(TH2D* h){
  TAxis *axis = h->GetYaxis();
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
