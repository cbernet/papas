//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMS_H
#define CMS_H

#include <vector>
#include <string>
#include "detector.h"

class Track;

/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */
class Particle;
class Cluster;
class Material;
class VolumeCylinder;

///ECAL Class (CMS implementation)
/** This ECAL is implmented specifically for CMS

      It is intended to be replaced by users to match the required detector characteristics

 */
class ECAL: public BaseECAL {
public:
   //AJRTODO check whether both cosntructors are needed
   ///constructor - allows the Material and Volume to be created on the fly
   ECAL(const std::string& name, const VolumeCylinder&& volume,
        const Material&& material, double eta_crack, double emin,
        const std::vector<double>&&
        eres);
   
   ///constructor - requires the Material and Volume to be already in existance
   ECAL(const std::string& name, const VolumeCylinder& volume,
        const Material& material , double eta_crack, double emin,
        const std::vector<double>&
        eres);
   
   double cluster_size(const Particle& ptc) const override;
   bool acceptance(const Cluster& cluster)  const override;
   double energy_resolution(double energy)  const override;
   //TODOAJR space_resolution(self, ptc):
private:
   double m_eta_crack;
   double m_emin;
   std::vector<double> m_eres;
};


class HCAL: public BaseHCAL {
public:
   //AJRTODO NB BaseECAl and BaseHcal are vert similar at the moment
   
   ///constructor - allows the Material and Volume to be created on the fly
   HCAL(const std::string& name, const VolumeCylinder&& volume,
        const Material&& material,
        const std::vector<double>&&
        eres);
   ///constructor - requires the Material and Volume to be already in existance
   HCAL(const std::string& name, const VolumeCylinder& volume,
        const Material& material ,
        const std::vector<double>&
        eres);
   
   double cluster_size(const Particle& ptc) const override;
   bool acceptance(const Cluster& cluster)  const override;
   double energy_resolution(double energy)  const override;
   //TODOAJR space_resolution(self, ptc):
private:
   std::vector<double> m_eres;
};

// Below here is TODO

class Tracker: public DetectorElement {
//TODO acceptance and resolution depend on the particle type
public:
   Tracker();
   bool acceptance(const Track& track);
   double pt_resolution(const Track& track);
protected:

private:
   //VolumeCylinder m_temp_volume;
   //Material m_temp_mat ;
   std::vector<double> m_eres;
};



class DField: public DetectorElement {
public:
   DField(double magnitude);
private:
   double m_magnitude;
};


class CMS: public BaseDetector {
public:
   CMS();
private:
   //std::map<std::string,DetectorElement*> m_elements;
};


#endif

/*AJRTODO def acceptance(self, cluster):
    energy = cluster.energy
    eta = abs(cluster.position.Eta())
    if eta < 3. :
        return energy>4.
    elif eta < 5.:
        return energy>7.
    else:
        return False

def space_resolution(self, ptc):
    pass



class Tracker(DetectorElement):
#TODO acceptance and resolution depend on the particle type

def __init__(self):
    volume = VolumeCylinder('tracker', 1.29, 1.99)
    mat = material.void
    super(Tracker, self).__init__('tracker', volume,  mat)

def acceptance(self, track):
    # return False
    pt = track.pt
    eta = abs(track.p3.Eta())
    if eta < 2.5 and pt>0.5:
        return random.uniform(0,1)<1. # CMS without tracker material effects
    else:
        return False

def pt_resolution(self, track):
    # TODO: depends on the field
    pt = track.pt
    return 5e-3




cms = CMS()
*/
