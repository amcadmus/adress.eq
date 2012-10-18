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

#include "GroFileManager.h"

using namespace std;

int main(int argc, char * argv[])
{
  string file = "confout.gro";
  double y0 = 0;
  double y1 = 0.3;

  std::vector<int >  resdindex;
  std::vector<std::string >   resdname;
  std::vector<std::string >   atomname;
  std::vector<int >  atomindex;
  std::vector<std::vector<double > >  posi;
  std::vector<std::vector<double > >  velo;
  std::vector<double >  boxsize;

  std::vector<int >  resdindex1;
  std::vector<std::string >   resdname1;
  std::vector<std::string >   atomname1;
  std::vector<int >  atomindex1;
  std::vector<std::vector<double > >  posi1;
  std::vector<std::vector<double > >  velo1;
  std::vector<double >  boxsize1;

  GroFileManager::read (file, resdindex, resdname, atomname, atomindex,
			posi, velo, boxsize);

  for (unsigned ii = 0; ii < resdindex.size() / 4; ++ii){
    if (posi[4*ii+3][0] < 0){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][0] += boxsize[0];
      }
    }
    else if (posi[4*ii+3][0] > boxsize[0]){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][0] -= boxsize[0];
      }
    }
    if (posi[4*ii+3][1] < 0){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][1] += boxsize[1];
      }
    }
    else if (posi[4*ii+3][1] > boxsize[1]){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][1] -= boxsize[1];
      }
    }
    if (posi[4*ii+3][2] < 0){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][2] += boxsize[2];
      }
    }
    else if (posi[4*ii+3][2] > boxsize[2]){
      for (unsigned dd = 0; dd < 4; ++dd){
	posi[4*ii+dd][2] -= boxsize[2];
      }
    }
    if (posi[4*ii+3][1] > y0 && posi[4*ii+3][1] < y1){
      for (unsigned dd = 0; dd < 4; ++dd){
	resdindex1.push_back(resdindex[4*ii+dd]);
	resdname1.push_back(resdname[4*ii+dd]);
	atomindex1.push_back(atomindex[4*ii+dd]);
	atomname1.push_back(atomname[4*ii+dd]);
	posi1.push_back(posi[4*ii+dd]);
	velo1.push_back(velo[4*ii+dd]);
      }
    }
  }

  boxsize1.resize (3);
  boxsize1[0] = boxsize[0];
  boxsize1[1] = y1;
  boxsize1[2] = boxsize[2];
  
  GroFileManager::write ("out.gro", resdindex1, resdname1, atomname1, atomindex1,
			 posi1, velo1, boxsize1);
  
  return 0;
}

