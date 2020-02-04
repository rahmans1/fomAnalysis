int draw1D(){
std::map<TString,TFile*> f;
std::map<TString,TH1D*> h;


std::map<TString, TString> gen{
 //     {"beam", "/global/scratch/rahmans/root/plotsForCDR/beam_V3_CDR_all_all.root"},
	{"moller", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/moller/moller_5_Quartz_hybrid.root"},
	{"elastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/elastic/elastic_5_Quartz_hybrid.root"},
	{"inelastic", "/volatile/halla/moller12gev/rahmans/root/fomStudy/101_jlabmap_kryp/inelastic/inelastic_5_Quartz_hybrid.root"}
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
THStack hs("hs", "Radial distribution at detector plane 26.5 m from target");
gStyle->SetPalette(56);
//ad->SetLogy();

for (Int_t i=0;i<3;i++){
	f[g[i]]=new TFile((gen[g[i]]).Data());
        h[g[i]]=(TH1D*) f[g[i]]->Get("pr");
        h[g[i]]->Scale(1e-3);
	h[g[i]]->SetLineColor(col[g[i]]);
        h[g[i]]->SetLineWidth(2);
       // h[g[i]]->SetFillColor(col[g[i]]);
        legend->AddEntry(h[g[i]], g[i]);
        hs.Add(h[g[i]]);

}

std::cout<< h["moller"]->Integral()*65*7 << std::endl;


hs.Draw("HISTnostack");
hs.GetXaxis()->SetRangeUser(500,1300);
hs.SetMaximum(0.02);
hs.SetMinimum(0);
//.SetTitle("Radial distribution of ee, ep, and ine [GHz/uA/(5mm){}^{2}]");
hs.GetXaxis()->SetTitle("r(mm)");
hs.GetYaxis()->SetTitle("Rate(GHz/uA/sep/5mm)");
legend->Draw("NDC");
//h["moller"]->GetXaxis()->SetRangeUser(-130,-50);
//.Draw("colz");
Float_t detinnerr[7]= {64.0, 68.0, 73.0, 80.5,  85.5, 107,117};
Float_t pos[7]={0.27,0.31,0.37,0.42, 0.6, 0.7, 0.8};
TLine *l;
for(int i=0;i<7;i++){
l=new TLine(detinnerr[i]*10,0, detinnerr[i]*10,0.02);
l->SetLineColor(11);
l->Draw();
if(i!=6){
label->DrawLatexNDC(pos[i],0.6, Form("#color[11]{Ring %d}",i+1));
}
}
c.Print("1DProfile.png");
return 0;

}




















