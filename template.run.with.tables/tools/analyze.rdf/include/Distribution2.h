#ifndef __Distribution2_h_NONEQMSM_wanghan__
#define __Distribution2_h_NONEQMSM_wanghan__

#include "Defines.h"
#include <vector>
#include <string>

using namespace std;

class Distribution2_Cylinder
{
public:
  double x0, x1, v0, v1;
  double hx, hv;
  double valuepp;
  unsigned nx, nv;
  vector<vector<double > > values;
  vector<vector<double > > backup_values;
  double backup_number;
  double backup_unbacked_count;
  vector<double > gridx;
  vector<double > gridv;
  double nframe;
public:
  Distribution2_Cylinder () {};
  Distribution2_Cylinder (const double & x0,
			 const double & x1,
			 const unsigned & nx,
			 const double & v0,
			 const double & v1,
			 const unsigned & nv);
  void reinit (const double & x0,
	       const double & x1,
	       const unsigned & nx,
	       const double & v0,
	       const double & v1,
	       const unsigned & nv);
public:
  void clear ();
  void deposit (const double & xx, const double & vv);
  void deposit (const double & xx, const double & vv, const double & scale);
  void average ();
  void average (const double & scale);
  void calTraj ();
  void print_x  (const string & filename) const;
  void print_xv (const string & filename) const;
  void print_x  (FILE * fp) const;
  void print_xv (FILE * fp) const;
  void print_along_x (const string & filename, const double x) const;
  void print_along_x (FILE * fp, const double x) const;
  void substract (const Distribution2_Cylinder & d);
  void add (const double & scalor,
	    const Distribution2_Cylinder & d);
  double getNframe () const {return nframe;}
// public:
//   void save (FILE * fp) const;
//   void load (FILE * fp);
}
    ;




#endif
