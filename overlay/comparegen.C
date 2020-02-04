#include "remolltypes.hh"
using namespace ROOT;

int comparegen(TString conf, TString particle, TString energy){

gStyle->SetOptStat(0);
std::map<TString, TString> gen{{"moller", Form("/global/scratch/rahmans/root/beampipeStudy/analyse_allring/moller_%s_400000_%s_%s.root", conf.Data(), particle.Data(), energy.Data())},{"elastic", Form("/global/scratch/rahmans/root/beampipeStudy/analyse_allring/elastic_%s_400000_%s_%s.root", conf.Data(), particle.Data(), energy.Data())}, {"beam", Form("/global/scratch/rahmans/root/beampipeStudy/analyse_allring/beam_%s_niagara_4000000_%s_%s.root", conf.Data(), particle.Data(), energy.Data())}};
std::map<TString,Color_t> col{ {"moller", kRed}, {"elastic", kGreen}, {"beam", kBlue}};
std::map<TString, TString> config{{"Vd","Default"}, {"Hd", "Default+He pipe"},
                                  {"H2","Default+He pipe+Kryptonite cylinder"}, 
                                  {"V3", "VVA"}, {"V4", "VVA+Kryptonite pipe"},
                                  {"V5", "VVA-Al windows"},
                                  {"H3", "HHA"}, {"V6", "VVA-Al windows-Alpipe"},
                                  {"H4", "HHA-Al windows"}, {"H5", "HHA-Al windows-Al pipe"}, {"H6","HHA-central magnetic field"} };


std::vector<TFile*> f;
std::vector<TH1D*> h;
std::vector<TH1D*> h_all;
std::vector<TH1D*> hclone;

THStack hs("hs","hs");

TCanvas c("c","c",800,600);


gStyle->SetPalette(56);
c.Print(Form("%s_%s_%s.pdf[", conf.Data(),particle.Data(),energy.Data()), "pdf");

TPad *padtitle=new TPad("padt","padt", 0,0.95, 1, 1);
padtitle->Draw();
padtitle->cd();
TLatex label;
label.SetTextAlign(23);
label.SetTextSize(0.65);
label.DrawLatexNDC(0.5,0.85, Form("#color[2]{%s}",config[conf].Data()));


c.cd();
TPad *pad1=new TPad("pad1","pad1", 0.0, 0.5, 0.5, 0.95);
pad1->Draw();
pad1->cd();
//c.cd(1);
gPad->SetLogy();
Int_t i=0;

for(std::map<TString,TString>::iterator it=gen.begin(); it!=gen.end();++it){
f.push_back(new TFile(it->second));
h.push_back((TH1D*) f[i]->Get("pr"));
h_all.push_back((TH1D*) f[i]->Get("r"));
h_all[i]->SetLineColor(col[it->first]);
h[i]->SetLineColor(col[it->first]);
hs.Add(h[i]);


if(it->first=="moller" || it->first=="elastic"){
hclone.push_back((TH1D*) h[i]->Clone(Form("hclone_%s", (it->first).Data())));
}

i++;

}
hclone[1]->Add(hclone[0]);
hclone[1]->SetLineColor(kBlack);
hs.SetTitle("Primary rate vs radius at det plane (GHz/uA/5mm)");
hs.Draw("HISTnostack");
hclone[1]->Draw("HISTsame");
label.SetTextSize(0.04);
label.DrawLatexNDC(0.5, 0.8, "#color[2]{ee}, #color[3]{ep}, ee+ep, #color[4]{beam}");

c.cd();
TPad *pad2=new TPad("pad2","pad2",0.5,0.5,1.0,0.95);
pad2->Draw();
pad2->cd();
//c.cd(2);
gPad->SetLogy();
THStack hs1("hs1","hs1");
hs1.Add(hclone[1]);
hs1.Add(h_all[0]);
hs1.SetTitle("Rate vs radius at det plane (GHz/uA/5mm)");
hs1.Draw("HIST");
hs1.SetMinimum(1e7);
hs1.SetMaximum(1e13);

TH1D* h1=(TH1D *) h_all[0]->Clone("h_all_beam");
h1->Add(hclone[1],-1);
h1->SetLineColor(kCyan);
h1->Draw("HISTsame");

TFile *f_allring_e=new TFile(Form("/global/scratch/rahmans/root/beampipeStudy/analyse_allring/beam_%s_niagara_4000000_electron_all.root",conf.Data()));
TH1D* h_allring_e=(TH1D*) f_allring_e->Get("r");
h_allring_e->Add(hclone[1],-1);
h_allring_e->SetLineColor(kMagenta);
h_allring_e->Draw("HISTsame");

label.DrawLatexNDC(0.5,0.8,"#color[4]{beam all} - (ee+ep) primaries = #color[7]{background}");
label.DrawLatexNDC(0.5,0.85,"beam charged - (ee+ep) primaries  = #color[6]{charged background}");


//c.cd(3);
c.cd();
gPad->SetLogz();
TPad *pad31=new TPad("pad31","pad31", 0.25,0, 0.5,0.25);
pad31->Draw();
pad31->cd();
TFile f_ring5(Form("/global/scratch/rahmans/root/beampipeStudy/analyse_ring5/beam_%s_niagara_4000000_%s_%s.root",conf.Data(), particle.Data(), energy.Data()));
TH2D* vrvz_ring5= (TH2D*) f_ring5.Get("vrvz_rcut");
vrvz_ring5->SetMarkerColor(kBlue);
vrvz_ring5->SetTitle("r vs z vertex of all particles in ee ring");
vrvz_ring5->Draw();

c.cd();
TPad *pad32=new TPad("pad32","pad32", 0,0.25,0.25,0.5);
pad32->Draw();
pad32->cd();
TFile f_ring5_e_lowp(Form("/global/scratch/rahmans/root/beampipeStudy/analyse_ring5/beam_%s_niagara_4000000_electron_lowene.root", conf.Data()));
TH2D* vrvz_ring5_e_lowp=(TH2D*) f_ring5_e_lowp.Get("vrvz_rcut");
vrvz_ring5_e_lowp->SetMarkerColor(kOrange);
vrvz_ring5_e_lowp->SetTitle("r vs z vertex of 1-10MeV electrons in ee ring");
vrvz_ring5_e_lowp->Draw("");

c.cd();
TPad *pad33=new TPad("pad33", "pad33", 0.25,0.25, 0.5,0.5);
pad33->Draw();
pad33->cd();
TFile f_ring5_e_midp(Form("/global/scratch/rahmans/root/beampipeStudy/analyse_ring5/beam_%s_niagara_4000000_electron_midene.root", conf.Data()));
TH2D* vrvz_ring5_e_midp=(TH2D*) f_ring5_e_midp.Get("vrvz_rcut");
vrvz_ring5_e_midp->SetMarkerColor(kOrange+3);
vrvz_ring5_e_midp->SetTitle("r vs z vertex of 10-100MeV electrons in ee ring"); 
vrvz_ring5_e_midp->Draw("");

c.cd();
TPad *pad34=new TPad("pad34", "pad34", 0,0, 0.25,0.25);
pad34->Draw();
pad34->cd();
TFile f_ring5_y_all(Form("/global/scratch/rahmans/root/beampipeStudy/analyse_ring5/beam_%s_niagara_4000000_photon_all.root", conf.Data()));
TH2D* vrvz_ring5_y_all=(TH2D*) f_ring5_y_all.Get("vrvz_rcut");
vrvz_ring5_y_all->SetMarkerColor(kOrange+6);
vrvz_ring5_y_all->SetTitle("r vs z vertex of all photons in ee ring");
vrvz_ring5_y_all->Draw("");

c.cd();
TPad *pad41=new TPad("pad41", "pad41", 0.75, 0.0, 1, 0.25);
pad41->Draw();
pad41->cd();
//c.cd(4);
gPad->SetLogy();
TH1D* vz_ring5=vrvz_ring5->ProjectionX("vz_rcut");
vz_ring5->Rebin(10);
vz_ring5->GetYaxis()->SetLabelSize(0.05);
vz_ring5->SetMarkerStyle(8);
vz_ring5->SetMarkerSize(1);
vz_ring5->SetMarkerColor(kBlue);
vz_ring5->SetLineColor(kBlue);
vz_ring5->SetTitle("z vertex of all particles in ee ring (Hz/uA/100mm)");
vz_ring5->Draw("");

c.cd();
TPad *pad42=new TPad("pad42", "pad42", 0.5, 0.25,0.75,0.5);
pad42->Draw();
pad42->cd(); 
gPad->SetLogy();
TH1D* vz_ring5_e_lowp=vrvz_ring5_e_lowp->ProjectionX("vz_rcut_e_lowp");
vz_ring5_e_lowp->Rebin(10);
vz_ring5_e_lowp->GetYaxis()->SetLabelSize(0.05);
vz_ring5_e_lowp->SetMarkerStyle(8);
vz_ring5_e_lowp->SetMarkerSize(1);
vz_ring5_e_lowp->SetMarkerColor(kOrange);
vz_ring5_e_lowp->SetLineColor(kOrange);
vz_ring5_e_lowp->SetTitle("z vertex of 1-10 MeV electrons in ee ring (Hz/uA/100mm)");
vz_ring5_e_lowp->Draw("");

c.cd();
TPad *pad43=new TPad("pad43", "pad43", 0.75, 0.25,1,0.5);
pad43->Draw();
pad43->cd();
gPad->SetLogy();
TH1D* vz_ring5_e_midp=vrvz_ring5_e_midp->ProjectionX("vz_rcut_e_midp");
vz_ring5_e_midp->Rebin(10);
vz_ring5_e_midp->GetYaxis()->SetLabelSize(0.05);
vz_ring5_e_midp->SetMarkerStyle(8);
vz_ring5_e_midp->SetMarkerSize(1);
vz_ring5_e_midp->SetMarkerColor(kOrange+3);
vz_ring5_e_midp->SetLineColor(kOrange+3);
vz_ring5_e_midp->SetTitle("z vertex of 10-100 MeV electrons in ee ring (Hz/uA/100mm)");
vz_ring5_e_midp->Draw("");

c.cd();
TPad *pad44=new TPad("pad44", "pad44", 0.5, 0.0,0.75,0.25);
pad44->Draw();
pad44->cd();
gPad->SetLogy();
TH1D* vz_ring5_y_all=vrvz_ring5_y_all->ProjectionX("vz_rcut_y_all");
vz_ring5_y_all->Rebin(10);
vz_ring5_y_all->GetYaxis()->SetLabelSize(0.05);
vz_ring5_y_all->SetMarkerStyle(8);
vz_ring5_y_all->SetMarkerSize(1);
vz_ring5_y_all->SetMarkerColor(kOrange+6);
vz_ring5_y_all->SetLineColor(kOrange+6);
vz_ring5_y_all->SetTitle("z vertex of all photons in ee ring (Hz/uA/100mm)");
vz_ring5_y_all->Draw("");



c.Print(Form("%s_%s_%s.pdf", conf.Data(),particle.Data(), energy.Data()));

c.Print(Form("%s_%s_%s.pdf]", conf.Data(),particle.Data(), energy.Data()), "pdf");

return 0;
}
