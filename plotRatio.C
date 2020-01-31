int plotRatio(TString source, TString out){

std::map<TString, TFile*> gen;

gen["moller"]=new TFile(Form("%s/moller/moller_5_Quartz.root", source.Data()));
gen["elastic"]=new TFile(Form("%s/elastic/elastic_5_Quartz.root", source.Data()));

std::vector<TString> generator{"moller", "elastic"};

std::map<TString, TH1D*> h;
std::map<TString, TH1D*> h_open;
std::map<TString, TH1D*> h_trans;
std::map<TString, TH1D*> h_closed;

for(int i=0;i<2;i++){

h[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr");
h_open[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_open");
h_trans[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_trans");
h_closed[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_closed");



}

TLine l1;
TLine l2;
TLine l3;

TCanvas *c=new TCanvas("c", "c", 800,600);
h_open["moller"]->Scale(1.0/1000);
h_open["moller"]->Draw();
h_open["elastic"]->Scale(1.0/1000);
h_open["elastic"]->Draw("same");
c->Print(Form("%s/overlay_open.png", out.Data()));


h_trans["moller"]->Scale(1.0/1000);
h_trans["moller"]->Draw();
h_trans["elastic"]->Scale(1.0/2000);
h_trans["elastic"]->Draw("same");
c->Print(Form("%s/overlay_trans.png", out.Data()));

h_closed["moller"]->Scale(1.0/1000);
h_closed["moller"]->Draw();
h_closed["elastic"]->Scale(1.0/2000);
h_closed["elastic"]->Draw("same");
c->Print(Form("%s/overlay_closed.png", out.Data()));

c->SetLogy();
l1.SetX1(800);
l1.SetX2(1150);
l1.SetY1(1);
l1.SetY2(1);
l2.SetX1(850);
l2.SetX2(850);
l2.SetY1(0);
l2.SetY2(1);
l3.SetX1(1110);
l3.SetX2(1110);
l3.SetY1(0);
l3.SetY2(1);

h_open["elastic"]->Divide(h_open["moller"]);
h_open["elastic"]->GetXaxis()->SetRangeUser(600,1300);
h_open["elastic"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_ratio_open.png", out.Data()));

h_trans["elastic"]->Divide(h_trans["moller"]);
l1.SetX1(800);
l1.SetX2(1150);
l1.SetY1(1);
l1.SetY2(1);
l2.SetX1(895);
l2.SetX2(895);
l2.SetY1(0);
l2.SetY2(1);
l3.SetX1(1120);
l3.SetX2(1120);
l3.SetY1(0);
l3.SetY2(1);
h_trans["elastic"]->GetXaxis()->SetRangeUser(600,1300);
h_trans["elastic"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_ratio_trans.png", out.Data()));

h_closed["elastic"]->Divide(h_closed["moller"]);
l1.SetX1(800);
l1.SetX2(1150);
l1.SetY1(1);
l1.SetY2(1);
l2.SetX1(905);
l2.SetX2(905);
l2.SetY1(0);
l2.SetY2(1);
l3.SetX1(1120);
l3.SetX2(1120);
l3.SetY1(0);
l3.SetY2(1);
h_closed["elastic"]->GetXaxis()->SetRangeUser(600,1300);
h_closed["elastic"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_ratio_closed.png", out.Data()));


return 0;
}
