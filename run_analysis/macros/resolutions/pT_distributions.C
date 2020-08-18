#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <string>

void pT_distributions(char *inputFileName, char *outputFileName, char *particleType, char *en, char *angle, const int momentum){

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
  std::vector<double > *truePt = 0;
  std::vector<double > *recoPt = 0;

  TBranch *b_trueID = 0;
  t->SetBranchAddress("trueID",&trueID,&b_trueID);

  TBranch *b_truePt = 0;
  t->SetBranchAddress("truePt",&truePt,&b_truePt);

  TBranch *b_recoPt = 0;
  t->SetBranchAddress("recoPt",&recoPt,&b_recoPt);

  Int_t nentries = t->GetEntries();

  double min = 0;
  double max = 0;

  if(momentum >= 500){
    min = -0.0002;
    max = 0.0002;
  }
  else if(momentum > 5){
    min = -0.002;
    max = 0.002;
  }
  else {
    min = -0.02;
    max = 0.02;
  }

  TH1D *ptHisto = new TH1D("ptHisto","ptHisto",10000,min,max);

  for(Int_t i = 0; i < nentries; i++){ //loop over events

    Long64_t tentry = t->LoadTree(i);
    b_trueID->GetEntry(tentry);
    b_truePt->GetEntry(tentry);
    b_recoPt->GetEntry(tentry); 

    for(UInt_t j = 0; j < truePt->size(); j++){ //loop over tracks
      if(trueID->at(j) == pID){ // fill only if particleType of the right type (matching inputFile)
	ptHisto->Fill((recoPt->at(j)-truePt->at(j))/(truePt->at(j)*truePt->at(j)));
      }
    } //end loop over tracks
    
  }// end loop over events


  while(ptHisto->GetBinContent(ptHisto->GetMaximumBin()) < 50){
    ptHisto->Rebin(2);
  }


  float factor = 3;
  //if(momentum>=200) factor = 1;
  //else factor = 3;

    //ptHisto->GetXaxis()->SetRangeUser(min,max);

  TF1 *fit = new TF1("fit","gaus(0)");
  fit->SetParLimits(1,-0.1*ptHisto->GetRMS(),0.1*ptHisto->GetRMS());
  fit->SetParLimits(2,0,0.1*ptHisto->GetRMS());
  ptHisto->Fit(fit,"RQ","",-factor*ptHisto->GetRMS(),factor*ptHisto->GetRMS());
    
  double sigma = fit->GetParameter(2);
  double sigmaErr = fit->GetParError(2);

  TTree *t_out = new TTree("fitPar","fitPar");
  t_out->Branch("sigma",&sigma,"sigma/D");
  t_out->Branch("sigmaErr",&sigmaErr,"sigmaErr/D"); 

  t_out->Fill();  

  TFile *f_out = TFile::Open(outputFileName,"UPDATE");
  TDirectory *cd_out = f_out->mkdir(Form("%s%s",en,angle));
  cd_out->cd();
  ptHisto->Write();    
  t_out->Write();
  f_out->Close();
    
}
