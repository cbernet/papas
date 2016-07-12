
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "TApplication.h"
#include "TROOT.h"
//#include "gtest/gtest.h"

#include "CMS.h"
#include "Cluster.h"
#include "MergedClusterBuilder.h"
#include "PFBlockBuilder.h"
#include "PFEvent.h"
#include "PFEventDisplay.h"
#include "PFReconstructor.h"
#include "PapasManager.h"
#include "Path.h"
#include "Ruler.h"
#include "SimParticle.h"
#include "Simulator.h"
#include "displaypfobjects.h"
#include "pTrack.h"
#include "random.h"

#include "AliceDisplay.h"
#include "Id.h"
#include "Log.h"
#include "StringFormatter.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"

// STL
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

using namespace papas;

Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs);
void processEvent(podio::EventStore& store, PapasManager& papasManager);
int example(int argc, char* argv[]);

int main(int argc, char* argv[]) {

  return example(argc, argv);
  // return longrun(argc,argv);
}

int example(int argc, char* argv[]) {
  // open up Pythia file
  auto reader = podio::ROOTReader();
  //reader.openFile("ee_ZH_Zmumu_Hbb.root");

  
  if( argc != 2) {
    std::cerr<<"Usage: ./mainexe filename"<<std::endl;
    return 1;
  }
  const char* fname = argv[1];
  try {
    reader.openFile(fname);
  }
  catch(std::runtime_error& err) {
    std::cerr<<err.what()<<". Quitting."<<std::endl;
    exit(1);
  }
  
  // read an event
  unsigned int eventNo = 0;
  auto store = podio::EventStore();
  store.setReader(&reader);
  reader.goToEvent(eventNo);

  // Create CMS detector and PapasManager
  CMS CMSDetector;
  auto papasManager = PapasManager(CMSDetector);

  // process event
  processEvent(store, papasManager);
  reader.endOfEvent();

  // outputs
  std::cout << "Generated Stable Particles" << std::endl;
  for (auto& p : papasManager.rawParticles()) {
    std::cout << "  " << p.second << std::endl;
  }
  std::cout << "Reconstucted Particles" << std::endl;
  for (auto& p : papasManager.reconstructedParticles()) {
    std::cout << "  " << p.second << std::endl;
  }

  papasManager.display();
  return EXIT_SUCCESS;
}

int longrun(int argc, char* argv[]) {

  // PDebug::On();  // physics debug output
  auto reader = podio::ROOTReader();
  reader.openFile("/Users/alice/fcc/cpp/papas/papas_cc/ee_ZH_Zmumu_Hbb_50000.root");

  unsigned int eventNo = 0;
  unsigned int nEvents = 1000;

  bool doDisplay = false;
  if (nEvents == 1) doDisplay = true;

  auto store = podio::EventStore();
  store.setReader(&reader);
  reader.goToEvent(eventNo);

  // Create CMS detector and PapasManager
  CMS CMSDetector;
  auto start = std::chrono::steady_clock::now();
  // auto papasManager = PapasManager(CMSDetector) ; //problem with .clear so removed for now

  for (unsigned i = eventNo; i < eventNo + nEvents; ++i) {
    // papasManager.clear();
    auto papasManager = PapasManager(CMSDetector);  // temporary needs fixing
    PDebug::write("Event: {}", i);
    if (i % 1000 == 0) {
      std::cout << "reading event " << i << std::endl;
    }
    if (i == eventNo) start = std::chrono::steady_clock::now();

    processEvent(store, papasManager);
    reader.endOfEvent();

    if (nEvents == 1) {
      std::cout << "Generated Stable Particles" << std::endl;
      for (auto& p : papasManager.rawParticles()) {
        std::cout << "  " << p.second << std::endl;
      }
      std::cout << "Reconstucted Particles" << std::endl;
      for (auto& p : papasManager.reconstructedParticles()) {
        std::cout << "  " << p.second << std::endl;
      }
      if (doDisplay) {
        papasManager.display();
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times = std::chrono::duration<double, std::milli>(diff).count();
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
  std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;
}

// TODO put this in papasmanager perhaps?
Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs) {
  // turns pythia particles into Papas particles and lodges them in the history
  TLorentzVector tlv;
  Particles particles;
  for (const auto& ptc : *ptcs) {
    if (ptc.Core().Status == 1) {  // only stable ones

      auto p4 = ptc.Core().P4;
      tlv.SetXYZM(p4.Px, p4.Py, p4.Pz, p4.Mass);
      int pdgid = abs(ptc.Core().Type);

      if (tlv.Pt() > 1e-5 && (pdgid != 12) && (pdgid != 14) && (pdgid != 16)) {
        IdType id = Id::makeParticleId();
        particles.emplace(id, papas::Particle(id, pdgid, ptc.Core().Charge, tlv, 1));
        PDebug::write("Made Papas{}", particles[id]);
      }
    }
  }
  // sort(particles.begin(), particles.end(),
  //     [](const papas::Particle& lhs, const papas::Particle& rhs) { return lhs.e() > rhs.e(); });
  return std::move(particles);
}

void processEvent(podio::EventStore& store, PapasManager& papasManager) {
  // make a papas particle collection from the next event
  // then run simulate and reconstruct
  const fcc::ParticleCollection* ptcs(nullptr);
  if (store.get("GenParticle", ptcs)) {
    Particles papasparticles = makePapasParticlesFromGeneratedParticles(ptcs);
    papasManager.simulateEvent(std::move(papasparticles));
    papasManager.reconstructEvent();
    store.clear();
  }
}
/*
 //TODO make a gunparticles example out of the following
int nParticles=0;
for (int i = 1; i < nParticles; i++) {
  SimParticle& photon = sim.addParticle(22, 0, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
  PDebug::write("Made {}", photon);
  sim.simulatePhoton(photon);
}

//  Hadrons
nParticles=0;
for (int i = 1; i < nParticles; i++) {
  SimParticle& hadron = sim.addParticle(211, 1, M_PI / 2. + 0.5 * (i + 1), 0, 40. * (i + 1));
  PDebug::write("Made {}", hadron);
  sim.simulateHadron(hadron);
}

//particle Gun
for (int i = 0; i < nParticles; i++) {
  Simulator siml = Simulator{CMSDetector};
  SimParticle& ptc = siml.addGunParticle(211, 1, -1.5, 1.5, 0.1, 10);
  PDebug::write("Made {}", ptc);
  if (!(ptc.charge() && ptc.pt() < 0.2)) siml.simulateHadron(ptc);
  PFEvent pfEvent{siml};  // for python test
  pfEvent.mergeClusters();
  Ids ids = pfEvent.mergedElementIds();
  PFBlockBuilder bBuilder{pfEvent, ids};
  pfEvent.setBlocks(bBuilder);  // for python
  PFReconstructor pfReconstructor{pfEvent};
  pfReconstructor.reconstruct();
}
 for (int i = 0; i < 1000; i++) {
 SimParticle& ptc = sim.addGunParticle(22, -1.5, 1.5, 0.1, 10);
 PDebug::write("Made {}", ptc);
 if (ptc.charge() && ptc.pt()<0.2)
 continue;
 sim.simulatePhoton(ptc);

 }
 for (int i = 0; i < 1000; i++) {
 SimParticle& ptc = sim.addGunParticle(130, -1.5, 1.5, 0.1, 10);
 PDebug::write("Made {}", ptc);
 if (ptc.charge() && ptc.pt()<0.2)
 continue;
 sim.simulateHadron(ptc);

 }*/
