int draw1DfA(){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;
std::map<TString,TH1D*> h1;

std::map<TString, TString> gen{
 //     {"beam", "/global/scratch/rahmans/root/plotsForCDR/beam_V3_CDR_all_all.root"},
	{"moller", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/moller/moller_5_Quartz_segmented.root"},
        {"elastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/elastic/elastic_5_Quartz_segmented.root"},
        {"inelastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/inelastic/inelastic_5_Quartz_segmented.root"}
};

std::map<TString, TString> gen1{
 //     {"beam", "/global/scratch/rahmans/root/plotsForCDR/beam_V3_CDR_all_all.root"},
	{"moller", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/FOM/moller/moller_5_Quartz_segmented.root"},
        {"elastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/FOM/elastic/elastic_5_Quartz_segmented.root"},
        {"inelastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/FOM/inelastic/inelastic_5_Quartz_segmented.root"}
};
 
std::vector<TString> g{"moller", "elastic", "inelastic"};

auto legend=new TLegend(0.75,0.7, 0.9,0.9,"brNDC");
legend->SetHeader("Particle Type", "C");

auto label= new TLatex();
label->SetTextAlign(23);
label->SetTextSize(0.04);
label->SetTextAngle(90);
std::map<TString, Color_t> col{
	{"moller", kBlack},
	{"elastic", kRed},
        {"inelastic", kGreen},
        {"beam", kBlue}
};

TCanvas c("c","c", 800, 600);
c.SetMargin(0.13,0.13,0.13,0.13);
THStack hs("hs", "f_{i}A_{i} distribution at detector plane 26.5 m from target");
gStyle->SetPalette(56);
//ad->SetLogy();


for (Int_t i=0;i<3;i++){
        f[g[i]]=new TFile((gen[g[i]]).Data());
        h[g[i]]=(TH1D*) f[g[i]]->Get("pr");
        h[g[i]]->Scale(1e-3);
        
}

h["moller"]->Add(h["elastic"]);
h["moller"]->Add(h["inelastic"]);


for (Int_t i=0;i<3;i++){
	f[g[i]]=new TFile((gen1[g[i]]).Data());
        h1[g[i]]=(TH1D*) f[g[i]]->Get("pr"); 
        h1[g[i]]->Scale(-1e-3);
        h1[g[i]]->Divide(h["moller"]);
	h1[g[i]]->SetLineColor(col[g[i]]);
        h1[g[i]]->SetLineWidth(2);
//        h1[g[i]]->SetFillColor(col[g[i]]);
        legend->AddEntry(h1[g[i]], g[i]);
        hs.Add(h1[g[i]]);

}

Double_t error=0;
Double_t integral=0;
std::cout<<"\n" ;
for(int i=0;i<3;i++){
integral=h1[g[i]]->IntegralAndError(0,-1,error);
std::cout<<"Integral "<<integral<< "+/-"<< error << std::endl;
}

hs.Draw("HISTnostack");
hs.GetXaxis()->SetRangeUser(500,1300);
hs.SetMaximum(190);
hs.SetMinimum(0);
//.SetTitle("Radial distribution of ee, ep, and ine [GHz/uA/(5mm){}^{2}]");
hs.GetXaxis()->SetTitle("r(mm)");
hs.GetYaxis()->SetTitle("f_{i}A_{i} (ppb/5mm)");
legend->Draw("NDC");
//h["moller"]->GetXaxis()->SetRangeUser(-130,-50);
//.Draw("colz");
Float_t detinnerr[7]= {64.0, 68.0, 73.0, 80.5,  85.5, 107,117};
Float_t pos[7]={0.27,0.31,0.37,0.42, 0.6, 0.7, 0.8};
TLine *l;
for(int i=0;i<7;i++){
l=new TLine(detinnerr[i]*10,0, detinnerr[i]*10,190);
l->SetLineColor(11);
l->Draw();
if(i!=6){
label->DrawLatexNDC(pos[i],0.6, Form("#color[11]{Ring %d}",i+1));
}
}
c.Print("1DProfile_fA.png");
return 0;

}




















