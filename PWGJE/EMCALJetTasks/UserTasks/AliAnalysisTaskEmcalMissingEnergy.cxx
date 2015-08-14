//
// Hadron Trigger & Jet Substructure.
//
// Summer Student: C.Durante - Supervisor: L.Cunqueiro

#include <TClonesArray.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <THnSparse.h>
#include <TTree.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TProfile.h>
#include <TChain.h>
#include <TSystem.h>
#include <TFile.h>
#include <TKey.h>
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TVector3.h"
#include "TVector2.h"
#include "AliVCluster.h"
#include "AliVTrack.h"
#include "AliEmcalJet.h"
#include "AliRhoParameter.h"
#include "AliLog.h"
#include "AliEmcalParticle.h"
#include "AliMCEvent.h"
#include "AliGenPythiaEventHeader.h"
#include "AliAODMCHeader.h"
#include "AliMCEvent.h"
#include "AliAnalysisManager.h"
#include "AliJetContainer.h"
#include "AliParticleContainer.h"
#include "AliPythiaInfo.h"
#include "TRandom3.h"
#include "AliPicoTrack.h"
#include "AliEmcalJetFinder.h"
#include "AliAODEvent.h"
#include "AliAnalysisTaskEmcalMissingEnergy.h"
#include <algorithm>

using std::cout;
using std::endl;

ClassImp(AliAnalysisTaskEmcalMissingEnergy)

//________________________________________________________________________
AliAnalysisTaskEmcalMissingEnergy::AliAnalysisTaskEmcalMissingEnergy() : 
AliAnalysisTaskEmcalJet("AliAnalysisTaskEmcalMissingEnergy", kTRUE),
  fContainer(0),
  fMinFractionShared(0),
  fJetShapeType(kData),
  fJetShapeSub(kNoSub),
  fJetSelection(kInclusive),
  fSubstructureVar(0),
  fPtThreshold(-9999.),
  fRMatching(0.2),
  fJetRadius(0.4),
  fSubJetRadius(0.2),
  fminpTTrig(20.),
  fmaxpTTrig(50.),
  fangWindowRecoil(0.6),
  fSemigoodCorrect(0),
  fHolePos(0),
  fHoleWidth(0), 
  fCentSelectOn(kTRUE),
fHTon(kTRUE),
fCentMin(0),
fCentMax(10),
fh2ResponseUW(0x0),
fh2ResponseW(0x0), 
fPhiJetCorr6(0x0), 
  fPhiJetCorr7(0x0),
  fEtaJetCorr6(0x0),
  fEtaJetCorr7(0x0),
  fPtJetCorr(0x0),
  fPtJet(0x0),
  fhpTjetpT(0x0),
  fhPt(0x0),
  fhJetPt(0x0),
  fhTrackPt(0x0),
  fhTriggerPt(0x0),
  fhJetTriggeredPt(0x0),
  fhTriggerVSjetPt(0x0),
  fhdPhiTrigVSjetPt(0x0),
  fhdPhiTrigVSjetPtNOCUT(0x0),
  fhdPhiPartVSjetPt(0x0),
  fhdPhiPartVSjetPtVSPartPt(0x0),
  fhPhi(0x0),
  fSubstructure(0),
  fHadronTrigger(0),
  fhTau1(0x0),
  fhTau2(0x0),
  fhTau3(0x0),
  fhTau1vsTau2(0x0),
  fhTau1vsTau3(0x0),
  fhTau2vsTau3(0x0),
  fhTau2OverTau1(0x0),
  fhTau3OverTau2(0x0),
  fhTau2vsJetPt(0x0),
  fhTau2OverTau1vsJetPt(0x0),
  fhTau1vsTau2vsTau3(0x0),
  fhTrackPt_JT(0x0)

{
  SetMakeGeneralHistograms(kTRUE);
}

//________________________________________________________________________
AliAnalysisTaskEmcalMissingEnergy::AliAnalysisTaskEmcalMissingEnergy(const char *name) : 
  AliAnalysisTaskEmcalJet(name, kTRUE),
  fContainer(0),
  fMinFractionShared(0),
  fJetShapeType(kData),
  fJetShapeSub(kNoSub),
  fJetSelection(kInclusive),
  fSubstructureVar(0),
  fPtThreshold(-9999.),
  fRMatching(0.2),
  fJetRadius(0.4),
  fSubJetRadius(0.2),
  fminpTTrig(20.),
  fmaxpTTrig(50.),
  fangWindowRecoil(0.6),
  fSemigoodCorrect(0),
  fHolePos(0),
  fHoleWidth(0), 
  fCentSelectOn(kTRUE),
  fHTon(kTRUE),
  fCentMin(0),
  fCentMax(10),
  fh2ResponseUW(0x0),
  fh2ResponseW(0x0),
  fPhiJetCorr6(0x0), 
  fPhiJetCorr7(0x0),
  fEtaJetCorr6(0x0),
  fEtaJetCorr7(0x0),
  fPtJetCorr(0x0),
  fPtJet(0x0),
  fhpTjetpT(0x0),
  fhPt(0x0),
  fhJetPt(0x0),
  fhTrackPt(0x0),
  fhTriggerPt(0x0),
  fhJetTriggeredPt(0x0),
  fhTriggerVSjetPt(0x0),
  fhdPhiTrigVSjetPt(0x0),
  fhdPhiTrigVSjetPtNOCUT(0x0),
  fhdPhiPartVSjetPt(0x0),
  fhdPhiPartVSjetPtVSPartPt(0x0),
  fhPhi(0x0),
  fSubstructure(0),
  fHadronTrigger(0),
  fhTau1(0x0),
  fhTau2(0x0),
  fhTau3(0x0),
  fhTau1vsTau2(0x0),
  fhTau1vsTau3(0x0),
  fhTau2vsTau3(0x0),
  fhTau2OverTau1(0x0),
  fhTau3OverTau2(0x0),
  fhTau2vsJetPt(0x0),
  fhTau2OverTau1vsJetPt(0x0),
  fhTau1vsTau2vsTau3(0x0),
  fhTrackPt_JT(0x0)

{
  // Standard constructor.

  SetMakeGeneralHistograms(kTRUE);
  
  DefineOutput(1, TTree::Class());
  
}

//________________________________________________________________________
AliAnalysisTaskEmcalMissingEnergy::~AliAnalysisTaskEmcalMissingEnergy()
{
  // Destructor.
}

