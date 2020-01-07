int plotFOM(TString source, TString out){

std::map<TString, TFile*> gen;

gen["moller"]=new TFile(Form("%s/moller/moller_5_Quartz.root", source.Data()));
gen["elastic"]=new TFile(Form("%s/elastic/elastic_5_Quartz.root", source.Data()));

std::vector<TString> generator{"moller", "elastic"};

std::map<TString, TH1D*> h;
std::map<TString, TH1D*> h_open;
std::map<TString, TH1D*> h_trans;
std::map<TString, TH1D*> h_closed;

for(int i=0;i<2;i++){

h[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_fom");
h_open[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_fom_open");
h_trans[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_fom_trans");
h_closed[generator[i]]=(TH1D*) gen[generator[i]]->Get("pr_fom_closed");

}

TLine l1;
TLine l2;
TLine l3;

TCanvas *c=new TCanvas("c", "c", 800,600);


l1.SetX1(800);
l1.SetX2(1150);
l1.SetY1(35);
l1.SetY2(35);
l2.SetX1(855);
l2.SetX2(855);
l2.SetY1(0);
l2.SetY2(35);
l3.SetX1(1070);
l3.SetX2(1070);
l3.SetY1(0);
l3.SetY2(35);
h_open["moller"]->Scale(65.0/1000);
h_open["moller"]->GetXaxis()->SetRangeUser(600,1300);
h_open["moller"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_fom_open.png", out.Data()));

l1.SetX1(900);
l1.SetX2(1060);
l1.SetY1(35);
l1.SetY2(35);
l2.SetX1(900);
l2.SetX2(900);
l2.SetY1(0);
l2.SetY2(35);
l3.SetX1(1060);
l3.SetX2(1060);
l3.SetY1(0);
l3.SetY2(35);
h_trans["moller"]->Scale(65.0/1000);
h_trans["moller"]->GetXaxis()->SetRangeUser(600,1300);
h_trans["moller"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_fom_trans.png", out.Data()));

l1.SetX1(915);
l1.SetX2(1055);
l1.SetY1(10);
l1.SetY2(10);
l2.SetX1(915);
l2.SetX2(915);
l2.SetY1(0);
l2.SetY2(10);
l3.SetX1(1055);
l3.SetX2(1055);
l3.SetY1(0);
l3.SetY2(10);
h_closed["moller"]->Scale(65.0/1000);
h_closed["moller"]->GetXaxis()->SetRangeUser(600,1300);
h_closed["moller"]->Draw("HIST");
l1.Draw();
l2.Draw();
l3.Draw();
c->Print(Form("%s/overlay_fom_closed.png", out.Data()));



return 0;
}
