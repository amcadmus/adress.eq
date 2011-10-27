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

#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_trr.h"
#include "GroFileManager.h"

#include <boost/program_options.hpp>
#include "Profile.h"

namespace po = boost::program_options;

// make sure the three atoms do not cross the box
static void
regulateCoord (rvec & r0,
	       rvec & r1,
	       rvec & r2,
	       const std::vector<double > & boxsize)
{
  for (int dd = 0; dd < 3; ++dd){
    double diff1 = r1[dd] - r0[dd];
    double diff2 = r2[dd] - r0[dd];
    if      (diff1 >  0.5 * boxsize[dd]){
      r1[dd] -= boxsize[dd];
    }
    else if (diff1 < -0.5 * boxsize[dd]){
      r1[dd] += boxsize[dd];
    }
    if      (diff2 >  0.5 * boxsize[dd]){
      r2[dd] -= boxsize[dd];
    }
    else if (diff2 < -0.5 * boxsize[dd]){
      r2[dd] += boxsize[dd];
    }
  }
}

int main (int argc, char * argv[])
{
  std::string cfilename;
  std::string tfilename;
  po::options_description desc ("Allow options");  

  float start_t, end_t;
  double h;
  
  desc.add_options()
      ("help,h", "print this message")
      ("start,s", po::value<float > (&start_t)->default_value (0.f),  "start time")
      ("end,e",   po::value<float > (&end_t)  ->default_value (0.f),  "end time, 0 is infinity")
      ("bin-size,b",  po::value<double > (&h)->default_value (1.),  "bin size")
      ("conf-file-name,f", po::value<std::string > (&cfilename)->default_value (std::string("conf.gro"), "conf file name"))
      ("traj-file-name,f", po::value<std::string > (&tfilename)->default_value (std::string("traj.trr"), "trajactory file name"));

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  // //////////////////////////////////////////////////
  // start processing
  // //////////////////////////////////////////////////

  std::vector<std::string> resdname, atomname;
  std::vector<int > resdindex, atomindex;
  std::vector<std::vector<double > > posi, velo;
  std::vector<double > boxsize (3,0.);
  GroFileManager::read (cfilename, resdindex, resdname, atomname, atomindex,
			posi, velo, boxsize);
  int nmol = 0;
  int nmolwa = 0;
  int nmolwb = 0;
  for (unsigned i = 0; i < atomname.size(); ++i){
    if (atomname[i] == "OW"){
      nmol ++;
    }
    if (resdname[i] == "WA"){
      nmolwa ++;
    }
    if (resdname[i] == "WB"){
      nmolwb ++;
    }    
  }

  char fname [1024];
  strncpy (fname, tfilename.c_str(), 1024);
  XDRFILE * fp = xdrfile_open (fname, "r");
  int natoms;
  if (read_trr_natoms (fname, &natoms) != 0){
    std::cerr << "wrong reading natoms" << std::endl;    
    exit (1);
  }
  std::cout << "# natom is " << natoms << std::endl;
  if (unsigned(natoms) != atomname.size()) {
    std::cerr << "conf and traj are not consistent" << std::endl;    
    exit (1);
  }
  int step;
  float time, lambda;
  matrix gbox;  
  rvec *xx, *vv, *ff;
  xx = (rvec *) malloc (sizeof(rvec) * natoms);
  vv = (rvec *) malloc (sizeof(rvec) * natoms);
  ff = (rvec *) malloc (sizeof(rvec) * natoms);
  float time_prec = 0.001;

  std::vector<double > tmp (3, 0.);
  std::vector<std::vector<double > > coms(nmol, tmp);
  std::vector<std::vector<double > > values(nmol, tmp);
  // std::vector<std::vector<double > > comswa(nmolwa, tmp);
  // std::vector<std::vector<double > > comswb(nmolwb, tmp);
  // std::vector<double > valueswa(nmolwa, 1.);
  // std::vector<double > valueswb(nmolwb, 1.);
  VectorProfile_PiecewiseConst prof;
  prof.reinit (boxsize, h);
  
  while (
      read_trr(fp, natoms, &step, &time, &lambda, gbox, xx, vv, ff)
      == 0){
    if (end_t != 0.f) {
      if (time < start_t - time_prec){
        continue;
      }
      else if (time > end_t + time_prec) {
        break;
      }
    }
    else {
      if (time < start_t - time_prec) continue;
    }
    std::cout << "#! loaded frame at time " << time << "ps   \r";  
    std::cout << std::flush;
    int countMol = 0;
    // int countMolwa = 0;
    // int countMolwb = 0;
    for (int i = 0; i < natoms; ++i){
      if (atomname[i] == "OW"){
	regulateCoord (xx[i], xx[i+1], xx[i+2], boxsize);
	std::vector<double > mycom (3, 0.);
	std::vector<double > myvalue (3, 0.);
	for (int dd = 0; dd < 3; ++dd){
	  mycom[dd] = xx[i][dd] * 16. + xx[i+1][dd] * 1. + xx[i+2][dd] * 1.;
	  myvalue[dd] += (
	      16. * vv[i+0][dd] +
	      1.  * vv[i+1][dd] +
	      1.  * vv[i+2][dd] );
	}
	for (int dd = 0; dd < 3; ++dd){
	  mycom[dd] /= 18.;
	  if (mycom[dd] >= boxsize[dd]) mycom[dd] -= boxsize[dd];
	  else if (mycom[dd] < 0) mycom[dd] += boxsize[dd];
	  values[countMol][dd] = myvalue[dd] / 18.;
	}
	coms[countMol] = mycom;
	countMol ++;
      }
      // if (resdname[i] == "WA"){
      // 	double myvalue = 0.;
      // 	for (int dd = 0; dd < 3; ++dd){
      // 	  comswa[countMolwa][dd] = xx[i][dd];
      // 	  myvalue += 0.5 * (12. * vv[i][dd] * vv[i][dd]);
      // 	}
      // 	valueswa[countMolwa] = myvalue * 1e3 / (1.38*6.02) * 2./3.;
      // 	countMolwa ++;
      // }
      // if (resdname[i] == "WB"){
      // 	double myvalue = 0.;
      // 	for (int dd = 0; dd < 3; ++dd){
      // 	  comswb[countMolwb][dd] = xx[i][dd];
      // 	  myvalue += 0.5 * (12. * vv[i][dd] * vv[i][dd]);
      // 	}
      // 	valueswb[countMolwb] = myvalue * 1e3 / (1.38*6.02) * 2./3.;
      // 	countMolwb ++;
      // }
    }
    std::cout << "nmol is " << countMol << std::endl;
    
    prof.deposit(coms, values);
    // profwa.deposit(comswa, valueswa);
    // profwb.deposit(comswb, valueswb);
  }
  
  // //////////////////////////////////////////////////
  // end processing
  // //////////////////////////////////////////////////

  prof.average();
  prof.print_x ("velo.water.x.out");
  prof.print_xz ("velo.water.xz.out");

  // DensityProfile_PiecewiseConst dp (filename, h);
  // dp.deposit (filename, start_t, end_t);
  // dp.print_x ("density.x.out");
  // dp.print_avg_x ("a.density.x.out");

  xdrfile_close(fp);
  free (xx);
  free (vv);
  free (ff);
  
  return 0;
}

  