//________________________________________________________________________
void AliAnalysisTaskEmcalMissingEnergy::UserCreateOutputObjects()
{
  // Create user output.

  AliAnalysisTaskEmcalJet::UserCreateOutputObjects();

  // trees

  fSubstructure = new TTree("fSubstructure","fSubstructure");
  fHadronTrigger = new TTree("fHadronTrigger","fHadronTrigger");

  const Int_t nVarS = 5 * 2 + 1;  
  const Int_t nVarH = 5; 
  
  fSubstructureVar = new Float_t [nVarS];
  fHadronTriggerVar = new Float_t [nVarH];

  TString *fSubstructureVarNames = new TString [nVarS];
  TString *fHadronTriggerVarNames = new TString [nVarH];
  
  fSubstructureVarNames[0] = "JetPt";
  fSubstructureVarNames[1] = "Tau1";
  fSubstructureVarNames[2] = "Tau2";
  fSubstructureVarNames[3] = "Tau3";
  fSubstructureVarNames[4] = "DeltaR2hardest";
  fSubstructureVarNames[5] = "JetPtMatch";
  fSubstructureVarNames[6] = "Tau1Match";
  fSubstructureVarNames[7] = "Tau2Match";
  fSubstructureVarNames[8] = "Tau3Match";
  fSubstructureVarNames[9] = "DeltaR2hardestMatch";
  fSubstructureVarNames[10]= "weightPythia";

  fHadronTriggerVarNames[0] = "TriggerPt";
  fHadronTriggerVarNames[1] = "JetPt";
  fHadronTriggerVarNames[2] = "DeltaPhiJetTrigger";
  fHadronTriggerVarNames[3] = "PartPt";
  fHadronTriggerVarNames[4] = "DeltaPhiJetParticles";

  for(Int_t ivar = 0; ivar<nVarS; ivar++) {
    cout<<"looping over variables to create the branches of my Substructure Tree"<<endl;
    fSubstructure->Branch(fSubstructureVarNames[ivar].Data(), &fSubstructureVar[ivar], Form("%s/F", fSubstructureVarNames[ivar].Data()));
  }

  for(Int_t ivar = 0; ivar<nVarH; ivar++) {
    cout<<"looping over variables to create the branches of my HadronTrigger Tree"<<endl;
    fHadronTrigger->Branch(fHadronTriggerVarNames[ivar].Data(), &fHadronTriggerVar[ivar], Form("%s/F", fHadronTriggerVarNames[ivar].Data()));
  }

  // hadron trigger output

  fhJetPt= new TH1F("fhJetPt", "fhJetPt;Jet p_{T} (GeV)", 100, 0, 100);
  fOutput->Add(fhJetPt);

  fhTrackPt= new TH1F("fhTrackPt", "fhTrackPt;Track p_{T} (GeV)", 100, 0, 100);
  fOutput->Add(fhTrackPt);

  fhTrackPt_JT= new TH1F("fhTrackPt_JT", "fhTrackPt_JT;Track p_{T} (GeV)", 100, 0, 100);
  fOutput->Add(fhTrackPt_JT);

  fhTriggerPt= new TH1F("fhTriggerPt", "fhTriggerPt;Trigger p_{T} (GeV)", 100, 0, 100);
  fOutput->Add(fhTriggerPt);

  fhJetTriggeredPt= new TH1F("fhJetTriggeredPt", "fhJetTriggeredPt;Triggered Jet p_{T} (GeV)", 100, 0, 100);
  fOutput->Add(fhJetTriggeredPt);

  fhTriggerVSjetPt= new TH2F("fhTriggerVSjetPt", "fhTriggerVSjetPt;Trigger p_{T} (GeV);Jet p_{T} (GeV)", 100, 0, 100,  100, 0, 100);
  fOutput->Add(fhTriggerVSjetPt);

  fhdPhiTrigVSjetPtNOCUT= new TH2F("fhdPhiTrigVSjetPtNOCUT", "fhdPhiTrigVSjetPtNOCUT;#Delta#phi (Trig-Jet);Jet p_{T} (GeV)", 100, -0.5*TMath::Pi(), 1.5*TMath::Pi(),  100, 0, 100);
  fOutput->Add(fhdPhiTrigVSjetPtNOCUT);

  fhdPhiTrigVSjetPt= new TH2F("fhdPhiTrigVSjetPt", "fhdPhiTrigVSjetPt;#Delta#phi (Trig-Jet);Jet p_{T} (GeV)", 100, TMath::Pi() - 0.6, TMath::Pi(),  100, 0, 100);
  fOutput->Add(fhdPhiTrigVSjetPt);


  fhdPhiPartVSjetPt= new TH2F("fhdPhiPartVSjetPt", "fhdPhiPartVSjetPt;#Delta#phi (Part-Jet);Jet p_{T} (GeV)", 100, -0.5*TMath::Pi(), 1.5*TMath::Pi(),  100, 0, 100);
  fOutput->Add(fhdPhiPartVSjetPt);

  fhdPhiPartVSjetPtVSPartPt= new TH3F("fhdPhiPartVSjetPtVSPartPt", "fhdPhiPartVSjetPtVSPartPt;#Delta#phi (Part-Jet);Jet p_{T} (GeV);Part p_{T} (GeV)", 100, -0.5*TMath::Pi(), 1.5*TMath::Pi(),  100, 0, 100, 100, 0, 100);
  fOutput->Add(fhdPhiPartVSjetPtVSPartPt);

  // substructure output

  fhTau1 = new TH1F("fhTau1", "fhTau1;#tau_{1}", 100, 0, 1);
  fOutput->Add(fhTau1);

  fhTau2 = new TH1F("fhTau2", "fhTau2;#tau_{2}", 100, 0, 1);
  fOutput->Add(fhTau2);

  fhTau3 = new TH1F("fhTau3", "fhTau3;#tau_{3}", 100, 0, 1);
  fOutput->Add(fhTau3);

  fhTau1vsTau2 = new TH2F("fhTau1vsTau2", "fhTau1vsTau2;#tau_{1};#tau_{2}", 100, 0, 1,  100, 0, 1);
  fOutput->Add(fhTau1vsTau2);
  
  fhTau1vsTau3 = new TH2F("fhTau1vsTau3", "fhTau1vsTau3;#tau_{1};#tau_{3}", 100, 0, 1,  100, 0, 1);
  fOutput->Add(fhTau1vsTau3);

  fhTau2vsTau3 = new TH2F("fhTau2vsTau3", "fhTau2vsTau3;#tau_{2};#tau_{3}", 100, 0, 1,  100, 0, 1);
  fOutput->Add(fhTau2vsTau3);

  fhTau1vsTau2vsTau3 = new TH3F("fhTau1vsTau2vsTau3", "fhTau1vsTau2vsTau3;#tau_{1};#tau_{2};#tau_{3}", 100, 0, 1,  100, 0, 1, 100, 0, 1);
  fOutput->Add(fhTau1vsTau2vsTau3);

  fhTau2OverTau1 = new TH1F("fhTau2OverTau1", "fhTau2OverTau1;#tau_{2}/#tau_{1}", 100, 0, 1);
  fOutput->Add(fhTau2OverTau1);

  fhTau3OverTau2 = new TH1F("fhTau3OverTau2", "fhTau3OverTau2;#tau_{3}/#tau_{2}", 100, 0, 1);
  fOutput->Add(fhTau3OverTau2);
  

  fhTau2vsJetPt = new TH2F("fhTau2vsJetPt", "fhTau2vsJetPt;#tau_{2}; Jet p_{T} (GeV)", 100, 0, 1,  100, 0, 100);
  fOutput->Add(fhTau2vsJetPt);

  fhTau2OverTau1vsJetPt = new TH2F("fhTau2OverTau1vsJetPt", "fhTau2OverTau1vsJetPt;#tau_{2}/#tau_{1}; Jet p_{T} (GeV)", 100, 0, 1,  100, 0, 100);
  fOutput->Add(fhTau2OverTau1vsJetPt);

  fOutput->Add(fSubstructure);
  fOutput->Add(fHadronTrigger);
  PostData(1, fOutput);  //  Post data for ALL output slots > 0 here.

}

