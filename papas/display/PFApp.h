//  Used for code experimentation
//  (Colin you can ignore for now)
//

#ifndef PFApp_hpp
#define PFApp_hpp

#include "papas/display/Display.h"
//#include "TApplication.h"

#include "TSystem.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/display/GDetector.h"
#include "papas/reconstruction/PFEventDisplay.h"
#include <TApplication.h>

namespace papas {

class Event;
class Detector;
class GDetector;

/// Experimental Wrapper for display to allow graphs to be plotted
class PFApp {
public:
  PFApp();
  void display(const Event& event, const Detector& det);
  // void display2(const Simulator& simulator, const PFEvent& pfEvent, const Particles& particles,
  // const Detector& det);  // under development for exprimental purposes.
  void run(int /*k*/) {
    gPad->Modified();
    gPad->Update();
    gSystem->ProcessEvents();
    // m_theApp.Run(k);
  }
  void terminate() {
    std::cout << "Terminate";
    // m_theApp.Terminate();
  }
  void jpg();

private:
  std::shared_ptr<GDetector> m_gdetector;
  PFEventDisplay* m_display;
};

}  // end namespace papas

#endif /* PFAPP_h */
