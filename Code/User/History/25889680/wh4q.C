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
#include <TView.h>

#include "vector"
#include "iostream"
#include "string"
#include "cstdlib"
#include "cstdint"

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
  
  for(int iarg = 0; iarg<argc; ++iarg){
    string opt = argv[iarg];
    if(opt=="-dec") { fEnableDec = true; cout<<">>> Enable decovolution."<<endl; }
    if(opt=="-cut") { fEnableCut = true; cout<<">>> Enable cut."<<endl; }
    if(opt=="-track") { fEnableTrack = true; cout<<">>> Enable track."<<endl; }
    if(opt=="-fit") { fEnableFit = true; cout<<">>> Enable fit."<<endl; }
  }

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
    return -1;
  }
  
	
//-----------------------------
// read wave data 
//-----------------------------
  TApplication* app = new TApplication("app", &argc, argv);

  BluetConfig* config = BluetConfig::GetInstance();
  config->ReadConfigFile();
  string DAQType = config->GetDAQType();
  string workMode = config->GetWorkMode();
  const double stampCutMin = config->GetTimeStampCutMin(); 
  const double stampCutMax = config->GetTimeStampCutMax(); 
  const int NhitCutMin = config->GetNhitCutMin();
  const int NhitCutMax = config->GetNhitCutMax();

  BluetWaveformMaster* wavemaster = new BluetWaveformMaster();
  wavemaster->SetNPadXY0(4,4);
  wavemaster->SetNPadXY1(4,4);
  wavemaster->SetNCanvas(10);
  wavemaster->SetCanvasSize(5200,2600);  
  wavemaster->InitCanvas();
  wavemaster->SetPicDir(Dir_Pic);


  
	
  char enter = 'c';
  int enterID = 0;



//-----------------------------
// define tree
//-----------------------------
	int Nhit{};
	uint32_t EventCount{};
	unsigned long long TimeStamp{};
	vector<int> *HitBoardID{nullptr};
	vector<int> *HitChipID{nullptr};
	vector<int> *HitChannelID{nullptr};
	vector<int> *WaveForm{nullptr};
	vector<double> *RawWaveForm{nullptr};
	vector<double> *AmpWaveForm{nullptr};
	vector<double> *PZWaveForm{nullptr};
	

  TString Treename{};
	if(DAQType=="MTPC")Treename = "mcpdata"; // for MTPC DAQ
	if(DAQType=="AGET")Treename = "agetdata"; // for AGET DAQ
  auto tpcTree = new TChain(Treename);
  for ( auto c: vname_froot)
  {
    tpcTree->Add(c);
  }
  
  tpcTree->SetBranchAddress("Nhit", &Nhit);
  tpcTree->SetBranchAddress("EventCount", &EventCount);
  tpcTree->SetBranchAddress("TimeStamp", &TimeStamp);
  tpcTree->SetBranchAddress("HitBoardID", &HitBoardID);
  tpcTree->SetBranchAddress("HitChipID", &HitChipID);
  tpcTree->SetBranchAddress("HitChannelID", &HitChannelID);
  tpcTree->SetBranchAddress("WaveForm", &WaveForm);
  
  if(workMode=="Sim"){
    tpcTree->SetBranchAddress("RawWaveForm", &RawWaveForm);
    tpcTree->SetBranchAddress("AmpWaveForm", &AmpWaveForm);
    tpcTree->SetBranchAddress("PZWaveForm", &PZWaveForm);
  }
  


//-----------------------------
// read wave data and draw
//-----------------------------
	const int Nevent = tpcTree->GetEntries();
	for(Long64_t i=0; (i<Nevent)&&(enter!='q'); i++){
		
    tpcTree->GetEntry(i);
    // 
    // <<< event cuts <<< 
    //
    if( i<enterID ) continue;
    if( (Nhit<NhitCutMin || Nhit>NhitCutMax) && fEnableCut ) continue;
    if( (TimeStamp<stampCutMin || TimeStamp>stampCutMax) && fEnableCut ) continue;
      
    cout<<"\n-----------------------"<<endl;
    cout<<"===> ievent = "<<i;
    cout<<"\n-----------------------"<<endl;
    cout<<"EventCount = "<<EventCount<<endl;
    cout<<"TimeStamp = "<<TimeStamp<<endl;
    cout<<"Nhit = "<<Nhit<<endl<<endl;
	
    // ------- fit and tracking --------- //
    wavemaster->SetEventCount(EventCount);   
    wavemaster->SetTimeStamp(TimeStamp);
    cout<<"SetTimeStamp"<<endl;
    wavemaster->SetWaveformInfo(Nhit, HitBoardID, HitChipID, HitChannelID, WaveForm, RawWaveForm, AmpWaveForm, PZWaveForm);
    cout<<"SetWaveformInfo"<<endl;
    wavemaster->DrawWaveform4();
    cout<<"DrawWaveform4"<<endl;    
    wavemaster->FitWave();
    cout<<"FitWaveformMode3"<<endl;        
    wavemaster->DrawFitFunctionMode4();
    wavemaster->OutputPicture();

    // FillHistogramsAndGraphs(WaveForm, HitBoardID, HitChipID, HitChannelID);
		
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
    //trackmaster->CloseCanvas();
	} // i loop end
	