//________________________________________________________________________
Bool_t AliAnalysisTaskEmcalMissingEnergy::Run()
{
  // Run analysis code here, if needed. It will be executed before FillHistograms().

  return kTRUE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskEmcalMissingEnergy::FillHistograms()
{

  if (fCentSelectOn) {
    if ((fCent>fCentMax) || (fCent<fCentMin)) return 0;
  }

  const Double_t eta_max = 0.9;
  const Double_t track_pt_min = 0.15;
  const Double_t jet_pt_min = 10;
  const Double_t phi_h = 0.6;
  
  // const Double_t epsilon = 0.00001;

  AliParticleContainer *partCont = GetParticleContainer(0);
  TClonesArray *tracksArray = partCont->GetArray();
  
  Float_t kWeight=1;

  if(!partCont || !tracksArray) return 0;
  AliPicoTrack *trigger = 0x0;
  AliPicoTrack *track = 0x0;

  AliEmcalJet* jetTEMP = NULL;
  AliJetContainer *JetCont = GetJetContainer(0);

  for(Int_t iTrack=0; iTrack <= tracksArray->GetEntriesFast(); iTrack++){ 
    track = static_cast<AliPicoTrack*>(tracksArray->At(iTrack));
    if (!track || track->Pt()<1) continue;
    fhTrackPt->Fill(track->Pt());
  }

  if(JetCont) {
    JetCont->ResetCurrentID();
    while((jetTEMP = JetCont->GetNextAcceptJet())) {
      if (!jetTEMP) continue;
      if (jetTEMP->Pt() < jet_pt_min) continue;
      if (TMath::Abs(jetTEMP->Eta()) > eta_max - fJetRadius) continue;
      fhJetPt->Fill(jetTEMP->Pt());
    }
  }

  // start Hadron Trigger

  Int_t trigger_index = SelectTrigger(fminpTTrig,fmaxpTTrig);

  if(trigger_index >= 0 && fHTon) {
    trigger = static_cast<AliPicoTrack*>(tracksArray->At(trigger_index));
    if (trigger) {
      fhTriggerPt->Fill(trigger->Pt());
      
      if(JetCont) {
	JetCont->ResetCurrentID();
	while((jetTEMP = JetCont->GetNextAcceptJet())) {
	  if (!jetTEMP) continue;
	  if (jetTEMP->Pt() < jet_pt_min) continue;
	  if (TMath::Abs(jetTEMP->Eta()) > eta_max - fJetRadius) continue;

	  Float_t dphi = RelativePhi(trigger->Phi(), jetTEMP->Phi()); // -pi to pi
  	  Float_t dphiFancy = RelativePhiFancy(trigger->Phi(), jetTEMP->Phi()); // -0.5pi to 1.5pi
  	  
  	  fhdPhiTrigVSjetPtNOCUT->Fill(dphiFancy,jetTEMP->Pt());
  	  
	  if (TMath::Abs(dphi) < TMath::Pi() - phi_h) continue; // trigger selection
    
	  fhJetTriggeredPt->Fill(jetTEMP->Pt());
	  fhTriggerVSjetPt->Fill(trigger->Pt(),jetTEMP->Pt());
	  fhdPhiTrigVSjetPt->Fill(TMath::Abs(dphi),jetTEMP->Pt());
    
	  //now I have a trigger and a recoil jet. Let's loop over all particles!
	  for(Int_t iTrack=0; iTrack <= tracksArray->GetEntriesFast(); iTrack++){ 
	    track = static_cast<AliPicoTrack*>(tracksArray->At(iTrack));
	    if (!track) continue;

	    if(TMath::Abs(track->Eta())>eta_max) continue;
	    if(track->Pt()<track_pt_min) continue;
	    //if(track->GetTrackType() == 2) continue;

	    Float_t DPHI = RelativePhiFancy(jetTEMP->Phi(), track->Phi()); // -0.5 pi to 1.5 pi
	    fhdPhiPartVSjetPt->Fill(DPHI,jetTEMP->Pt());
	    fhdPhiPartVSjetPtVSPartPt->Fill(DPHI,jetTEMP->Pt(),track->Pt());
	    fhTrackPt_JT->Fill(track->Pt());

	    fHadronTriggerVar[0] = trigger->Pt();
	    fHadronTriggerVar[1] = jetTEMP->Pt();
	    fHadronTriggerVar[2] = TMath::Abs(dphi);
	    fHadronTriggerVar[3] = track->Pt();
	    fHadronTriggerVar[4] = DPHI;
	    
	    fHadronTrigger->Fill();
	  }
	}
      }
    }
  }
  ////////////////////////////////////////// end hadron trigger


  ////////////////////////////////////////// start jet substructure

  AliEmcalJet *jet1 = NULL; 
  AliEmcalJet *subJet = NULL, *subJetPart = NULL;
  AliEmcalJet *subJet1hardest = NULL, *subJet1hardestPart = NULL;
  AliEmcalJet *subJet2hardest = NULL, *subJet2hardestPart = NULL;
  AliEmcalJet *subJet3hardest = NULL, *subJet3hardestPart = NULL;

  AliPicoTrack *mainJetParticle = 0x0, *mainJetParticlePart = 0x0;
  AliPicoTrack *subJetPartiicle = 0x0, *subJetPartiiclePart = 0x0;

  AliEmcalJetFinder *Reclusterer = new AliEmcalJetFinder("SubJetFinder"); //JetFinder Object for reclustered jets from MC particle Level data
  AliEmcalJetFinder *ReclustererPart = new AliEmcalJetFinder("SubJetFinderPart"); //JetFinder Object for 
  Int_t *ordered;
  Int_t *orderedPart;

  const AliVVertex *vert = InputEvent()->GetPrimaryVertex();
  Double_t dVtx[3]={vert->GetX(),vert->GetY(),vert->GetZ()};

  Reclusterer->SetRadius(fSubJetRadius);
  Reclusterer->SetJetMinPt(track_pt_min);
  Reclusterer->SetJetAlgorithm(0); //  0  for anti-kt     1 for kt

  Int_t SubJetCounter = 0, SubJetCounterPart = 0;
  
  Double_t tau1_num = 0., tau1_numPart = 0.;
  Double_t tau2_num = 0., tau2_numPart = 0.;
  Double_t tau3_num = 0., tau3_numPart = 0.;
  Double_t tau_den = 0., tau_denPart = 0.;
  Double_t dR1 = 0., dR1Part = 0.;
  Double_t dR2 = 0., dR2Part = 0.;
  Double_t dR3 = 0., dR3Part = 0.;
  Double_t dRmin = 0., dRminPart = 0.;
  Double_t dR2hardest = 0., dR2hardestPart = 0.;
  
  if(JetCont){
    JetCont->ResetCurrentID();
    
    while((jet1 = JetCont->GetNextAcceptJet())) {      // loop over main Jets found in the event
      if(!jet1) continue;
      if (jet1->Pt() < jet_pt_min) continue;
      if (TMath::Abs(jet1->Eta()) > eta_max - fJetRadius) continue;
      
      AliEmcalJet* jet2 = 0x0;
      AliEmcalJet* jet3=0x0;
      AliEmcalJet *jetUS = 0x0;
      Int_t ifound=0;
      Int_t ilab=-1;
      if (fJetShapeType != kData) {
        AliPythiaInfo *partonsInfo = 0x0;
        AliJetContainer *jetContTrue = GetJetContainer(1);
        AliJetContainer *jetContUS = GetJetContainer(2);


      	if (fJetShapeSub == kConstSub) {
      	  for(Int_t i = 0; i<jetContUS->GetNJets(); i++) {
      	    jetUS = jetContUS->GetJet(i);
	    if(jetUS->GetLabel()==jet1->GetLabel()) {
      	      ifound++;
	      if(ifound==1) ilab = i;
	    }
	  }
	  if(ilab == -1) continue;
	  jetUS=jetContUS->GetJet(ilab);
          
	  jet2=jetUS->ClosestJet();
	}
	
	if(!(fJetShapeSub==kConstSub)) jet2 = jet1->ClosestJet();
	if (!jet2) {
	  Printf("jet2 does not exist, returning");
	  continue;
	}
        
	if(fJetShapeType==kDetEmbPart){
	  AliJetContainer *jetContPart=GetJetContainer(3);
	  jet3=jet2->ClosestJet();
          
	  if(!jet3){
	    Printf("jet3 does not exist, returning");
	    continue;}
	  cout<<"jet 3 exists"<<jet3->Pt()<<endl;

	  if(ReclustererPart->AliEmcalJetFinder::Filter(jet3, jetContPart, dVtx)){  //reclustering jet3 using the jetfinderobject ReclustererPart                

      ReclustererPart->SetRadius(fSubJetRadius);
      ReclustererPart->SetJetMinPt(track_pt_min);
      ReclustererPart->SetJetAlgorithm(0); //  0  for anti-kt     1 for kt
	    SubJetCounterPart = ReclustererPart->GetNumberOfJets();

      //orderedPart = new Int_t[SubJetCounterPart];
      orderedPart = JetHard(ReclustererPart);

	  }
	
	}

  if (!(fJetShapeType == kPythiaDef)) {
    partonsInfo = (AliPythiaInfo*) jetContTrue->GetPythiaInfo();
    if(!partonsInfo) return 0;
  } else {
    partonsInfo = (AliPythiaInfo*) JetCont->GetPythiaInfo();
    jet2=jet1;
    if(!partonsInfo) return 0;
  }
  if (!(fJetShapeType == kPythiaDef)){
    kWeight = partonsInfo->GetPythiaEventWeight();
  }

}


	if(Reclusterer->AliEmcalJetFinder::Filter(jet1, JetCont, dVtx)){  //reclustering mainJets using the jetfinderobject Reclusterer                

	  SubJetCounter = Reclusterer->GetNumberOfJets(); // Number of reclustered SubJets in each original jet                                            

    //ordered = new Int_t[SubJetCounter];
    ordered = JetHard(Reclusterer);

    for (Int_t i=0; i<SubJetCounter; i++){ //Loops through each SubJet in a reclustered jet 
      subJet = Reclusterer->GetJet(ordered[i]);
      cout<<"SubJet: "<<i+1<<"/"<<SubJetCounter<<" | subJetPt/JetPt: "<<subJet->Pt()<<"/"<<jet1->Pt()<<endl;
    }

	  tau_den = TauDen(jet1, 0);
	  if(fJetShapeType==kDetEmbPart){
	    tau_denPart = TauDen(jet3, 3);
	    if (tau_denPart <= 0. || 0 == SubJetCounterPart) continue;
	    fSubstructureVar[5] = jet3->Pt();
	  }

	  if (tau_den <= 0. || 0 == SubJetCounter) continue;

	  fSubstructureVar[0] = jet1->Pt();

	  if (1 == SubJetCounter) { 
	    subJet1hardest = Reclusterer->GetJet(ordered[SubJetCounter - 1]);
	    tau1_num = Tau1Num(jet1,subJet1hardest,0);

	    if (tau1_num == 0. ) continue;
	    fhTau1->Fill(tau1_num/tau_den);
	    fSubstructureVar[1] = tau1_num/tau_den;
	    fSubstructureVar[2] = -1;
	    fSubstructureVar[3] = -1;
	    fSubstructureVar[4] = -1;
	    if (fJetShapeType == kData) {
	      fSubstructureVar[5] = -2;
	      fSubstructureVar[6] = -2;
	      fSubstructureVar[7] = -2;
	      fSubstructureVar[8] = -2;
	      fSubstructureVar[9] = -2;
        fSubstructureVar[10]= 1;
	    }
	  }

	  if(fJetShapeType==kDetEmbPart){
	    if (1 == SubJetCounterPart) { 
	      subJet1hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 1]);
	      tau1_numPart = Tau1Num(jet3,subJet1hardestPart,3);

	      if (tau1_numPart == 0. ) continue;
	      fSubstructureVar[6] = tau1_numPart/tau_denPart;
	      fSubstructureVar[7] = -1;
	      fSubstructureVar[8] = -1;
	      fSubstructureVar[9] = -1;
        fSubstructureVar[10]= kWeight;
	    }
	  }
	

	  if (2 == SubJetCounter) { 
	    subJet1hardest = Reclusterer->GetJet(ordered[SubJetCounter - 1]);
	    subJet2hardest = Reclusterer->GetJet(ordered[SubJetCounter - 2]);

	    tau1_num = Tau1Num(jet1,subJet1hardest,0);
	    tau2_num = Tau2Num(jet1,subJet1hardest,subJet2hardest,0);
	    dR2hardest = R_distance(subJet1hardest,subJet2hardest);

	    if (tau1_num == 0. || tau2_num == 0.) continue;
	    fhTau1->Fill(tau1_num/tau_den);
	    fhTau2->Fill(tau2_num/tau_den);
	    fhTau1vsTau2->Fill(tau1_num/tau_den,tau2_num/tau_den);
	    fhTau2OverTau1->Fill(tau2_num/tau1_num);
	    fhTau2vsJetPt->Fill(tau2_num/tau_den,jet1->Pt());
	    fhTau2OverTau1vsJetPt->Fill(tau2_num/tau1_num,jet1->Pt());

	    fSubstructureVar[1] = tau1_num/tau_den;
	    fSubstructureVar[2] = tau2_num/tau_den;
	    fSubstructureVar[3] = -1;
	    fSubstructureVar[4] = dR2hardest;
	    if (fJetShapeType == kData) {
	      fSubstructureVar[5] = -2;
	      fSubstructureVar[6] = -2;
	      fSubstructureVar[7] = -2;
	      fSubstructureVar[8] = -2;
	      fSubstructureVar[9] = -2;
        fSubstructureVar[10]= 1;
	    }
	  }

	  if(fJetShapeType==kDetEmbPart){
	    if (2 == SubJetCounterPart) { 
	      subJet1hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 1]);
	      subJet2hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 2]);

	      tau1_numPart = Tau1Num(jet3,subJet1hardestPart,3);
	      tau2_numPart = Tau2Num(jet3,subJet1hardestPart,subJet2hardestPart,3);
	      dR2hardestPart = R_distance(subJet1hardestPart,subJet2hardestPart);

	      if (tau1_numPart == 0. || tau2_numPart == 0.) continue;
	      fSubstructureVar[6] = tau1_numPart/tau_denPart;
	      fSubstructureVar[7] = tau2_numPart/tau_denPart;
	      fSubstructureVar[8] = -1;
	      fSubstructureVar[9] = dR2hardestPart;
        fSubstructureVar[10]= kWeight;
	    }

	  }

	  if (SubJetCounter > 2) { 
	    subJet1hardest = Reclusterer->GetJet(ordered[SubJetCounter - 1]);
	    subJet2hardest = Reclusterer->GetJet(ordered[SubJetCounter - 2]);
	    subJet3hardest = Reclusterer->GetJet(ordered[SubJetCounter - 3]);

	    tau1_num = Tau1Num(jet1,subJet1hardest,0);
	    tau2_num = Tau2Num(jet1,subJet1hardest,subJet2hardest,0);
	    tau3_num = Tau3Num(jet1,subJet1hardest,subJet2hardest,subJet3hardest,0);
	    dR2hardest = R_distance(subJet1hardest,subJet2hardest);

	    if (tau1_num == 0. || tau2_num == 0. || tau3_num == 0.) continue;
	    fhTau1->Fill(tau1_num/tau_den);
	    fhTau2->Fill(tau2_num/tau_den);
	    fhTau3->Fill(tau3_num/tau_den);
	    fhTau1vsTau2->Fill(tau1_num/tau_den,tau2_num/tau_den);
	    fhTau1vsTau3->Fill(tau1_num/tau_den,tau3_num/tau_den);
	    fhTau2vsTau3->Fill(tau2_num/tau_den,tau3_num/tau_den);
	    fhTau1vsTau2vsTau3->Fill(tau1_num/tau_den,tau2_num/tau_den,tau3_num/tau_den); 
	    fhTau2OverTau1->Fill(tau2_num/tau1_num);
	    fhTau3OverTau2->Fill(tau3_num/tau2_num);
	    fhTau2vsJetPt->Fill(tau2_num/tau_den,jet1->Pt());
	    fhTau2OverTau1vsJetPt->Fill(tau2_num/tau1_num,jet1->Pt());

	    fSubstructureVar[1] = tau1_num/tau_den;
	    fSubstructureVar[2] = tau2_num/tau_den;
	    fSubstructureVar[3] = tau3_num/tau_den;
	    fSubstructureVar[4] = dR2hardest;
	    if (fJetShapeType == kData) {
	      fSubstructureVar[5] = -2;
	      fSubstructureVar[6] = -2;
	      fSubstructureVar[7] = -2;
	      fSubstructureVar[8] = -2;
	      fSubstructureVar[9] = -2;
        fSubstructureVar[10]= 1;
	    }

	  }

	  if(fJetShapeType==kDetEmbPart){

	    if (SubJetCounterPart > 2) { 
	      subJet1hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 1]);
	      subJet2hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 2]);
	      subJet3hardestPart = ReclustererPart->GetJet(ordered[SubJetCounterPart - 3]);

	      tau1_numPart = Tau1Num(jet3,subJet1hardestPart,3);
	      tau2_numPart = Tau2Num(jet3,subJet1hardestPart,subJet2hardestPart,3);
	      tau3_numPart = Tau3Num(jet3,subJet1hardestPart,subJet2hardestPart,subJet3hardestPart,3);
	      dR2hardestPart = R_distance(subJet1hardestPart,subJet2hardestPart);

	      if (tau1_numPart == 0. || tau2_numPart == 0. || tau3_numPart == 0.) continue;
	      fSubstructureVar[6] = tau1_numPart/tau_denPart;
	      fSubstructureVar[7] = tau2_numPart/tau_denPart;
	      fSubstructureVar[8] = tau3_numPart/tau_denPart;
	      fSubstructureVar[9] = dR2hardestPart;
        fSubstructureVar[10]= kWeight;
	    }
	  }

	  fSubstructure->Fill();     
      }
    }
  }


  ////////////////////////////////////////// end jet substructure

  return kTRUE;
}

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetMass(AliEmcalJet *jet,Int_t jetContNb=0) {
  //calc subtracted jet mass
  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtracted();
  else 
    return jet->M();
}

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Angularity(AliEmcalJet *jet, Int_t jetContNb = 0){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  if (!jet->GetNumberOfTracks())
    return 0; 
  Double_t den=0.;
  Double_t num = 0.;
  AliVParticle *vp1 = 0x0;
  for(UInt_t i = 0; i < jet->GetNumberOfTracks(); i++) {
    vp1 = static_cast<AliVParticle*>(jet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));

    if (!vp1){
      Printf("AliVParticle associated to constituent not found");
      continue;
    }

    Double_t dphi = RelativePhi(vp1->Phi(),jet->Phi());
    Double_t dr2 = (vp1->Eta()-jet->Eta())*(vp1->Eta()-jet->Eta()) + dphi*dphi;
    Double_t dr = TMath::Sqrt(dr2);
    num=num+vp1->Pt()*dr;
    den=den+vp1->Pt();
  }
  return num/den;
} 

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetAngularity(AliEmcalJet *jet, Int_t jetContNb = 0) {

  if((fJetShapeSub==kDerivSub) && (jetContNb==0))
    return jet->GetSecondOrderSubtractedAngularity();
  else
    return Angularity(jet, jetContNb);

}


