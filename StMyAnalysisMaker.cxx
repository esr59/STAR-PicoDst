#include "StMyAnalysisMaker.h"
#include "StRoot/StPicoDstMaker/StPicoDst.h"
#include "StRoot/StPicoDstMaker/StPicoTrack.h"
#include "StRoot/StPicoDstMaker/StPicoMtdHit.h"
#include "StRoot/StPicoDstMaker/StPicoEvent.h"
#include "StRoot/StPicoDstMaker/StPicoV0.h"
#include "StRoot/StPicoDstMaker/StPicoDstMaker.h"
#include"StLorentzVector.hh"
#include "StThreeVectorF.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "math.h"


ClassImp(StMyAnalysisMaker)

//-----------------------------------------------------------------------------
StMyAnalysisMaker::StMyAnalysisMaker(const char* name, StPicoDstMaker *picoMaker, const char* outName)
  : StMaker(name)
{
  mPicoDstMaker = picoMaker;
  mPicoDst = 0;
  mOutName = outName;
}

//----------------------------------------------------------------------------- 
StMyAnalysisMaker::~StMyAnalysisMaker()
{ /*  */ }

//----------------------------------------------------------------------------- 
Int_t StMyAnalysisMaker::Init() {
  DeclareHistograms();
  return kStOK;
}

//----------------------------------------------------------------------------- 
Int_t StMyAnalysisMaker::Finish() {
  if(mOutName!="") {
    TFile *fout = new TFile(mOutName.Data(),"RECREATE");
    fout->cd();
    WriteHistograms();
    fout->Close();
  }
  return kStOK;
}

//-----------------------------------------------------------------------------
void StMyAnalysisMaker::DeclareHistograms() {
 /* mKsM     = new TH1F("KsM","",300,0.35,0.65);
  mLambdaM = new TH1F("LambdaM","",400,1.05,1.25);
  mLbarM   = new TH1F("LbarM","",400,1.05,1.25);
  mKsRM     = new TH1F("KsRM","",300,0.35,0.65);
  mLambdaRM = new TH1F("LambdaRM","",400,1.05,1.25);
  mLbarRM   = new TH1F("LbarRM","",400,1.05,1.25);
  mdedxvspt     = new TH2F("dedxvspt","",500,0.,5.0,500,0.,50.);
  mKsDecayL     = new TH2F("KsDecayL","",500,0.,5.0,500,0.,50.);
  mLambdaDecayL = new TH2F("LambdaDecayL","",500,0.,5.0,500,0.,50.);
  mLbarDecayL   = new TH2F("LbarDecayL","",500,0.,5.0,500,0.,50.);
  hTriggerPt = new TH1F("hTriggerPt","Trigger Pt",100,1,10);*/
  hTracks = new TH1F("hTracks","",250,1,1400);
  hZvertex = new TH1F("hZvertex","Primary Vertex Z Location",250,-200,200);
  hgTracks = new TH1F("hgTracks","Global Tracks",250,0,5000);
  hrankPver = new TH1F("hrankPver","Ranking of the Primary Vertex",50,-20,20);
  hPtAll = new TH1F("hPtAll","Pt of All Tracks",100,-1,10);
  hPtMTD = new TH1F("hPtMTD","Pt of Tracks with MTD Hit",100,-1,10);
  hPtHFT = new TH1F("hPtHFT","Pt of Tracks with HFT Hit",100,-1,10);
  hPtAllvsVz = new TH2F("hPtAllvsVz","Pt of Global Tracks vs Vertex Z",500,-100.,100.,500,0.,5.5);
  hPtHFTvsVz = new TH2F("hPtHFTvsVz","Pt of Tracks with HFT Hit vs Vertex Z",500,-100.,100.,500,0.,5.5);
  hmlambda = new TH1F("hmasslambda","Invariant Mass of Lambda",1000,1.0,2.0);
}

//-----------------------------------------------------------------------------
void StMyAnalysisMaker::WriteHistograms() {
 /* mKsM->Write();
  mLambdaM->Write();
  mLbarM->Write();
  mKsRM->Write();
  mLambdaRM->Write();
  mLbarRM->Write();
  mdedxvspt->Write();
  mKsDecayL->Write();
  mLambdaDecayL->Write();
  mLbarDecayL->Write();
  hTriggerPt->Write();*/
  hTracks->Write();
  hZvertex->Write();
  hgTracks->Write();
  hrankPver->Write();
  hPtAll->Write();
  hPtMTD->Write();
  hPtHFT->Write();
  hPtAllvsVz->Write();
  hPtHFTvsVz->Write();
  hmlambda->Write();
}

//----------------------------------------------------------------------------- 
void StMyAnalysisMaker::Clear(Option_t *opt) {
}

