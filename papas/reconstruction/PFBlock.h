#ifndef RECONSTRUCTION_PFBLOCK_H
#define RECONSTRUCTION_PFBLOCK_H

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Identifier.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/graphtools/Edge.h"
#include <iostream>
#include <string>

#include <iostream>
#include <string>

namespace papas {

/** A Particle Flow Block (PFBlock) stores a set of element ids that are connected to each other
 together with the edge data (distances/linkage) for each possible edge combination

 class attributes:

 IdType m_uniqueid : the block's unique id generated from Id class
 Ids m_elementIds : list of uniqueids of its elements

 Edges m_edges : Unordered map of all the edge combinations in the block
          use  edge(id1,id2) to find an edge
 static int tempBlockCount: sequential numbering of blocks (useful for debugging/tracing etc)

 Usage:
 block = PFBlock(element_ids,  edges, 'r')
 os << block;
 */

class PFBlock {

public:
  /** Constructor
   @param[in] element_ids vector of uniqueids of the elements to go in this block [id1,id2,...]
   @param[inout] edges is an unordered map of edges, it must contain at least all needed edges. It is not a
   problem if it contains additional edges as only the ones needed will be extracted. Note that edges that are
   extracted will be removed from the Edges object and will become owned by the PFBlock
   @param[in] subtype The subtype for the Identifier of the block eg 's' for split block
   */
  PFBlock(const Ids& elementIds, Edges& edges, unsigned int index,
          char subtype = 'u');  // relevant parts of edges will be removed and become owned by PFBlock
  PFBlock(PFBlock&& pfblock) = default;  // allow move
  const Ids& elementIds() const { return m_elementIds; }  ///< returns vector of all ids in the block
  const Edge& findEdge(Edge::EdgeKey key) const; ///< return Edge corresponding to Edge key
  /**
  Returns list of all edges of a given edge type that are connected to a given id.
  The list is sorted in order of increasing egde distances
  @param[in] uniqueid : is the id of item of interest
  @param[in] edgetype : is an optional type of edge. If specified then only links of the given edgetype will be returned
  @return vector of EdgeKeys of the selected linked edges
 */
  std::list<Edge::EdgeKey> linkedEdgeKeys(IdType uniqueid, Edge::EdgeType matchtype = Edge::EdgeType::kUnknown) const;

  /**
  Returns list of all linked ids of a given edge type that are connected to a given id
   @param[in] uniqueId : is the id of item of interest
   @param[in] edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
   @return vector of ids that are linked to the uniqueid
  */
  Ids linkedIds(IdType uniqueId, Edge::EdgeType edgetype = Edge::EdgeType::kUnknown) const;

  std::string shortName() const;  ///< Short descriptor of block such as E3H1T2 (three Ecals, 1 Hcal, 2 tracks)
  int countEcal() const;          ///< Counts how many ecal cluster ids are in the block
  int countHcal() const;          ///< Counts how many hcal cluster ids are in the block
  int countTracks() const;        ///< Counts how many tracks are in the block
  int size() const { return m_elementIds.size(); } ///< length of the element_unqiueids
  IdType id() const { return m_uniqueId; };  ///<Unique ID of the block
  const Edges& edges() const { return m_edges; } ///<Unordered map of all the edges in a block
  std::string info() const; ///< printable one line summary of a Block
  std::string elementsString() const; ///< String listing all elements in a Block
  std::string edgeMatrixString() const; ///< String representation of matrix of edges in a block
  const Edge& edge(IdType id1, IdType id2) const; ///<locate an edge corresponding to two ids

private:
  PFBlock(PFBlock& pfblock) = default;  // avoid copying of blocks
  PFBlock(const PFBlock& pfblock) = default;
  PFBlock& operator=(const PFBlock&) = default;

  IdType m_uniqueId;          ///<  uniqueid for this block
  Ids m_elementIds;           ///<  ids of elements in this block ordered by type and decreasing energy
  Edges m_edges;              ///< all the edges for elements in this block
  static int tempBlockCount;  ///< sequential numbering of blocks, not essential but helpful for debugging
};

std::ostream& operator<<(std::ostream& os, const PFBlock& block);

}  // end namespace papas
#endif /* PFBlock_h */
