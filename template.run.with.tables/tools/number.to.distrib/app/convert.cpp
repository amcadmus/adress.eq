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
#include "BlockAverage.h"
using namespace std;

int main(int argc, char * argv[])
{
  std::string ofile;
  unsigned every,numBlocks;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("every,e",  po::value<unsigned > (&every)->default_value(1), "bin size")
      ("nblock,n", po::value<unsigned > (&numBlocks)->default_value(16), "number of blocks")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("number.distrib.out"), "the output file");
  
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  std::vector<int > data;
  std::vector<double > ddata;
  int tmpv;
  while (scanf ("%d", &tmpv) == 1){
    data.push_back (tmpv);
    ddata.push_back (double(tmpv));
  }

  int min = 1000000;
  int max = 0;
  for (unsigned ii = 0; ii < data.size(); ++ii){
    if (data[ii] < min) min = data[ii];
    if (data[ii] > max) max = data[ii];
  }
  min--;
  max++;
  std::vector<double > hist ((max - min)/every + 1, 0.);
  for (unsigned ii = 0; ii < data.size(); ++ii){
    hist[(data[ii] - min)/every] += 1. / double(data.size());
  }

  BlockAverage ba;
  ba.processData (ddata, numBlocks);

  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (unsigned ii = 0; ii < (max - min)/every + 1; ++ii){
    fprintf (fp, "%f %f\n", min + (ii + 0.5) * every, hist[ii]);
  }
  fclose (fp);

  if (data.size() == 0){
    cout << "no data" << endl;
    return 0;
  }
  double mu = 0.;
  double sigma = 0.;
  for (unsigned ii = 0; ii < data.size(); ++ii){
    mu += data[ii] / double(data.size());
  }
  for (unsigned ii = 0; ii < data.size(); ++ii){
    sigma += (data[ii] - mu) * (data[ii] - mu);
  }
  sigma = sqrt(sigma / (double(data.size()) - 1.));
  // double mu = ba.getAvg();
  // double sigma = sqrt(ba.getVar());
  cout << mu << " " << sigma << endl;
  
  int nprint = (max - min) * 10;
  double hh =  double(max - min) / double(nprint);
  ofile = string("gaussian") + ofile;
  fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    cerr << "cannot open file " << ofile << endl;
    return 1;
  }
  for (int ii = 0; ii < nprint; ++ii){
    double xx = double(min) + double(ii) * hh;
    double yy = 1./(sqrt(2.*M_PI) * sigma) * exp (-(xx - mu) * (xx - mu) / (2. * sigma * sigma));
    fprintf (fp, "%f %f\n", xx, yy);
  }
  fclose (fp);

  return 0;
}

