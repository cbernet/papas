#ifndef PapasEvent_h
#define PapasEvent_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Identifier.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"

namespace papas {
/**
 *  @brief The PapasEvent stores pointers to collections of Clusters, Tracks, Blocks, Particles in its
 *  internal ClusterCollections, Track Collections etc. It also has a vector
 *  of one of more histories which describe the historical connections between the objects in the PapasEvent.
 *
 *  The PapasEvent is a lightweight obejct that can be used from Papas Standalone or from
 *  Gaudi modules.
 *    The collections stored in the PapasEvent are unordered_maps of unordered_maps eg an unordered map of Clusters.
 *    The collections are indexed by the typeAndSubtype of the identifiers of each item 
 *    (eg of each Cluster in Clusters)
 *       Therefore each collection to be stored must contain only one typeAndSubtype
 *       Examples of typeAndSubtype are:
 *       "es" ecal-smeared
 *       "ps" particle-simulated
 *       etc see also Identifier class
 *      The types are
 *        e = ecal
 *        h = hcal
 *        p = particle
 *        t = track
 *        b = block (particle flow block)
 *      The subtypes may include
 *        t = true
 *        s = smeared or simulated
 *        g = generated
 *        r = reconstructed
 *        m = merged
 Usage example:
 @code
  m_papasEvent.addCollection(ecalClusters);
  m_papasEvent.addCollection(hcalClusters);
  ...
  const Cluster& cluster1 = m_papasEvent.cluster(id1);
 @endcode
 *
 * @author  Alice Robson
 * @date    2016-12-01
 */
class PapasEvent {
public:
  /// @brief  Constructor
  PapasEvent();
  /**
   *   @brief  adds a pointer to a Clusters collection (unordered map) into the PapasEvent
   *   @param[in]  clusters: unordered map of Clusters, all of which have the same Identifier typeAndSubtype
   *               typeAndSubtype will be used as the map index, eg "em" for ecals-merged.
   */
  void addCollection(const Clusters& clusters);
  /**
   *   @brief  adds a pointer to a Tracks collection (unordered map) into the PapasEvent
   *   @param[in]  tracks: unordered map of Tracks, all of which have the same Identifier typeAndSubtype
   *               typeAndSubtype will be used as the map index, eg "tt" for track-true.
   */
  void addCollection(const Tracks& tracks);
  /**
   *   @brief  adds a pointer to a Blocks collection (unordered map) into the PapasEvent
   *   @param[in]  blocks: unordered map of Blocks, all of which have the same Identifier typeAndSubtype
   *               typeAndSubtype will be used as the map index, eg "br" for blocks-raw.
   */
  void addCollection(const Blocks& blocks);
  /**
   *   @brief  adds a pointer to a  SimParticles collection (unordered map) into the PapasEvent
   *   @param[in]  blocks: unordered map of SimParticles, all of which have the same Identifier typeAndSubtype
   *               typeAndSubtype will be used as the map index, eg "pr" for particles-reconstructed.
   */
  void addCollection(const SimParticles& particles);
  /**
   *   @brief  adds a pointer to a hsitory Nodes collection (unordered map) into the PapasEvent
   *   @param[in]  history: unordered map of Nodes, all of which have the same Identifier typeAndSubtype.
   *               typeAndSubtype will be used as the map index, eg "pr" for particles-reconstructed.
   */
  void addHistory(const Nodes& history);
  /**
   *   @brief  returns list of historys as a const reference
   *
   */
  const ListNodes& history() const { return m_historys; };
  /**
   *   @brief  returns true if a collection with the same typeAndSubtype as the identifier is found
   *   @param[in]  id: The identifier of an object
   */
  bool hasCollection(IdType id) const;
  /**
   *   @brief  returns true if a collection with matching type and subtype  is found
   *   @param[in]  type: The type of an object eg Identifier::kEcalCluster
   *   @param[in]  subtype: The subtype of an object eg 'm' for merged
   */
  bool hasCollection(const Identifier::ItemType type, const Identifier::SubType subtype) const;
  /**
   *   @brief  returns true if an object matching the Identifier is found in the PapasEvent collections
   *   @param[in]  id: the Identifier of an object
   */
  bool hasObject(IdType id) const;
  /**
   *   @brief  returns a Clusters collection with typeAndSubtype that match the identifier
   *   @param[in]  id: the Identifier of an object
   */
  const Clusters& clusters(IdType id) const;
  /**
   *   @brief  returns a Clusters collection matching type and subtype
   *   @param[in]  type: The type of an object eg Identifier::kEcalCluster
   *   @param[in]  subtype: The subtype of an object eg 'm' for merged
   */
  const Clusters& clusters(const Identifier::ItemType type, const Identifier::SubType subtype) const;
  /**
   *   @brief  returns a Clusters collection matching type and subtype as a string
   *   @param[in]  typeAndSubtype: The type and subtype of an object as a string eg "em" for ecals-merged.
   */
  const Clusters& clusters(const std::string& typeAndSubtype) const;
  /**
   *   @brief  returns a Cluster with the required id
   *   @param[in]  id: the Identifier of an object
   */
  const Cluster& cluster(IdType id) const { return clusters(id).at(id); };

