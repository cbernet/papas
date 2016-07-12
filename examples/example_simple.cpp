
//
//  example_simple.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "CMS.h"
#include "PFReconstructor.h"
#include "PapasManager.h"
#include "Simulator.h"
#include "PythiaConnector.h"
#include "AliceDisplay.h"
#include "Log.h"

// STL
#include <chrono>
#include <iostream>



int main(int argc, char* argv[]) {
  
  randomgen::setEngineSeed(0xdeadbeef); //make results reproduceable
  
  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  //open the Pythia file fname
  try {
    auto pythiaConnector = PythiaConnector(fname);
    
    // Create CMS detector and PapasManager
    papas::CMS CMSDetector;
    papas::PapasManager papasManager{CMSDetector};
    
    //read and process a single event
    unsigned int eventNo = 0;
    pythiaConnector.processEvent(eventNo, papasManager);
    
    //write out the reconstructed particles to a root file
    pythiaConnector.writeParticlesROOT("simpleeg.root", papasManager.reconstructedParticles());
    
    // write inputs and outputs to screen
    std::cout << "Generated Stable Particles" << std::endl;
    for ( const auto& p : papasManager.rawParticles()) {
      std::cout << "  " << p.second << std::endl;
    }
    std::cout << "Reconstructed Particles" << std::endl;
    for (const auto& p : papasManager.reconstructedParticles()) {
      std::cout << "  " << p.second << std::endl;
    }
    
    //produce papas display
    papasManager.display();
    return EXIT_SUCCESS;
  }
  catch (std::runtime_error& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  }
  
}
