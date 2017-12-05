/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */


#ifdef PairInclude
#include "pair_hooke_viscous.h"
#endif


#ifdef PAIR_CLASS

PairStyle(hooke/viscous,PairHookeViscous)

#else

#ifndef LMP_PAIR_HOOKE_VISCOUS_H
#define LMP_PAIR_HOOKE_VISCOUS_H
#include <string>
#include <vector>
#include "pair.h"

namespace LAMMPS_NS {

class PairHookeViscous : public Pair {
 public:
  PairHookeViscous(class LAMMPS *);
  virtual ~PairHookeViscous();
  virtual void compute(int, int);
  virtual void settings(int, char **);
  void coeff(int, char **);
  void init_style();
  void init_list(int, class NeighList *);
  double init_one(int, int);
  void write_restart(FILE *);
  void read_restart(FILE *);
  void write_restart_settings(FILE *);
  void read_restart_settings(FILE *);
  void reset_dt();
  virtual double single(int, int, int, int, double, double, double, double &);
  int pack_forward_comm(int, int *, double *, int, int *);
  void unpack_forward_comm(int, int, double *);
  double memory_usage();
  void *extract(const char *, int &);

 protected:
  double kn,kt,gamman,gammat,xmu;
  int dampflag;
  double dt;
  int freeze_group_bit;
  int history;

  int neighprev;
  double *onerad_dynamic,*onerad_frozen;
  double *maxrad_dynamic,*maxrad_frozen;

  //------Viscosity Forces--------//
    std::string fluidViscosityFile; //file to be read in for viscosity information
  std::vector<double> fluidViscosity;//n x 1 vector
  std::string VelFieldFileX; 
  std::string VelFieldFileY;
    std::vector<double> VelFieldX;
    std::vector<double> VelFieldY;
    //std::vector<double> VelFieldZ;
  //------------------------------//

  class FixShearHistory *fix_history;

  // storage of rigid body masses for use in granular interactions

  class Fix *fix_rigid;    // ptr to rigid body fix, NULL if none
  double *mass_rigid;      // rigid mass for owned+ghost atoms
  int nmax;                // allocated size of mass_rigid

  void allocate();
};

}

#endif
#endif

/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.  Check the input script syntax and compare to the
documentation for the command.  You can use -echo screen as a
command-line option when running LAMMPS to see the offending line.

E: Incorrect args for pair coefficients

Self-explanatory.  Check the input script or data file.

E: Pair granular requires atom attributes radius, rmass

The atom style defined does not have these attributes.

E: Pair granular requires ghost atoms store velocity

Use the comm_modify vel yes command to enable this.

E: Pair granular with shear history requires newton pair off

This is a current restriction of the implementation of pair
granular styles with history.

E: Could not find pair fix ID

A fix is created internally by the pair style to store shear
history information.  You cannot delete it.

*/