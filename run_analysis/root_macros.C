void fill_duplicates_histos(TTree *t_per_1, TString var, TH1F* h_total_1, TH1F *h_duplicates_1, int minNhits = 3, bool debug = false){

  vector<double> *trk_var = 0;
//  vector<double> *trk_pt_1 = 0;
//  vector<double> *trk_theta_1 = 0;
//  vector<double> *trk_phi_1 = 0;
  vector<double> *trk_nhits_1 = 0;
  vector<double> *mc_simPt_1 = 0;
  vector<double> *mc_simTheta_1 = 0;

  TBranch *b_trk_var = 0;
  t_per_1->SetBranchAddress(var,&trk_var,&b_trk_var);
//  TBranch *b_trk_pt_1 = 0;
//  t_per_1->SetBranchAddress("recoPt",&trk_pt_1,&b_trk_pt_1);
//  TBranch *b_trk_theta_1 = 0;
//  t_per_1->SetBranchAddress("recoTheta",&trk_theta_1,&b_trk_theta_1);
//  TBranch *b_trk_phi_1 = 0;
//  t_per_1->SetBranchAddress("recoPhi",&trk_phi_1,&b_trk_phi_1);
  TBranch *b_trk_nhits_1 = 0;
  t_per_1->SetBranchAddress("recoNhits",&trk_nhits_1,&b_trk_nhits_1);
  TBranch *b_mc_simPt_1 = 0;
  t_per_1->SetBranchAddress("truePt",&mc_simPt_1,&b_mc_simPt_1);
  TBranch *b_mc_simTheta_1 = 0;
  t_per_1->SetBranchAddress("trueTheta",&mc_simTheta_1,&b_mc_simTheta_1);

  Int_t nEntries_1 = t_per_1->GetEntries();
  if(debug) std::cout << nEntries_1 << " entries " << std::endl;

  for(Int_t i=0; i < nEntries_1; i++){

    Long64_t per_entry = t_per_1->LoadTree(i);
    b_trk_var->GetEntry(per_entry);
//    b_trk_pt_1->GetEntry(per_entry);
//    b_trk_theta_1->GetEntry(per_entry);
//    b_trk_phi_1->GetEntry(per_entry);
    b_trk_nhits_1->GetEntry(per_entry);
    b_mc_simPt_1->GetEntry(per_entry);
    b_mc_simTheta_1->GetEntry(per_entry);

    //std::cout << "Loop on top of " << mc_simTheta_1->size() << " tracks." << std::endl;

    std::map<double, int> first_simTheta;
    std::set<int> iter_duplicates;
    for(UInt_t j = 0; j < mc_simTheta_1->size(); j++){ //loop over tracks
      if(trk_nhits_1->at(j) >= minNhits){
        h_total_1->Fill(trk_var->at(j));
        //std::cout << "Reconstructuble" << std::endl;
      } else {
        //std::cout << "Not reconstructuble" << std::endl;
      }

      double current_simTheta = mc_simTheta_1->at(j);
      std::map<double, int>::iterator it = first_simTheta.find(current_simTheta);
      if (it != first_simTheta.end()){
        //std::cout << "Multiple found : " << it->second << std::endl;
        //check also the simPt
        if(mc_simPt_1->at(j) == mc_simPt_1->at(it->second)){
          iter_duplicates.insert(j);
          iter_duplicates.insert(it->second);
        }
      } else {
        //std::cout << "Multiple not found" << std::endl;
        first_simTheta[current_simTheta] = j;
      }
    }

    for(auto j : iter_duplicates){ //loop over duplicates
      //std::cout << "Duplicates #" <<  j << std::endl;
      //std::cout << " with theta = " << mc_simTheta_1->at(j) <<  "and pt = " << mc_simPt_1->at(j) << std::endl;
      if(trk_nhits_1->at(j) >= minNhits){
        h_duplicates_1->Fill(trk_var->at(j));
      } else {
        //std::cout << "Not reconstructuble" << std::endl;
      }
    }
    first_simTheta.clear();
    iter_duplicates.clear();
  }

  //std::cout << "Entries duplicates: " << h_duplicates_1->GetEntries() << std::endl;
  //std::cout << "Entries reconstructed: " << h_total_1->GetEntries() << std::endl;

}
