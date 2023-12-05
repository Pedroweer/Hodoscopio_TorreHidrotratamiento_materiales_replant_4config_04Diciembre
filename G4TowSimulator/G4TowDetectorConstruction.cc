// implementation of the G4TowDetectorConstruction class
#include <iostream>
#include <cmath>
#include "G4TowDetectorConstruction.h"
// #include "G4VUserDetectorConstruction.hh"// agreguè
#include "Materials.h"

#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
using namespace std;


G4TowDetectorConstruction::G4TowDetectorConstruction(Event& theEvent) : 
	G4VUserDetectorConstruction(),
	fEvent(theEvent)
{ 
}

G4TowDetectorConstruction::~G4TowDetectorConstruction() 
	{ }

G4VPhysicalVolume*
G4TowDetectorConstruction::CreateDetector()
{

	CreateWorld();
	//CreateGround();
	PlaceDetector(fEvent);
	return physWorld;
}

void
G4TowDetectorConstruction::CreateWorld()
{

	// size definitions
	const Event::Config &cfg = fEvent.GetConfig();	

	// double fGroundSizeX = 5 * CLHEP::m;
	// double fGroundSizeY = 5 * CLHEP::m;
	// double fGroundSizeZ = 1 * CLHEP::m;

	double fWorldSizeX = 12 * CLHEP::m;
	double fWorldSizeY = 22 * CLHEP::m;
	double fWorldSizeZ = 22 * CLHEP::m;

	solidWorld  = new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);
	logicWorld = new G4LogicalVolume(solidWorld, Materials().Air, "World");
	physWorld  =  new G4PVPlacement(nullptr, G4ThreeVector(), "World", logicWorld, 0, false, 0, fCheckOverlaps);




//#####
//###################--------DATOS GEOMÈTRICOS DE LA TORRE CILÌNDRICA A MODELAR------------#################################
        //  - PEDRO ARANGO -

	//++++++++++ Datos de orientacion, centro y espesores ++++++++++++++++++++ 
        double startAngle =0;  // Starting angle (usually 0)
        double spanningAngle = 360; // Spanning angle in degrees (360 mean a full circle)
	double AngleX=-45 * deg;// ángulo de inclinación del eje de visión del Hodoscopio Ideal
	G4RotationMatrix* rotationCyl = new G4RotationMatrix();
	rotationCyl->rotateX(90 * deg);
	rotationCyl->rotateX(AngleX);
	double SenAng=-sin(AngleX);// signo menos porque el angulo apunta al Z negativo
        double CosAng=cos(AngleX);
	double CoorZc = 0 * CLHEP::cm;// coordenada horizontal del centro del cilindro
        double CoorYc = 0 * CLHEP::cm;// coordenada vertical del centro del cilindro
	G4ThreeVector positionCyl(0,CoorYc,CoorZc);
        double espesorI = 2.4 * CLHEP::cm;// espesor de la envolvente de acero 
        double espesorII = 10 * CLHEP::cm;// espesor de la envolvente de Calorifugado
        double espesorIII = 0.1 * CLHEP::cm;// espesor de la envolvente de Aluminio


	//++++++++ Datos de Torre de Hidrotratamiento de Lecho Fijo ++++++++++++++++++++++
	double RadioLecho = 200 * CLHEP::cm;// radio del cilindro catalizador
        double HeightIner01=15.24* CLHEP::cm; // altura de Seccion Inerte 01
        double HeightIner02=15.24* CLHEP::cm; // altura de Seccion Inerte 02
        double HeighLecho01=0* CLHEP::cm; // Altura de lecho fijo 01 ( densidad normal )
        double HeighLecho02=1200* CLHEP::cm; // Altura de lecho fijo 02 ( densidad incrementada )
	int incrLecho=4; // Incremento de densidad lecho2:    1 inc 20%;   2 inc 40%;    3 inc 60%;    4 inc 74%;
        bool Lecho2Incrementado=true; // "true" si consideramos porción con densidad aumentada y "false" si no lo consideramos
       	double HeightCyl = HeightIner01+HeightIner02+HeighLecho01+HeighLecho02;                  // altura total del cilindro de hidrotratamiento
	double CoorY01 =CoorYc+(HeightCyl/2-HeightIner01/2)*CosAng;
        double CoorY02 =CoorY01-(HeightIner01/2+HeighLecho01/2)*CosAng;
        double CoorY03 =CoorY02-(HeighLecho01/2+HeighLecho02/2)*CosAng;
        double CoorY04 =CoorY03-(HeighLecho02/2+HeightIner02/2)*CosAng;
        double CoorZ01 =CoorZc+(HeightCyl/2-HeightIner01/2)*SenAng;
        double CoorZ02 =CoorZ01-(HeightIner01/2+HeighLecho01/2)*SenAng;
        double CoorZ03 =CoorZ02-(HeighLecho01/2+HeighLecho02/2)*SenAng;
        double CoorZ04 =CoorZ03-(HeighLecho02/2+HeightIner02/2)*SenAng;



	//++++++++ Datos de torre de destilaciòn +++++++++++++++++++
	
	

        //++++++++++++ Definiendo si se considera o no ciertos materiales en el modelo +++++++++++
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