//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::PTD(AliEmcalJet *jet, Int_t jetContNb = 0){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  if (!jet->GetNumberOfTracks())
    return 0; 
  Double_t den = 0.;
  Double_t num = 0.;
  AliVParticle *vp1 = 0x0;
  for(UInt_t i = 0; i < jet->GetNumberOfTracks(); i++) {
    vp1 = static_cast<AliVParticle*>(jet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));

    if (!vp1){
      Printf("AliVParticle associated to constituent not found");
      continue;
    }

    num=num+vp1->Pt()*vp1->Pt();
    den=den+vp1->Pt();
  }
  return TMath::Sqrt(num)/den;
} 

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetpTD(AliEmcalJet *jet, Int_t jetContNb = 0) {
  //calc subtracted jet mass
  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtractedpTD();
  else
    return PTD(jet, jetContNb);

}

//_____________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Circularity(AliEmcalJet *jet, Int_t jetContNb = 0){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  if (!jet->GetNumberOfTracks())
    return 0; 
  Double_t mxx = 0.;
  Double_t myy = 0.;
  Double_t mxy = 0.;
  int nc = 0;
  Double_t sump2 = 0.;
  Double_t pxjet=jet->Px();
  Double_t pyjet=jet->Py();
  Double_t pzjet=jet->Pz();
  
  
  //2 general normalized vectors perpendicular to the jet
  TVector3  ppJ1(pxjet, pyjet, pzjet);
  TVector3  ppJ3(- pxjet* pzjet, - pyjet * pzjet, pxjet * pxjet + pyjet * pyjet);
  ppJ3.SetMag(1.);
  TVector3  ppJ2(-pyjet, pxjet, 0);
  ppJ2.SetMag(1.);
  AliVParticle *vp1 = 0x0;
  for(UInt_t i = 0; i < jet->GetNumberOfTracks(); i++) {
    vp1 = static_cast<AliVParticle*>(jet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));  
    
    if (!vp1){
      Printf("AliVParticle associated to constituent not found");
      continue;
    }
    
    TVector3 pp(vp1->Px(), vp1->Py(), vp1->Pz());

    //local frame
    TVector3 pLong = pp.Dot(ppJ1) / ppJ1.Mag2() * ppJ1;
    TVector3 pPerp = pp - pLong;
    //projection onto the two perpendicular vectors defined above
    
    Float_t ppjX = pPerp.Dot(ppJ2);
    Float_t ppjY = pPerp.Dot(ppJ3);
    Float_t ppjT = TMath::Sqrt(ppjX * ppjX + ppjY * ppjY);
    if(ppjT<=0) return 0;
    
    mxx += (ppjX * ppjX / ppjT);
    myy += (ppjY * ppjY / ppjT);
    mxy += (ppjX * ppjY / ppjT);
    nc++;
    sump2 += ppjT;}

  if(nc<2) return 0;
  if(sump2==0) return 0;
  // Sphericity Matrix
  Double_t ele[4] = {mxx / sump2, mxy / sump2, mxy / sump2, myy / sump2};
  TMatrixDSym m0(2,ele);

  // Find eigenvectors
  TMatrixDSymEigen m(m0);
  TVectorD eval(2);
  TMatrixD evecm = m.GetEigenVectors();
  eval  = m.GetEigenValues();
  // Largest eigenvector
  int jev = 0;
  //  cout<<eval[0]<<" "<<eval[1]<<endl;
  if (eval[0] < eval[1]) jev = 1;
  TVectorD evec0(2);
  // Principle axis
  evec0 = TMatrixDColumn(evecm, jev);
  Double_t compx=evec0[0];
  Double_t compy=evec0[1];
  TVector2 evec(compx, compy);
  Double_t circ=0;
  if(jev==1) circ=2*eval[0];
  if(jev==0) circ=2*eval[1];

  return circ;
}