//-----------------------------
// save waveform
//-----------------------------



//-----------------------------
// free memories
//-----------------------------



//-----------------------------
// end of main
//-----------------------------
    //app->Run();
	return 0;

} // end of main







void FillHistogramsAndGraphs(const std::vector<int> *vec, const vector<int> *HitBoardID,const vector<int> *HitChipID,const vector<int> *HitChannelID) {
    // 示例数据填充
    BMCPAnodeStrip *anodestrip = BMCPAnodeStrip::GetInstance();
    const int Nbin = 128; // 每个TH1D的bin数量
    TMultiGraph *mg = new TMultiGraph(); // 创建TMultiGraph
    TMultiGraph *mgy = new TMultiGraph(); // 创建TMultiGraph
    vector<int> strips;
    vector<TGraph*> graphs;
    int iG = 0;
    // 按每128个值分组处理vector
    for (size_t i = 0; i < vec->size(); i += Nbin, iG++) {
        // 创建一个TH1D对象
        TH1D *h1 = new TH1D(Form("h%d", int(i/Nbin)), "Histogram", Nbin, 0, Nbin);
        // 填充TH1D
        for (size_t j = i; j < i + Nbin && j < vec->size(); ++j) {
            h1->SetBinContent(j - i + 1, (*vec)[j]);
        }

        // 将TH1D的数据点填充到TGraph
        TGraph *gr = new TGraph();
        for (int k = 1; k <= Nbin; ++k) {
            gr->SetPoint(k-1, k-1, h1->GetBinContent(k));
        }
        // 将TGraph添加到TMultiGraph
        strips.push_back(anodestrip->map_to_strip((*HitBoardID)[iG], (*HitChipID)[iG], (*HitChannelID)[iG]));
        graphs.push_back(gr);
    }
    cout<<"store"<<endl;
    for(int i = 0; i<256; i++){
      int select_graph = -1;
      for(int igraph = 0; igraph<strips.size(); igraph++){
        if(strips[igraph]==i)
          select_graph = igraph;
      }
      if(select_graph != -1 ){
        if(i<128){
            graphs[select_graph]->SetLineColor(kRed);
            mg->Add(graphs[select_graph]);
        }
        else{
            graphs[select_graph]->SetLineColor(kRed);
            mgy->Add(graphs[select_graph]);
        }
      }
      else{
        if(i<128){
            TGraph *gr = new TGraph();
            for (int k = 1; k <= 128; ++k) {
                gr->SetPoint(k-1, k-1, 0);
            }
            gr->SetLineWidth(0);
            mg->Add(gr);
        }
        else{
            TGraph *gr = new TGraph();
            for (int k = 1; k <= 128; ++k) {
                gr->SetPoint(k-1, k-1, 0);
            }
            gr->SetLineWidth(0);
            mgy->Add(gr);
        }
          
      }
    }
    cout<<"mg->Draw(ALP"<<endl;
    // 绘制TMultiGraph
    TCanvas *c1 = new TCanvas("c1", "Multiple Graphs", 800, 600);
    mg->Draw("a fb l3d"); // 使用线和点的方式绘制

    // 可以调整TMultiGraph的标题、坐标轴标题等属性
    mg->SetTitle("Multiple Graphs;X axis;Y axis");
    TView *view = TView::CreateView(1);
    view->RotateView(120, 30);
    c1->Update();
    c1->SaveAs("/dg_workfs/BESG/lusupeng22/DghpcData/BLUET-WNRI-v4/Scripts/2D.png");
    TCanvas *c2 = new TCanvas("c2", "Multiple Graphs", 800, 600);
    mgy->Draw("a fb l3d"); // 使用线和点的方式绘制
    TView *view2 = TView::CreateView(1);
    view2->RotateView(30, 30);
    c2->Update();
    c2->SaveAs("/dg_workfs/BESG/lusupeng22/DghpcData/BLUET-WNRI-v4/Scripts/2D——yaxis.png");

    
}