if(Lecho2Incrementado) { if(HeighLecho01==0) {HeighLecho01=1* CLHEP::cm;} else {};
}else{
};






       // secciòn inerte 01
        solidBrick01=new G4Tubs("LeadBrick01", 0, RadioLecho,HeightIner01/2 , startAngle, spanningAngle);
        G4VisAttributes Apar01(G4Colour::Cyan());
        logicBrick01 = new G4LogicalVolume(solidBrick01, Materials().Steel, "LeadBrick01");
        logicBrick01->SetVisAttributes(Apar01);
        G4ThreeVector positionCyl01(0,CoorY01 ,CoorZ01);// coordenadas del Centro del cilindro
        physBrick01 = new G4PVPlacement(rotationCyl,positionCyl01, logicBrick01, "LeadBrick01", logicWorld, false, 0, cfg.fCheckOverlaps);
 
	// secciòn lecho sin residuos
	solidBrick02=new G4Tubs("LeadBrick02", 0, RadioLecho,HeighLecho01/2 , startAngle, spanningAngle);
        G4VisAttributes Apar02(G4Colour::Grey());
        logicBrick02 = new G4LogicalVolume(solidBrick02, Materials().LechoFijo, "LeadBrick02");
        logicBrick02->SetVisAttributes(Apar02);
        G4ThreeVector positionCyl02(0,CoorY02 ,CoorZ02);// coordenadas del Centro del cilindro
        physBrick02 = new G4PVPlacement(rotationCyl,positionCyl02, logicBrick02, "LeadBrick02", logicWorld, false, 0, cfg.fCheckOverlaps);
         
        if (Lecho2Incrementado){	
	// secciòn de lecho con residuos - densidad incrementada
        solidBrick03=new G4Tubs("LeadBrick03", 0, RadioLecho,HeighLecho02/2 , startAngle, spanningAngle);
        G4VisAttributes Apar03(G4Colour::Red());
        if (incrLecho=1){logicBrick03 = new G4LogicalVolume(solidBrick03, Materials().LechoFijo_20porciento, "LeadBrick03");}// 20 de material
	else { if (incrLecho=2){logicBrick03 = new G4LogicalVolume(solidBrick03, Materials().LechoFijo_40porciento, "LeadBrick03");}// 40 de mater
	else{ if(incrLecho=3){ logicBrick03 = new G4LogicalVolume(solidBrick03, Materials().LechoFijo_60porciento, "LeadBrick03");}// 60 de mater
	else{ logicBrick03 = new G4LogicalVolume(solidBrick03, Materials().LechoFijo_74porciento, "LeadBrick03"); } } } // 74 de mater
        logicBrick03->SetVisAttributes(Apar03);
        G4ThreeVector positionCyl03(0,CoorY03 ,CoorZ03);// coordenadas del Centro del cilindro
        physBrick03 = new G4PVPlacement(rotationCyl,positionCyl03, logicBrick03, "LeadBrick03", logicWorld, false, 0, cfg.fCheckOverlaps);
	}else{}; 
	// secciòn inerte 02
        solidBrick04=new G4Tubs("LeadBrick04", 0, RadioLecho,HeightIner02/2 , startAngle, spanningAngle);
        G4VisAttributes Apar04(G4Colour::Cyan());
        logicBrick04 = new G4LogicalVolume(solidBrick04, Materials().Steel, "LeadBrick04");
        logicBrick04->SetVisAttributes(Apar04);
        G4ThreeVector positionCyl04(0,CoorY04 ,CoorZ04);// coordenadas del Centro del cilindro
        physBrick04 = new G4PVPlacement(rotationCyl,positionCyl04, logicBrick04, "LeadBrick04", logicWorld, false, 0, cfg.fCheckOverlaps);
        
        
        // Envolvente de Acero
        solidBrick05=new G4Tubs("LeadBrick05", RadioLecho,RadioLecho + espesorI ,HeightCyl/2 , startAngle, spanningAngle);
        G4VisAttributes Apar05(G4Colour::Cyan());
        logicBrick05 = new G4LogicalVolume(solidBrick05, Materials().Steel, "LeadBrick05");
        logicBrick05->SetVisAttributes(Apar05);
        physBrick05 = new G4PVPlacement(rotationCyl,positionCyl, logicBrick05, "LeadBrick05", logicWorld, false, 0, cfg.fCheckOverlaps);


        //Envolvente de Calorifugado
        solidBrick06=new G4Tubs("LeadBrick06",RadioLecho+espesorI,RadioLecho+espesorI+espesorII,HeightCyl/2,startAngle,spanningAngle);
        G4VisAttributes Apar06(G4Colour::Brown());
        logicBrick06 = new G4LogicalVolume(solidBrick06, Materials().Air, "LeadBrick06");
        logicBrick06->SetVisAttributes(Apar06);
        physBrick06 = new G4PVPlacement(rotationCyl,positionCyl, logicBrick06, "LeadBrick06", logicWorld, false, 0, cfg.fCheckOverlaps);
        

        //Envolvente de Aluminio
        solidBrick07=new G4Tubs("LeadBrick07",RadioLecho+espesorI+espesorII,RadioLecho+espesorI+espesorII+espesorIII,HeightCyl/2,startAngle,spanningAngle);
        G4VisAttributes Apar07(G4Colour::Gray());
        logicBrick07 = new G4LogicalVolume(solidBrick07, Materials().Alum, "LeadBrick07");
        logicBrick07->SetVisAttributes(Apar07);
        physBrick07 = new G4PVPlacement(rotationCyl,positionCyl, logicBrick07, "LeadBrick07", logicWorld, false, 0, cfg.fCheckOverlaps);
        


