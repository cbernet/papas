//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PARTICLE_H
#define PARTICLE_H

#include "TLorentzVector.h"
#include "TVector3.h"
#include "papas/datatypes/Definitions.h"

#include "papas/utility/PDebug.h"

namespace papas {
/// Main Particle class
///
/// Contains 4-momentum vector, particle id and accessor functions
/// Intended to be used as the base for derived Particle classes

class Particle {
public:
  Particle(); ///<Constructor
  /** Constructor
   @param[in] pdgid : particle type
   @param[in] charge : charge
   @param[in] tlv : 4-momentum, px, py, pz, E
   @param[in] status : status code, e.g. from generator. 1:stable
   @param[in] startVector : start vertex (3d point)
   @param[in] endVertex : end vertex (3d point)
   */
  Particle(int pdgid, double charge, const TLorentzVector& tlv, double status = 1,
           const TVector3& startVector = TVector3(0, 0, 0),
           const TVector3& endVertex = TVector3(0, 0, 0));
  /** Constructor
   @param[in] pdgid : particle type
   @param[in] charge : charge
   */
  Particle(int pdgid, double charge);
  const TLorentzVector& p4() const { return m_tlv; }   ///< 4-momentum, px, py, pz, E
  TVector3 p3() const { return m_tlv.Vect(); }  ///< 3-momentum px, py, pz,
  double e() const { return m_tlv.E(); }               ///<Energy
  double pt() const { return m_tlv.Pt(); }             ///<transverse momentum (magnitude of p3 in transverse plane)
  double theta() const { return M_PI / 2 - m_tlv.Theta(); }  ///< angle w/r to transverse plane
  
  /**pseudo-rapidity (-ln(tan self._tlv.Theta()/2)).
  theta = 0 -> eta = +inf
  theta = pi/2 -> 0
  theta = pi -> eta = -inf*/
  double eta() const { return m_tlv.Eta(); }                 
  double phi() const { return m_tlv.Phi(); }                 ///<azymuthal angle (from x axis, in the transverse plane)
  double mass() const { return m_tlv.M(); }                  ///< mass
  int pdgId() const { return m_pdgId; }                      ///< particle type (an integer value)
  double charge() const { return m_charge; }                 ///< particle charge
  bool status() const { return m_status; }                   ///<status code, e.g. from generator. 1:stable.
  const TVector3& startVertex() const { return m_startVertex; }  ///<start vertex (3d point)
  const TVector3& endVertex() const { return m_endVertex; }      ///<end vertex (3d point)
  std::string info() const;                                      ///< text descriptor of the particle
private:
  TLorentzVector m_tlv;    ///<4-momentum, px, py, pz, E
  int m_pdgId;             ///<particle type
  double m_charge;         ///<particle charge
  double m_status;         ///< status code, e.g. from generator. 1:stable
  TVector3 m_startVertex;  ///<start vertex (3d point)
  TVector3 m_endVertex;    ///<end vertex (3d point)
};

std::ostream& operator<<(std::ostream& os, const Particle& particle);

}  // end namespace papas

#endif