//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetCircularity(AliEmcalJet *jet, Int_t jetContNb =0 ) {
  //calc subtracted jet mass

  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtractedCircularity();
  else
    return Circularity(jet, jetContNb);
}

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::LeSub(AliEmcalJet *jet, Int_t jetContNb =0 ){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  if (!jet->GetNumberOfTracks())
    return 0;
  Double_t den=0.;
  Double_t num = 0.;
  AliVParticle *vp1 = 0x0;
  AliVParticle *vp2 = 0x0;
  std::vector<int> ordindex;
  ordindex=jet->SortConstituentsPt(jetCont->GetParticleContainer()->GetArray());
  //Printf("Nbof const = %d", jet->GetNumberOfTracks());
  //Printf("ordindex[0] = %d, ordindex[1] = %d", ordindex[0], ordindex[1]);

  if(ordindex.size()<2) return -1;

  vp1 = static_cast<AliVParticle*>(jet->TrackAt(ordindex[0], jetCont->GetParticleContainer()->GetArray()));
  if (!vp1){
    Printf("AliVParticle associated to Leading constituent not found");
    return -1;
  }

  vp2 = static_cast<AliVParticle*>(jet->TrackAt(ordindex[1], jetCont->GetParticleContainer()->GetArray()));
  if (!vp2){
    Printf("AliVParticle associated to Subleading constituent not found");
    return -1;
  }


  num=vp1->Pt();
  den=vp2->Pt();
  //Printf("vp1->Pt() =%f, vp2->Pt() =%f", vp1->Pt(), vp2->Pt());

  return num-den;
} 

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetLeSub(AliEmcalJet *jet, Int_t jetContNb =0) {
  //calc subtracted jet mass

  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtractedLeSub();
  else
    return LeSub(jet, jetContNb);

}

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetJetNumberOfConstituents(AliEmcalJet *jet,Int_t jetContNb=0) {
  //calc subtracted jet mass

  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtractedConstituent();
  else
    return jet->GetNumberOfTracks();

}


