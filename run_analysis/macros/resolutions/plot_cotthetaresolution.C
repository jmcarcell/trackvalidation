#include "/afs/cern.ch/user/e/eleogran/CLICdpStyle.C"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"

using namespace std;

void plot_cotthetaresolution(char *inputFileName){

  CLICdpStyle();

  const int nAngles = 9;
  double angles[nAngles] = {10,20,30,40,50,60,70,80,89};
  double anglesErr[nAngles] = {0};

  std::string str1("1GeV");
  std::string str10("10GeV");
  std::string str100("100GeV");
  double resolutions_1GeV[nAngles] = {0};
  double resolutionsErr_1GeV[nAngles] = {0};
  double resolutions_10GeV[nAngles] = {0};
  double resolutionsErr_10GeV[nAngles] = {0};
  double resolutions_100GeV[nAngles] = {0};
  double resolutionsErr_100GeV[nAngles] = {0};
  int iter1 = 0;
  int iter10 = 0;
  int iter100 = 0;

  TFile *f = TFile::Open(inputFileName,"READ");
  TIter next(f->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())){
    const char *nameDir = key->GetName();
    string stringDir(nameDir,15);
    if( stringDir.find(str1) != -1){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_1GeV[iter1] = sigma * 17.4533;
	resolutionsErr_1GeV[iter1] = sigmaErr * 17.4533;
	iter1++;
      }   
    }
    else if( stringDir.find(str10) != -1){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_10GeV[iter10] = sigma * 17.4533;
	resolutionsErr_10GeV[iter10] = sigmaErr * 17.4533;
	iter10++;
      }   
    }
    else if( stringDir.find(str100) != -1){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_100GeV[iter100] = sigma * 17.4533;
	resolutionsErr_100GeV[iter100] = sigmaErr * 17.4533;
	iter100++;
      }   
    }
  }

  TGraphErrors *gr_1GeV = new TGraphErrors(nAngles,angles,resolutions_1GeV,anglesErr,resolutionsErr_1GeV);  
  TGraphErrors *gr_10GeV = new TGraphErrors(nAngles,angles,resolutions_10GeV,anglesErr,resolutionsErr_10GeV);
  TGraphErrors *gr_100GeV = new TGraphErrors(nAngles,angles,resolutions_100GeV,anglesErr,resolutionsErr_100GeV);


  TCanvas *c = new TCanvas("c","c",900,800);
  TPad *pad1 = new TPad();
  c->cd();
  pad1->SetGrid();
  pad1->SetLogy();
  pad1->SetTicks();
  pad1->SetLeftMargin(0.2);
  pad1->Draw();
  pad1->cd();
  gr_1GeV->SetTitle("");
  gr_1GeV->GetXaxis()->SetTitle("#theta [#circ]");
  gr_1GeV->GetYaxis()->SetTitle("Resolution in cot(#theta) [mrad]");
  gr_1GeV->GetYaxis()->SetTitleOffset(1.3);
  gr_1GeV->Draw("ap");
  gr_1GeV->SetMaximum(2e0);
  gr_1GeV->SetMinimum(7e-4);
  gr_1GeV->SetMarkerColor(kBlack);
  gr_1GeV->SetLineColor(kBlack);
  gr_1GeV->SetMarkerStyle(20);
  gr_1GeV->SetMarkerSize(1.2);
  gr_10GeV->Draw("psame");
  gr_10GeV->SetMarkerColor(kRed);
  gr_10GeV->SetLineColor(kRed);
  gr_10GeV->SetMarkerStyle(21);
  gr_10GeV->SetMarkerSize(1.2);
  gr_100GeV->Draw("psame");
  gr_100GeV->SetMarkerColor(kBlue);
  gr_100GeV->SetLineColor(kBlue);
  gr_100GeV->SetMarkerStyle(22);
  gr_100GeV->SetMarkerSize(1.2);

  std::string inputFile(inputFileName,50);
  std::string muon("muon");
  std::string electron("electron");
  std::string pion("pion");

  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.205, 0.939349, "CLICdp"); // work in progress");

  auto legend0 = new TLegend(0.613586,0.688144,0.825167,0.877577);
  legend0->SetTextSize(0.03);
  if( inputFile.find(muon) != -1 ) legend0->SetHeader("Single #mu^{-}");
  else if( inputFile.find(electron) != -1 ) legend0->SetHeader("Single e^{-}");
  else if( inputFile.find(pion) != -1 ) legend0->SetHeader("Single #pi^{-}");
  legend0->AddEntry(gr_1GeV,"p_{T} = 1 GeV","p");
  legend0->AddEntry(gr_10GeV,"p_{T} = 10 GeV","p");
  legend0->AddEntry(gr_100GeV,"p_{T} = 100 GeV","p");
  legend0->Draw();

  
  if( inputFile.find(muon) != -1 )
    c->SaveAs("figures/muons/cotthetaRes_vs_theta.eps");
  else if( inputFile.find(electron) != -1 )
    c->SaveAs("figures/electrons/cotthetaRes_vs_theta.eps");
  else if( inputFile.find(pion) != -1 )
    c->SaveAs("figures/pions/cotthetaRes_vs_theta.eps");
}
