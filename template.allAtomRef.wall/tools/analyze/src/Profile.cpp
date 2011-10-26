#include "Profile.h"
#include <string.h>

// Profile_PiecewiseConst::
// Profile_PiecewiseConst (const std::string & filename,
// 			       const double & refh)
// {
//   reinit_xtc (filename, refh);
// }

void Profile_PiecewiseConst::
reinit (const std::vector<double > & boxsize_,
	const double & refh)
{
  boxsize = boxsize_;
  nx = unsigned (boxsize[0] / refh);
  ny = unsigned (boxsize[1] / refh);
  nz = unsigned (boxsize[2] / refh);
  hx = boxsize[0] / nx;
  hy = boxsize[1] / ny;
  hz = boxsize[2] / nz;
  nele = nx * ny * nz;
  
  clear ();
}

void Profile_PiecewiseConst::
clear ()
{
  profile.clear();
  profile.resize (nx * ny * nz, 0.);
  ndata.clear();
  ndata.resize (nx * ny * nz, 0.);
}

void Profile_PiecewiseConst::
deposit (const std::vector<std::vector<double > > & posis,
	 const std::vector<double > & values)
{
  for (unsigned i = 0; i < posis.size(); ++i){
    double tmp;
    tmp = posis[i][0];
    if      (posis[i][0] >= boxsize[0]) tmp -= boxsize[0];
    else if (posis[i][0] <  0)          tmp += boxsize[0];
    unsigned ix = unsigned (tmp / hx);
    tmp = posis[i][1];
    if      (posis[i][1] >= boxsize[1]) tmp -= boxsize[1];
    else if (posis[i][1] <  0)          tmp += boxsize[1];
    unsigned iy = unsigned (tmp / hy);
    tmp = posis[i][2];
    if      (posis[i][2] >= boxsize[2]) tmp -= boxsize[2];
    else if (posis[i][2] <  0)          tmp += boxsize[2];
    unsigned iz = unsigned (tmp / hz);
    profile[index3to1(ix, iy, iz)] += values[i];
    ndata[index3to1(ix, iy, iz)] += 1.;
  }  
}

void Profile_PiecewiseConst::
average ()
{
  // double totalprofile
  // int total = 0;
  for (unsigned i = 0; i < nele; ++i){
    if (ndata[i] != 0){
      profile[i] /= ndata[i];
    }
    // total += ndata[i] ;
  }
  // std::cout << "total ndata is " << total << std::endl;
}

void Profile_PiecewiseConst::    
print_x (const std::string & file) const 
{

  FILE * fp = fopen (file.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << file << std::endl;
    exit(1);
  }

  for (unsigned i = 0; i < nx; ++i){
    // double sum = 0.;
    // for (unsigned j = 0; j < ny; ++j){
    //   for (unsigned k = 0; k < nz; ++k){
    // 	sum += profile[index3to1(i, j, k)];
    //   }
    // }
    // fprintf (fp, "%f %f\n", (i + 0.5) * hx, sum / ny / nz);
    fprintf (fp, "%f %f\n", (i + 0.5) * hx, profile[index3to1(i, ny/2, nz/2)]);
  }

  fclose (fp);
}

void Profile_PiecewiseConst::    
print_avg_x (const std::string & file) const 
{
  FILE * fp = fopen (file.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << file << std::endl;
    exit(1);
  }

  for (unsigned i = 0; i < nx; ++i){
    double sum = 0.;
    for (unsigned j = 0; j < ny; ++j){
      for (unsigned k = 0; k < nz; ++k){
    	sum += profile[index3to1(i, j, k)];
      }
    }
    fprintf (fp, "%f %f\n", (i + 0.5) * hx, sum / ny / nz);
    // fprintf (fp, "%f %f\n", (i + 0.5) * hx, profile[index3to1(i, 0, 0)]);
  }

  fclose (fp);
}

void Profile_PiecewiseConst::    
print_xz (const std::string & file) const 
{
  FILE * fp = fopen (file.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << file << std::endl;
    exit(1);
  }

  for (unsigned i = 0; i < nx; ++i){
    double vx = (i + 0.5) * hx;
    for (unsigned j = 0; j < nz; ++j){
      double vz = (j + 0.5) * hz;
      double sum = 0.;
      double nsum = 0.;
      for (unsigned k = 0; k < ny; ++k){
	unsigned index = index3to1(i, k, j);
      	sum += profile[index] * ndata[index];
	nsum += ndata[index];
      }
      if (nsum != 0){
	sum /= nsum;
      }
      fprintf (fp, "%f %f %f\n", vx, vz, sum);
      // fprintf (fp, "%f %f %f\n", vx, vy, profile[index3to1(i,j,0)]);
    }
    fprintf (fp, "\n");
  }

  fclose (fp);
}

