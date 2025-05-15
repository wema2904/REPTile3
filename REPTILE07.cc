/* this is the first attempt to write a code for Geant4
Ian Crocker, 2 Mar 2008*/

#include "G4RunManager.hh"   //| 
#include "G4UImanager.hh"    //| General part of GEANT4 simulation, no need to change
#include "G4UIterminal.hh"   //|

#include "DetectorConstruction.hh"   //|
#include "DetectorMessenger.hh"   //|
#include "PhysicsList.hh"            //|
#include "PrimaryGeneratorAction.hh" //|head files for reptile
#include "G4VisExecutive.hh"                              //|
// 20 March 08  -->                  //|
#include "RunAction.hh"              //|
#include "TrackingAction.hh"         //|
#include "EventAction.hh"            //|
#include "StackingAction.hh"         //|
#include "SetSensDet.hh"             //|
// <-- 20 March 08  

#include "Randomize.hh"
#include "time.h"



int main(int argc,char** argv)
{
	//choose the Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	//set random seed with system time
	G4long seed = time(NULL);
	CLHEP::HepRandom::setTheSeed(seed);

	// Construct the default run manager
	G4RunManager* runManager = new G4RunManager;

	// set mandatory initialization classes
	G4VUserDetectorConstruction* detector = new DetectorConstruction;
	runManager->SetUserInitialization(detector);

	G4VUserPhysicsList* physics = new PhysicsList;
	runManager->SetUserInitialization(physics);

	// set mandatory user action class
	G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction;
	runManager->SetUserAction(gen_action);

// 20 March 08  -->	
    runManager->SetUserAction(new RunAction);
    runManager->SetUserAction(new EventAction);
    runManager->SetUserAction(new TrackingAction);
    runManager->SetUserAction(new StackingAction);
// <-- 20 March 08  

	// Initialize G4 kernel
	runManager->Initialize();

	// Get the pointer to the UI manager and set verbosities
	//G4UImanager* UI = G4UImanager::GetUIpointer();
	//UI->ApplyCommand("/run/verbose 1");
	//UI->ApplyCommand("/event/verbose 1");
	//UI->ApplyCommand("/tracking/verbose 1");

	// Start a run
	//G4int numberOfEvent = 1;
	//runManager->BeamOn(numberOfEvent);


	auto visManager = new G4VisExecutive(argc, argv);
	visManager->Initialize();

  if(argc>1) // execute an argument macro file if exist
  {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);    
  }
  else{ // start interactive session
    G4UIsession* session = new G4UIterminal();
    session->SessionStart();
    delete session;
  }
  /**
	G4UIsession * session = 0;
	session = new G4UIterminal();
	UI->ApplyCommand("/control/execute vis.mac");     
	session->SessionStart();
	delete session;
    */ 
	#ifdef G4VIS_USE
		delete visManager;
	#endif   

	// Job termination
	//
	// Free the store: user actions, physics_list and detector_description are
	//                 owned and deleted by the run manager, so they should not
	//                 be deleted in the main() program !
	//
	delete runManager;

	return 0;
}