//----------------------------------------------------------------------------- 
Int_t StMyAnalysisMaker::Make() {

  if(!mPicoDstMaker) {
    LOG_WARN << " No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }

  mPicoDst = mPicoDstMaker->picoDst();

  if(!mPicoDst) {
    LOG_WARN << " No PicoDst! Skip! " << endm;
    return kStWarn;
  }

  StPicoEvent* mPicoEvent = mPicoDst->event();
  unsigned int ntracks = mPicoDst->numberOfTracks();
  
  //Number of tracks histogram
  hTracks->Fill(ntracks);
  
  //Primary Z Vertex Histogram cout<<"  my vertex z value:  "<<mPicoEvent->primaryVertex().z()<<endl;
  hZvertex->Fill(mPicoEvent->primaryVertex().z());
  
  //Number of Global Tracks Histogram
  hgTracks->Fill(mPicoEvent->numberOfGlobalTracks());
  
  //Ranking of Primary Vertex
  hrankPver->Fill(mPicoEvent->ranking());
  
  //Track Loop 
 

  //Mass of Proton and Pion
  float mpion = 0.13957;
  float mproton = 0.93828;
  
  
  for(unsigned short itrack=0;itrack<ntracks;itrack++){
    //cout << "checking" << endl;
    StPicoTrack* trk = (StPicoTrack*)mPicoDst->track(itrack);
    if(!trk){ continue; }

    // get index of MTD 
    int MTDpid = trk->mtdPidTraitsIndex();
    
    //Pt of All Tracks
    hPtAll->Fill(trk->gPt());
    
    if(MTDpid>0){
      //Pt of Tracks with MTD Hits
      hPtMTD->Fill(trk->gPt());
    }
    
    if(trk->isHFTTrack()){
      //Global Tracks with HFT Hit
      hPtHFT->Fill(trk->gPt());
      //Pt of HFT Tracks vs Vz
      hPtHFTvsVz->Fill(mPicoEvent->primaryVertex().z(),trk->gPt());
    }

    //Pt of All Tracks vs Vz
    hPtAllvsVz->Fill(mPicoEvent->primaryVertex().z(),trk->gPt());
    
    //Choose only Protons
    if(trk->isPrimary() && trk->nSigmaProton()<2 && trk->nSigmaProton()>-2 && trk->gPt()>0.2 && trk->charge()>0){
      
      StThreeVectorF protonmom = trk->pMom();
      float protonpx = protonmom.x();
      float protonpy = protonmom.y();
      float protonpz = protonmom.z();

      
      //	  cout<<"mymom pt:   "<<mymom.perp()<<"    "<<trk->gPt()<<endl;
      //   cout<<"mymomentum all:   "<<mymom.magnitude()<<"    "<<trk->gPtot()<<endl;
      
      //Going over the tracks again to get Pions
      for(unsigned short itrk=0;itrk<ntracks;itrk++)
	{
	  StPicoTrack* track = (StPicoTrack*) mPicoDst->track(itrk);
	  if(!track){continue;}
	  if(itrack==itrk){continue;}
	  //Choose only Pions
	    if(track->isPrimary() && track->nSigmaPion()<2 && track->nSigmaPion()>-2 && track->gPt()>0.2 && track->charge()<0)
	    {
	      
	      StThreeVectorF pionmom = track->pMom();
	      float pionpx = pionmom.x();
	      float pionpy = pionmom.y();
	      float pionpz = pionmom.z();

	      
	      //Pion Momentum
	      float pionP = pionmom.magnitude();
	      
	      //Proton Momentum
	      float protonP = protonmom.magnitude();
	      
	      
	      //Energy of Proton and Pion
	      float epion = sqrt(pow(pionP,2.0)+pow(mpion,2.0));
	      float eproton =sqrt(pow(protonP,2.0)+pow(mproton,2.0));
	      
	      //Invariant Mass of Lambda
	      float mlambda = sqrt(pow((mproton),2.0)+pow((mpion),2.0)+(2.0*epion*eproton)-(2.0*((protonpx*pionpx)+(protonpy*pionpy)+(protonpz*pionpz))));
	      
	
	      //Fill the histogram for invariant mass of lambda
	      hmlambda->Fill(mlambda);
	    }
	    
	   	      
	} 
    }
    
    
    
	
    
  }
  
  //-------------------------------------------------------------------------------

//  mPicoDst->Print();
//  mPicoDst->printTracks();

  /*int nKs = mPicoDst->numberOfKs();
  for(int i=0;i<nKs;i++) {
    StPicoV0 *ks = (StPicoV0*)mPicoDst->ks(i);
    if(!ks) continue;
    mKsM->Fill(ks->m());
    mKsDecayL->Fill(ks->momentum().perp(), ks->decayLength());

//    cout << " ****** i = " << i << " m = " << ks->m() << endl;
//    cout << " daughter mom = " << ks->momentum(pos) << "\t" << ks->momentum(neg) << endl;
    ks->rotateTrack(neg);
//    cout << " daughter mom after rotation = " << ks->momentum(pos) << "\t" << ks->momentum(neg) << endl;
    ks->setParticleHypothesis(pion,pion);
    mKsRM->Fill(ks->m());
//    cout << " new mass = " << ks->m() << endl;
  }

  int nL = mPicoDst->numberOfLambda();
  for(int i=0;i<nL;i++) {
    StPicoV0 *lambda = (StPicoV0*)mPicoDst->lambda(i);
    if(!lambda) continue;
    mLambdaM->Fill(lambda->m());
    mLambdaDecayL->Fill(lambda->momentum().perp(), lambda->decayLength());

    lambda->rotateTrack(neg);
    lambda->setParticleHypothesis(proton,pion);
    mLambdaRM->Fill(lambda->m());
  }

  int nLbar = mPicoDst->numberOfLbar();
  for(int i=0;i<nLbar;i++) {
    StPicoV0 *lbar = (StPicoV0*)mPicoDst->lbar(i);
    if(!lbar) continue;
    mLbarM->Fill(lbar->m());
    mLbarDecayL->Fill(lbar->momentum().perp(), lbar->decayLength());
    }*/

  return kStOK; 
}

