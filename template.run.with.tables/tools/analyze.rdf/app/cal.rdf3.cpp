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
#include "Rdf3.h"

int main(int argc, char * argv[])
{
  ValueType begin, end, rup, refh, cellSize;
  std::string ifile, ofile, method;
  ValueType x0, x1;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "print this message")
    ("begin,b", po::value<ValueType > (&begin)->default_value(0.f), "start time")
    ("end,e",   po::value<ValueType > (&end  )->default_value(0.f), "end   time")
    ("x0", po::value<ValueType > (&x0)->default_value(0.f), "lower bound of the interval")
    ("x1", po::value<ValueType > (&x1)->default_value(1.f), "upper bound of the interval, if x1 == 0, use the whole box")
    ("rup,u",   po::value<ValueType > (&rup)->default_value(1.0f), "max r to make rdf")
    ("refh",  po::value<ValueType > (&refh)->default_value(0.1f), "bin size")
    ("cell-size,c", po::value<ValueType > (&cellSize)->default_value(1.f), "cell list radius")
    ("method,m",  po::value<std::string > (&method)->default_value ("adress"), "type of simulation to analyze")
    ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file")
    ("output,o",  po::value<std::string > (&ofile)->default_value ("rdf.out"), "the output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  if (x0 > x1) {
    ValueType tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }  
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# [x0,  x1 ]: " << x0 << " " << x1 << std::endl;
  std::cout << "# rup: " << rup << std::endl;
  std::cout << "# refh: " << refh << std::endl;
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
  if (read_xtc (fp, natoms, &step, &time, box, xx, &prec) != 0) {
    std::cerr << "error reading file " << ifile << std::endl;
    return 1;
  }
  
  int nmolecules = 0;
  if (method == std::string("adress")){
    nmolecules = natoms / 4;
  }
  else if (method == std::string("atom")){
    nmolecules = natoms / 3;
  }
  else if (method == std::string("cg")){
    nmolecules = natoms;
  }
  else {
    std::cerr << "wrong method" << std::endl;
    return 1;
  }

  VectorType vbox;
  vbox.x = box[0][0];
  vbox.y = box[1][1];
  vbox.z = box[2][2];
  if (cellSize >= .5 * vbox.x){
    std::cerr << "the cell size should be less than half of the box size" << std::endl;
    return 1;
  }
  
  std::vector<std::vector<ValueType > > coms;
  coms.reserve (nmolecules);

  CellList clist (nmolecules, vbox, cellSize);

  Rdf3 myrdf;
  myrdf.reinit (rup, refh, x0, x1, rup*1, rup*1);

  int countread = 0;
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
    if (countread++ % 1 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }
    
    coms.clear ();
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i*4+3][0] <  0        ) xx[i*4+3][0] += box[0][0];
	else if (xx[i*4+3][0] >= box[0][0]) xx[i*4+3][0] -= box[0][0];
	if      (xx[i*4+3][1] <  0        ) xx[i*4+3][1] += box[1][1];
	else if (xx[i*4+3][1] >= box[1][1]) xx[i*4+3][1] -= box[1][1];
	if      (xx[i*4+3][2] <  0        ) xx[i*4+3][2] += box[2][2];
	else if (xx[i*4+3][2] >= box[2][2]) xx[i*4+3][2] -= box[2][2];
	std::vector<ValueType > tmp(3);
	tmp[0] = xx[i*4+3][0];
	tmp[1] = xx[i*4+3][1];
	tmp[2] = xx[i*4+3][2];
	coms.push_back(tmp);
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      for (int i = 0; i < nmol; ++i){
	std::vector<ValueType > com(3, 0.);
	for (int dd = 0; dd < 3; ++dd){
	  ValueType dx1, dx2;
	  dx1 = xx[i*3+1][dd] - xx[i*3+0][dd];
	  dx2 = xx[i*3+2][dd] - xx[i*3+0][dd];
	  if (dx1 > 0.5 * box[dd][dd]) {dx1 -= box[dd][dd]; printf ("hit\n");}
	  if (dx1 <-0.5 * box[dd][dd]) {dx1 += box[dd][dd]; printf ("hit\n");}
	  if (dx2 > 0.5 * box[dd][dd]) {dx2 -= box[dd][dd]; printf ("hit\n");}
	  if (dx2 <-0.5 * box[dd][dd]) {dx2 += box[dd][dd]; printf ("hit\n");}
	  com[dd] = 16. * xx[i*3+0][dd] +
	    1. * (xx[i*3+0][dd] + dx1) +
	    1. * (xx[i*3+0][dd] + dx2);
	  com[dd] /= 18.;
	  if      (com[dd] <  0          ) com[dd] += box[dd][dd];
	  else if (com[dd] >= box[dd][dd]) com[dd] -= box[dd][dd];
	}
	coms.push_back (com);
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      for (int i = 0; i < nmol; ++i){
	if      (xx[i][0] <  0        ) xx[i][0] += box[0][0];
	else if (xx[i][0] >= box[0][0]) xx[i][0] -= box[0][0];
	if      (xx[i][1] <  0        ) xx[i][1] += box[1][1];
	else if (xx[i][1] >= box[1][1]) xx[i][1] -= box[1][1];
	if      (xx[i][2] <  0        ) xx[i][2] += box[2][2];
	else if (xx[i][2] >= box[2][2]) xx[i][2] -= box[2][2];
	std::vector<ValueType > tmp(3);
	tmp[0] = xx[i][0];
	tmp[1] = xx[i][1];
	tmp[2] = xx[i][2];
	coms.push_back(tmp);
      }
    }

    clist.rebuild (coms);
    myrdf.deposit (coms, vbox, clist);
  }
  printf ("\n");
  
  xdrfile_close (fp);
  free (xx);


  myrdf.calculate();
  myrdf.print ("rdf3");
  
  FILE *fout = fopen (ofile.c_str(), "w");
  if (fout == NULL){
    std::cerr << "cannot open file " << ofile << std::endl;
    exit (1);
  }

  fprintf (fout, "%f %f\n", 0., myrdf.getValue(0));
  for (unsigned i = 1; i < myrdf.getN(); ++i){
    fprintf (fout, "%f %f\n", (i) * refh, myrdf.getValue(i));
  }

  fclose (fout);

  return 0;
}

