#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"

void phi_distributions(char *inputFileName, char *outputFileName, char *particleType, char *en, char *angle){

  std::string particle(particleType,15);
  std::string muon("muon");
  std::string electron("electron");
  std::string pion("pion");

  int pID = 0;
  if( particle.find(muon) != -1)
    pID = 13;
  else if( particle.find(electron) != -1 )
    pID = 11;
  else if( particle.find(pion) != -1 )
    pID = -211;

  TFile *f = TFile::Open(inputFileName,"READ");
  if(!f) return;

  TTree *t;
  f->GetObject("perfTree",t);

  std::vector<int > *trueID = 0;
  std::vector<double > *truePhi = 0;
  std::vector<double > *recoPhi = 0;

  TBranch *b_trueID = 0;
  t->SetBranchAddress("trueID",&trueID,&b_trueID);

  TBranch *b_truePhi = 0;
  t->SetBranchAddress("truePhi",&truePhi,&b_truePhi);

  TBranch *b_recoPhi = 0;
  t->SetBranchAddress("recoPhi",&recoPhi,&b_recoPhi);

  Int_t nentries = t->GetEntries();

  double min = -1;
  double max = 1;

  TH1D *phiHisto = new TH1D("phiHisto","phiHisto",10000,min,max);

  for(Int_t i = 0; i < nentries; i++){ //loop over events

    Long64_t tentry = t->LoadTree(i);
    b_trueID->GetEntry(tentry);
    b_truePhi->GetEntry(tentry);
    b_recoPhi->GetEntry(tentry); 

    for(UInt_t j = 0; j < truePhi->size(); j++){ //loop over tracks
      if(trueID->at(j) == pID){ // fill only if particleType of the right type (matching inputFile)
	phiHisto->Fill(recoPhi->at(j)-truePhi->at(j));
      }
    } //end loop over tracks
    
  }// end loop over events

  
  while(phiHisto->GetBinContent(phiHisto->GetMaximumBin()) < 50){
    phiHisto->Rebin(2);
  }

    float factor = 3;

    //ptHisto->GetXaxis()->SetRangeUser(min,max);

  TF1 *fit = new TF1("fit","gaus(0)");
  fit->SetParLimits(1,-0.1*phiHisto->GetRMS(),0.1*phiHisto->GetRMS());
  fit->SetParLimits(2,0,0.1*phiHisto->GetRMS());
  phiHisto->Fit(fit,"RQ","",-factor*phiHisto->GetRMS(),factor*phiHisto->GetRMS());
    
  sigma = fit->GetParameter(2);
  sigmaErr = fit->GetParError(2);

  TTree *t_out = new TTree("fitPar","fitPar");
  t_out->Branch("sigma",&sigma,"sigma/D"); 
  t_out->Branch("sigmaErr",&sigmaErr,"sigmaErr/D");

  t_out->Fill();  

  TFile *f_out = TFile::Open(outputFileName,"UPDATE");
  TDirectory *cd_out = f_out->mkdir(Form("%s%s",en,angle));
  cd_out->cd();
  phiHisto->Write();    
  t_out->Write();
  f_out->Close();
    
}
