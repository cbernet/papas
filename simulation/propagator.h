
#ifndef propogator_h
#define propogator_h
#include "detector.h"
#include "pfobjects.h"


class Propagator {
public:
   Propagator(const BaseDetector& det);
   virtual void propagateOne(SimParticle& ptc, fastsim::enumLayer Layer,
                             bool inner = true) = 0;
   virtual void propagateOne(SimParticle& ptc, std::string cylindername,
                             double cylinderz, double cylinderradius) = 0;

protected:
   const BaseDetector& m_detector;

};

class StraightLinePropagator : public Propagator {
public:
   using Propagator::Propagator;
   void propagateOne(SimParticle& ptc, fastsim::enumLayer Layer,
                     bool inner = true) override;
   void propagateOne(SimParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius) override;

private:


};

class HelixPropagator : public Propagator {
public:
   HelixPropagator(const BaseDetector& det, double field);
   void propagateOne(SimParticle& ptc, fastsim::enumLayer Layer,
                     bool inner = true) override;
   void propagateOne(SimParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius) override;
private:
   double m_field;

};
#endif

