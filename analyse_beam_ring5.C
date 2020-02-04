using namespace ROOT;

/**Usage: source file, output file, generator, ring number(1-6) and module name (Quartz/LG)**/

int analyse_beam_ring5(TString source, TString out, TString gen, Int_t ring, TString mod){

TChain T("T");
T.Add(Form("%s", source.Data())); // Adding source file
Int_t nEvents= T.GetEntries();  // Number of primary events
Double_t weight;
if(gen=="beam"){
	weight= 1e-9*(1.0/nEvents)*(1.0/(7*1.602e-13)); // Divide by current and number of septants. So, the Y-axis gets units of GHz/uA/sep. 
}else{
        weight= 1e-9/85.0/7.0;
}

TFile f(Form("%s", out.Data()), "RECREATE");

std::map<TString, TH1D*> h;
std::map<TString, TH1D*> h_low;
std::map<TString, TH1D*> h_mid;
std::map<TString, TH1D*> h_high;


std::vector<TString> particle{"all", "primary", "electron", "positron", "photon", "other"};
std::vector<TString> energy{"all", "low", "mid", "high"};

/* pr= primary for the generator, e= electron, p=positron, y=photon, o=others */

for(Int_t i=0;i<6;i++){
  for(Int_t j=0;j<4;j++){
h[particle[i]+energy[j]]=new TH1D(Form("pr_%s_%s", particle[i].Data(), energy[j].Data()), Form("%s, Ring = %d, Sector = all, Generator = %s, Part = %s",particle[i].Data(), ring, gen.Data(), mod.Data()), 400, 0, 2000);
  }
}


Double_t fRate=0;
remollEvent_t *fEvent=0;
std::vector<remollGenericDetectorHit_t>  *fHit=0;
T.SetBranchAddress("ev", &fEvent);
T.SetBranchAddress("hit", &fHit);
T.SetBranchAddress("rate", &fRate);



for (size_t j=0;j< nEvents;j++){
	T.GetEntry(j);
        if (gen=="beam"){
	        fRate=1.0;
        }
        for (size_t i=0;i<fHit->size();i++){
                remollGenericDetectorHit_t hit=fHit->at(i);
               
                Bool_t hit_planedet = hit.det==28 ;
                
		Double_t sepdiv=2*TMath::Pi()/7.0;
   		Int_t sec=0;
   		Double_t phi=atan2(hit.y,hit.x);
   		if (phi<0) {phi+=2*TMath::Pi();}
  
   		Double_t secphi = fmod(phi, 2*TMath::Pi()/7);
  		if (secphi<TMath::Pi()/28.0){sec=1;}           // closed 
   		else if (secphi<3*TMath::Pi()/28.0){sec=2; }    // transition
   		else if (secphi<5*TMath::Pi()/28.0) {sec=3;}   // open
   		else if (secphi<7*TMath::Pi()/28.0) {sec=2;}   // transition
  		else {sec=1;}  //closed 

                Bool_t hit_radial=0;
                std::map<Int_t, Double_t> min_closed;
                std::map<Int_t, Double_t> max_closed;
                std::map<Int_t, Double_t> min_trans;
                std::map<Int_t, Double_t> max_trans;
                std::map<Int_t, Double_t> min_open;
                std::map<Int_t, Double_t> max_open;
 
                if (mod=="Quartz"){

                  min_open[1]=640; max_open[1]=680;
                  min_open[2]=680; max_open[2]=730;
                  min_open[3]=730; max_open[3]=805;
                  min_open[4]=805; max_open[4]=855;
                  min_open[5]=875; max_open[5]=1090;
                  min_open[6]=1070; max_open[6]=1170;

                  min_trans[1]=640; max_trans[1]=680;
                  min_trans[2]=680; max_trans[2]=730;
                  min_trans[3]=730; max_trans[3]=827.5;
                  min_trans[4]=827.5; max_trans[4]=900;
                  min_trans[5]=920; max_trans[5]=1080;
                  min_trans[6]=1060; max_trans[6]=1170;

                  min_closed[1]=640; max_closed[1]=680;
                  min_closed[2]=680; max_closed[2]=730;
                  min_closed[3]=730; max_closed[3]=835;
                  min_closed[4]=835; max_closed[4]=915;
                  min_closed[5]=935; max_closed[5]=1075;
                  min_closed[6]=1055; max_closed[6]=1170;

		} else{

                  max_open[1]=1900; min_open[1]=680;
                  max_open[2]=1900; min_open[2]=730;
                  max_open[3]=1900; min_open[3]=805;
                  max_open[4]=1900; min_open[4]=855;
                  max_open[5]=1900; min_open[5]=1070;
                  max_open[6]=1900; min_open[6]=1170;

                  max_trans[1]=1900; min_trans[1]=680;
                  max_trans[2]=1900; min_trans[2]=730;
                  max_trans[3]=1900; min_trans[3]=827.5;
                  max_trans[4]=1900; min_trans[4]=900;
                  max_trans[5]=1900; min_trans[5]=1060;
                  max_trans[6]=1900; min_trans[6]=1170;

                  max_closed[1]=1900; min_closed[1]=680;
                  max_closed[2]=1900; min_closed[2]=730;
                  max_closed[3]=1900; min_closed[3]=835;
                  max_closed[4]=1900; min_closed[4]=915;
                  max_closed[5]=1900; min_closed[5]=1055;
                  max_closed[6]=1900; min_closed[6]=1170;

		}

                std::vector<Bool_t> ene_value{1, hit.p>=1 && hit.p<100, hit.p>=100 && hit.p<1000, hit.p>=1000};

                /**** The radial ranges of open, closed and transition sector ****/		
               if (sec==1) {
		    if (mod=="Quartz"){
			hit_radial=hit.r>=min_closed[ring] && hit.r<max_closed[ring];
		    } else{
			hit_radial=hit.r>=min_closed[ring] && hit.r<=max_closed[ring];
                    }
		}
                if (sec==2) {
                    if (mod=="Quartz"){
                        hit_radial=hit.r>=min_trans[ring] && hit.r<max_trans[ring];
                    } else{
                        hit_radial=hit.r>=min_trans[ring] && hit.r<=max_trans[ring];
                    }
		}
                if (sec==3) {
                    if (mod=="Quartz"){
                        hit_radial=hit.r>=min_open[ring] && hit.r<max_open[ring];
                    } else{
                        hit_radial=hit.r>=min_open[ring] && hit.r<=max_open[ring];
                    }
                }
                
                Bool_t hit_cutoff = hit.p<1;    // Cut off all particles with energy less than 1 MeV
                Int_t max_track=0; // maximum track of primary
                if (gen=="moller"){
			max_track=2;
                } else{
                        max_track=1;
                }
                
		std::map<TString, Bool_t> hit_pid;


                for (Int_t j=0; j<4;j++){
                  hit_pid["all"+energy[j]]= 1 && ene_value[j];            
                    if(gen=="beam"){
			hit_pid["primary"+energy[j]]=hit.pid==11 && hit.vz<=-3800 && ene_value[j];
			hit_pid["electron"+energy[j]]=hit.pid==11 && hit.vz>-3800 && ene_value[j];
			hit_pid["positron"+energy[j]]=hit.pid==-11 && ene_value[j];
			hit_pid["photon"+energy[j]]= hit.pid==22 && ene_value[j]; 
			hit_pid["other"+energy[j]]= hit.pid!=11 && hit.pid!=-11 && hit.pid!=22 && ene_value[j];
		    }else{
			hit_pid["primary"+energy[j]]= hit.trid<=max_track && ene_value[j];
                        hit_pid["electron"+energy[j]]=hit.pid==11 && hit.trid>max_track && ene_value[j];
			hit_pid["positron"+energy[j]]=hit.pid==-11 && ene_value[j];
			hit_pid["photon"+energy[j]]=hit.pid==22 && ene_value[j];
			hit_pid["other"+energy[j]]=hit.pid!=11 && hit.pid!=-11 && hit.pid!=22 && ene_value[j];
		    }// Chooses cut on particle based on input
                
                }
               
                if (hit_cutoff || !hit_planedet) { continue; }

                for (Int_t i=0;i<6;i++){
                   for(Int_t j=0;j<4;j++){
     
		  	if (hit_radial && hit_pid[particle[i]+energy[j]]){
/****                         
 	                   if (sec==3) {
				 h_open[particle[i]]->Fill(hit.r, (fRate)*weight);
                                 h_fom_open[particle[i]]->Fill(hit.r, (fRate)*(fEvent->A)*(fEvent->A)*weight);
				 h_asy_open[particle[i]]->Fill(fEvent->A, fRate*weight);
			   }
                           if(sec==2) {
				 h_trans[particle[i]]->Fill(hit.r, (fRate)*weight);
                                 h_fom_trans[particle[i]]->Fill(hit.r, (fRate)*(fEvent->A)*(fEvent->A)*weight);
                                 h_asy_trans[particle[i]]->Fill(fEvent->A, fRate*weight);
                           }
                           if(sec==1) {
				 h_closed[particle[i]]->Fill(hit.r, (fRate)*weight);
                                 h_fom_closed[particle[i]]->Fill(hit.r, (fRate)*(fEvent->A)*(fEvent->A)*weight);
                                 h_asy_closed[particle[i]]->Fill(fEvent->A, fRate*weight);
                           }
****/
                           h[particle[i]+energy[j]]->Fill(hit.r, (fRate)*weight);
                        }
		   }
		} 
       }
}


for (Int_t i=0; i<6; i++){
    for (Int_t j=0; j<4; j++){
	h[particle[i]+energy[j]]->Write("", TObject::kOverwrite);
/**** 
        h_open[particle[i]]->Write("", TObject::kOverwrite);
        h_trans[particle[i]]->Write("", TObject::kOverwrite);
        h_closed[particle[i]]->Write("", TObject::kOverwrite);
        h_fom[particle[i]]->Write("", TObject::kOverwrite);
        h_fom_open[particle[i]]->Write("", TObject::kOverwrite);
        h_fom_trans[particle[i]]->Write("", TObject::kOverwrite);
        h_fom_closed[particle[i]]->Write("", TObject::kOverwrite);
        h_asy[particle[i]]->Write("", TObject::kOverwrite);
        h_asy_open[particle[i]]->Write("", TObject::kOverwrite);
        h_asy_trans[particle[i]]->Write("", TObject::kOverwrite);
        h_asy_closed[particle[i]]->Write("", TObject::kOverwrite);
****/
    }
}

return 0;
}
