The files in this folder should be enough to replicate the REPTIle-3 Geant analysis.  

A brief overview:

- REPTile3 is the Geant simulation.
- /src is the source code for the simulation - you can change the instrument geometry in src/DetectorConstruction.cc
- This code is adapted from the Geant4 codes used to simulate REPTile and REPTile-2 and so has some holdover code and files from those. An effort has been made to reduce this.
- The build folder contains 3D models must be loaded to successfully compile the Geant simulation