//______________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Sigma2(AliEmcalJet *jet, Int_t jetContNb=0){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  if (!jet->GetNumberOfTracks())
    return 0; 
  Double_t mxx    = 0.;
  Double_t myy    = 0.;
  Double_t mxy    = 0.;
  int  nc     = 0;
  Double_t sump2  = 0.;

  AliVParticle *vp1 = 0x0;
  for(UInt_t i = 0; i < jet->GetNumberOfTracks(); i++) {
    vp1 = static_cast<AliVParticle*>(jet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));

    if (!vp1){
      Printf("AliVParticle associated to constituent not found");
      continue;
    }

    Double_t ppt=vp1->Pt();
    Double_t dphi = RelativePhi(vp1->Phi(),jet->Phi());
     
    Double_t deta = vp1->Eta()-jet->Eta();
    mxx += ppt*ppt*deta*deta;
    myy += ppt*ppt*dphi*dphi;
    mxy -= ppt*ppt*deta*TMath::Abs(dphi);
    nc++;
    sump2 += ppt*ppt;

  }  
  if(nc<2) return 0;
  if(sump2==0) return 0;
  // Sphericity Matrix
  Double_t ele[4] = {mxx , mxy , mxy , myy };
  TMatrixDSym m0(2,ele);

  // Find eigenvectors
  TMatrixDSymEigen m(m0);
  TVectorD eval(2);
  TMatrixD evecm = m.GetEigenVectors();
  eval  = m.GetEigenValues();
  // Largest eigenvector
  int jev = 0;
  //  cout<<eval[0]<<" "<<eval[1]<<endl;
  if (eval[0] < eval[1]) jev = 1;
  TVectorD evec0(2);
  // Principle axis
  evec0 = TMatrixDColumn(evecm, jev);
  Double_t compx=evec0[0];
  Double_t compy=evec0[1];
  TVector2 evec(compx, compy);
  Double_t sig=0;
  if(jev==1) sig=TMath::Sqrt(TMath::Abs(eval[0])/sump2);
  if(jev==0) sig=TMath::Sqrt(TMath::Abs(eval[1])/sump2);

  return sig;

}

