///
/// Step: 1) Define the detectors (box --> logical volume (material) --> physical volume (locations etc))
///       2) SetSensDet (based on the material, it will identify which detector it is and set the sensitivity accordingly)
///       3) SetSensitiveDetector
/// Adapted from REPTile-2 Geant Code by Lengying Khoo and Quinton Schiller


#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SetSensDet.hh"
#include <G4SubtractionSolid.hh>
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <G4NistManager.hh>
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
// CADMESH //
#include "CADMesh.hh"
DetectorConstruction::DetectorConstruction()
 :  //fDetectorMessenger(0),
    logic_w(0), logic_alMain(0),logic_alCap(0), logic_alShimFront(0),logic_alShimBack(0),logic_frontcoll(0) ,logic_coll(0), logic_coll_embed(0),logic_coll_embed1(0),logic_coll_embed2(0), logic_coll_embed3(0), logic_coll_embed4(0), logic_coll_embed5(0),logic_coll_embed6(0),logic_coll_tooth_W1(0), logic_coll_tooth_W2(0), logic_coll_tooth_W3(0), logic_coll_tooth_W4(0), logic_coll_tooth_W5(0), logic_coll_tooth_W6(0), logic_coll_tooth_W7(0), logic_al_ann1(0), logic_al_chm(0), logic_al_end(0), logic_al_endann(0), logic_be(0),logic_w_ann1(0), logic_w_ann2(0), logic_w_ann_Al1(0), logic_w_ann_Al2(0), logic_w_ann_Al3(0),logic_w_chm(0),logic_w_end(0),logic_w_endenh(0),logic_w_fendenh(0),logic_w_endenh_Al1(0),logic_w_endenh_Al2(0),logic_d1(0), logic_d1outer(0), logic_d2(0), logic_d2outer(0), logic_d3(0), logic_d3outer(0),logic_d4(0),logic_d4outer(0), logic_fastener1(0),logic_fastener2(0),logic_fastener3(0),
    physi_w(0), physi_alMain(0),physi_alCap(0),physi_alShimBack(0),physi_alShimFront(0), physi_frontcoll(0), physi_coll(0), physi_coll_embed(0),physi_coll_embed1(0), physi_coll_embed2(0), physi_coll_embed3(0), physi_coll_embed4(0), physi_coll_embed5(0),physi_coll_embed6(0),physi_coll_tooth_W1(0), physi_coll_tooth_W2(0), physi_coll_tooth_W3(0), physi_coll_tooth_W4(0),physi_coll_tooth_W5(0), physi_coll_tooth_W6(0), physi_coll_tooth_W7(0), physi_al_ann1(0), physi_al_chm(0), physi_al_end(0), physi_al_endann(0), physi_be(0),physi_w_ann1(0),physi_w_ann2(0), physi_w_chm(0), physi_w_end(0),physi_w_endenh(0),physi_w_fendenh(0),physi_w_endenh_Al1(0),physi_w_endenh_Al2(0), physi_d1(0), physi_d2(0), physi_d3(0), physi_d4(0),physi_d1outer(0), physi_d2outer(0), physi_d3outer(0), physi_d4outer(0),physi_fastener1(0),physi_fastener2(0),physi_fastener3(0)
{

    //G4cout << "----> DetectorMessenger-Before." << G4endl;          
      fDetectorMessenger = new DetectorMessenger(this);
      windowDepth    = 0.01 *mm;
      siliconDepth   = 1.5 *mm;
      }
