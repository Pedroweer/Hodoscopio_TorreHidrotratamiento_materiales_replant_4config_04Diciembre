#ifndef G4TowSimulator_h
#define G4TowSimulator_h 1

// C++ libraries
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

// Framework libraries
#include "Particle.h"
#include "Event.h"
#include "Detector.h"
//#include "Detector_Ideal.hh"//agreguè esto

class G4RunManager;

class G4TowSimulator 
{
  public:
  
    G4TowSimulator();
    virtual ~G4TowSimulator() {;}

    // main methods of the application
    void Initialize(Event& evt, std::string fileName);
    bool RunSimulation(Event& evt);
    void WriteEventInfo(Event& evt);

    // static members 
    static Particle currentParticle;
    static G4TowSimulator* fG4TowSimulator;

    // name of configuration file
    std::string fCfgFile;

  private:

		// flags for configuration (see .json file)
    std::string fInputFile;
    std::string fOutputFile;
    std::string fDetectorList;
    std::string fDetectorProperties;
    SimData::SimulationMode fSimulationMode;
    SimData::InjectionMode fInjectionMode;

    bool fGeoVisOn = true;
    bool fTrajVisOn = false;
    int fVerbosity = 1;
    std::string fRenderFile = "VRML2FILE";
    std::string fPhysicsName = "QGSP_BERT_HP";

  	friend class G4TowDetectorConstructor;
  	friend class G4TowPrimaryGenerator;
};


#endif
