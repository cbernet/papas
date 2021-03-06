#include "papas/graphtools/EventRuler.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Event.h"
#include "papas/datatypes/Track.h"
#include "papas/graphtools/Distance.h"

namespace papas {

EventRuler::EventRuler(const Event& papasevent) : m_ruler(), m_event(papasevent) {}

Distance EventRuler::distance(IdType id1, IdType id2) const {
  //figure out the object types and then call ClusterCluster or ClusterTrack distance measures
  if (Identifier::isCluster(id1) && Identifier::isCluster(id2))
    if (Identifier::itemType(id1) == Identifier::itemType(id2))
      return std::move(clusterClusterDistance(id1, id2));
    else  // hcal ecal not linked
      return Distance();
  else if (Identifier::isTrack(id2) && Identifier::isCluster(id1))
    return std::move(clusterTrackDistance(id1, id2));
  else if (Identifier::isTrack(id1) && Identifier::isCluster(id2))
    return std::move(clusterTrackDistance(id2, id1));
  else if (Identifier::isTrack(id1) && Identifier::isTrack(id2))
    return std::move(Distance());
  std::cout << Identifier::itemType(id1) << ":" << Identifier::itemType(id2) << std::endl;
  throw "Distance between ids could not be computed";
  return std::move(Distance());
}

Distance EventRuler::clusterClusterDistance(IdType id1, IdType id2) const {
  //find the objects and use ruler to find distance
  const Cluster& cluster1 = m_event.cluster(id1);
  const Cluster& cluster2 = m_event.cluster(id2);
  return m_ruler.clusterClusterDistance(cluster1, cluster2);
}

Distance EventRuler::clusterTrackDistance(IdType clustId, IdType trackId) const {
  //find the objects and use ruler to find distance
  const Cluster& cluster = m_event.cluster(clustId);
  const Track& track = m_event.track(trackId);
  return m_ruler.clusterTrackDistance(cluster, track);
}

}  // end namespace papas
