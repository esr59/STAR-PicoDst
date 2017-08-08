#ifndef StMyAnalysisMaker_h
#define StMyAnalysisMaker_h

#include "StMaker.h"
class StPicoDst;
class StPicoDstMaker;
class TString;
class TH1F;
class TH2F;

class StMyAnalysisMaker : public StMaker {
  public:
    StMyAnalysisMaker(const char *name, StPicoDstMaker *picoMaker, const char *outName);
    virtual ~StMyAnalysisMaker();
    
    virtual Int_t Init();
    virtual Int_t Make();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t Finish();
    
    void    DeclareHistograms();
    void    WriteHistograms();
    
  private:
    StPicoDstMaker *mPicoDstMaker;
    StPicoDst      *mPicoDst;
    
    TString    mOutName;
    
    /*TH1F*      mKsM;
    TH1F*      mLambdaM;
    TH1F*      mLbarM;    
    TH1F*      mKsRM;
    TH1F*      mLambdaRM;
    TH1F*      mLbarRM;
    
    TH2F*      mdedxvspt;
    TH2F*      mKsDecayL;
    TH2F*      mLambdaDecayL;
    TH2F*      mLbarDecayL;

    TH1F* hTriggerPt;*/

//------------------------------------------------------------------------------

     TH1F* hTracks;
     TH1F* hZvertex;
     TH1F* hgTracks;
     TH1F* hrankPver;
     TH1F* hPtAll;
     TH1F* hPtMTD;
     TH1F* hPtHFT;

     //invariant mass of lambda by using tracks via decay of proton and pions
     TH1F* hmlambda;

     //2D Histograms

     TH2F* hPtAllvsVz;
     TH2F* hPtHFTvsVz;

    
                    
    ClassDef(StMyAnalysisMaker, 1)
};

#endif
