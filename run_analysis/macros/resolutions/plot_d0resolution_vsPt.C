#include "/afs/cern.ch/user/e/eleogran/CLICdpStyle.C"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <string>

using namespace std;

void plot_d0resolution_vsPt(char *inputFileName){

  CLICdpStyle();

  const int nEnergies = 11;
  double energies[nEnergies] = {1,2,5,10,20,50,100,200,500,1000,1500};
  double energiesErr[nEnergies] = {0};

  std::string str10("10deg");
  std::string str30("30deg");
  std::string str50("50deg"); 
  std::string str70("70deg");
  std::string str89("89deg");
  double resolutions_10deg[nEnergies] = {0};
  double resolutionsErr_10deg[nEnergies] = {0};
  double resolutions_30deg[nEnergies] = {0};
  double resolutionsErr_30deg[nEnergies] = {0};
  double resolutions_50deg[nEnergies] = {0};
  double resolutionsErr_50deg[nEnergies] = {0};
  double resolutions_70deg[nEnergies] = {0};
  double resolutionsErr_70deg[nEnergies] = {0};
  double resolutions_89deg[nEnergies] = {0};
  double resolutionsErr_89deg[nEnergies] = {0};
  int iter10 = 0;
  int iter30 = 0;
  int iter50 = 0;
  int iter70 = 0;
  int iter89 = 0;

  TFile *f = TFile::Open(inputFileName,"READ");
  TIter next(f->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())){
    const char *nameDir = key->GetName();
    string stringDir(nameDir, 15);
    if( stringDir.find(str10)!= -1 ){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_10deg[iter10] = sigma * 1000;
	resolutionsErr_10deg[iter10] = sigmaErr * 1000;
	iter10++;
      }
    }
    else if( stringDir.find(str30)!= -1 ){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_30deg[iter30] = sigma * 1000;
	resolutionsErr_30deg[iter30] = sigmaErr * 1000;
	iter30++;
      }
    }
    else if( stringDir.find(str50)!= -1 ){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_50deg[iter50] = sigma * 1000;
	resolutionsErr_50deg[iter50] = sigmaErr * 1000;
	iter50++;
      }
    }
   else if( stringDir.find(str70)!= -1 ){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_70deg[iter70] = sigma * 1000;
	resolutionsErr_70deg[iter70] = sigmaErr * 1000;
	iter70++;
      }
    }
   else if( stringDir.find(str89)!= -1 ){
      TDirectory *dir = (TDirectory*)key->ReadObj();
      TTree *fitPar = (TTree*)dir->Get("fitPar");
      double sigma = 0;
      double sigmaErr = 0;
      fitPar->SetBranchAddress("sigma",&sigma);
      fitPar->SetBranchAddress("sigmaErr",&sigmaErr);
      int entries = (int)fitPar->GetEntries();
      for(int k=0; k<entries; k++){
	fitPar->GetEntry(k);
	resolutions_89deg[iter89] = sigma * 1000;
	resolutionsErr_89deg[iter89] = sigmaErr * 1000;
	iter89++;
      }
    }
  }
  
  TGraphErrors *gr_10deg = new TGraphErrors(nEnergies,energies,resolutions_10deg,energiesErr,resolutionsErr_10deg);
  TGraphErrors *gr_30deg = new TGraphErrors(nEnergies,energies,resolutions_30deg,energiesErr,resolutionsErr_30deg);
  TGraphErrors *gr_50deg = new TGraphErrors(nEnergies,energies,resolutions_50deg,energiesErr,resolutionsErr_50deg);
  TGraphErrors *gr_70deg = new TGraphErrors(nEnergies,energies,resolutions_70deg,energiesErr,resolutionsErr_70deg);
  TGraphErrors *gr_89deg = new TGraphErrors(nEnergies,energies,resolutions_89deg,energiesErr,resolutionsErr_89deg);
  /*
  TF1 *dieci = new TF1("dieci","[0]+[1]/(x*pow(TMath::Sin(10*TMath::Pi()/180.),1.5))",0,1500);
  TF1 *trenta = new TF1("trenta","[0]+[1]/(x*pow(sin(30*TMath::Pi()/180.),1.5))",0,1500);
  TF1 *cinquanta = new TF1("cinquanta","[0]+[1]/(x*pow(sin(50*TMath::Pi()/180.),1.5))",0,1500);
  TF1 *settanta = new TF1("settanta","[0]+[1]/(x*pow(sin(70*TMath::Pi()/180.),1.5))",0,1500);
  TF1 *novanta = new TF1("novanta","[0]+[1]/(x*pow(sin(89*TMath::Pi()/180.),1.5))",0,1500);
  
  dieci->SetLineStyle(2);
  dieci->SetLineColor(kBlue);
  trenta->SetLineStyle(2);
  trenta->SetLineColor(kRed);
  cinquanta->SetLineStyle(2);
  cinquanta->SetLineColor(kMagenta);
  settanta->SetLineStyle(2);
  settanta->SetLineColor(kGreen+2);
  novanta->SetLineStyle(2);
  novanta->SetLineColor(kBlack);

  gr_10deg->Fit("dieci","R");
  gr_30deg->Fit("trenta","R");
  gr_50deg->Fit("cinquanta","R");
  gr_70deg->Fit("settanta","R");
  gr_89deg->Fit("novanta","R");

  cout << "10deg) a: " << dieci->GetParameter(0) << "\t b: " << dieci->GetParameter(1) << endl;
  cout << "30deg) a: " << trenta->GetParameter(0) << "\t b: " << trenta->GetParameter(1) << endl;
  cout << "50deg) a: " << cinquanta->GetParameter(0) << "\t b: " << cinquanta->GetParameter(1) << endl;
  cout << "70deg) a: " << settanta->GetParameter(0) << "\t b: " << settanta->GetParameter(1) << endl;
  cout << "90deg) a: " << novanta->GetParameter(0) << "\t b: " << novanta->GetParameter(1) << endl;
  */
  TCanvas *c = new TCanvas("c","c",900,800);
  TPad *pad1 = new TPad();
  c->cd();
  pad1->SetGrid();
  pad1->SetLogx();
  pad1->SetLogy();
  pad1->SetLeftMargin(0.2);
  pad1->SetTicks();
  pad1->Draw();
  pad1->cd();
  gr_10deg->SetTitle("");
  gr_10deg->GetXaxis()->SetTitle("p_{T} [GeV]");
  gr_10deg->GetXaxis()->SetTitleOffset(1.1);
  gr_10deg->GetYaxis()->SetTitle("#sigma(#Deltad_{0}) [#mum]");
  gr_10deg->GetYaxis()->SetTitleOffset(1.3);
  //gr_10deg->GetXaxis()->SetRangeUser(0.9,1600);
  gr_10deg->Draw("ap");
  gr_10deg->SetMaximum(1e2);
  gr_10deg->SetMinimum(1e0);
  gr_10deg->SetMarkerColor(kBlue);
  gr_10deg->SetLineColor(kBlue);
  gr_10deg->SetMarkerStyle(22);
  gr_10deg->SetMarkerSize(1.3);
  gr_30deg->Draw("psame");
  gr_30deg->SetMarkerColor(kRed);
  gr_30deg->SetLineColor(kRed);
  gr_30deg->SetMarkerStyle(21);
  gr_30deg->SetMarkerSize(1.1);
  gr_50deg->Draw("psame");
  gr_50deg->SetMarkerColor(6);
  gr_50deg->SetLineColor(6);
  gr_50deg->SetMarkerStyle(33);
  gr_50deg->SetMarkerSize(1.3);
  gr_70deg->Draw("psame");
  gr_70deg->SetMarkerColor(kGreen+2);
  gr_70deg->SetLineColor(kGreen+2);
  gr_70deg->SetMarkerStyle(34);
  gr_70deg->SetMarkerSize(1.3);
  gr_89deg->Draw("psame");
  gr_89deg->SetMarkerColor(kBlack);
  gr_89deg->SetLineColor(kBlack);
  gr_89deg->SetMarkerStyle(20);
  gr_89deg->SetMarkerSize(1.1);

  std::string inputFile(inputFileName,50);
  std::string muon("muon");
  std::string electron("electron");
  std::string pion("pion");

  TLatex *text = new TLatex();
  text->SetTextSize(0.035);
  text->DrawTextNDC(0.205, 0.939349, "CLICdp"); // work in progress");

  auto legend0 = new TLegend(0.583519,0.626289,0.85412,0.893041);
  legend0->SetTextSize(0.03);
  if( inputFile.find(muon) != -1 ) legend0->SetHeader("Single #mu^{-}");
  else if( inputFile.find(electron) != -1 ) legend0->SetHeader("Single e^{-}");
  else if( inputFile.find(pion) != -1 ) legend0->SetHeader("Single #pi^{-}");
  legend0->AddEntry(gr_10deg,"#theta = 10#circ","p");
  legend0->AddEntry(gr_30deg,"#theta = 30#circ","p");
  legend0->AddEntry(gr_50deg,"#theta = 50#circ","p");
  legend0->AddEntry(gr_70deg,"#theta = 70#circ","p");  
  legend0->AddEntry(gr_89deg,"#theta = 89#circ","p");
  //legend0->AddEntry(novanta,"a #oplus b / (p sin^{3/2}#theta)");

  legend0->Draw();

  
  if( inputFile.find(muon) != -1 )
    c->SaveAs("figures/muons/d0Res_vs_pt.eps");
  else if( inputFile.find(electron) != -1 )
    c->SaveAs("figures/electrons/d0Res_vs_pt.eps");
  else if( inputFile.find(pion) != -1 )
    c->SaveAs("figures/pions/d0Res_vs_pt.eps");

}
