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
#include <time.h>

#include "GroFileManager.h"
#include <boost/program_options.hpp>

void regulateCoord (std::vector<std::vector<double > > & posi,
		    const std::vector<double > & boxsize)
{
  for (int i = 0; i < int(posi.size()); i+=3){
    double diff0 = posi[i+1][0] - posi[i][0];
    double diff1 = posi[i+2][0] - posi[i][0];
    if (diff0 > boxsize[0] * 0.5){
      posi[i+1][0] -= boxsize[0];
    }
    else if (diff0 < -boxsize[0] * 0.5){
      posi[i+1][0] += boxsize[0];
    }
    if (diff1 > boxsize[0] * 0.5){
      posi[i+2][0] -= boxsize[0];
    }
    else if (diff1 < -boxsize[0] * 0.5){
      posi[i+2][0] += boxsize[0];
    }
    for (int dd = 0; dd < 3; ++dd){
      if (posi[i][dd] >= boxsize[dd]){
	posi[i+0][dd] -= boxsize[dd];
	posi[i+1][dd] -= boxsize[dd];
	posi[i+2][dd] -= boxsize[dd];
      }
      else if (posi[i][dd] < 0.){
	posi[i+0][dd] += boxsize[dd];
	posi[i+1][dd] += boxsize[dd];
	posi[i+2][dd] += boxsize[dd];
      }
    }	
  }
}

namespace po = boost::program_options;

int main (int argc, char * argv[])
{
  std::string ifilename;
  std::string ofilename;
  po::options_description desc ("Allow options");  

  double refCellSize;
  int nx;
  double gap;
  
  desc.add_options()
      ("help,h", "print this message")
      ("refCellSize,n", po::value<double > (&refCellSize)->default_value (0.35),  "ref")
      ("nx", po::value<int > (&nx)->default_value (3),  "number of layers on x")
      ("gap,g", po::value<double > (&gap)->default_value (0.1),  "gap between the wall and original the simulation box")
      ("output-file-name,o", po::value<std::string > (&ofilename)->default_value (std::string("out.gro"), "output conf file name"))
      ("input-file-name,f",  po::value<std::string > (&ifilename)->default_value (std::string("conf.gro"), "input conf file name"));

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }

  std::vector<int >  resdindex;
  std::vector<std::string >   resdname;
  std::vector<std::string >   atomname;
  std::vector<int >  atomindex;
  std::vector<std::vector<double > >  posi;
  std::vector<std::vector<double > >  velo;
  std::vector<double >  boxsize;
  
  GroFileManager::read (ifilename, resdindex, resdname, atomname, atomindex,
			posi, velo, boxsize);
  std::vector<double > hh(3);
  hh[0] = refCellSize;
  int ny = boxsize[1] / refCellSize;
  int nz = boxsize[2] / refCellSize;
  hh[1] = boxsize[1] / double (ny);
  hh[2] = boxsize[2] / double (nz);

  regulateCoord (posi, boxsize);
  double shift1 = nx * hh[0] + gap;
  double shift2 = shift1 + boxsize[0] + gap;
  boxsize[0] += shift1 * 2;
  
  int natom = posi.size();
  int naddHalf = nx * ny * nz * 2;
  int natomNew = natom + naddHalf + naddHalf;
  printf ("# %dx2 new atoms are added, number density is %f\n",
	  naddHalf, 2 / (hh[0] * hh[1] * hh[2]));
  resdindex.resize(natomNew);
  resdname.resize(natomNew);
  atomindex.resize(natomNew);
  atomname.resize(natomNew);
  posi.resize(natomNew);
  velo.resize(natomNew);

  for (int i = 0; i < natom; ++i){
    posi[i][0] += shift1;
  }
  int indexShift = 0;
  for (int ix = 0; ix < nx; ++ix){
    for (int iy = 0; iy < ny; ++iy){
      for (int iz = 0; iz < nz; ++iz){
	resdindex[natom+indexShift] = resdindex[natom-1] + indexShift + 1;
	atomindex[natom+indexShift] = atomindex[natom-1] + indexShift + 1;
	resdname[natom+indexShift] = "WA";
	atomname[natom+indexShift] = "C";
	posi[natom+indexShift].resize(3);
	posi[natom+indexShift][0] = (ix+0.25) * hh[0];
	posi[natom+indexShift][1] = (iy+0.25) * hh[1];
	posi[natom+indexShift][2] = (iz+0.25) * hh[2];
	velo[natom+indexShift].resize(3, 0.);
	indexShift ++;
	resdindex[natom+indexShift] = resdindex[natom-1] + indexShift + 1;
	atomindex[natom+indexShift] = atomindex[natom-1] + indexShift + 1;
	resdname[natom+indexShift] = "WA";
	atomname[natom+indexShift] = "C";
	posi[natom+indexShift].resize(3);
	posi[natom+indexShift][0] = (ix+0.25) * hh[0] + 0.5 * hh[0];
	posi[natom+indexShift][1] = (iy+0.25) * hh[1] + 0.5 * hh[1];
	posi[natom+indexShift][2] = (iz+0.25) * hh[2] + 0.5 * hh[2];
	velo[natom+indexShift].resize(3, 0.);
	indexShift ++;
      }
    }
  }

  for (int ix = 0; ix < nx; ++ix){
    for (int iy = 0; iy < ny; ++iy){
      for (int iz = 0; iz < nz; ++iz){
	resdindex[natom+indexShift] = resdindex[natom-1] + indexShift + 1;
	atomindex[natom+indexShift] = atomindex[natom-1] + indexShift + 1;
	resdname[natom+indexShift] = "WB";
	atomname[natom+indexShift] = "C";
	posi[natom+indexShift].resize(3);
	posi[natom+indexShift][0] = (ix+0.25) * hh[0] + shift2;
	posi[natom+indexShift][1] = (iy+0.25) * hh[1];
	posi[natom+indexShift][2] = (iz+0.25) * hh[2];
	velo[natom+indexShift].resize(3, 0.);
	indexShift ++;
	resdindex[natom+indexShift] = resdindex[natom-1] + indexShift + 1;
	atomindex[natom+indexShift] = atomindex[natom-1] + indexShift + 1;
	resdname[natom+indexShift] = "WB";
	atomname[natom+indexShift] = "C";
	posi[natom+indexShift].resize(3);
	posi[natom+indexShift][0] = (ix+0.25) * hh[0] + 0.5 * hh[0] + shift2;
	posi[natom+indexShift][1] = (iy+0.25) * hh[1] + 0.5 * hh[1];
	posi[natom+indexShift][2] = (iz+0.25) * hh[2] + 0.5 * hh[2];
	velo[natom+indexShift].resize(3, 0.);
	indexShift ++;
      }
    }
  }

  GroFileManager::write (ofilename,
			 resdindex, resdname,
			 atomname, atomindex,
			 posi, velo, boxsize);
  FILE * fp = fopen ("topol.top", "w");
  fprintf (fp, "#include \"system.itp\"\n[ molecules ]\nSOL %d\nTWALLA %d\nTWALLB %d\n",
	   natom/3, naddHalf, naddHalf);
  fclose (fp);
  
  return 0;
}

  
