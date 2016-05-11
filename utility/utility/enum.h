//
//  enums.h
//  fastsim
//
//  Created by Alice Robson on 12/01/16.
//
//

#ifndef enums_h
#define enums_h
#include "enummanager.h"

namespace fastsim {
  //ENUM_WITH_STRINGS(enumDataType , PARTICLE, CLUSTER, TRACK)
   ENUM_WITH_STRINGS(enumLayer , NONE, HCAL, ECAL, TRACKER)
   ENUM_WITH_STRINGS(enumSubtype , RAW, MERGED, SMEARED)
   ENUM_WITH_STRINGS(enumSource , NONE, SIMULATION, RECONSTRUCTION, OBSERVED)
}


#endif /* Header_h */
