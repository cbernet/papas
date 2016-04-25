//
//  PFReconstructor.hpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#ifndef PFReconstructor_h
#define PFReconstructor_h

#include <unordered_map>
#include "TVector3.h"
#include "Identifier.h"
#include "directedacyclicgraph.h"

class PFEvent;
class PFBlock;
class SimParticle;
class Cluster;
class Track;
class TVector3;

//TODO simparticles is not right
//TODO store typedefs centrally
typedef long longID;
typedef std::unordered_map<longID, SimParticle> Particles;
typedef std::vector<longID> IDs;
//typedef std::unordered_map<longID, Cluster> Clusters;
typedef DAG::Node<longID> PFNode;
typedef std::unordered_map<longID, PFNode> Nodes;
typedef std::unordered_map<longID, PFBlock> Blocks;
//typedef std::unordered_map<longID, Track> Tracks;

class PFReconstructor {
  
public:
  PFReconstructor(PFEvent& pfEvent);// TODO  detector, logger):
  void reconstruct();
  
private:
  Blocks simplifyBlock(PFBlock& block);
  void reconstructBlock(const PFBlock& block);
  void reconstructHcal(const PFBlock& block, longID hcalID);
  SimParticle reconstructTrack(const Track& track);
  SimParticle reconstructCluster(const Cluster& cluster, fastsim::enumLayer layer, double energy = -1,
                                 TVector3 vertex= TVector3());
  void insertParticle(const PFBlock& block, SimParticle&& particle);
  double neutralHadronEnergyResolution(const Cluster& hcal) const;
  double nsigmaHcal(const Cluster& cluster) const;
  
  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;
  Blocks m_blocks; //TODO should this be a reference - nb blocks will get changed (disactivated during splitting)
  Particles m_particles; //owns the particles it makes
  bool m_hasHistory;
  IDs m_unused;
  std::unordered_map<longID, bool> m_locked;
  
};

#endif /* PFReconstructor_h */
