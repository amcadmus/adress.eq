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

#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_xtc.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include "StringSplit.h"
using namespace std;

int main(int argc, char * argv[])
{
  double x0, x1;
  std::string ifile, ofile, method;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "print this message")
    ("x0", po::value<double > (&x0)->default_value(0.f), "lower bound of the interval")
    ("x1", po::value<double > (&x1)->default_value(1.f), "upper bound of the interval")
    ("input,f",   po::value<std::string > (&ifile)->default_value ("in.tab"), "the input table")
    ("output,o",  po::value<std::string > (&ofile)->default_value ("out.tab"), "the output table");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  if (x0 > x1){
    double tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }

  FILE * fp = fopen (ifile.c_str(), "r");
  FILE * fpo = fopen (ofile.c_str(), "w");
  if (fp == NULL) {
    cerr << "cannot open file " << ifile << endl;
    exit (1);
  }
  if (fpo == NULL) {
    cerr << "cannot open file " << ifile << endl;
    exit (1);
  }
  
  char * line = NULL;
  size_t len = 0;
  vector<string > out;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1){
    if (line[0] =='#') continue;
    out.clear();
    StringOperation::split (line, out);
    if (out.size() < 3){
      cerr << "invalid line of file " << ifile << endl;
      return 1;
    }
    double xx = atof(out[0].c_str());
    double uu1 = atof(out[1].c_str());
    double ff1 = atof(out[2].c_str());
    double uu2 = atof(out[3].c_str());
    double ff2 = atof(out[4].c_str());
    double uu3 = atof(out[5].c_str());
    double ff3 = atof(out[6].c_str());
    if (xx >= x0 && xx <= x1){
      double swu = cos ((xx - x0) / (x1 - x0) * 0.5 * M_PI);
      swu = swu * swu;
      double swv =
	  2. *
	  cos ((xx - x0) / (x1 - x0) * 0.5 * M_PI) *
	  sin ((xx - x0) / (x1 - x0) * 0.5 * M_PI) * 0.5 * M_PI / (x1 - x0);
      ff1 = swv * uu1 + swu * ff1;
      uu1 *= swu;
      ff2 = swv * uu2 + swu * ff2;
      uu2 *= swu;
      ff3 = swv * uu3 + swu * ff3;
      uu3 *= swu;
      printf ("%f %f %f\n", xx, swu, swv);
    }
    if (xx <= x1){
      fprintf (fpo, "%f %.8e %.8e  %.8e %.8e  %.8e %.8e\n",
	       xx, uu1, ff1, uu2, ff2, uu3, ff3);
    }
    else{
      fprintf (fpo, "%f %.8e %.8e  %.8e %.8e  %.8e %.8e\n",
	       xx, 0., 0., 0., 0., 0., 0.);
    }
  }
  
  fclose (fp);
  fclose (fpo);

  free (line);
  return 0;
}

