//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  SIMULATOR_H
#define  SIMULATOR_H
#include <string>
#include <unordered_map>
#include <map>
#include "enums.h"
#include "detector.h"
#include "datatypes.h"
#include "directedacyclicgraph.h"

#include "Identifier.h"
#include "propagator.h"

class Particle;

//TODO home for typedefs
typedef DAG::Node<long> SimNode;
typedef std::unordered_map<long, SimNode> Nodes; ///TODO rename to Nodes
typedef std::unordered_map<long, Cluster> Clusters;
typedef std::unordered_map<long, Cluster> SmearedECALClusters;
typedef std::unordered_map<long, Cluster> SmearedHCALClusters;
typedef std::unordered_map<long, Cluster> ECALClusters;
typedef std::unordered_map<long, Cluster> HCALClusters;
typedef std::unordered_map<long, Track> Tracks;
typedef std::unordered_map<long, SimParticle> Particles;
typedef std::vector<long> IDs;

class Simulator {
  
public:
  Simulator(const BaseDetector&);  //AJRTODO add logger
  void simulate();
  void simulatePhoton(SimParticle& ptc);
  void simulateHadron(SimParticle& ptc);
  
  SimParticle& addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex= TVector3(0., 0., 0.));
  //const Clusters& clusters() const {return m_clusters;} ;
  const Cluster& cluster(long clusterID) const;
  
  
  Clusters ECALClusters() const {return m_ECALClusters;} ///<Copy of ECAL clusters
  Clusters HCALClusters() const {return m_HCALClusters;}
  Clusters smearedECALClusters() const {return m_smearedECALClusters;}
  Clusters smearedHCALClusters() const {return m_smearedHCALClusters;}
  
  Tracks   tracks() const {return m_tracks;}
  Tracks   smearedTracks() const {return m_smearedTracks;}
  Nodes& historyNodes() { return m_nodes; }
  //const Tracks& tracks() const {return m_tracks;}
  const Particles& particles() const {return m_particles;}
  
  //const Clusters& smearedClusters() const  ;
  
  void testing();
  IDs linkedECALSmearedClusterIDs(long nodeid);
  IDs linkedParticleIDs (long nodeid);
  IDs parentParticleIDs (long nodeid);
  //Clusterset ExportSimulatedClusters() const;
  //Particleset ExportSimulatedParticles() const;
  
private:
  void propagate(SimParticle& ptc,const SurfaceCylinder &); //more args needed
  
  
  IDs linkedRawTrackIDs(long nodeid);
  IDs linkedSmearedTrackIDs(long nodeid);
  
  IDs linkedIDs(long nodeid);
  
  long makeClusterID(fastsim::enumLayer layer  , fastsim::enumSubtype subtype);
  long makeParticleID(fastsim::enumSource source);
  
  long addECALCluster(SimParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addHCALCluster(SimParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addSmearedCluster(long parentClusterID);
  
  Cluster makeCluster(SimParticle& ptc, long parentid,fastsim::enumLayer layer,
                      double fraction = 1., double csize = 0.);
  Cluster makeSmearedCluster(long parentClusterID);
  
  const Track& addTrack(SimParticle& ptc);
  const Track& makeTrack(long trackid, TVector3 pos, double charge, std::shared_ptr<Path> path);
  
  Track makeSmearedTrack(long smearedTrackID, const Track& track) ;
  long addSmearedTrack( const Track& track, bool accept = false);
  
  void addNode(const long newid, const long parentid = 0);
  std::shared_ptr<const DetectorElement> elem(fastsim::enumLayer layer);
  
  IDs getMatchingIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer, fastsim::enumSubtype type, fastsim::enumSource source);
  IDs getMatchingParentIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer, fastsim::enumSubtype type, fastsim::enumSource source);
  
  //Clusters m_clusters;    /// all clusters
  Clusters m_ECALClusters;
  Clusters m_HCALClusters;
  Clusters m_smearedECALClusters;
  Clusters m_smearedHCALClusters;
  Tracks   m_tracks;        /// pre smeared tracks
  Tracks   m_smearedTracks; /// smeared tracks
  Particles m_particles;    /// all particles
  
  //this will have a keyed entry for everything that has
  //been simulated and so acts as a lookup table
  Nodes m_nodes;
  
  const BaseDetector& m_detector;
  StraightLinePropagator m_propStraight;
  HelixPropagator m_propHelix;
};

#endif

