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

#include "StringSplit.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char * argv[])
{
  std::string ifile;
  std::string ofile;
  po::options_description desc ("Allow options");  
  double shift, extend;
  
  desc.add_options()
      ("help,h", "print this message")
      ("shift,s", po::value<double > (&shift)->default_value (0., "shift value"))
      // ("shift,s"  po::value<double > (&shift )->default_value (0., "shift value"))
      ("extend,e", po::value<double > (&extend)->default_value (10., "extend"))
      ("output-file-name,o", po::value<std::string > (&ofile)->default_value (std::string("out.tf.xvg"), "output conf file name"))
      ("input-file-name,i",  po::value<std::string > (&ifile)->default_value (std::string("in.tf.xvg"), "input conf file name"));

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count("help") || vm.count("h")){
    std::cout << desc<< "\n";
    return 0;
  }
  if (shift < 0){
    std::cerr << "shift should be > 0" << std::endl;
    return 1;
  }

  std::vector<double > xx, uu, ff;

  FILE * fp = fopen (ifile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open file " << ifile << std::endl;
    return 1;
  }
  char line[10240];
  while (NULL != fgets (line, sizeof(line), fp)){
    if (line[0] == '#') continue;
    std::vector<std::string > out;
    StringOperation::split (std::string(line), out);
    if (out.size() != 3){
      std::cerr << "wrong file format" << std::endl;
      return 1;
    }
    xx.push_back (atof(out[0].c_str()));
    uu.push_back (atof(out[1].c_str()));
    ff.push_back (atof(out[2].c_str()));
  }
  fclose (fp);

  double xstep = xx[1] - xx[0];
  fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    return 1;
  }
  for (double myx = 0.; myx < xx[0]+shift-1e-6; myx += xstep){
    fprintf (fp, "%f %f %f\n", myx, uu[0], 0.);
  }
  for (unsigned i = 0; i < xx.size(); ++i){
    fprintf (fp, "%f %f %f\n", (xx[i] += shift), uu[i], ff[i]);
  }
  if (extend > xx.back()){
    for (double myx = xx.back() + xstep; myx <= extend; myx += xstep){
      fprintf (fp, "%f %f %f\n", myx, 0., 0.);
    }
  }
  fclose (fp);
  
  // std::vector<double > xx1, uu1, ff1;
  // std::vector<double > xx2, uu2, ff2;
  // for (double myx = 0.; myx < xx[0]; myx += xstep){
  //   xx1.push_back (myx);
  //   uu1.push_back (0.);
  //   ff1.push_back (0.);
  // }
  // for (unsigned i = 0; i < xx.size(); ++i){
  //   xx[i] += shift;
  // }
  // if (extend > xx.back()){
  //   for (double myx = extend + xstep; myx <= extend; myx += xstep){
  //     xx2.push_back(myx);
  //   uu2.push_back (0.);
  //   ff2.push_back (0.);
  //   }
  // }

  return 0;
}