//________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::GetSigma2(AliEmcalJet *jet, Int_t jetContNb=0) {
  //calc subtracted jet mass

  if((fJetShapeSub==kDerivSub)&&(jetContNb==0))
    return jet->GetSecondOrderSubtractedSigma2();
  else
    return Sigma2(jet, jetContNb);

}

//________________________________________________________________________
Int_t AliAnalysisTaskEmcalMissingEnergy::SelectTrigger(Float_t minpT, Float_t maxpT){

  AliParticleContainer *partCont = GetParticleContainer(0);
  TClonesArray *tracksArray = partCont->GetArray();
  
  if(!partCont || !tracksArray) return -99999;
  AliAODTrack *track = 0x0;
  AliEmcalParticle *emcPart = 0x0;
  AliPicoTrack *picoTrack = 0x0;
  
  TList *trackList = new TList();
  Int_t triggers[100];
  for (Int_t iTrigger=0; iTrigger<100; iTrigger++) triggers[iTrigger] = 0;
  Int_t iTT = 0;
  
  for(Int_t iTrack=0; iTrack <= tracksArray->GetEntriesFast(); iTrack++){

    if ((fJetShapeSub == kNoSub) || (fJetShapeSub == kDerivSub)) {
      picoTrack = static_cast<AliPicoTrack*>(tracksArray->At(iTrack));
      if (!picoTrack) continue;
      
      
      if(TMath::Abs(picoTrack->Eta())>0.9) continue;
      if(picoTrack->Pt()<0.15) continue;
      if(picoTrack->GetTrackType() == 2) continue;

      //if ((picoTrack->Pt()>8) && (picoTrack->Pt()<9)) Printf("picoTrackLabel = %d", picoTrack->GetTrackType());
      
      if ((picoTrack->Pt() >= minpT) && (picoTrack->Pt()< maxpT)) {
        trackList->Add(picoTrack);
        triggers[iTT] = iTrack;
        iTT++;
      }
    }
    else if (fJetShapeSub == kConstSub){
      emcPart = static_cast<AliEmcalParticle*>(tracksArray->At(iTrack));
      if (!emcPart) continue;
      if(TMath::Abs(emcPart->Eta())>0.9) continue;
      if (emcPart->Pt()<0.15) continue;
      
      if ((emcPart->Pt() >= minpT) && (emcPart->Pt()< maxpT)) {
        trackList->Add(emcPart);
        triggers[iTT] = iTrack;
        iTT++;
      }
    }
    else{
      track = static_cast<AliAODTrack*>(tracksArray->At(iTrack));
      if (!track) continue;
      if(TMath::Abs(track->Eta())>0.9) continue;
      if (track->Pt()<0.15) continue;
      if (!(track->TestFilterBit(768))) continue;
      
      if ((track->Pt() >= minpT) && (track->Pt()< maxpT)) {
        trackList->Add(track);
        triggers[iTT] = iTrack;
        iTT++;
        
      }
    }
  }

  if (iTT == 0) return -99999;
  Int_t nbRn = 0, index = 0 ; 
  TRandom3* random = new TRandom3(0); 
  nbRn = random->Integer(iTT);

  index = triggers[nbRn];
  //Printf("iTT Total= %d, nbRn = %d, Index = %d",iTT, nbRn, index );
  return index; 
  
}

//__________________________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::RelativePhi(Double_t mphi,Double_t vphi){

  if (vphi < -1*TMath::Pi()) vphi += (2*TMath::Pi());
  else if (vphi > TMath::Pi()) vphi -= (2*TMath::Pi());
  if (mphi < -1*TMath::Pi()) mphi += (2*TMath::Pi());
  else if (mphi > TMath::Pi()) mphi -= (2*TMath::Pi());
  double dphi = mphi-vphi;
  if (dphi < -1*TMath::Pi()) dphi += (2*TMath::Pi());
  else if (dphi > TMath::Pi()) dphi -= (2*TMath::Pi());
  return dphi;//dphi in [-Pi, Pi]
}


//________________________________________________________________________
Bool_t AliAnalysisTaskEmcalMissingEnergy::RetrieveEventObjects() {
  //
  // retrieve event objects
  //
  if (!AliAnalysisTaskEmcalJet::RetrieveEventObjects())
    return kFALSE;

  return kTRUE;
}

