
//
//  main.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
//C++
#include <iostream>
#include <cstdlib>

//ROOT
#include "TROOT.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TApplication.h"


//SSD libs
#include "fastsim/my_utilities.h"
#include "detectors/material.h"
#include "detectors/geometry.h"
#include "detectors/CMS.h"
#include "particle.h"
#include "datatypes.h"
#include "simulation/simulator.h"
#include "path.h"
#include "displaygeometry.h"
#include "displaycore.h"
#include "displaypfobjects.h"
#include "TVector3.h"



int main(int argc, char* argv[]){
   
   
   
   // ROOT App to allow graphs to be plotted
   TApplication theApp("App", &argc, argv);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }
   
   //Create CMS detector
   CMS CMSDetector;
   
   //Create simulator
   Simulator sim= Simulator{CMSDetector};
   
   //simulate some photons
   for (int i=1; i<2;i++  )
   { //todo  lower case m
      TLorentzVector tlvphoton=makeParticleLorentzVector(22,  M_PI/2. +0.025*i, M_PI/2.+0.3*i, 100.*(i));
      SimParticle photon = SimParticle(22, tlvphoton) ;
      sim.addParticle(photon);
      sim.simulatePhoton(photon);
      
   }
   //lower case
   sim.Experiment(); //Write lists of connected items
   
 
   //TODO try to remove/reduce use of shared_ptrs here.
   Display display = Display({Projection::xy,Projection::yz});
   std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
   display.Register(gdetector, 0);
  
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   //GDetector gdetector{CMSDetector};
   // display.Register(std::move(gdetector), 0);
   
   //plot clusters
   for (auto & cl : sim.getClusters())
   {
      std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cl.second)) ;
      display.Register(gcluster,2);
      
   }
   display.Draw();
   
   //theApp.Run();
   
   return EXIT_SUCCESS;
}



void test_helix()
{//Helix path test
   TLorentzVector p4 = TLorentzVector();
   p4.SetPtEtaPhiM(1, 0, 0, 5.11e-4);
   Helix helix(3.8, 1, p4,TVector3(0,0,0));
   double length = helix.getPathLength(1.0e-9);
   TVector3 junk = helix.getPointAtTime(1e-9);
   std::cout<<"Helix point: " <<junk.X() << " " << junk.Y() << " " << junk.Z()<<" ";
   std::cout<< "\nlength"<<length<< " " << helix.getDeltaT(length)<<std::endl;
   
}
void test_Structures()
{
   //testing cylinders etc
   std::cout << "Try base classes\n";
   Material M(fastsim::enumLayer::ECAL,1,1);
   SurfaceCylinder S("empty");
   VolumeCylinder V("new", 4, 6, 3, 6);
}


struct A
{
   virtual void foo() const = 0;
   void bar();
};

struct B : A
{
   //void foo()  override; // Error: B::foo does not override A::foo
   // (signature mismatch)
   void foo() const override; // OK: B::foo overrides A::foo
                              //void bar() override; // Error: A::bar is not virtual
};

void B::foo() const
{
   std::cout <<"foo";
}


/*using boost::any_cast;
typedef std::list<boost::any> many;


void append_int(many & values, int value)
{
   boost::any to_append = value;
   values.push_back(to_append);
}*/


int old_main_stuff(int argc, char* argv[])
{
   
   B b;
   b.foo();
   
   
   std::unordered_map<long,TVector3> rootvec;
   TVector3 tv1{0,0,0};
   TVector3 tv2{0,0,0};
   rootvec[0]=std::move(tv1);
   rootvec[2]=std::move(tv2);
   rootvec.emplace(3,TVector3{0,0,0});
   
   // all new
   TApplication theApp("App", &argc, argv);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }
   //testing();
   //theApp.Run();
   return 0;
}

void test_graphs()
{//Testing graphics
   Display display = Display({Projection::xy,Projection::yz});
   //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
   
   TVector3 vpos(1.,.5,.3);
   Cluster cluster=  Cluster(10., vpos, 1.,Identifier::makeECALClusterID() );
   std::vector<TVector3> tvec;
   tvec.push_back(TVector3(0.,0.,0.));
   tvec.push_back(TVector3(1.,1.,1.));
   tvec.push_back(TVector3(2.,2.,2.));
   
   
   /*std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
   std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
   display.Register(gtrajectories,1);
   display.Register(gcluster,2);
   display.Draw();*/
   
   //Testing graphics
   /* TVector3 vpos(1.,.5,.3);
    Cluster cluster=  Cluster(10., vpos, 1.,Identifier::makeECALClusterID() );
    std::vector<TVector3> tvec;
    
    std::cout <<"cluster "<< cluster.getPt()<<"\n";
    
    std::vector<TVector3> tvec;
    tvec.push_back(TVector3(0.,0.,0.));
    tvec.push_back(TVector3(1.,1.,1.));
    tvec.push_back(TVector3(2.,2.,2.));
    
    
    Display display = Display({enumProjection::xy,enumProjection::yz});
    //Display display = Display({Projection::xy,Projection::yz,Projection::ECAL_thetaphi ,Projection::HCAL_thetaphi });
    
    std::shared_ptr<GDetector> gdetector (new GDetector(CMSDetector));
    display.Register(gdetector, 0);
    
    std::shared_ptr<GTrajectories> gtrajectories (new GTrajectories(tvec)) ;// simulator.ptcs)
    std::shared_ptr<GTrajectories> gcluster (new GTrajectories(cluster)) ;
    display.Register(gtrajectories,1);
    display.Register(gcluster,2);
    display.Draw();*/
   
   
}


void testing() { //change to concrete object or unique pointer is there is an issue
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   
   const int n = 20;
   double x[n], y[n];
   for (int i=0;i<n;i++) {
      x[i] = i;
      y[i] = 2*i;
      std::cout<<x[i]<<"\t"<<y[i]<<std::endl;
   }
   
   TGraph *gr = new TGraph(n,x,y);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->SetTitle("a simple graph");
   gr->GetXaxis()->SetTitle("X title");
   gr->GetYaxis()->SetTitle("Y title");
   gr->Draw("ACP");
   
   c1->Update();
   c1->Modified();
   c1->Connect("Closed()", "TApplication", gApplication, "Terminate()"); //new
}