DetectorConstruction::~DetectorConstruction()
{
    //;
  delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	//____________________ materials ____________________ 

	G4double a;  // atomic mass
	G4double z;  // atomic number
	G4double density;
	G4double ncomponents;

	// Aluminum
	G4Material* Al = 
	new G4Material("Aluminum", z= 13.,  a= 26.9815*g/mole, density= 2.6989*g/cm3);

	// Tungsten
    G4Material* W =
        // spring '08 value: new G4Material("Tungsten", z= 74., a= 183.84*g/mole, density= 19.25*g/cm3); // wikipedia
	new G4Material("Tungsten", z= 70, a= 171.8*g/mole, density= 16.7*g/cm3); // 90% W; 10% Cu

        // Tantalum
	G4Material* Ta = new G4Material("Tantalum",z= 73., a= 180.95*g/mole, density= 16.65*g/cm3);

	// Silicon
	G4Material* Si = new G4Material("Silicon",z= 14., a= 28.0855*g/mole, density= 2.33*g/cm3);
  
	// Beryllium
	G4Material* Be = new G4Material("Beryllium",z= 4., a= 9.0122*g/mole, density= 1.848*g/cm3);

        //from 3M Scotch Weld Epoxy Adhesive EC-2216 B/A Translucent
        //Epoxy (for FR4 )
        // Use NIST database for elements and materials whereever possible.
	G4NistManager* man = G4NistManager::Instance();
	G4Element* elC  = man->FindOrBuildElement("C");
    G4Element* elH  = man->FindOrBuildElement("H");
    G4Element* elO  = man->FindOrBuildElement("O");
    G4Element* elN  = man->FindOrBuildElement("N");
    
    G4Material *EpoxyBase = new G4Material("EpoxyBase",  density = 1.17*g/cm3, ncomponents=3);
    EpoxyBase->AddElement(elH, 21); // Hydrogen
    EpoxyBase->AddElement(elO,  3); // Oxygen
    EpoxyBase->AddElement(elC, 28); // Carbon
    
    G4Material *EpoxyAccelerator = new G4Material("EpoxyAccelerator",  density = 0.97*g/cm3, ncomponents=4);
    EpoxyAccelerator->AddElement(elH, 24); // Hydrogen
    EpoxyAccelerator->AddElement(elN,  2); // Nitrogen
    EpoxyAccelerator->AddElement(elO,  3); // Oxygen
    EpoxyAccelerator->AddElement(elC, 10); // Carbon
    
    //Density reference: https://www.globalspec.com/industrial-directory/density_epoxy_adhesives
    G4Material *Epoxy = new G4Material("Epoxy",  density = 1.31*g/cm3, ncomponents=2);
    Epoxy->AddMaterial(EpoxyBase, 50*perCent);
    Epoxy->AddMaterial(EpoxyAccelerator,  50*perCent);
    
    //Density reference: https://www.sciencedirect.com/science/article/pii/S0168583X15004437
    G4Material *EpoxyTungsten = new G4Material("EpoxyW",  density =  11*g/cm3, ncomponents=2);
    EpoxyTungsten->AddMaterial(Epoxy, 36*perCent);
    EpoxyTungsten->AddMaterial(W,  64*perCent);
        
	// Near Vacuum
	G4double atomicNumber = 1.;
	G4double massOfMole = 1.008*g/mole;
	G4double vac_density = 1.e-25*g/cm3;
	G4double temperature = 2.73*kelvin;
	G4double pressure = 3.e-18*pascal;
	G4Material* Vacuum = new G4Material("interGalactic", atomicNumber,massOfMole, vac_density, kStateGas,temperature, pressure);

	//Set the window material and depth
	windowMaterialBe = Be; // this is to ensure the Be and Al properties are the same as what we set and also limit the options we have for window material
	windowMaterialAl = Al;
	windowMaterial = windowMaterialBe;
	
        //Air?
	//G4double atomicNumber = 7.;
	//G4double massOfMole = 14*g/mole;
	//G4double vac_density = 1.5e-3*g/cm3;
	//G4double temperature = 296.*kelvin;
	//G4double pressure = 101325.*pascal;
	//G4Material* Vacuum = new G4Material("interGalactic", atomicNumber,massOfMole, vac_density, kStateGas,temperature, pressure);
	
	//____________________ solids ____________________

	// world volume (w)
	G4double w_fl = 15*cm; // full length
	G4double w_hl = 0.5*w_fl; // half length
	
	G4Box* solid_w = new G4Box("world",w_hl,w_hl,w_hl);
	logic_w = new G4LogicalVolume(solid_w,Vacuum,"world",0,0,0);
	physi_w = new G4PVPlacement(0,G4ThreeVector(),logic_w,"world",0,false,0);
    
    /* Aluminum Shell */
    auto mesh_alMain = CADMesh::TessellatedMesh::FromOBJ("./REPTile3AlShell.obj");
    G4VSolid* solid_alMain = mesh_alMain->GetSolid();
    solid_alMain ->SetName("solid_alMain");
    G4double z_alMain= -16.2*mm-1*mm;
    logic_alMain = new G4LogicalVolume(solid_alMain, Al,"logical_alMain", 0, 0, 0);
	G4RotationMatrix* rm = new G4RotationMatrix();
	rm->rotateX(-90.*deg);
    physi_alMain = new G4PVPlacement( rm, G4ThreeVector(-38.7*mm, -38.7*mm, z_alMain), logic_alMain, "physical_alMain", logic_w, false,0);
	
	// Collimator 
	// Tooth 1
	G4double coll_t1_x=-17*mm; // x location
	G4double coll_t1_y=-17*mm; // y location
	G4double coll_t1_z=-16.2*mm; // z location
	auto mesh_coll_t1 = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_TaTooth.obj");
    G4VSolid* solid_coll_t1 = mesh_coll_t1->GetSolid();
    solid_coll_t1 ->SetName("solid_coll_t1");
    rm = new G4RotationMatrix();
    logic_coll_t1 = new G4LogicalVolume(solid_coll_t1,Ta,"coll_t1",0,0,0);
    physi_coll_t1 = new G4PVPlacement(rm,G4ThreeVector(coll_t1_x,coll_t1_y,coll_t1_z),
                       logic_coll_t1,"coll_t1",logic_w,false,0);
    /*
    // Tooth 2
	G4double coll_t2_x=-17*mm; // x location
	G4double coll_t2_y=-17*mm; // y location
	G4double coll_t2_z=-10.0*mm; // z location
	auto mesh_coll_t2 = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_TaTooth.obj");
    G4VSolid* solid_coll_t2 = mesh_coll_t2->GetSolid();
    solid_coll_t2 ->SetName("solid_coll_t2");
    logic_coll_t2 = new G4LogicalVolume(solid_coll_t2,Ta,"coll_t2",0,0,0);
    physi_coll_t2 = new G4PVPlacement(rm,G4ThreeVector(coll_t2_x,coll_t2_y,coll_t2_z),
                       logic_coll_t2,"coll_t2",logic_w,false,0);
   
    // Tooth 3
	G4double coll_t3_x=-17*mm; // x location
	G4double coll_t3_y=-17*mm; // y location
	G4double coll_t3_z=-3.06*mm; // z location
	auto mesh_coll_t3 = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_TaTooth.obj");
    G4VSolid* solid_coll_t3 = mesh_coll_t3->GetSolid();
    solid_coll_t3 ->SetName("solid_coll_t2");
    logic_coll_t3 = new G4LogicalVolume(solid_coll_t3,Ta,"coll_t3",0,0,0);
    physi_coll_t3 = new G4PVPlacement(rm,G4ThreeVector(coll_t3_x,coll_t3_y,coll_t3_z),
                       logic_coll_t3,"coll_t3",logic_w,false,0);
    */
    // Teeth 2 and 3 updated REPTile3 Models     
   	// Tooth 2
	G4double coll_t2_x=-17*mm; // x location
	G4double coll_t2_y=-17*mm; // y location
	G4double coll_t2_z=-10.0*mm; // z location
	auto mesh_coll_t2 = CADMesh::TessellatedMesh::FromOBJ("./REPTile3_Tooth082.obj");
    G4VSolid* solid_coll_t2 = mesh_coll_t2->GetSolid();
    rm = new G4RotationMatrix();
	rm->rotateX(-90.*deg);
    solid_coll_t2 ->SetName("solid_coll_t2");
    logic_coll_t2 = new G4LogicalVolume(solid_coll_t2,Ta,"coll_t2",0,0,0);
    physi_coll_t2 = new G4PVPlacement(rm,G4ThreeVector(coll_t2_x,coll_t2_y,coll_t2_z),
                       logic_coll_t2,"coll_t2",logic_w,false,0);
   
    // Tooth 3
	G4double coll_t3_x=-17*mm; // x location
	G4double coll_t3_y=-17*mm; // y location
	G4double coll_t3_z=-3.06*mm; // z location
	auto mesh_coll_t3 = CADMesh::TessellatedMesh::FromOBJ("./REPTile3_GroovedTooth063.obj");
    G4VSolid* solid_coll_t3 = mesh_coll_t3->GetSolid();
    solid_coll_t3 ->SetName("solid_coll_t2");
    logic_coll_t3 = new G4LogicalVolume(solid_coll_t3,Ta,"coll_t3",0,0,0);
    physi_coll_t3 = new G4PVPlacement(rm,G4ThreeVector(coll_t3_x,coll_t3_y,coll_t3_z),
                       logic_coll_t3,"coll_t3",logic_w,false,0);
    
    // Spacer 1
    G4double coll_s1_d = -1*coll_t1_z+coll_t2_z-1.0*mm; // depth
    G4double coll_s1_hd=0.5*coll_s1_d*mm; // half depth
    G4double coll_s1_ir=15.0*mm; // inner radius
    G4double coll_s1_or=17.0*mm; // outer radius
    G4double coll_s1_sta=0.0*deg; // start angle
    G4double coll_s1_spa=360*deg; // span angle
    G4double coll_s1_x=0.0*mm; // x location
    G4double coll_s1_y=0.0*mm; // y location
    G4double coll_s1_z=coll_t1_z+coll_s1_hd+1.0*mm; // z location
    G4Tubs* solid_coll_s1 = new G4Tubs("coll_s1",coll_s1_ir,coll_s1_or,coll_s1_hd,coll_s1_sta,coll_s1_spa);
    logic_coll_s1 = new G4LogicalVolume(solid_coll_s1,Ta,"coll_s1",0,0,0);
    physi_coll_s1 = new G4PVPlacement(0,G4ThreeVector(coll_s1_x,coll_s1_y,coll_s1_z),logic_coll_s1,"coll_s1",logic_w,false,0);
    
    // Spacer 2
    G4double coll_s2_d = -1*coll_t2_z+coll_t3_z-1.0*mm-2.2*mm; // depth
    G4double coll_s2_hd=0.5*coll_s2_d*mm; // half depth
    G4double coll_s2_ir=15.0*mm; // inner radius
    G4double coll_s2_or=17.0*mm; // outer radius
    G4double coll_s2_sta=0.0*deg; // start angle
    G4double coll_s2_spa=360*deg; // span angle
    G4double coll_s2_x=0.0*mm; // x location
    G4double coll_s2_y=0.0*mm; // y location
    G4double coll_s2_z=coll_t2_z+coll_s2_hd+1.0*mm; // z location
    G4Tubs* solid_coll_s2 = new G4Tubs("coll_s2",coll_s2_ir,coll_s2_or,coll_s2_hd,coll_s2_sta,coll_s2_spa);
    logic_coll_s2 = new G4LogicalVolume(solid_coll_s2,Ta,"coll_s2",0,0,0);
    physi_coll_s2 = new G4PVPlacement(0,G4ThreeVector(coll_s2_x,coll_s2_y,coll_s2_z),logic_coll_s2,"coll_s2",logic_w,false,0);
                       
                       
	
	// beryllium disc
	//G4double be_d=0.15*mm; // depth
	G4double be_d=windowDepth; // depth
	G4double be_hd=0.5*windowDepth*mm; // half depth
	G4double be_ir=0.0*mm; // inner radius
	G4double be_or=15.0*mm; // outer radius
	G4double be_sta=0.0*deg; // start angle
	G4double be_spa=360*deg; // span angle
	G4double be_x=0.0*mm; // x location
	G4double be_y=0.0*mm; // y locationcoll
	G4double be_z=-0.5*mm-1.26*mm-0.12*mm+be_hd; // z location
	G4Tubs* solid_be = new G4Tubs("FOV_disc",be_ir,be_or,0.5*windowDepth,be_sta,be_spa);
    logic_be = new G4LogicalVolume(solid_be,windowMaterial,"FOV_disc",0,0,0);
    physi_be = new G4PVPlacement(0,G4ThreeVector(be_x,be_y,be_z),logic_be,"FOV_disc",logic_w,false,0);
    G4cout << "----> The window's z is " << be_z << G4endl;
    G4cout << "----> The window's material is " << logic_be->GetMaterial() << G4endl;
	
	 // tungsten front annulus 2 (houses: Be window and Ta_tooth)
    G4double w_ann_d2=1.3*mm; // depth
    G4double w_ann_hd2=0.5*w_ann_d2*mm; // half depth
    G4double w_ann_ir2=17.0*mm; // inner radius
    G4double w_ann_or2=33.7*mm; // outer radius | 30.5625 - 5.0
    G4double w_ann_sta2=0.0*deg; // start angle
    G4double w_ann_spa2=360*deg; // span angle
    G4double w_ann_x2=0.0*mm; // x location
    G4double w_ann_y2=0.0*mm; // y location
    G4double w_ann_z2=-0.5*mm -1.26*mm-w_ann_hd2; // z location
    G4Tubs* solid_w_ann2 = new G4Tubs("W_annulus2_s1",w_ann_ir2,w_ann_or2,w_ann_hd2,w_ann_sta2,w_ann_spa2);
    logic_w_ann2 = new G4LogicalVolume(solid_w_ann2,W,"W_annulus2_s1",0,0,0);
    physi_w_ann2 = new G4PVPlacement(0,G4ThreeVector(w_ann_x2,w_ann_y2,w_ann_z2),logic_w_ann2,"W_annulus2_s1",logic_w,false,0);
	
	// Shielding subject to change, will likely first change to solid tungsten tubs, then obj once we settle on shielding
	// tungsten front annulus
	G4double w_ann_d1=2.2*mm; // depth
	G4double w_ann_hd1=0.5*w_ann_d1*mm; // half depth
	G4double w_ann_ir1=15.0*mm; // inner radius
	G4double w_ann_or1=33.7*mm; // outer radius | 30.5625 - 5.0
	G4double w_ann_sta1=0.0*deg; // start angle
	G4double w_ann_spa1=360*deg; // span angle
	G4double w_ann_x1=0.0*mm; // x location
	G4double w_ann_y1=0.0*mm; // y location
	G4double w_ann_z1=-0.5*mm -1.26*mm-w_ann_d2-w_ann_hd1; // z location
	G4Tubs* solid_w_ann1 = new G4Tubs("W_annulus1_s1",w_ann_ir1,w_ann_or1,w_ann_hd1,w_ann_sta1,w_ann_spa1);
    logic_w_ann1 = new G4LogicalVolume(solid_w_ann1,W,"W_annulus1_s1",0,0,0);
    physi_w_ann1 = new G4PVPlacement(0,G4ThreeVector(w_ann_x1,w_ann_y1,w_ann_z1),logic_w_ann1,"W_annulus1_s1",logic_w,false,0);
    
    
    G4cout << "----> Total Thickness tungsten font annulus " << w_ann_d1+w_ann_d2 << G4endl;
	
//    // Alminum Shim Front // For imported items: the x, y are not started from (0,0) but the edge is (0,0). So, need to figure out the center
//    auto mesh_alShimFront = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_AlShimFront.obj");
//    G4VSolid* solid_alShimFront = mesh_alShimFront->GetSolid();
//    solid_alShimFront ->SetName("solid_alShimFront");
//    G4double x_alShimFront= -29.1*mm;
//    G4double y_alShimFront= -25.5 *mm;
//    G4double z_alShimFront= frontcoll_d+coll_d+al_ann1_d+w_ann_d;
//    logic_alShimFront = new G4LogicalVolume(solid_alShimFront, Al,"logical_alShimFront", 0, 0, 0);
//    physi_alShimFront = new G4PVPlacement( 0, G4ThreeVector(x_alShimFront, y_alShimFront, z_alShimFront), logic_alShimFront, "physical_alShimFront", logic_w, false,0);
    
	// tungsten chamber
	G4double w_chm_d= 19.0*mm; // depth
	G4double w_chm_hd=0.5*w_chm_d*mm; // half depth
	G4double w_chm_ir=30.2*mm; // inner radius
	G4double w_chm_or=33.7*mm; // outer radius
	G4double w_chm_sta=0.0*deg; // start angle
	G4double w_chm_spa=360*deg; // span angle
	G4double w_chm_x=0.0*mm; // x location
	G4double w_chm_y=0.0*mm; // y location
	G4double w_chm_z=-0.5*mm -1.26*mm+w_chm_hd; // z location
	G4Tubs* solid_w_chm = new G4Tubs("W_chamber",w_chm_ir,w_chm_or,w_chm_hd,w_chm_sta,w_chm_spa);
    logic_w_chm = new G4LogicalVolume(solid_w_chm,W,"W_chamber",0,0,0);
    physi_w_chm = new G4PVPlacement(0,G4ThreeVector(w_chm_x,w_chm_y,w_chm_z),logic_w_chm,"W_chamber",logic_w,false,0);
    
    //G4cout << "----> Total chamber location " << frontcoll_d+coll_d+al_ann1_d+w_ann_d1+w_ann_d2+w_chm_hd << G4endl;

//    // Aluminum Shim Back
//    auto mesh_alShimBack = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_AlShimBack.obj");
//    G4VSolid* solid_alShimBack = mesh_alShimBack->GetSolid();
//    solid_alShimBack ->SetName("solid_alShimBack");
//    G4double x_alShimBack= -29.175*mm;
//    G4double y_alShimBack= -25.4*mm;
//    G4double z_alShimBack= frontcoll_d+coll_d+al_ann1_d+w_ann_d+w_chm_d;
//    logic_alShimBack = new G4LogicalVolume(solid_alShimBack, Al,"logical_alShimBack", 0, 0, 0);
//    physi_alShimBack = new G4PVPlacement( 0, G4ThreeVector(x_alShimBack, y_alShimBack, z_alShimBack), logic_alShimBack, "physical_alShimBack", logic_w, false,0);

	// Tungsten rear shielding, inset
    G4double w_fendenh_d=2.0*mm; // depth
    G4double w_fendenh_hd=0.5*w_fendenh_d*mm; // half depth
    G4double w_fendenh_ir=0.0*mm; // inner radius
    G4double w_fendenh_or=30.2*mm; // outer radius
    G4double w_fendenh_sta=0.0*deg; // start angle
    G4double w_fendenh_spa=360*deg; // span angle
    G4double w_fendenh_x=0.0*mm; // x location
    G4double w_fendenh_y=0.0*mm; // y location
    G4double w_fendenh_z=-0.5*mm-1.26*mm+w_chm_d-w_fendenh_hd; // z location
    G4Tubs* solid_w_fendenh = new G4Tubs("W_fendenh_plate",w_fendenh_ir,w_fendenh_or,
                                          w_fendenh_hd,w_fendenh_sta,w_fendenh_spa);
    logic_w_fendenh = new G4LogicalVolume(solid_w_fendenh,W,"W_fendenh_plate",0,0,0);
    physi_w_fendenh = new G4PVPlacement(0,G4ThreeVector(w_fendenh_x,w_fendenh_y,w_fendenh_z),
                                         logic_w_fendenh,"W_fendenh_plate",logic_w,false,0);
	// Tungsten end plate
    G4double w_end_d=1.5*mm; // depth
	G4double w_end_hd=0.5*w_end_d*mm; // half depth
	G4double w_end_ir=0.0*mm; // inner radius
	G4double w_end_or=33.7*mm; // outer radius
	G4double w_end_sta=0.0*deg; // start angle
	G4double w_end_spa=360*deg; // span angle
	G4double w_end_x=0.0*mm; // x location
	G4double w_end_y=0.0*mm; // y location
	G4double w_end_z=-0.5*mm-1.26*mm+w_chm_d+w_end_hd; // z location
	G4Tubs* solid_w_end = new G4Tubs("W_end_plate",w_end_ir,w_end_or,w_end_hd,w_end_sta,w_end_spa);
    logic_w_end = new G4LogicalVolume(solid_w_end,W,"W_end_plate",0,0,0);
    physi_w_end = new G4PVPlacement(0,G4ThreeVector(w_end_x,w_end_y,w_end_z),logic_w_end,"W_end_plate",logic_w,false,0);
	
    // Tungsten rear shielding, inset 2
    G4double w_endenh_d=2.0*mm; // depth
    G4double w_endenh_hd=0.5*w_endenh_d*mm; // half depth
    G4double w_endenh_ir=0.0*mm; // inner radius
    G4double w_endenh_or=30.2*mm; // outer radius
    G4double w_endenh_sta=0.0*deg; // start angle
    G4double w_endenh_spa=360*deg; // span angle
    G4double w_endenh_x=0.0*mm; // x location
    G4double w_endenh_y=0.0*mm; // y location
    G4double w_endenh_z=-0.5*mm-1.26*mm+w_chm_d+w_end_d+w_endenh_hd; // z location
    G4Tubs* solid_w_endenh = new G4Tubs("W_endenh_plate",w_endenh_ir,w_endenh_or,
                                        w_endenh_hd,w_endenh_sta,w_endenh_spa);
    logic_w_endenh = new G4LogicalVolume(solid_w_endenh,W,"W_endenh_plate",0,0,0);
    physi_w_endenh = new G4PVPlacement(0,G4ThreeVector(w_endenh_x,w_endenh_y,w_endenh_z),
                                       logic_w_endenh,"W_endenh_plate",logic_w,false,0);
    
    //G4cout << "----> Total Length " << frontcoll_d+coll_d+al_ann1_d+w_ann_d+w_chm_d+w_end_d+w_endenh_d << G4endl;
    // detectors (d)
    // detector 1 (d1)
	G4double d1_d = 1.5*mm; // depth
	G4double d1_hd = 0.5*d1_d*mm; // half depth
	G4double d1_ir = 0.0*mm; // inner radius
	G4double d1_or = 10.0*mm; // outer radius
	G4double d1_sta = 0.0*deg; // start angle
	G4double d1_spa = 360*deg; // span angle
	G4double d1_x=0.0*mm; // x location
	G4double d1_y=0.0*mm; // y location
	G4double d1_z=0.0*mm + d1_hd; // z location
	G4Tubs* solid_d1 = new G4Tubs("detector_1",d1_ir,d1_or,d1_hd,d1_sta,d1_spa);
	logic_d1 = new G4LogicalVolume(solid_d1,Si,"detector_1",0,0,0);
	physi_d1 = new G4PVPlacement(0,G4ThreeVector(d1_x,d1_y,d1_z),logic_d1,"detector_1",logic_w,false,0);

    // detector 1 outer (d1)
    G4double d1outer_d = 1.5*mm; // depth
    G4double d1outer_hd = 0.5*d1outer_d*mm; // half depth
    G4double d1outer_ir = 10.0*mm; // inner radius
    G4double d1outer_or = 20.0*mm; // outer radius
    G4double d1outer_sta = 0.0*deg; // start angle
    G4double d1outer_spa = 360*deg; // span angle
    G4double d1outer_x = 0.0*mm; // x location
    G4double d1outer_y = 0.0*mm; // y location
    G4double d1outer_z=0.0*mm + d1_hd; // z location
    G4Tubs* solid_d1outer = new G4Tubs("detector_1Outer",d1outer_ir,d1outer_or,d1outer_hd,d1outer_sta,d1outer_spa);
    logic_d1outer = new G4LogicalVolume(solid_d1outer,Si,"detector_1Outer",0,0,0);
    physi_d1outer = new G4PVPlacement(0,G4ThreeVector(d1outer_x,d1outer_y,d1outer_z),logic_d1outer,"detector_1Outer",logic_w,false,0);
    
	// detector 2 (d2)
    G4double d2_d = 1.5*mm; // depth
    G4double d2_hd = 0.5*d2_d*mm; // half depth
    G4double d2_ir = 0.0*mm; // inner radius
    G4double d2_or = 10.0*mm; // outer radius
    G4double d2_sta = 0.0*deg; // start angle
    G4double d2_spa = 360*deg; // span angle
    G4double d2_x=0.0*mm; // x location
    G4double d2_y=0.0*mm; // y location
    G4double d2_z=3.98*mm + d2_hd; // z location
    G4Tubs* solid_d2 = new G4Tubs("detector_2",d2_ir,d2_or,d2_hd,d2_sta,d2_spa);
    logic_d2 = new G4LogicalVolume(solid_d2,Si,"detector_2",0,0,0);
    physi_d2 = new G4PVPlacement(0,G4ThreeVector(d2_x,d2_y,d2_z),logic_d2,"detector_2",logic_w,false,0);

    // detector 2 outer (d2)
    G4double d2outer_d = 1.5*mm; // depth
    G4double d2outer_hd = 0.5*d2outer_d*mm; // half depth
    G4double d2outer_ir = 10.0*mm; // inner radius
    G4double d2outer_or = 20.0*mm; // outer radius
    G4double d2outer_sta = 0.0*deg; // start angle
    G4double d2outer_spa = 360*deg; // span angle
    G4double d2outer_x = 0.0*mm; // x location
    G4double d2outer_y = 0.0*mm; // y location
    G4double d2outer_z = 3.98*mm + d2_hd; // z location
    G4Tubs* solid_d2outer = new G4Tubs("detector_2Outer",d2outer_ir,d2outer_or,d2outer_hd,d2outer_sta,d2outer_spa);
    logic_d2outer = new G4LogicalVolume(solid_d2outer,Si,"detector_2Outer",0,0,0);
    physi_d2outer = new G4PVPlacement(0,G4ThreeVector(d2outer_x,d2outer_y,d2outer_z),logic_d2outer,"detector_2Outer",logic_w,false,0);
    
	// detector 3 (d3)
	G4double d3_d = 1.5*mm; // depth
	G4double d3_hd = 0.5*d3_d*mm; // half depth
	G4double d3_ir = 0.0*mm; // inner radius
	G4double d3_or = 10.0*mm; // outer radius
	G4double d3_sta = 0.0*deg; // start angle
	G4double d3_spa = 360*deg; // span angle
	G4double d3_x=0.0*mm; // x location
	G4double d3_y=0.0*mm; // y location
	G4double d3_z=7.96*mm + d3_hd; // z location
	G4Tubs* solid_d3 = new G4Tubs("detector_3",d3_ir,d3_or,d3_hd,d3_sta,d3_spa);
	logic_d3 = new G4LogicalVolume(solid_d3,Si,"detector_3",0,0,0);
	physi_d3 = new G4PVPlacement(0,G4ThreeVector(d3_x,d3_y,d3_z),logic_d3,"detector_3",logic_w,false,0);

    // detector 3 outer (d3)
    G4double d3outer_d = 1.5*mm; // depth
    G4double d3outer_hd = 0.5*d3outer_d*mm; // half depth
    G4double d3outer_ir = 10.0*mm; // inner radius
    G4double d3outer_or = 20.0*mm; // outer radius
    G4double d3outer_sta = 0.0*deg; // start angle
    G4double d3outer_spa = 360*deg; // span angle
    G4double d3outer_x = 0.0*mm; // x location
    G4double d3outer_y = 0.0*mm; // y location
    G4double d3outer_z = 7.96*mm + d3_hd; // z location
    G4Tubs* solid_d3outer = new G4Tubs("detector_3Outer",d3outer_ir,d3outer_or,d3outer_hd,d3outer_sta,d3outer_spa);
    logic_d3outer = new G4LogicalVolume(solid_d3outer,Si,"detector_3Outer",0,0,0);
    physi_d3outer = new G4PVPlacement(0,G4ThreeVector(d3outer_x,d3outer_y,d3outer_z),logic_d3outer,"detector_3Outer",logic_w,false,0);
    
	// detector 4 (d4)
	G4double d4_d = 1.5*mm; // depth
	G4double d4_hd = 0.5*d4_d*mm; // half depth
	G4double d4_ir = 0.0*mm; // inner radius
	G4double d4_or = 10.0*mm; // outer radius
	G4double d4_sta = 0.0*deg; // start angle
	G4double d4_spa = 360*deg; // span angle
	G4double d4_x=0.0*mm; // x location
	G4double d4_y=0.0*mm; // y location
	G4double d4_z=11.94*mm + d4_hd; // z location
	G4Tubs* solid_d4 = new G4Tubs("detector_4",d4_ir,d4_or,d4_hd,d4_sta,d4_spa);
	logic_d4 = new G4LogicalVolume(solid_d4,Si,"detector_4",0,0,0);
	physi_d4 = new G4PVPlacement(0,G4ThreeVector(d4_x,d4_y,d4_z),logic_d4,"detector_4",logic_w,false,0);

    // detector 4 outer (d4)
    G4double d4outer_d = 1.5*mm; // depth
    G4double d4outer_hd = 0.5*d4outer_d*mm; // half depth
    G4double d4outer_ir = 10.0*mm; // inner radius
    G4double d4outer_or = 20.0*mm; // outer radius
    G4double d4outer_sta = 0.0*deg; // start angle
    G4double d4outer_spa = 360*deg; // span angle
    G4double d4outer_x = 0.0*mm; // x location
    G4double d4outer_y = 0.0*mm; // y location
    G4double d4outer_z = 11.94*mm + d4_hd; // z location
    G4Tubs* solid_d4outer = new G4Tubs("detector_4Outer",d4outer_ir,d4outer_or,d4outer_hd,d4outer_sta,d4outer_spa);
    logic_d4outer = new G4LogicalVolume(solid_d4outer,Si,"detector_4Outer",0,0,0);
    physi_d4outer = new G4PVPlacement(0,G4ThreeVector(d4outer_x,d4outer_y,d4outer_z),logic_d4outer,"detector_4Outer",logic_w,false,0);
	
	/* Aluminum Cap */
    auto mesh_alCap = CADMesh::TessellatedMesh::FromOBJ("./REPTile2c_AlCap_1023.obj");
    G4VSolid* solid_alCap = mesh_alCap->GetSolid();
    solid_alCap ->SetName("solid_alCap");
    G4double x_alCap= -38.7*mm;
    G4double y_alCap= -38.7*mm;
    G4double z_alCap= -0.5*mm-1.26*mm+w_chm_d+w_end_d+w_endenh_d;
    logic_alCap = new G4LogicalVolume(solid_alCap, Al,"logical_alCap", 0, 0, 0);
	
    physi_alCap = new G4PVPlacement(0, G4ThreeVector(x_alCap, y_alCap, z_alCap), logic_alCap, "physical_alCap", logic_w, false,0);
	
	
	// Constraint volume for spherical cap
    G4double constvol_zl = 30*mm; // z length
    G4double constvol_hzl = 0.5*constvol_zl*mm; // half z length
    G4double constvol_xl = 70.0*mm; // x length
    G4double constvol_yl = 70.0*mm; // y length
    G4double constvol_x = 0.0*mm; // x location
    G4double constvol_y = 0.0*mm; // y location
    G4double constvol_z = -constvol_zl-16.2*mm; // z location
    G4Box* solid_constvol = new G4Box("constvol",constvol_xl,constvol_yl,constvol_zl);
    logic_constvol = new G4LogicalVolume(solid_constvol,Vacuum,"logical_constvol",0,0,0);
    physi_constvol = new G4PVPlacement(0,G4ThreeVector(constvol_x,constvol_y,constvol_z),logic_constvol,"physical_constvol",logic_w,false,0);
	
    G4cout << "----> Detector setup done " << G4endl;
    

	//____________________ visible attributes ____________________

	VisAtt_w = new G4VisAttributes(G4Colour(0.0,0.0,0.0));
	VisAtt_w->SetVisibility(true);
	logic_w->SetVisAttributes(VisAtt_w);
    
    VisAtt_alMain = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    VisAtt_alMain->SetVisibility(true);
    logic_alMain->SetVisAttributes(VisAtt_alMain);
    
    VisAtt_alCap = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    VisAtt_alCap->SetVisibility(true);
    logic_alCap->SetVisAttributes(VisAtt_alCap);
    
//    VisAtt_alShimFront = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_alShimFront->SetVisibility(true);
//    logic_alShimFront->SetVisAttributes(VisAtt_alShimFront);
//
//    VisAtt_alShimBack = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_alShimBack->SetVisibility(true);
//    logic_alShimBack->SetVisAttributes(VisAtt_alShimBack);
    
//collimator and its teeth
//    VisAtt_frontcoll = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_frontcoll->SetVisibility(true);
//    logic_frontcoll->SetVisAttributes(VisAtt_frontcoll);
//
//    VisAtt_coll = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_coll->SetVisibility(true);
//    logic_coll->SetVisAttributes(VisAtt_coll);
    
//    VisAtt_coll_embed = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //embeded heavy shielding
//    VisAtt_coll_embed->SetVisibility(true);
//    logic_coll_embed->SetVisAttributes(VisAtt_coll_embed);
 
 /*   
    VisAtt_coll = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll->SetVisibility(true);
    logic_coll->SetVisAttributes(VisAtt_coll);
   */ 
    VisAtt_coll_t1 = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll_t1->SetVisibility(true);
    logic_coll_t1->SetVisAttributes(VisAtt_coll_t1);
    
    VisAtt_coll_t2 = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll_t2->SetVisibility(true);
    logic_coll_t2->SetVisAttributes(VisAtt_coll_t2);
    
    VisAtt_coll_t3 = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll_t3->SetVisibility(true);
    logic_coll_t3->SetVisAttributes(VisAtt_coll_t3);
    
    VisAtt_coll_s1 = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll_s1->SetVisibility(true);
    logic_coll_s1->SetVisAttributes(VisAtt_coll_s1);
    
    VisAtt_coll_s2 = new G4VisAttributes(G4Colour(0.0,1.0,1.0));  //W1
    VisAtt_coll_s2->SetVisibility(true);
    logic_coll_s2->SetVisAttributes(VisAtt_coll_s2);

//    VisAtt_al_ann1 = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_al_ann1->SetVisibility(true);
//    logic_al_ann1->SetVisAttributes(VisAtt_al_ann1);
//
////    VisAtt_al_ann2 = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
////    VisAtt_al_ann2->SetVisibility(true);
////    logic_al_ann2->SetVisAttributes(VisAtt_al_ann2);
//
//    VisAtt_al_chm = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_al_chm->SetVisibility(true);
//    logic_al_chm->SetVisAttributes(VisAtt_al_chm);

//    VisAtt_al_end = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_al_end->SetVisibility(true);
//    logic_al_end->SetVisAttributes(VisAtt_al_end);
//
//    VisAtt_al_endann = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
//    VisAtt_al_endann->SetVisibility(true);
//    logic_al_endann->SetVisAttributes(VisAtt_al_endann);

    VisAtt_be = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
    VisAtt_be->SetVisibility(true);
    logic_be->SetVisAttributes(VisAtt_be);
	
//    VisAtt_w_ann = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
//    VisAtt_w_ann->SetVisibility(true);
//    logic_w_ann->SetVisAttributes(VisAtt_w_ann);

    VisAtt_w_ann1 = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_ann1->SetVisibility(true);
    logic_w_ann1->SetVisAttributes(VisAtt_w_ann1);
    
    VisAtt_w_ann2 = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_ann2->SetVisibility(true);
    logic_w_ann2->SetVisAttributes(VisAtt_w_ann2);
    
    VisAtt_w_chm = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_chm->SetVisibility(true);
    logic_w_chm->SetVisAttributes(VisAtt_w_chm);

    VisAtt_w_end = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_end->SetVisibility(true);
    logic_w_end->SetVisAttributes(VisAtt_w_end);
    
    VisAtt_w_endenh = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_endenh->SetVisibility(true);
    logic_w_endenh->SetVisAttributes(VisAtt_w_endenh);
    
    VisAtt_w_fendenh = new G4VisAttributes(G4Colour(0.5,0.0,1.0));
    VisAtt_w_fendenh->SetVisibility(true);
    logic_w_fendenh->SetVisAttributes(VisAtt_w_fendenh);
    
    VisAtt_d1 = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d1->SetVisibility(true);
    logic_d1->SetVisAttributes(VisAtt_d1);
    
    VisAtt_d1Outer = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d1Outer->SetVisibility(true);
    logic_d1outer->SetVisAttributes(VisAtt_d1Outer);
    
	VisAtt_d2 = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d2->SetVisibility(true);
    logic_d2->SetVisAttributes(VisAtt_d2);
    
    VisAtt_d2Outer = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d2Outer->SetVisibility(true);
    logic_d2outer->SetVisAttributes(VisAtt_d2Outer);

	VisAtt_d3 = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d3->SetVisibility(true);
    logic_d3->SetVisAttributes(VisAtt_d3);
    
    VisAtt_d3Outer = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d3Outer->SetVisibility(true);
    logic_d3outer->SetVisAttributes(VisAtt_d3Outer);
    
	VisAtt_d4 = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d4->SetVisibility(true);
    logic_d4->SetVisAttributes(VisAtt_d4);

    VisAtt_d4Outer = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    VisAtt_d4Outer->SetVisibility(true);
    logic_d4outer->SetVisAttributes(VisAtt_d4Outer);
	
    VisAtt_constvol = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    VisAtt_constvol->SetVisibility(true);
    logic_constvol->SetVisAttributes(VisAtt_constvol);
	
    
    G4cout << "----> Visualization setup done " << G4endl;

   // designate as sensitive detectors 

  sdManager = G4SDManager::GetSDMpointer();

  //BEGIN OLD CODE FOR 2 SENSITIVE DETECTORS
 //COMMENTED BY DLT ON 3 APRIL 2008
 //G4String SiliconSensDetNames[2] = {"detector1","detector2"};
 //
 // SiSD[0] = new SetSensDet(SiliconSensDetNames[0]);
 // SiSD[1] = new SetSensDet(SiliconSensDetNames[1]);
 // sdManager->AddNewDetector(SiSD[0]);
 // sdManager->AddNewDetector(SiSD[1]);
 // logic_d1->SetSensitiveDetector(SiSD[0]);
 // logic_d2->SetSensitiveDetector(SiSD[1]);
 //END OLD CODE FOR 2 DETECTORS

 //BEGIN NEW CODE FOR 8 SENSITIVE DETECTORS
 //BY DLT 3 APRIL 2008
 G4String SiliconSensDetNames[8] = {"detector_1","detector_1Outer","detector_2","detector_2Outer","detector_3","detector_3Outer","detector_4","detector_4Outer"};

 SiSD[0] = new SetSensDet(SiliconSensDetNames[0]);
 SiSD[1] = new SetSensDet(SiliconSensDetNames[1]);
 SiSD[2] = new SetSensDet(SiliconSensDetNames[2]);
 SiSD[3] = new SetSensDet(SiliconSensDetNames[3]);
 SiSD[4] = new SetSensDet(SiliconSensDetNames[4]);
 SiSD[5] = new SetSensDet(SiliconSensDetNames[5]);
 SiSD[6] = new SetSensDet(SiliconSensDetNames[6]);
 SiSD[7] = new SetSensDet(SiliconSensDetNames[7]);


 sdManager->AddNewDetector(SiSD[0]);
 sdManager->AddNewDetector(SiSD[1]);
 sdManager->AddNewDetector(SiSD[2]);
 sdManager->AddNewDetector(SiSD[3]);
 sdManager->AddNewDetector(SiSD[4]);
 sdManager->AddNewDetector(SiSD[5]);
 sdManager->AddNewDetector(SiSD[6]);
 sdManager->AddNewDetector(SiSD[7]);


 logic_d1->SetSensitiveDetector(SiSD[0]);
 logic_d1outer->SetSensitiveDetector(SiSD[1]);
 logic_d2->SetSensitiveDetector(SiSD[2]);
 logic_d2outer->SetSensitiveDetector(SiSD[3]);
 logic_d3->SetSensitiveDetector(SiSD[4]);     
 logic_d3outer->SetSensitiveDetector(SiSD[5]);
 logic_d4->SetSensitiveDetector(SiSD[6]);
 logic_d4outer->SetSensitiveDetector(SiSD[7]);
 //END NEW CODE FOR 8 DETECTORS
  



	return physi_w;
}

