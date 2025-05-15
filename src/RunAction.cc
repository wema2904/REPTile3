//
//
//

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "G4CsvAnalysisManager.hh"

using G4AnalysisManager = G4CsvAnalysisManager;

RunAction::RunAction()
: G4UserRunAction(), fHistoManager(0)
{
    //HistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    //delete fHistoManager;
}



void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("REPTile3Results.csv");
      analysisManager->CreateNtuple("REPTile2","Deposited Energy");
  analysisManager->CreateNtupleDColumn("Particle Energy");
  analysisManager->CreateNtupleDColumn("D1");
  analysisManager->CreateNtupleDColumn("D1G");
  analysisManager->CreateNtupleDColumn("D2");
  analysisManager->CreateNtupleDColumn("D2G");
  analysisManager->CreateNtupleDColumn("D3");
  analysisManager->CreateNtupleDColumn("D3G");
  analysisManager->CreateNtupleDColumn("D4");
  analysisManager->CreateNtupleDColumn("D4G");
  // analysisManager->CreateNtupleDColumn("Theta"); //Particle origin angle from z axis
  analysisManager->CreateNtupleDColumn("Particle Velocity Theta"); //Particle initial velocity angle from z axis
  analysisManager->FinishNtuple();
  analysisManager->OpenFile();
}

/**

**/


void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

  //compute statistics: mean and rms
  //

  //print
  //
 
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << G4endl;
     
}
