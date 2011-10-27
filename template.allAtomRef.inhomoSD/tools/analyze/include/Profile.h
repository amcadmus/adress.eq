#ifndef __Profile_h_wanghan__
#define __Profile_h_wanghan__

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>

#define CPLUSPLUS

class Profile_PiecewiseConst
{
  std::vector<double > boxsize;
  unsigned nx, ny, nz;
  double   hx, hy, hz;
  unsigned nele;
  std::vector<double > profile;
  std::vector<double > ndata;
  int natoms;
public:
  inline unsigned index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const;
  inline void     index1to3 (unsigned& input,
			     unsigned& ix, unsigned& iy, unsigned& iz) const;
public:
  void reinit (const std::vector<double > & boxsize,
	       const double & refh);
  void clear ();
  void deposit (const std::vector<std::vector<double > > & posis,
		const std::vector<double > & values);
  void average ();
public:
  const double & getProfile (const unsigned & ix,
			     const unsigned & iy,
			     const unsigned & iz) const
      {return profile[index3to1(ix, iy, iz)];}
  const std::vector<double > & getProfile () const {return profile;}
  inline const double & getValue (const double & xx,
				  const double & yy,
				  const double & zz) const;
  const unsigned & getNx () const {return nx;}
  const unsigned & getNy () const {return ny;}
  const unsigned & getNz () const {return nz;}
  // const double & getHx () const {return hx;}
  // const double & getHy () const {return hy;}
  // const double & getHz () const {return hz;}
  const int & getNatoms () const {return natoms;}
  const std::vector<double > & getBox () const {return boxsize;}
public:
  void print_x  (const std::string & filename) const;
  void print_avg_x (const std::string & filename) const;
  void print_xz (const std::string & filename) const;
};

    
unsigned Profile_PiecewiseConst::
index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const
{
  return iz + nz * (iy + ny * ix);
}

void Profile_PiecewiseConst::
index1to3 (unsigned& input,
	   unsigned& ix, unsigned& iy, unsigned& iz) const
{
  unsigned tmp = input;
  iz = tmp % (nz);
  tmp = (tmp - iz) / nz;
  iy = tmp % (ny);
  ix =  (tmp - iy) / ny;
}

const double & Profile_PiecewiseConst::
getValue (const double & xx,
	  const double & yy,
	  const double & zz) const
{
  unsigned ix = unsigned (xx / hx);
  unsigned iy = unsigned (yy / hy);
  unsigned iz = unsigned (zz / hz);
  return getProfile (ix, iy, iz);
}



class VectorProfile_PiecewiseConst
{
  std::vector<double > boxsize;
  unsigned nx, ny, nz;
  double   hx, hy, hz;
  unsigned nele;
  std::vector<std::vector<double > > profile;
  std::vector<double > ndata;
  int natoms;
public:
  inline unsigned index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const;
  inline void     index1to3 (unsigned& input,
			     unsigned& ix, unsigned& iy, unsigned& iz) const;
public:
  void reinit (const std::vector<double > & boxsize,
	       const double & refh);
  void clear ();
  void deposit (const std::vector<std::vector<double > > & posis,
		const std::vector<std::vector<double > > & values);
  void average ();
public:
  // const double & getProfile (const unsigned & ix,
  // 			     const unsigned & iy,
  // 			     const unsigned & iz) const
  //     {return profile[index3to1(ix, iy, iz)];}
  const std::vector<std::vector<double > > &
  getProfile () const {return profile;}
  inline const double & getValue (const double & xx,
				  const double & yy,
				  const double & zz) const;
  const unsigned & getNx () const {return nx;}
  const unsigned & getNy () const {return ny;}
  const unsigned & getNz () const {return nz;}
  // const double & getHx () const {return hx;}
  // const double & getHy () const {return hy;}
  // const double & getHz () const {return hz;}
  const int & getNatoms () const {return natoms;}
  const std::vector<double > & getBox () const {return boxsize;}
public:
  void print_x  (const std::string & filename) const;
  void print_avg_x (const std::string & filename) const;
  void print_xz (const std::string & filename) const;
  void print_avg_xz (const std::string & filename) const;
};

    
unsigned VectorProfile_PiecewiseConst::
index3to1 (unsigned  ix, unsigned  iy, unsigned  iz) const
{
  return iz + nz * (iy + ny * ix);
}

void VectorProfile_PiecewiseConst::
index1to3 (unsigned& input,
	   unsigned& ix, unsigned& iy, unsigned& iz) const
{
  unsigned tmp = input;
  iz = tmp % (nz);
  tmp = (tmp - iz) / nz;
  iy = tmp % (ny);
  ix =  (tmp - iy) / ny;
}

// const double & VectorProfile_PiecewiseConst::
// getValue (const double & xx,
// 	  const double & yy,
// 	  const double & zz) const
// {
//   unsigned ix = unsigned (xx / hx);
//   unsigned iy = unsigned (yy / hy);
//   unsigned iz = unsigned (zz / hz);
//   return getProfile (ix, iy, iz);
// }






#endif