void DetectorConstruction::SetWindowMaterial(G4String materialChoice) 
{

  if (materialChoice == "Al") {
     logic_be -> SetMaterial(windowMaterialAl);
     //windowMaterial = windowMaterialAl;
     
     G4cout 
          << G4endl 
          << "----> The window is made of " << materialChoice << G4endl;
    G4cout 
          << G4endl 
          << "----> The window is made of " << logic_be->GetMaterial() << G4endl; 
    }
  if (materialChoice == "Be") {
     logic_be -> SetMaterial(windowMaterialBe);
     //windowMaterial = windowMaterialBe;
     
     G4cout 
          << G4endl 
          << "----> The window is made of " << materialChoice << G4endl; 
  }

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    //G4RunManager::GetRunManager()->ReinitializeGeometry();
    G4cout 
          << G4endl 
          << "----> The window is made of " << logic_be->GetMaterial() << G4endl; 
}
 

void DetectorConstruction::SetWindowDepth(G4double val) {
    if (val > 0.0) {
        windowDepth = val;
        //G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
        G4RunManager::GetRunManager()->ReinitializeGeometry();
         G4cout 
          << G4endl 
          << "----> The window's depth has changed to " << windowDepth << G4endl; 
    }
    else { 
     G4cout 
          << G4endl 
          << "----> The window depth choice must be greater than 0. -Command is ignored." << G4endl;
    };

    
}
