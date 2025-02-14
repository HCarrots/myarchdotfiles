#ifndef __BLUETDATAMODEL_
#define __BLUETDATAMODEL_

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TString.h"
#include <array>
#include "BluetConfig.h"

class BluetWavePara {
 public:
  int HitStripID;  
  double HitT;
  double HitAmp;
  double HitTau;
  double HitPeakVal;
  double HitBaseline;
  double HitChi2;
  double HitNDF;
  double HitTime; // ns  in 10 ms time window  

  BluetWavePara() {}
  ~BluetWavePara() {}

  inline void Reset() {
    HitStripID = -999;
    HitT=HitAmp=HitTau=HitPeakVal=HitBaseline=HitChi2=HitNDF=HitTime=-999.; 
  } 

  ClassDefInline(BluetWavePara, 1);  

};

class BluetWaveHit {
 public:
  uint32_t EventCount;
  unsigned long long TimeStamp;  
  std::vector<BluetWavePara> WavePara;

  BluetWaveHit(): EventCount(-999), TimeStamp(-999) {}
  ~BluetWaveHit() {}

  inline void Reset() {
    EventCount = -999;
    TimeStamp = -999;
    WavePara.clear(); 
  } 
  
  ClassDefInline(BluetWaveHit, 1);  
};

class BluetHit {
 public:
  TString HitTypeName;
  double HitPos;
  double HitT;
  double HitAmp;
  double HitTau;
  double HitPeakVal;
  double HitBaseline;
  double HitChi2;
  int HitBoardID, HitChipID, HitChannelID;
  
  //BluetHitWave HitWave;
  //BluetHit() { HitWave = BluetHitWave(); }
  
  inline void Reset() { 
    HitTypeName = "none";
    HitPos=HitT=HitAmp=HitTau=HitPeakVal=HitBaseline=HitChi2=-999.; 
    HitBoardID=HitChipID=HitChannelID=-999; 
  }
  ClassDefInline(BluetHit, 1);
};

/*class BluetSparkHits {
 public:
  std::vector<double> HitAmp;
  std::vector<double> HitDist;
  std::vector<double> Residual;
  std::vector<double> UnbiasedResidual;
  std::vector<double> UnbiasedResidualX;
  std::vector<double> UnbiasedResidualY;
  
  BluetTrHits() {}
  ~BluetTrHits() {}

  inline void InsertHitAmp(const double& amp) { HitAmp.push_back(amp); }
  inline void InsertHitDist(const double& dist) { HitDist.push_back(dist); }
  inline void InsertHitResidual(const bool isUnb, const double& r, const int& opt = 2) {
    if (!isUnb) Residual.push_back(r);
    else {
      if (opt == 2) UnbiasedResidual.push_back(r);
      if (opt == 1) UnbiasedResidualY.push_back(r);
      if (opt == 0) UnbiasedResidualX.push_back(r);
    }
  }
  inline void Reset(){
    HitAmp.clear();
    HitDist.clear();
    Residual.clear();
    UnbiasedResidual.clear();
    UnbiasedResidualX.clear();
    UnbiasedResidualY.clear();
  }
  ClassDefInline(BluetTrHits, 1);
};*/

class BluetSpark {
 public:
  int Nhits;
  int NhitsX;
  int NhitsY;
  double SparkSumE;
  double TminX;
  double TmaxX;
  double TminY;
  double TmaxY;
  double SigmaX;
  double SigmaY;
  double SigmaT;
  std::array<double, 2>CentroidPos ;
  double TOF0; /**< TOF before calibration */
  //double TrPhi;
  //double TrCosTheta;
  //double TrRawLength;
  //double ZmaxHit_xy[2];
  //double Zmax_xy[2];
  //BluetTrHits TrHits;
  
  //BluetTrack() { TrHits = BluetTrHits(); }
  BluetSpark() {}
  ~BluetSpark() {}
  inline void Reset(){
    Nhits = NhitsX = NhitsY = -999.;
    SparkSumE = TminX = TmaxX = TminY = TmaxY =  -999.;
    SigmaX = SigmaY = SigmaT = -999.;
    CentroidPos[0] = CentroidPos[1] = -999.;
    TOF0 = -999.;
  }
  ClassDefInline(BluetSpark, 1);
};

class BluetEvent {
 public:
  uint32_t EventCount;
  unsigned long long TimeStamp;  

  BluetEvent() {}
  ~BluetEvent() {}

/*   std::vector<BluetHit> Hits; */
  std::vector<BluetSpark> Sparks;
  inline void Reset() { 
    TimeStamp=0; 
    EventCount=-999; 
    Sparks.clear(); 
  }
  ClassDefInline(BluetEvent, 1);
};

/*class DST {
 public:
  Int_t fNhit;
  UInt_t fEventCount;
  ULong64_t fTimeStamp;
  std::vector<Int_t> *fHitBoardID;
  std::vector<Int_t> *fHitChipID;
  std::vector<Int_t> *fHitChannelID;
  std::vector<Int_t> *fWaveForm;
  
  void Clear() { Int_t *ptr = (Int_t *)this;
    Int_t size = sizeof(DST)/sizeof(Int_t);
    for (Int_t i = 0; i < size; ++i) ptr[i] = 0;
  }

  void SetAddress(TTree *tree) {
    tree->SetBranchAddress("Nhit", &fNhit);
    tree->SetBranchAddress("EventCount", &fEventCount);
    tree->SetBranchAddress("TimeStamp", &fTimeStamp);
    tree->SetBranchAddress("HitBoardID", &fHitBoardID);
    tree->SetBranchAddress("HitChipID", &fHitChipID);
    tree->SetBranchAddress("HitChannelID", &fHitChannelID);
    tree->SetBranchAddress("WaveForm", &fWaveForm);
  }
};*/


/*class DstFill : public DST {

 public:
  TChain     *fChain;
  TFile      *fFile;
  TTree      *fTree;
  BluetEvent *fEvent;
  Int_t       fEntry;
  Int_t       fWaveLength;
  Double_t    fSamplingTime;
  Double_t    fVdrift;
  Double_t    fDriftLength;
  Int_t       fMinvotes;
  Int_t       fNlines;
  Double_t    fDx;
  BluetConfig    *fConfig;
  BluetPadMaster *fPadMaster;

  //nlohmann::json data_event;
  ofstream o;
  
 DstFill(TChain *ch) : 
   fChain(ch), fFile(0), fTree(0), fEntry(-1), fEvent(0), fWaveLength(0), fSamplingTime(0), fVdrift(0), fDriftLength(0), fConfig(0), fPadMaster(0), fMinvotes(3), fNlines(0), fDx(3.03)
    {
      SetAddress(ch);
    }

  void LoadTree(Int_t entry) { fEntry = fChain->LoadTree(entry); Clear(); }
  void GetEntry() { fChain->GetEntry(fEntry); }
  
  void InitHists(const TString fname);
  void InitHists(const TString fname, const Double_t v);

  void  EventLoop(void);
  void  FillRecHit(void);
  void  FillRecTrk(void);

  void Fill(const char *hname, Double_t x, Double_t y, Double_t w = 1);
  void Fill1D(const char *hname, Double_t x, Double_t w = 1);
};*/


#endif //__DATAMODEL__
