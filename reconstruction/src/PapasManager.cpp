//
//  PapasManager.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#include "PapasManager.h"
#include "AliceDisplay.h"
#include "Id.h"
#include "PFBlockBuilder.h"

#include "PDebug.h"
#include "PFReconstructor.h"

namespace papas {

PapasManager::PapasManager(Detector& detector)
    : m_detector(detector), m_simulator(detector, m_history), m_pfEvent(m_simulator) {}

void PapasManager::simulateEvent(Particles&& particles) {
  m_particles = std::move(particles);

  // order the particles according to id
  std::vector<Id::Type> ids;

  ids.reserve(m_particles.size());
  for (auto kv : m_particles) {
    ids.push_back(kv.first);
  }

  #if WITHSORT
  std::sort(ids.begin(), ids.end(),
            [&](IdType i, IdType j) { return (m_particles.at(i).e() > m_particles.at(j).e()); });
#endif
  for (const auto& id : ids) {
    // std::cout << id<< m_particles.at(id)<<std::endl;
    m_history.emplace(id, std::move(PFNode(id)));  ///< insert the raw particle ids into the history
    m_simulator.simulateParticle(m_particles.at(id), id);
  }
  m_pfEvent.mergeClusters();
}

void PapasManager::reconstructEvent() {

  // get the ids of smeared and merged tracks and clusters that are to be reconstruced
  Ids ids = m_pfEvent.mergedElementIds();

  // create the blocks of linked ids
  auto bBuilder = PFBlockBuilder(ids, m_pfEvent);

  // do the reconstruction of the blocks
  auto pfReconstructor = PFReconstructor(m_pfEvent);
  pfReconstructor.reconstruct(bBuilder.blocks());
  m_pfEvent.setReconstructedParticles(std::move(pfReconstructor.particles()));
}

void PapasManager::clear() {
  m_pfEvent.clear();
  m_history.clear();
  m_particles.clear();
  m_simulator.clear();
  Id::reset();
}

void PapasManager::display(bool jpg) {

  //PFApp myApp{}; // I think this should turn into a PapasManager member
  
  m_app.display(m_simulator, m_pfEvent, m_particles, m_detector);
  if (jpg)
    m_app.jpg();
 
}
  void PapasManager::show() {
    
    //move to PFApp;
    
    gSystem->ProcessEvents();
    
  }


}  // end namespace papas