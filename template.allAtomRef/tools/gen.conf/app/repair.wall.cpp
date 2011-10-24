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


namespace po = boost::program_options;

int main (int argc, char * argv[])
{
  std::string ifilename;
  std::string ofilename;
  std::string rfilename;
  po::options_description desc ("Allow options");
  
  desc.add_options()
      ("help,h", "print this message")
      ("input-file-name,f",     po::value<std::string > (&ifilename)->default_value (std::string("conf.gro"), "input conf file name"))
      ("reference-file-name,r", po::value<std::string > (&rfilename)->default_value (std::string("ref.gro"), "reference conf file name"))
      ("output-file-name,o",    po::value<std::string > (&ofilename)->default_value (std::string("out.gro"), "output conf file name"));

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }

  std::vector<int >  resdindex0, resdindex1;
  std::vector<std::string >   resdname0, resdname1;
  std::vector<std::string >   atomname0, atomname1;
  std::vector<int >  atomindex0, atomindex1;
  std::vector<std::vector<double > >  posi0, posi1;
  std::vector<std::vector<double > >  velo0, velo1;
  std::vector<double >  boxsize0, boxsize1;
  
  GroFileManager::read (ifilename, resdindex0, resdname0, atomname0, atomindex0,
			posi0, velo0, boxsize0);
  GroFileManager::read (rfilename, resdindex1, resdname1, atomname1, atomindex1,
			posi1, velo1, boxsize1);

  for (unsigned i = 0; i < posi0.size(); ++i){
    if (i == posi1.size()){
      std::cerr << "input and reference files are not consistent, exit" << std::endl;
      exit(1);
    }
    if (resdname0[i] == "WA"){
      if (resdname1[i] != "WA"){
	std::cerr << "input and reference files are not consistent, exit" << std::endl;
	exit(1);
      }
      posi0[i] = posi1[i];
    }
    if (resdname0[i] == "WB"){
      if (resdname1[i] != "WB"){
	std::cerr << "input and reference files are not consistent, exit" << std::endl;
	exit(1);
      }
      posi0[i] = posi1[i];
    }
  }

  GroFileManager::write (ofilename,
			 resdindex0, resdname0,
			 atomname0, atomindex0,
			 posi0, velo0, boxsize0);
  
  return 0;
}

  