//_______________________________________________________________________
void AliAnalysisTaskEmcalMissingEnergy::Terminate(Option_t *) 
{
  // Called once at the end of the analysis.

  // fTreeObservableTagging = dynamic_cast<TTree*>(GetOutputData(1));
  // if (!fTreeObservableTagging){
  //   Printf("ERROR: fTreeObservable not available"); 
  //   return;
  // }

}

//_______________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::Minimum(Double_t x, Double_t y) 
{
  if (x <= y) return x;
  else return y;
}

//_______________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::Minimum(Double_t x, Double_t y, Double_t z) 
{
  Double_t minimum = x;
  if (y < minimum) minimum = y;
  if (z < minimum) minimum  = z;
  return minimum;
}

//_______________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::R_distance(AliEmcalJet *jet1, AliEmcalJet *jet2) 
{
  Double_t dPhi, dEta;
  dPhi = RelativePhi(jet1->Phi(),jet2->Phi());
  dEta = jet1->Eta() - jet2->Eta();
  return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//_______________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::R_distance(Double_t phi1, Double_t eta1, Double_t phi2, Double_t eta2) 
{
  Double_t dPhi, dEta;
  dPhi = RelativePhi(phi1,phi2);
  dEta = eta1 - eta2;
  return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//_______________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::R_distance(AliEmcalJet *jet,  AliPicoTrack *part) 
{
  Double_t dPhi, dEta;
  dPhi = RelativePhi(jet->Phi(),part->Phi());
  dEta = jet->Eta() - part->Eta();
  return TMath::Sqrt(dEta*dEta + dPhi*dPhi);
}

//__________________________________________________________________________________
Double_t AliAnalysisTaskEmcalMissingEnergy::RelativePhiFancy(Double_t mphi,Double_t vphi){

  Double_t dPhi = mphi - vphi;
  
  if(dPhi>2*TMath::Pi()) dPhi -= 2*TMath::Pi();
  if(dPhi<-2*TMath::Pi()) dPhi += 2*TMath::Pi();
  if(dPhi<-0.5*TMath::Pi()) dPhi += 2*TMath::Pi();
  if(dPhi>1.5*TMath::Pi()) dPhi -= 2*TMath::Pi();
  
  return dPhi; // [-0.5 pi, 1.5 pi]
}


//__________________________________________________________________________________
Int_t * AliAnalysisTaskEmcalMissingEnergy::JetHard(AliEmcalJetFinder *finder){

  Int_t *indeces; 
  Int_t NumberOfJets = finder->GetNumberOfJets();
  AliEmcalJet *temp;
  Float_t mom;
  Int_t ind;

  Float_t *PtArray;
  PtArray = new Float_t[NumberOfJets];
  indeces = new Int_t[NumberOfJets];

  for(Int_t i=0;i<NumberOfJets;i++){
  temp = finder->GetJet(i);
  PtArray[i] = temp->Pt();
  indeces[i] = i; 
  }

  for(Int_t i=0; i<NumberOfJets - 1; i++){
    for(Int_t j=0; j<(NumberOfJets - i - 1); j++){
      if(PtArray[j]>PtArray[j+1]){
        mom=PtArray[j];
        PtArray[j]=PtArray[j+1];
        PtArray[j+1]=mom;

        ind = indeces[j];
        indeces[j]=indeces[j+1];
        indeces[j+1]=ind;
      }
    }
  }

  return indeces;
}

//__________________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::TauDen(AliEmcalJet *mainJet, Int_t jetContNb = 0){

  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  AliPicoTrack *mainJetParticle = 0x0;
  if (!mainJet->GetNumberOfTracks()) return 0; 
  Float_t tau_den = 0.;
  for (Int_t i = 0; i < mainJet->GetNumberOfTracks(); i++) { //compute tau denominator once and for all
    mainJetParticle =  static_cast<AliPicoTrack*>(mainJet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));
    tau_den += mainJetParticle->Pt() * fJetRadius;
  }
  return tau_den;
}

//__________________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Tau1Num(AliEmcalJet *mainJet, AliEmcalJet *subJet1hardest, Int_t jetContNb = 0){

  Float_t dR1;
  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  AliPicoTrack *mainJetParticle = 0x0;
  if (!mainJet->GetNumberOfTracks()) return 0; 
  Float_t tau1_num = 0.;
  for (Int_t i = 0; i < mainJet->GetNumberOfTracks(); i++) { // tau1 computation
    mainJetParticle =  static_cast<AliPicoTrack*>(mainJet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));
    dR1 = R_distance(subJet1hardest,mainJetParticle);
    tau1_num += mainJetParticle->Pt() * dR1;
  }
  return tau1_num;
}

//__________________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Tau2Num(AliEmcalJet *mainJet, AliEmcalJet *subJet1hardest, AliEmcalJet *subJet2hardest, Int_t jetContNb = 0){

  Float_t dR1, dR2, dRmin;
  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  AliPicoTrack *mainJetParticle = 0x0;
  if (!mainJet->GetNumberOfTracks()) return 0; 
  Float_t tau2_num = 0.;
  for (Int_t i = 0; i < mainJet->GetNumberOfTracks(); i++) { //tau2 computation
    mainJetParticle =  static_cast<AliPicoTrack*>(mainJet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));
    dR1 = R_distance(subJet1hardest,mainJetParticle);
    dR2 = R_distance(subJet2hardest,mainJetParticle);
    dRmin = Minimum(dR1, dR2);
    tau2_num += mainJetParticle->Pt() * dRmin;
  }
  return tau2_num;
}

//__________________________________________________________________________________
Float_t AliAnalysisTaskEmcalMissingEnergy::Tau3Num(AliEmcalJet *mainJet, AliEmcalJet *subJet1hardest, AliEmcalJet *subJet2hardest, AliEmcalJet *subJet3hardest, Int_t jetContNb = 0){

  Float_t dR1, dR2, dR3, dRmin;
  AliJetContainer *jetCont = GetJetContainer(jetContNb);
  AliPicoTrack *mainJetParticle = 0x0;
  if (!mainJet->GetNumberOfTracks()) return 0; 
  Float_t tau3_num = 0.;
  for (Int_t i = 0; i < mainJet->GetNumberOfTracks(); i++) { //tau3 computation
    mainJetParticle =  static_cast<AliPicoTrack*>(mainJet->TrackAt(i, jetCont->GetParticleContainer()->GetArray()));
    dR1 = R_distance(subJet1hardest,mainJetParticle);
    dR2 = R_distance(subJet2hardest,mainJetParticle);
    dR3 = R_distance(subJet3hardest,mainJetParticle);
    dRmin = Minimum(dR1,dR2,dR3);
    tau3_num += mainJetParticle->Pt() * dRmin;
  }
  return tau3_num;
}