//++++++++++++++++++++++++ HASTA AQUÌ +++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Blindaje = false; // se define si se va incluir blindaje de Hodoscopio o no
   if (Blindaje)
   {        //########################## Esructura
        double Y_centro_panelI =-800 * CLHEP::cm; //ubicaciòn del centro del primer panel (con esto se ubican todos los blindajes)
        double Z_centro_panelI = -500 * CLHEP::cm;

	// lead bricks I (CAPA DE PLOMO ATENUADORA)
        double fBrickSizeXI = 28.5 * CLHEP::cm;
        double fBrickSizeYI = 28.5 * CLHEP::cm;
        double fBrickSizeZI = 1.0 * CLHEP::cm;

        solidBrickI = new G4Box("LeadBrickI", fBrickSizeXI, fBrickSizeYI, fBrickSizeZI);
        logicBrickI = new G4LogicalVolume(solidBrickI, Materials().Lead, "LeadBrickI");
	physBrickI = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI,Z_centro_panelI+5.0*CLHEP::cm), logicBrickI, "LeadBrickI", logicWorld, false, 0, cfg.fCheckOverlaps); 

	G4VisAttributes* negro = new G4VisAttributes(G4Colour::Black());
	logicBrickI->SetVisAttributes(negro);

        //########################################## RECORDAR QUE EL PANEL I ESTA EN (0,Y_centro_panelI,Z_centro_panelI)
	// Recubrimeinto de Hierro I
	double fIronCoverSizeX = 25.5 * CLHEP::cm;
	double fIronCoverSizeY = 25.5 * CLHEP::cm;
	double fIronCoverSizeZ = 0.2 * CLHEP::cm;

	solidIronCoverI = new G4Box("IronCoverI", fIronCoverSizeX, fIronCoverSizeY, fIronCoverSizeZ);
	logicIronCoverI = new G4LogicalVolume(solidIronCoverI, Materials().Iron, "IronCoverI");
	physIronCoverI = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI ,Z_centro_panelI+3.0 * CLHEP::cm), logicIronCoverI, "IronCoverI", logicWorld, false, 0, cfg.fCheckOverlaps);

        G4VisAttributes* rojo = new G4VisAttributes(G4Colour::Red());
        logicIronCoverI->SetVisAttributes(rojo);

        //##################### II 
	double fIronCoverSizeXII = 25.5 * CLHEP::cm;
	double fIronCoverSizeYII = 25.5 * CLHEP::cm;
	double fIronCoverSizeZII = 0.2 * CLHEP::cm;

	solidIronCoverII = new G4Box("IronCoverII", fIronCoverSizeXII, fIronCoverSizeYII, fIronCoverSizeZII);
	logicIronCoverII = new G4LogicalVolume(solidIronCoverII, Materials().Iron, "IronCoverII");
	physIronCoverII = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI ,Z_centro_panelI-3.0 * CLHEP::cm), logicIronCoverII, "IronCoverII", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverII->SetVisAttributes(rojo);


        //######################################### EL PANEL II ESTA EN (0,Y_centro_panelI,Z_centro_panelI-200.0 CM)
        //#################### III

	double posicionsegundopanel = -200.0 * CLHEP::cm;

	double fIronCoverSizeXIII = 25.5 * CLHEP::cm;
	double fIronCoverSizeYIII = 25.5 * CLHEP::cm;
	double fIronCoverSizeZIII = 0.2 * CLHEP::cm;

	solidIronCoverIII = new G4Box("IronCoverIII", fIronCoverSizeXIII, fIronCoverSizeYIII, fIronCoverSizeZIII);
	logicIronCoverIII = new G4LogicalVolume(solidIronCoverIII, Materials().Iron, "IronCoverIII");
	physIronCoverIII = new G4PVPlacement(nullptr, G4ThreeVector(0.,Y_centro_panelI,Z_centro_panelI+posicionsegundopanel+3), logicIronCoverIII, "IronCoverIII", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverIII->SetVisAttributes(rojo);

        //33333333333333333333 IV

	double fIronCoverSizeXIV = 25.5 * CLHEP::cm;
	double fIronCoverSizeYIV = 25.5 * CLHEP::cm;
	double fIronCoverSizeZIV = 0.2 * CLHEP::cm;

	solidIronCoverIV = new G4Box("IronCoverIV", fIronCoverSizeXIV, fIronCoverSizeYIV, fIronCoverSizeZIV);
	logicIronCoverIV = new G4LogicalVolume(solidIronCoverIV, Materials().Iron, "IronCoverIV");
	physIronCoverIV = new G4PVPlacement(nullptr, G4ThreeVector(0., Y_centro_panelI,Z_centro_panelI+posicionsegundopanel-3), logicIronCoverIV, "IronCoverIV", logicWorld, false, 0, cfg.fCheckOverlaps);

        logicIronCoverIV->SetVisAttributes(rojo);
  } else {};
}


    void
    G4TowDetectorConstruction::PlaceDetector(Event& theEvent)
   { 
	
	const Event::Config &cfg = theEvent.GetConfig();
	// loop in detector vector
	for (auto detIt = theEvent.DetectorRange().begin(); detIt != theEvent.DetectorRange().end(); detIt++) {
		auto& currentDet = detIt->second;
		BuildDetector(logicWorld, currentDet, theEvent, cfg.fCheckOverlaps);
	}

   } 

  G4VPhysicalVolume* 
  G4TowDetectorConstruction::Construct() 
   { 

	if (!physWorld) {
		//CreateElements();
		return CreateDetector();
	}
	return physWorld;

   } 