  // TODO doxygen as above
  const Tracks& tracks(const Identifier::SubType subtype) const { return *m_tracksCollection.at(subtype); };
  const Tracks& tracks(IdType id) const { return tracks(Identifier::subtype(id)); };
  const Track& track(IdType id) const { return tracks(id).at(id); };

  const SimParticles& particles(const Identifier::SubType subtype) const { return *m_particlesCollection.at(subtype); };
  const SimParticles& particles(IdType id) const { return particles(Identifier::subtype(id)); };
  const SimParticle& particle(IdType id) const { return particles(id).at(id); };

  const Blocks& blocks(const Identifier::SubType subtype) const { return *m_blocksCollection.at(subtype); };
  const Blocks& blocks(IdType id) const { return blocks(Identifier::subtype(id)); };
  const PFBlock& block(IdType id) const { return blocks(id).at(id); };
  ;

  void clear();

private:
  /**
   *   @brief  templated class method used by the AddCollection methods to check that typeAndSubype match and that
   *           this collection type does not already exist. It then adds the collection into the PapasEvent.
   */
  template <class T>
  void
  addCollectionInternal(const std::unordered_map<IdType, T>& collection,
                        std::unordered_map<Identifier::SubType, const std::unordered_map<IdType, T>*>& collections);
  /// Unordered map of pointers to unordered map of (concrete) Ecal Clusters
  CollectionClusters m_ecalClustersCollection;
  /// Unordered map of pointers to unordered map of (concrete) Hcal Clusters
  CollectionClusters m_hcalClustersCollection;
  /// Unordered map of pointers to unordered map of (concrete) Tracks
  CollectionTracks m_tracksCollection;
  /// Unordered map of pointers to unordered map of (concrete) SimParticles
  CollectionParticles m_particlesCollection;
  /// Unordered map of pointers to Blocks
  CollectionBlocks m_blocksCollection;
  /// Vector of History objects.
  ListNodes m_historys;
};

template <class T>
void PapasEvent::addCollectionInternal(
    const std::unordered_map<IdType, T>& collection,
    std::unordered_map<Identifier::SubType, const std::unordered_map<IdType, T>*>& collections) {
  IdType firstId = 0;
  for (const auto& it : collection) {
    if (!firstId) {
      firstId = it.first;
      if (hasCollection(firstId)) throw "Collection already exists";
    }
    if (Identifier::typeAndSubtype(it.first) != Identifier::typeAndSubtype(firstId)) {
      throw "more than one typeandSubtype found in collection";
    }
  }
  collections.emplace(Identifier::subtype(firstId), &collection);
};
}

#endif /* PapasEvent_h */
