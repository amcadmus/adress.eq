#include "Distribution2.h"
#include <cmath>

Distribution2_Cylinder::
Distribution2_Cylinder (const double & x0_,
		 const double & x1_,
		 const unsigned & nx_,
		 const double & v0_,
		 const double & v1_,
		 const unsigned & nv_)
{
  reinit (x0_, x1_, nx_, v0_, v1_, nv_);
}


void Distribution2_Cylinder::
reinit (const double & x0_,
	const double & x1_,
	const unsigned & nx_,
	const double & v0_,
	const double & v1_,
	const unsigned & nv_)
{
  x0 = x0_;
  x1 = x1_;
  v0 = v0_;
  v1 = v1_;
  nx = nx_;
  nv = nv_;

  hx = (x1 - x0) / double (nx);
  hv = (v1 - v0) / double (nv);
  valuepp = 1.;
  // valuepp = 1./(hx*hv);
  
  gridx.resize(nx);
  gridv.resize(nv);

  for (unsigned ii = 0; ii < gridx.size(); ++ii){
    gridx[ii] = x0 + (0.5 + ii) * hx;
  }
  for (unsigned ii = 0; ii < gridv.size(); ++ii){
    gridv[ii] = v0 + (0.5 + ii) * hv;
  }

  values.resize(nx);
  for (unsigned ii = 0; ii < nx; ++ii){
    values[ii].resize(nv);
  }
  backup_values.resize(nx);
  for (unsigned ii = 0; ii < nx; ++ii){
    backup_values[ii].resize(nv);
  }

  backup_number = 1e6;
  backup_unbacked_count = 0.;
  clear ();
}


void Distribution2_Cylinder::
clear ()
{
  nframe = 0.;
  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      values[ii][jj] = 0.;
      backup_values[ii][jj] = 0.;
    }
  }
}

void Distribution2_Cylinder::
deposit (const double & xx,
	 const double & vv)
{
  deposit (xx, vv, 1.);
}

void Distribution2_Cylinder::
deposit (const double & xx,
	 const double & vv,
	 const double & scale)
{
  int ix = (xx - x0) / hx;
  int iv = (vv - v0) / hv;
  if ((ix < 0 || ix >= int(nx)) || (iv < 0 || iv >= int(nv))){
    return;
  }
  else {
    nframe += 1.;
    backup_unbacked_count += 1.;
    values[ix][iv] += valuepp * scale;
    if (backup_unbacked_count >= backup_number){
      // std::cout << "backuped" << std::endl;
      /* fprintf (stderr, "backup: %.16e %.16e\n", backup_unbacked_count, backup_number); */
      for (unsigned ii = 0; ii < nx; ++ii){
	for (unsigned jj = 0; jj < nv; ++jj){
	  /* if (ii == nx/2 && jj == nv/2){ */
	  /*   fprintf (stderr,"average: %.16e %.16e    %.16e\n" , backup_values[ii][jj], values[ii][jj], nframe); */
	  /* } */
	  backup_values[ii][jj] += values[ii][jj];
	  values[ii][jj] = 0.;
	}
      }
      backup_unbacked_count = 0.;
    }
  }
}

void Distribution2_Cylinder::
average ()
{
  if (nframe == 0.) return;
  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      // if (ii == nx/2 && jj == nv/2){
      // 	fprintf (stderr,"average: %.16e %.16e     %.16e\n" , backup_values[ii][jj], values[ii][jj], nframe);
      // }
      values[ii][jj] += backup_values[ii][jj];
      double r1 = jj * hv;
      double r2 = r1 + hv;
      double volume = M_PI * (r2*r2 - r1*r1) * hx;
      // values[ii][jj] /= volume * double(nframe);
      values[ii][jj] /= volume;
    }
  }  
}

void Distribution2_Cylinder::
average (const double & scale)
{
  // if (nframe == 0.) return;
  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      // if (ii == nx/2 && jj == nv/2){
      // 	fprintf (stderr,"average: %.16e %.16e     %.16e\n" , backup_values[ii][jj], values[ii][jj], nframe);
      // }
      values[ii][jj] += backup_values[ii][jj];
      // double r1 = jj * hv;
      // double r2 = r1 + hv;
      // double volume = M_PI * (r2*r2 - r1*r1) * hx;
      // // values[ii][jj] /= volume * double(nframe);
      // values[ii][jj] /= volume;
      values[ii][jj] *= scale;
    }
  }  
}


#include <iostream>
#include <fstream>

void Distribution2_Cylinder::
print_xv (const string & filename) const
{
  FILE * fp = fopen (filename.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << std::endl;
    return;
  }
  print_xv (fp);
  fclose(fp);
}

void Distribution2_Cylinder::
print_x (const string & filename) const
{
  FILE * fp = fopen (filename.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << std::endl;
    return;
  }
  print_x (fp);
  fclose(fp);
}


void Distribution2_Cylinder::
print_xv (FILE * fp) const
{
  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      fprintf (fp, "%f %f %.16e\n", gridx[ii], gridv[jj], values[ii][jj]);
    }
    fprintf (fp, "\n");
  }
}

void Distribution2_Cylinder::
print_x (FILE * fp) const
{
  for (unsigned ii = 0; ii < nx; ++ii){
    double avg = 0.;
    for (unsigned jj = 0; jj < nv; ++jj){
      avg += values[ii][jj] * hv;
    }
    fprintf (fp, "%f %.16e\n", gridx[ii], avg);
  }
}

void Distribution2_Cylinder::
print_along_x (const string & filename, const double x) const
{
  FILE * fp = fopen (filename.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << std::endl;
    return;
  }
  print_along_x (fp, x);
  fclose(fp);  
}

void Distribution2_Cylinder::
print_along_x (FILE * fp, const double x) const
{
  for (unsigned ii = 0; ii < nx; ++ii){
    if (fabs(gridx[ii] - x) <= 0.5 * hx){
      for (unsigned jj = 0; jj < nv; ++jj){
	fprintf (fp, "%f %.16e\n", gridv[jj], values[ii][jj]);
      }
      return;
    }
  }    
}

void Distribution2_Cylinder::
substract (const Distribution2_Cylinder & d)
{
  if (d.nx != nx || d.nv != nv){
    std::cerr << "unmatch distributions, do nothing" << std::endl;
    return;
  }

  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      values[ii][jj] -= d.values[ii][jj];
    }
  }
}

void Distribution2_Cylinder::
add (const double & scalor,
     const Distribution2_Cylinder & d)
{
  if (d.nx != nx || d.nv != nv){
    std::cerr << "unmatch distributions, do nothing" << std::endl;
    return;
  }

  for (unsigned ii = 0; ii < nx; ++ii){
    for (unsigned jj = 0; jj < nv; ++jj){
      values[ii][jj] += scalor * d.values[ii][jj];
    }
  }
}

