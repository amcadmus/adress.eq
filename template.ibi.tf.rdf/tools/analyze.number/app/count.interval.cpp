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

int main(int argc, char * argv[])
{
  float begin, end, x0, x1;
  std::string ifile, ofile, method;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("x0", po::value<float > (&x0)->default_value(0.f), "lower bound of the interval")
      ("x1", po::value<float > (&x1)->default_value(1.f), "upper bound of the interval")
      ("method,m",  po::value<std::string > (&method)->default_value ("adress"), "type of simulation to analyze")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("number.out"), "the output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# [x0, x1]: " << x0 << " " << x1 << std::endl;
  std::cout << "# method: " << method << std::endl;
  std::cout << "# input: " << ifile << std::endl;
  std::cout << "###################################################" << std::endl;  
  
  XDRFILE *fp;
  int natoms, step;
  float time;
  matrix box;
  rvec * xx;
  float prec = 1000;
  float time_prec = .01;

  char tmpfname[1024];
  strncpy (tmpfname, ifile.c_str(), 1023);
  int c;
  if ((c = read_xtc_natoms (tmpfname, &natoms)) == 0) {
    // printf ("%d %d\n", c, natoms);
    xx = (rvec *) malloc (sizeof(rvec) * natoms);
  }
  else {
    // printf ("%d %d\n", c, natoms);
    fprintf (stderr, "error read_xtc_natoms");
    exit (1);
  }

  fp = xdrfile_open (ifile.c_str(), "r");
  if (fp == NULL){
    std::cerr << "cannot open file " << ifile << std::endl;
    exit (1);
  }
  FILE *fout = fopen (ofile.c_str(), "w");
  if (fout == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    exit (1);
  }
  
  while (read_xtc (fp, natoms, &step, &time, box, xx, &prec) == 0){
    if (end != 0.f) {
      if (time < begin - time_prec){
	continue;
      }
      else if (time > end + time_prec) {
	break;
      }	
    }
    else {
      if (time < begin - time_prec) continue;
    }

    int count = 0;
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i*4][0] <  0        ) xx[i*4][0] += box[0][0];
	else if (xx[i*4][0] >= box[0][0]) xx[i*4][0] -= box[0][0];
	if (xx[i*4][0] >= x0 && xx[i*4][0] < x1){
	  count ++;
	}
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      for (int i = 0; i < nmol; ++i){
	float comx;
	float dx1, dx2;
	dx1 = xx[i*3+1][0] - xx[i*3+0][0];
	dx2 = xx[i*3+2][0] - xx[i*3+0][0];
	if (dx1 > 0.5 * box[0][0]) dx1 -= box[0][0];
	if (dx1 <-0.5 * box[0][0]) dx1 += box[0][0];
	if (dx2 > 0.5 * box[0][0]) dx2 -= box[0][0];
	if (dx2 <-0.5 * box[0][0]) dx2 += box[0][0];	
	comx = 16. * xx[i*3+0][0] +
	    1. * (xx[i*3+0][0] + dx1) +
	    1. * (xx[i*3+0][0] + dx2);
	if (comx >= x0 && comx < x1){
	  count ++;
	}
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i][0] <  0        ) xx[i][0] += box[0][0];
	else if (xx[i][0] >= box[0][0]) xx[i][0] -= box[0][0];
	if (xx[i][0] >= x0 && xx[i][0] < x1){
	  count ++;
	}
      }
    }

    fprintf (fout, "%f %d\n", time, count);
  }
  
  xdrfile_close (fp);
  free (xx);
  fclose (fout);
  
  return 0;
}

