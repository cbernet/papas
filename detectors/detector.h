//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DETECTOR_H
#define DETECTOR_H


#include <string>
//#include <unordered_map>
#include <map>
#include <list>
//#include <memory>

//#include "enum.h"


#include "material.h"
#include "geometry.h"
class Particle;
class Material;
class VolumeCylinder;
class Cluster;
class Track;
///DetectorElement
/**
 Class base for ECAL, HCAL, Track and field
*/


//TODO rename this t DEtector Ekenebt
class DetectorElement {
public:
   DetectorElement(fastsim::enumLayer layer, const VolumeCylinder&& volume,
                       const Material&&
                       material); ///< allows the Material and Volume to be created on the fly

   DetectorElement(fastsim::enumLayer layer, const VolumeCylinder&  volume,
                       const Material&
                       material); ///< requires the Material and Volume to be already in existance
   const VolumeCylinder& getVol() const  {return m_volume; }///< return the volume cyclinder
   fastsim::enumLayer getLayer() const {return m_layer;};
protected:
   VolumeCylinder m_volume;
   Material m_material;
   fastsim::enumLayer m_layer;
private:
};

/**
 Holds virtual functions that the user must define when creating their own ECAL class
*/
// get rid of this
class ECAL :public DetectorElement {
public:
   using DetectorElement::DetectorElement;
   virtual double energyResolution(double energy) const = 0;
   virtual double clusterSize(const Particle& ptc) const = 0 ;
   virtual bool   acceptance(const Cluster& ptc) const = 0;
   //virtual bool   acceptance(const Track& track) const= 0;
   //virtual double space_resolution(Particle* ptc)=0;
private:

};

class HCAL :public DetectorElement {
public:
   using DetectorElement::DetectorElement;
   virtual double energyResolution(double energy) const = 0;
   virtual double clusterSize(const Particle& ptc) const = 0 ;
   virtual bool   acceptance(const Cluster& ptc) const = 0;
   //virtual bool   acceptance(const Track& track) const= 0;
   //virtual double space_resolution(Particle* ptc)=0;
private:
   
};

class Field: public DetectorElement {
public:
   using DetectorElement::DetectorElement;
   //virtual double ptResolution(const Track&) const = 0;
   //virtual bool   acceptance(const Track&) const = 0;
   //virtual bool   acceptance(const Track& track) const= 0;
   //virtual double space_resolution(Particle* ptc)=0;
protected:
   double m_magnitude;
};


class Tracker: public DetectorElement {
public:
   using DetectorElement::DetectorElement;
   virtual double getPtResolution(const Track&) const = 0;
   virtual bool   acceptance(const Track&) const = 0;   //virtual bool   acceptance(const Track& track) const= 0;
   //virtual double space_resolution(Particle* ptc)=0;
protected:
   //double m_magnitude;
};

///BaseDetector
/**
   Class from which user can provide their own detector code
   It must contain an HCAL, ECAL, Tracker and Field element
 */
class BaseDetector {
public:
   BaseDetector();
   //BaseDetector(std::shared_ptr<const DetectorElement> ECAL ,std::shared_ptr<const DetectorElement> HCAL);
   //BaseDetector( DetectorElement& ECAL, DetectorElement& HCAL);
   //BaseDetector( DetectorElement&& ECAL,DetectorElement&& HCAL);
   const std::list<SurfaceCylinder>&
   getSortedCylinders(); ///AJRTODO make this simply return the list (or a copy) - sort on                                initialisation
   std::shared_ptr<const DetectorElement> getElement(fastsim::enumLayer layer)
   const;
   std::shared_ptr<const ECAL> getECAL() const {return m_ECAL;};
   std::shared_ptr<const HCAL> getHCAL() const {return m_HCAL;};
   std::shared_ptr<const Tracker> getTracker() const {return m_Tracker;};
   std::shared_ptr<const Field> getField() const {return m_Field;};
   /*const DetectorElement& getElement( fastsim::enumLayer layer) const;
   const DetectorElement& getECAL() const;
   const DetectorElement& getHCAL() const;*/
protected:
   //AJRTODO may replace this with explicit HCAL, ECAL etc
   //need a po inter in this so that it i polymorphic
   //TODO decide if should be a unique pointer etc?
   //std::map<fastsim::enumLayer, std::shared_ptr<const DetectorElement>> m_detectorElements;
   //NOW const?
   //std::map<fastsim::enumLayer, DetectorElement> m_detectorElements;
   //const DetectorElement& m_ECAL; //allow for polymorphism
   //const DetectorElement& m_HCAL;
   //DetectorElement& Field;
   //DetectorElement& Treacker;
   //CMSECAL cmsthing;
   //ECAL (cmsthings);
   std::shared_ptr<const ECAL> m_ECAL;
   std::shared_ptr<const HCAL> m_HCAL;
   std::shared_ptr<const Tracker> m_Tracker;
   std::shared_ptr<const Field> m_Field;

   
private:
   std::list<SurfaceCylinder> m_cylinders; // or use pointers here?

};

#endif