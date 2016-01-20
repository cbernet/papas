//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream> //TODOAJR remove
#include "simulator.h"
#include "CMS.h"
#include "identifier.h"
#include "enums.h"

using fastsim::enumLayer;

Simulator::Simulator(const BaseDetector& d)
   : m_detector(d), m_propStraight(d)
{
   //m_propStraightLine= StraightLinePropagator();
   //m_propHelix;
}

void  Simulator::simulatePhoton(SimParticle& ptc)
{
   Propagate(ptc);
   const Cluster& ecalclust = addECALCluster(ptc); //add the position into this
   smearCluster(ecalclust);
}

void  Simulator::Propagate(SimParticle& ptc)
{
   //TODO make handle helix also
   m_propStraight.propagateOne(ptc, fastsim::enumLayer::ECAL, false);
}

void Simulator::addParticle(const SimParticle& ptc)
{
   long nodeid = ptc.getID();
   m_particles[nodeid] = ptc; //add particle into map of particles
   addNode(nodeid); //add node to graph
}


const Cluster& Simulator::addCluster(SimParticle& ptc, fastsim::enumLayer layer,
                                     double fraction, double csize)
{
   //TODO ENUM?
   std::string clyname = m_detector.getElement(layer)->getVol().InnerName();
   long clusterid = Identifier::makeClusterID(layer, fastsim::enumSubtype::RAW);
   double energy = ptc.getP4().E() * fraction;

   if (csize == 0) { //or could make the defalt value -1?? check Colin
      csize = m_detector.getECAL()->clusterSize(ptc);
   }
   TVector3 pos = ptc.getPathPosition(
                     clyname); //assume path already set in particle

   const Cluster& cluster = makeCluster(clusterid, energy, pos, csize);
   addNode(cluster.getID(), ptc.getID());
   return std::move(cluster); //check this defaults OK
}


const Cluster& Simulator::addECALCluster(SimParticle& ptc, double fraction,
      double csize)
{
   return addCluster(ptc, fastsim::enumLayer::ECAL, fraction, csize);
}

const Cluster& Simulator::addHCALCluster(SimParticle& ptc, double fraction,
      double csize)
{
   return addCluster(ptc, fastsim::enumLayer::HCAL, fraction, csize);
}

const Cluster& Simulator::makeCluster(long clusterid, double energy,
                                      TVector3& pos, double csize)
{
   ///create cluster and add into the map of clusters
   m_clusters.emplace(clusterid, std::move(Cluster{energy, pos, csize, clusterid}));
   return m_clusters[clusterid];
}


const Cluster& Simulator::smearCluster(const Cluster& parent)
{

   fastsim::enumLayer layer = Identifier::getLayer(parent.getID());
   long newclusterid = Identifier::makeClusterID(layer,
                       fastsim::enumSubtype::SMEARED);

   //make a new cluster and add into the clusterset
   double energy = parent.getEnergy() * 0.8;// TODO random.gauss(1, eres);
   //double eres=getElem(layer)->energyResolution(energy);
   m_clusters[newclusterid] = {energy, parent.getPosition(), parent.getSize(), newclusterid};
   //m_clusters.emplace(std::move(new Cluster{energy, cl.getPosition(),cl.getSize(),newclusterid}));

   //create a new node and connect into the graph
   addNode(newclusterid, parent.getID());
   return m_clusters[newclusterid];
}


void Simulator::addNode(long newid, const long parentid)
{

   //add the new node into the set of all nodes
   m_nodes[newid] = {newid};

   if (parentid) {
      simNode parent = m_nodes[parentid];
      simNode child = m_nodes[newid];
      child.addParent(&parent);
      parent.addChild(&child);
   }
}

std::shared_ptr<const DetectorElement> Simulator::getElem(
   fastsim::enumLayer layer)
{
   return m_detector.getElement(layer);
}
/*
const DetectorElement& Simulator::getElem(fastsim::enumLayer layer)
{
   return m_detector.getElement(layer);
}*/





/*

 Cluster&& Simulator::makeCluster(Particle& ptc, fastsim::enumLayer layer,
 double fraction, double size)
 {
 //adds a cluster in a given detector, with a given fraction of
 //the particle energy.'''
 std::shared_ptr<const DetectorElement> detectorElem = m_detector.getElement(
 layer)   ;

 //propagator(ptc).propagate_one(ptc,
 //detectorElem.volume.inner)

 //AJRTODO this comes next
 //m_propStraightLine.propagate_one(ptc,detectorElem->volume.inner);

 // last field does not appear to be used                                   m_detector.elements['field'].magnitude )
 if (size == 0.) {
 //    size = detectorElem->cluster_size(ptc);
 }
 std::string cylname = detectorElem->getVolume()->InnerName();
 // short term simplification
 TVector3* tvec = new TVector3(.5, .5,
 .5); //AJRTODO must make cluster remove the tvec
 Cluster cluster(0.8,
 *tvec,
 size,
 cylname, ptc);

 PTNode* = graph.addcluster(cluster*, "cluster","smeared","ECAL");



 ///Cluster cluster(ptc.p4().E()*fraction,
 //                 ptc.points[cylname],
 size,
 cylname, ptc);



 //ptc.clusters[cylname] = cluster;
 return std::move(cluster);
 }

 */






