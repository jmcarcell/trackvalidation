#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"

void d0_distributions(char *inputFileName, char *outputFileName, char *particleType, char *en, char *angle, int mom){

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
  std::vector<double > *trueD0 = 0;
  std::vector<double > *recoD0 = 0;

  TBranch *b_trueID = 0;
  t->SetBranchAddress("trueID",&trueID,&b_trueID);

  TBranch *b_trueD0 = 0;
  t->SetBranchAddress("trueD0",&trueD0,&b_trueD0);

  TBranch *b_recoD0 = 0;
  t->SetBranchAddress("recoD0",&recoD0,&b_recoD0);

  Int_t nentries = t->GetEntries();

  double min = -0.02;
  double max = 0.02;

 if(mom == 1 || mom == 2){
    std::cout << "reset bin" << std::endl;
    min = -1;
    max = 1;
  }

  TH1D *d0Histo = new TH1D("d0Histo","d0Histo",10000,min,max);

  for(Int_t i = 0; i < nentries; i++){ //loop over events

    Long64_t tentry = t->LoadTree(i);
    b_trueID->GetEntry(tentry);
    b_trueD0->GetEntry(tentry);
    b_recoD0->GetEntry(tentry); 

    for(UInt_t j = 0; j < trueD0->size(); j++){ //loop over tracks
      if(trueID->at(j) == pID){ // fill only if particleType of the right type (matching inputFile)
	d0Histo->Fill(recoD0->at(j)-trueD0->at(j));
      }
    } //end loop over tracks
    
  }// end loop over events


  while(d0Histo->GetBinContent(d0Histo->GetMaximumBin()) < 50){
    d0Histo->Rebin(2);
  }

    float factor = 3;

    //ptHisto->GetXaxis()->SetRangeUser(min,max);

  TF1 *fit = new TF1("fit","gaus(0)");
  fit->SetParLimits(1,-0.1*d0Histo->GetRMS(),0.1*d0Histo->GetRMS());
  fit->SetParLimits(2,0,0.1*d0Histo->GetRMS());
  d0Histo->Fit(fit,"RQ","",-factor*d0Histo->GetRMS(),factor*d0Histo->GetRMS());
    
  sigma = fit->GetParameter(2);
  sigmaErr = fit->GetParError(2);

  TTree *t_out = new TTree("fitPar","fitPar");
  t_out->Branch("sigma",&sigma,"sigma/D"); 
  t_out->Branch("sigmaErr",&sigmaErr,"sigmaErr/D");

  t_out->Fill();  

  TFile *f_out = TFile::Open(outputFileName,"UPDATE");
  TDirectory *cd_out = f_out->mkdir(Form("%s%s",en,angle));
  cd_out->cd();
  d0Histo->Write();    
  t_out->Write();
  f_out->Close();
    
}
