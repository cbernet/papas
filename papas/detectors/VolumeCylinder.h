//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef VOLUMECYLINDER_H
#define VOLUMECYLINDER_H

#include "TVector3.h"
#include "papas/datatypes/Definitions.h"
#include "papas/detectors/SurfaceCylinder.h"

namespace papas {

/**
  @brief Defines inner and outer cyclinders of a detector element
 */
class VolumeCylinder {
public:
  /** Constructor
   *
   * @param[in] layer  Enum describing which layer the cyclinder corresponds to
   * @param[in] outerrad Outer cylinder radius
   * @param[in] outerz Outer cylinder maximum z value (-z, z)
   * @param[in] innerrad Inner cylinder radius. Default value is 0.
   * @param[in] innerz Inner cylinder maximum z value (-z, z). Default value is 0.
   */
  VolumeCylinder(papas::Layer layer, double outerrad, double outerz, double innerrad = 0., double innerz = 0.);
  ~VolumeCylinder();
  /**  @brief checks if a point is inside the volumes
   * @param[in] point position (3d) to check
   */
  bool contains(const TVector3& point) const;
  const SurfaceCylinder& inner() const { return m_inner; }        ///< inner cyclinder of volume
  const SurfaceCylinder& outer() const { return m_outer; }        ///< outer cyclinder of volume
  papas::Position innerLayer() const { return m_inner.layer(); }  ///< enum describing layer of inner cyclinder

private:
  SurfaceCylinder m_outer; ///< outer cyclinder of volume
  SurfaceCylinder m_inner; ///< inner cyclinder of volume
};
}  // end namespace papas
#endif
