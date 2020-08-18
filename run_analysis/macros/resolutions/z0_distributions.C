#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"

void z0_distributions(char *inputFileName, char *outputFileName, char *particleType, char *en, char *angle, int momentum){

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
  std::vector<double > *trueZ0 = 0;
  std::vector<double > *recoZ0 = 0;

  TBranch *b_trueID = 0;
  t->SetBranchAddress("trueID",&trueID,&b_trueID);

  TBranch *b_trueZ0 = 0;
  t->SetBranchAddress("trueZ0",&trueZ0,&b_trueZ0);

  TBranch *b_recoZ0 = 0;
  t->SetBranchAddress("recoZ0",&recoZ0,&b_recoZ0);

  Int_t nentries = t->GetEntries();

  double min = -1;
  double max = 1;
  if(momentum >= 20){
    min = -0.2;
    max = 0.2;
  }

  TH1D *z0Histo = new TH1D("z0Histo","z0Histo",10000,min,max);

  for(Int_t i = 0; i < nentries; i++){ //loop over events

    Long64_t tentry = t->LoadTree(i);
    b_trueID->GetEntry(tentry);
    b_trueZ0->GetEntry(tentry);
    b_recoZ0->GetEntry(tentry); 

    for(UInt_t j = 0; j < trueZ0->size(); j++){ //loop over tracks
      if(trueID->at(j) == pID){ // fill only if particleType of the right type (matching inputFile)
	z0Histo->Fill(recoZ0->at(j)-trueZ0->at(j));
      }
    } //end loop over tracks
    
  }// end loop over events


  while(z0Histo->GetBinContent(z0Histo->GetMaximumBin()) < 50){
    z0Histo->Rebin(2);
  }

  float factor = 3; //1;

    //ptHisto->GetXaxis()->SetRangeUser(min,max);

  TF1 *fit = new TF1("fit","gaus(0)");
  fit->SetParLimits(1,-0.1*z0Histo->GetRMS(),0.1*z0Histo->GetRMS());
  fit->SetParLimits(2,0,0.1*z0Histo->GetRMS());
  z0Histo->Fit(fit,"RQ","",-factor*z0Histo->GetRMS(),factor*z0Histo->GetRMS());
    
  sigma = fit->GetParameter(2);
  sigmaErr = fit->GetParError(2);

  TTree *t_out = new TTree("fitPar","fitPar");
  t_out->Branch("sigma",&sigma,"sigma/D"); 
  t_out->Branch("sigmaErr",&sigmaErr,"sigmaErr/D");

  t_out->Fill();  

  TFile *f_out = TFile::Open(outputFileName,"UPDATE");
  TDirectory *cd_out = f_out->mkdir(Form("%s%s",en,angle));
  cd_out->cd();
  z0Histo->Write();    
  t_out->Write();
  f_out->Close();
    
}
