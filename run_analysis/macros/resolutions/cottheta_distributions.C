#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

void cottheta_distributions(char *inputFileName, char *outputFileName, char *particleType, char *en, char *angle){

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
  std::vector<double > *trueTheta = 0;
  std::vector<double > *recoTheta = 0;

  TBranch *b_trueID = 0;
  t->SetBranchAddress("trueID",&trueID,&b_trueID);

  TBranch *b_trueTheta = 0;
  t->SetBranchAddress("trueTheta",&trueTheta,&b_trueTheta);

  TBranch *b_recoTheta = 0;
  t->SetBranchAddress("recoTheta",&recoTheta,&b_recoTheta);

  Int_t nentries = t->GetEntries();

  double min = -1;
  double max = 1;

  TH1D *thetaHisto = new TH1D("thetaHisto","thetaHisto",10000,min,max);

  for(Int_t i = 0; i < nentries; i++){ //loop over events

    Long64_t tentry = t->LoadTree(i);
    b_trueID->GetEntry(tentry);
    b_trueTheta->GetEntry(tentry);
    b_recoTheta->GetEntry(tentry); 

    for(UInt_t j = 0; j < trueTheta->size(); j++){ //loop over tracks
      if(trueID->at(j) == pID){ // fill only if particleType of the right type (matching inputFile)
	//thetaHisto->Fill(1./TMath::Tan(TMath::Pi()*(recoTheta->at(j)-trueTheta->at(j))/180.));
	thetaHisto->Fill(1./TMath::Tan(TMath::Pi()*recoTheta->at(j)/180.) - 1./TMath::Tan(TMath::Pi()*trueTheta->at(j)/180.));
      }
    } //end loop over tracks
    
  }// end loop over events

  /*
  while(thetaHisto->GetBinContent(thetaHisto->GetMaximumBin()) < 50){
    thetaHisto->Rebin(2);
  }
*/
  float factor = 3;
    //ptHisto->GetXaxis()->SetRangeUser(min,max);

  TF1 *fit = new TF1("fit","gaus(0)");
  fit->SetParLimits(1,-0.1*thetaHisto->GetRMS(),0.1*thetaHisto->GetRMS());
  fit->SetParLimits(2,0,0.1*thetaHisto->GetRMS());
  thetaHisto->Fit(fit,"RQ","",-factor*thetaHisto->GetRMS(),factor*thetaHisto->GetRMS());
    
  sigma = fit->GetParameter(2);
  sigmaErr = fit->GetParError(2);

  TTree *t_out = new TTree("fitPar","fitPar");
  t_out->Branch("sigma",&sigma,"sigma/D"); 
  t_out->Branch("sigmaErr",&sigmaErr,"sigmaErr/D");

  t_out->Fill();  

  TFile *f_out = TFile::Open(outputFileName,"UPDATE");
  TDirectory *cd_out = f_out->mkdir(Form("%s%s",en,angle));
  cd_out->cd();
  thetaHisto->Write();    
  t_out->Write();
  f_out->Close();
    
}
