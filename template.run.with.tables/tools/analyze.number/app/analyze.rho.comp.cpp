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
#include "BlockAverage.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
  float begin, end, refh;
  std::string ifile, ofile, method;
  float yzSize;
  unsigned numBlocks;
  
  po::options_description desc ("Allow options");
  desc.add_options()
      ("help,h", "print this message")
      ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
      ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
      ("refh", po::value<float > (&refh)->default_value(2.f), "x size of the bin")
      ("yz-size", po::value<float > (&yzSize), "yz size of the bin")
      ("method,m",  po::value<std::string > (&method)->default_value ("adress"), "type of simulation to analyze")
      ("nblock,n", po::value<unsigned > (&numBlocks)->default_value(16), "number of blocks")
      ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file")
      ("output,o",  po::value<std::string > (&ofile)->default_value ("number.out"), "the output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }
  if (!vm.count("yz-size")){
    yzSize = refh;
  }

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
    xx = (rvec *) malloc (sizeof(rvec) * natoms);
  }
  else {
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
  if (read_xtc (fp, natoms, &step, &time, box, xx, &prec) != 0) {
    std::cerr << "error reading file " << ifile << std::endl;
    return 1;
  }

  if (box[1][1] != box[2][2]){
    std::cerr << "unexpected box size" << std::endl;
    return 1;
  }
  int ny = box[1][1] / yzSize;
  yzSize = box[1][1] / double(ny);
  int nz = ny;
  int nyz = ny * nz;
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# x bin size: " << refh << std::endl;
  std::cout << "# actual yz bin size: " << yzSize << std::endl;
  std::cout << "# method: " << method << std::endl;
  std::cout << "# input: " << ifile << std::endl;
  std::cout << "###################################################" << std::endl;

  // int nmolecules = 0;
  if (method == std::string("adress")){
    // nmolecules = natoms / 4;
  }
  else if (method == std::string("atom")){
    // nmolecules = natoms / 3;
  }
  else if (method == std::string("cg")){
    // nmolecules = natoms;
  }
  else {
    std::cerr << "wrong method" << std::endl;
    return 1;
  }
  
  int nbin;
  float offset;
  float marginh;
  float ep = 1e-3;
  nbin = int(0.5 * box[0][0] / refh) * 2;
  if (fabs (box[0][0] - refh * nbin) <= ep){
    offset = 0.;
    marginh = 0.;
    fprintf (fout, "# box: %f %f %f, nbin: %d \n#",
	     box[0][0], box[1][1], box[2][2], nbin);
    for (int i = 0; i < nbin; ++i){
      fprintf (fout, " %f", refh);
    }
    fprintf (fout, "\n#");
    for (int i = 0; i < nbin; ++i){
      fprintf (fout, " %f", (0.5 + i) * refh);
    }
    fprintf (fout, "\n");
  }
  else {
    marginh = 0.5 * (box[0][0] - refh * nbin);
    offset = refh - marginh;
    nbin += 2;
    fprintf (fout, "# box: %f %f %f, nbin: %d \n#",
	     box[0][0], box[1][1], box[2][2], nbin);
    fprintf (fout, " %f", marginh);
    for (int i = 0; i < nbin-2; ++i){      
      fprintf (fout, " %f", refh);
    }
    fprintf (fout, " %f", marginh);
    fprintf (fout, "\n#");

    fprintf (fout, " %f", 0.5 * marginh);
    for (int i = 0; i < nbin-2; ++i){      
      fprintf (fout, " %f", marginh + (0.5 + i) * refh);
    }
    fprintf (fout, " %f", box[0][0] - 0.5 * marginh);
    fprintf (fout, "\n");
  }

  std::vector<std::vector<std::vector<double > > > counts;
  counts.resize (nbin);
  for (unsigned i = 0; i < counts.size(); ++i){
    counts[i].resize (nyz);
  }
  
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
    if (countread % 100 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }
    
    for (unsigned i = 0; i < counts.size(); ++i){
      for (unsigned j = 0; j < counts[i].size(); ++j){
	counts[i][j].push_back (0.);
      }
    }    
    
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      for (int i = 0; i < nmol; ++i){
	for (int dd = 0; dd < 3; ++dd){
	  if      (xx[i*4+3][dd] <  0          ) xx[i*4+3][dd] += box[dd][dd];
	  else if (xx[i*4+3][dd] >= box[dd][dd]) xx[i*4+3][dd] -= box[dd][dd];
	}
	int xposi = (xx[i*4+3][0] + offset) / refh;
	int yposi = (xx[i*4+3][1]) / yzSize;
	int zposi = (xx[i*4+3][2]) / yzSize;
	if (xposi >= nbin) {
	  std::cout << "out of range, xposi: " << xposi << " nbin: " << nbin << std::endl;
	  xposi = nbin - 1;
	}
	if (yposi >= ny){
	  std::cout << "out of range, yposi: " << yposi << " nbin: " << ny << std::endl;
	  yposi = ny - 1;
	}
	if (zposi >= nz){
	  std::cout << "out of range, zposi: " << zposi << " nbin: " << nz << std::endl;
	  zposi = nz - 1;
	}
	int yzposi = yposi * nz + zposi;
	counts[xposi][yzposi][countread] += 1.;
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      for (int i = 0; i < nmol; ++i){
	std::vector<float > com(3, 0.);
	for (int dd = 0; dd < 3; ++dd){
	  float dx1, dx2;
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
	int xposi = (com[0] + offset) / refh;
	int yposi = (com[1]) / yzSize;
	int zposi = (com[2]) / yzSize;
	if (xposi >= nbin) {
	  std::cout << "out of range, xposi: " << xposi << " nbin: " << nbin << std::endl;
	  xposi = nbin - 1;
	}
	if (yposi >= ny){
	  std::cout << "out of range, yposi: " << yposi << " nbin: " << ny << std::endl;
	  yposi = ny - 1;
	}
	if (zposi >= nz){
	  std::cout << "out of range, zposi: " << zposi << " nbin: " << nz << std::endl;
	  zposi = nz - 1;
	}
	int yzposi = yposi * nz + zposi;
	counts[xposi][yzposi][countread] += 1.;
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      for (int i = 0; i < nmol; ++i){
	for (int dd = 0; dd < 3; ++dd){
	  if      (xx[i][dd] <  0          ) xx[i][dd] += box[dd][dd];
	  else if (xx[i][dd] >= box[dd][dd]) xx[i][dd] -= box[dd][dd];
	}
	int xposi = (xx[i][0] + offset) / refh;
	int yposi = (xx[i][1]) / yzSize;
	int zposi = (xx[i][2]) / yzSize;
	if (xposi >= nbin) {
	  std::cout << "out of range, xposi: " << xposi << " nbin: " << nbin << std::endl;
	  xposi = nbin - 1;
	}
	if (yposi >= ny){
	  std::cout << "out of range, yposi: " << yposi << " nbin: " << ny << std::endl;
	  yposi = ny - 1;
	}
	if (zposi >= nz){
	  std::cout << "out of range, zposi: " << zposi << " nbin: " << nz << std::endl;
	  zposi = nz - 1;
	}
	int yzposi = yposi * nz + zposi;
	counts[xposi][yzposi][countread] += 1.;
      }
    }

    countread ++;
  }
  printf ("\n");
  
  xdrfile_close (fp);
  free (xx);

  // FILE *fout1 = fopen ("density.out", "w");
  // if (fout1 == NULL){
  //   std::cerr << "cannot open file " << ofile << std::endl;
  //   exit (1);
  // }

  BlockAverage ba;
  printf ("# rho  rho_error  comp  comp_error \n");
  for (unsigned i = 0; i < counts.size(); ++i){
    double avg_avg = 0.;
    double avg_avg_err= 0.;
    double avg_var= 0.;
    double avg_var_err= 0.;
    for (unsigned j = 0; j < counts[i].size(); ++j){
      ba.processData (counts[i][j], numBlocks);
      avg_avg += ba.getAvg();
      avg_avg_err += ba.getAvgError();
      avg_var += ba.getVar();
      avg_var_err += ba.getVarError();
      // printf ("ndata: %d\n", counts[i][j].size());
    }
    double scalor = 1./double(counts[i].size());
    avg_avg *= scalor;
    avg_avg_err *= 2. * sqrt(scalor);
    avg_var *= scalor;
    avg_var_err *= 2. * sqrt(scalor);
    double myx;
    if (fabs (box[0][0] - refh * nbin) <= ep){
      myx = (0.5 + i) * refh;
    }
    else {
      if (i == 0) myx = 0.5 * marginh;
      else if (int(i) < nbin - 1) myx = marginh + (i - 0.5) * refh;
      else myx = box[0][0] - 0.5 * marginh;
    }
    fprintf (fout, "%f %e %e   %e %e   %e\n",
	     myx,
	     avg_avg, avg_avg_err,
	     avg_var, avg_var_err,
	     avg_var / avg_avg);

    double binSize;
    if (fabs (box[0][0] - refh * nbin) <= ep){
      binSize = refh;
    }
    else {
      if (i == 0) binSize = marginh;
      else if (int(i) < nbin - 1) binSize = refh;
      else binSize = marginh;
    }
    // printf ("# my bin size is %f\n", binSize);
    double volume = binSize * yzSize * yzSize;
    double temperature = 300.;
    double rho = avg_avg / volume;
    double rho_err = avg_avg_err / volume;
    rho *= 18e-3 / (6.02e23 * 1e-27);
    rho_err *= 18e-3 / (6.02e23 * 1e-27);
    double comp = avg_var / (avg_avg * avg_avg) * volume / (1.38 *temperature) * 1e-4;
    double comp_err =
	avg_var_err / (avg_avg * avg_avg) +
	2 * avg_var * avg_avg_err / (avg_avg * avg_avg * avg_avg);
    comp_err *= volume / (1.38 *temperature) * 1e-4;
    printf ("%f %e  %e  %e  %e \n",
	    myx,
	    rho, rho_err,
	    comp, comp_err);
  }

  

  fclose (fout);
  
  return 0;
}

