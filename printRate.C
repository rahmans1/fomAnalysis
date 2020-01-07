int printRate(TString conf,Int_t events, TString particle, TString energy){


TFile f(Form("/scratch/j/jmammei/rahmans/root/beampipeStudy/integrate/%s_%i_%s_%s.root", conf.Data(), events,particle.Data(),energy.Data()));




TH1D *r=(TH1D*) f.Get("r_rcut_closed");
Double_t error=0;
Double_t rate=65*1e-9*r->IntegralAndError(0,-1,error);
if(conf=="real_conf7"){
printf("%3.2f %3.2f\n", rate/(1.602*1e-13),error*65*1e-9/(1.602*1e-13));
}else{
printf("%3.2f %3.2f\n", rate,error*65*1e-9);
}


return 0;
}
