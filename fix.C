int fix(TString file){
Int_t corrupt=0;


TFile f(file);
if(f.IsZombie() || f.GetSize()<1000){
  std::cout<<Form("%s is zombie",file.Data())<< std::endl;
  corrupt=1;
}

if(f.TestBit(TFile::kRecovered)){
  std::cout<<Form("%s is corrupt but recovered",file.Data())<< std::endl;
  corrupt=1;
} 

return corrupt;
}
