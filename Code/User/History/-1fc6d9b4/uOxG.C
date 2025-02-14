//--------------------------------------------------------------------------------------
// ----- DrawWave -----
// version: v1.0
// code log:
// 1) This program can draw the waveforms in each event of TPC data.
// author: yih@ihep.ac.cn
// date: 2019-11-27
//
// version: v1.1
// code log:
// 1) add -fit option to set fitting on or off
// author: yih@ihep.ac.cn
// date: 2023-01-16
//
// version: v2.0
// code log:
// 1) use BluetWaveformMaster to draw and fit waveforms
// author: yih@ihep.ac.cn
// date: 2023-03-15
//
// ----- DrawEvent -----
// version: v1.0
// code log:
// 1) draw waveforms, hits and tracks at the same time.
// author: yih@ihep.ac.cn
// date: 2023-03-22
//--------------------------------------------------------------------------------------

#include "TFile.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TText.h"
#include "TSystem.h"
#include "TMultiGraph.h"
#include "TView.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>

#include "BluetConfig.h"
#include "BMCPAnodeStrip.h"
#include "BluetWaveformMaster.h"
#include "BluetMCPAnodeMaster.h"
#include "BluetDataModel.h"


using namespace std;
void FillHistogramsAndGraphs(const std::vector<int> *vec, const vector<int> *HitBoardID,const vector<int> *HitChipID,const vector<int> *HitChannelID) ;

int main(int argc, char** argv){
//-----------------------------
//suppress output to terminal
//-----------------------------
  gErrorIgnoreLevel = kWarning; 

//-----------------------------
// set batch mode
//-----------------------------
//gInterpreter->Declare("#include \"../BluetAna/include/BluetDataModel.h\"");
  gInterpreter->Declare("#include \"/home/hooke/works/wnri-bluet-v5/BluetAna/include/BluetDataModel.h\"");
  gROOT->SetBatch();  
//-----------------------------
// define variables 
//-----------------------------
  cout<<"===> argc = "<<argc<<endl;

  auto Dir_Pic = argv[1];
  bool fEnableFit = false;
  bool fEnableCut = false;
  bool fEnableTrack = false;
  bool fEnableDec = false;
  
  std::vector<TString> vname_froot{};
  for (size_t i = 0; i < argc; i++)
  {
      TString frootname = argv[i];
      if (frootname.Contains(".root"))
      {
        if (gSystem->AccessPathName(frootname.Data()))
        {
            cout<<"*** Can not open " << frootname  << " !!! ***"<<endl;
        }else{
            vname_froot.emplace_back(frootname);
        }
      }
  }
  
  if (vname_froot.size()==0)
  {
    cout<<"no Root File can be used!!!"<<endl;
    return -1;
  }

  BluetConfig* config = BluetConfig::GetInstance();

  BluetMCPAnodeMaster* anodemaster = BluetMCPAnodeMaster::GetInstance();
  anodemaster->SetPicDir(Dir_Pic);  
  anodemaster->SetCanvasSize(2*1024, 3*1024);
  anodemaster->SetNCanvas(20);
  anodemaster->InitCanvas();
//-----------------------------
// define tree
//-----------------------------
/*   TApplication* app = new TApplication("app", &argc, argv);
 */
  //input tree
  auto bluetwavehit = new BluetWaveHit;
  BluetEvent* fEvent{nullptr}; 

  auto Treename = "WaveTree";
  auto WaveTree = new TChain(Treename);
  auto WaveTree = std::make_unique<TChain>(Treename);
  for ( auto c: vname_froot)
  {
    WaveTree->Add(c);
    cout<<"===< "<< c <<endl;    
  }  
  WaveTree->SetBranchAddress("BluetWaveHit", &bluetwavehit) ;


  char enter = 'c';
  int enterID = 0;  

//-----------------------------
// read wave data and draw
//-----------------------------
	auto Nevent = WaveTree->GetEntries();
	for(Long64_t i=0; (i<Nevent)&&(enter!='q'); i++){
    WaveTree->GetEntry(i);
    if( i<enterID ) continue;
    cout<<"\n-----------------------"<<endl;
    cout<<"===> ievent = "<<i;
    cout<<"\n-----------------------"<<endl;
    cout<<"EventCount = "<<bluetwavehit->EventCount<<endl;
    cout<<"TimeStamp = "<<bluetwavehit->TimeStamp<<endl;
    anodemaster->SetWaveHit(bluetwavehit);
    anodemaster->ReEvent(true, true);
    anodemaster->OutputPicture();
    // ------ ui command enter ---------- // 
    cout<<"\n===> Enter command: \n";
    cout<<"[b]: back \n";
    cout<<"[c]: continue \n";
    cout<<"[n]: your entry number \n";
    cout<<"[q]: quit \n";
    cout<<"Your command: ";
	  cin>>enter;
    if(enter=='n') { cout<<"Please enter EntryID: "; cin>>enterID; i = -1; }
    if(enter=='b') i=i-2;    
  }
  
	return 0;
} // end of main





