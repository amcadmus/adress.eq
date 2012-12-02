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

#include <boost/program_options.hpp>
#include "StringSplit.h"
#define MaxLineLength 10240

namespace po = boost::program_options;

using namespace std;

int main(int argc, char * argv[])
{
  std::string i0file, i1file, ofile;
  char nameline0 [MaxLineLength];
  char nameline1 [MaxLineLength];
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("input-0",   po::value<std::string > (&i0file)->default_value ("rdf.0.out"), "the input rdf file")
      ("input-1",   po::value<std::string > (&i1file)->default_value ("rdf.1.out"), "the input rdf file")
      ("output,o",   po::value<std::string > (&ofile)->default_value ("rdf.diff.out"), "the input rdf file");
      
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  
  // std::cout << "###################################################" << std::endl;
  // std::cout << "# begin->end: " << begin << " " << end << std::endl;
  // std::cout << "# block: " << numBlock << std::endl;
  // std::cout << "# top file: " << tfile << std::endl;
  // std::cout << "###################################################" << std::endl;  


  std::ifstream fp0 (i0file.c_str());
  if (!fp0){
    std::cerr << "cannot open file " << i0file << std::endl;
    return 1;
  }
  std::ifstream fp1 (i1file.c_str());
  if (!fp1){
    std::cerr << "cannot open file " << i1file << std::endl;
    return 1;
  }
  FILE * fp = fopen (ofile.c_str(), "w");
  if (fp == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    return 1;
  }

  while (fp0.getline(nameline0, MaxLineLength)){
    fp1.getline(nameline1, MaxLineLength);
    // if (rvalue == 0){
    //   cerr << "inconsistent file " << endl;
    //   return 1;
    // }
    if (nameline0[0] == '#') continue;
    vector<string > ss0, ss1;
    StringOperation::split (string(nameline0), ss0);
    StringOperation::split (string(nameline1), ss1);
    if (ss0.size() != ss1.size()){
      cerr << "inconsistent line " << endl;
      return 1;
    }
    if (ss0.size() != 2){
      cerr << "wrong line colume " << nameline0  << endl;
      return 1;
    }
    else {
      double v0 = atof(ss0[1].c_str());
      double v1 = atof(ss1[1].c_str());
      fprintf (fp, "%s %.16e\n", ss0[0].c_str(), v0 - v1);
    }
  }

  fclose (fp);
  
  return 0;
}
