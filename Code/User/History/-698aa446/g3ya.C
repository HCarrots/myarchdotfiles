//----------------------------------------------------------------------------------------------
// version: v1.0
// code log:
// 1) waveforms fitting and hits tracking with BluetWaveformMaster and BluetTrackMaster.
// 2) fitting and tracking are based on BluetWaveFit and BluetHough3D developed by yangli.
// author: yih@ihep.ac.cn
// date: 2023-03-30
//
// version: v2.0
// code log:
// 1) modigy for the MCP data analysis
// author: yih@ihep.ac.cn
// date: 2023-01-16
//----------------------------------------------------------------------------------------------

#include "TFile.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TString.h"
#include "TObjString.h"
#include "TROOT.h"

#include "vector"
#include "iostream"
#include "string"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#include "BluetConfig.h"
#include "BluetDataModel.h"
#include "BluetMCPAnodeMaster.h"
#include "BluetWaveformMaster.h"
#include <ROOT/RLogger.hxx>

//#include "BluetWaveFit.h"
//#include "BluetPadMaster.h"

using namespace std;
using namespace TMath;


//########################################
// main()
//########################################
int main(int argc, char** argv){

//-----------------------------
//suppress output to terminal
//-----------------------------
  gErrorIgnoreLevel = kWarning; 

//-----------------------------
// define variables 
//-----------------------------
  gInterpreter->Declare("#include"+ \"../BluetAna/include/BluetDataModel.h\"");
  string infilename = (string)argv[1];
  TString tfilename = infilename;
  const int Njob = atoi(argv[2]); 
  const string jobID = argv[3];
  const string fileID = argv[4];
  const string outdir = argv[5];
  bool fEnableTrack = false;
  
  BluetConfig* config = BluetConfig::GetInstance(atoi(fileID.c_str()), atoi(jobID.c_str()));
  config->ReadConfigFile();
  
  BluetWaveformMaster* wavemaster = new BluetWaveformMaster();


  const string workMode = config->GetWorkMode();
  const string daqType = config->GetDAQType();


  string readfile = infilename;
  cout<<"===> inputfile name : "<<readfile<<endl;
  if(gSystem->AccessPathName(readfile.c_str())){
    cout<<"*** Can not open root file !!! ***"<<endl;
    return -1;
  }
  TFile* infile = TFile::Open(infilename.c_str(), "READ");
  string outfilename = outdir + "/_" + jobID + ".root";
  cout<<"===> outputfile name : "<<outfilename<<endl;
  TFile* outfile = TFile::Open(outfilename.c_str(), "recreate");
  outfile->cd();

//-----------------------------
// define tree
//-----------------------------
  int Nhit;
  int EventID;
  uint32_t EventCount;
  unsigned long long TimeStamp;
  vector<int> *HitBoardID = 0;
  vector<int> *HitChipID = 0;
  vector<int> *HitChannelID = 0;
  vector<int> *WaveForm = 0;
  vector<double> *RawWaveForm;
  vector<double> *AmpWaveForm;
  vector<double> *PZWaveForm;

  TTree* mcpTree = nullptr;
  /* if(daqType=="AGET") infile->GetObject("agetdata", mcpTree);  */// for AGET DAQ
  if(daqType=="MTPC") infile->GetObject("mcpdata", mcpTree); // for MTPC DAQ
  mcpTree->SetBranchAddress("Nhit", &Nhit);
  mcpTree->SetBranchAddress("EventCount", &EventCount);
  mcpTree->SetBranchAddress("TimeStamp", &TimeStamp);
  mcpTree->SetBranchAddress("HitBoardID", &HitBoardID);
  mcpTree->SetBranchAddress("HitChipID", &HitChipID);
  mcpTree->SetBranchAddress("HitChannelID", &HitChannelID);
  mcpTree->SetBranchAddress("WaveForm", &WaveForm);

  if(workMode=="Sim"){
      mcpTree->SetBranchAddress("RawWaveForm", &RawWaveForm);
      mcpTree->SetBranchAddress("AmpWaveForm", &AmpWaveForm);
      mcpTree->SetBranchAddress("PZWaveForm", &PZWaveForm);
  }
    
  //BluetHit fHit;
  auto fWaveHit = new BluetWaveHit();
  auto eventTree = new TTree("WaveTree", "WaveTree contains the wave info.");
  eventTree->Branch("BluetWaveHit", &fWaveHit);
  eventTree->SetDirectory(outfile);
  wavemaster->SetEvent(fWaveHit);


//-----------------------------
// read wave data and draw
//-----------------------------
	
  //const int Nevent = 500;
  const int Nevent = mcpTree->GetEntries();
  const int Njobeve = Nevent/Njob;
  int startID = 0;
  int stopID = 0;
  startID = atoi(jobID.c_str())*Njobeve;
  if( (Njob-atoi(jobID.c_str()))>1 ) stopID = startID + Njobeve;
  if( (Njob-atoi(jobID.c_str()))==1 ) stopID = Nevent;
  cout<<"Total Event = "<<Nevent<<endl;
  cout<<"===> Start data reading ..."<<endl;
	//stopID = 10000;
  for(int i=startID; i<stopID; i++){
    if( Njobeve>=100 && (i-startID)%(Njobeve/5)==0 ){ 
        cout<<"JobID = "<<jobID<<", ievent = "<<(i-startID)<<", ["<<(i-startID)*100/Njobeve<<"%]"<<endl;
    }
    
    // ----- fitting -----
    mcpTree->GetEntry(i);
    wavemaster->SetTimeStamp(TimeStamp);
    wavemaster->SetEventCount(EventCount);
    wavemaster->SetWaveformInfo(Nhit, HitBoardID, HitChipID, HitChannelID, WaveForm, RawWaveForm, AmpWaveForm, PZWaveForm);
    wavemaster->FitWave();
    wavemaster->FillTree(eventTree);
    
  } // end of read data loop
	
  cout<<"===> JobID "<<jobID<<": Read data loop end."<<endl;

//-----------------------------
// save file
//-----------------------------

  eventTree->Write();
  outfile->Write();  
  outfile->Close();
  delete outfile;
	

//-----------------------------
// free memories
//-----------------------------


//-----------------------------
// end of main
//-----------------------------
	return 0;

} // end of main


