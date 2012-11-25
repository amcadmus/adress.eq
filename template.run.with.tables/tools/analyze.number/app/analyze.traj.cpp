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

void trajAnalyzerInOut (const vector<double > & timeRecord,
			const vector<vector<int> > & indicator,
			const double & minInterval,
			FILE * fpout)
{
  double lastTime = timeRecord[0];
  double lastIdx = 0;
  for (unsigned tt = 1; tt < timeRecord.size(); ++tt){
    if (timeRecord[tt] - lastTime >= minInterval){
      int countIn = 0;
      int countOut = 0;
      for (unsigned ii = 0; ii < indicator.size(); ++ii){
	if (indicator[ii][lastIdx] ==-1 && indicator[ii][tt] == 1){
	  countIn ++;
	}
	if (indicator[ii][lastIdx] == 1 && indicator[ii][tt] ==-1){
	  countOut ++;
	}
      }
      fprintf (fpout, "%f %d %d\n", timeRecord[tt], countIn, countOut);
      lastTime = timeRecord[tt];
      lastIdx = tt;
    }
  } 
}

void trajAnalyzerStay (const vector<double > & timeRecord,
		       const vector<vector<int> > & indicator,
		       const double & intervalStay,
		       FILE * fpout)
{
  vector<vector<double> > intervalLengths (indicator.size());
  for (unsigned ii = 0; ii < indicator.size(); ++ii){
    int status = 0;  // 0 not counting, 1, -1 counting
    // double currentIntervalLength = timeRecord[1] - timeRecord[0];
    double currentIntervalLength = 0.0;
    for (unsigned tt = 0; tt < indicator[ii].size(); ++tt){
      if (status == 0){
	status = indicator[ii][tt];
      }
      else {
	if (status == indicator[ii][tt]){
	  currentIntervalLength += timeRecord[tt] - timeRecord[tt-1];
	}
	else {
	  intervalLengths[ii].push_back (currentIntervalLength * status);
	  // currentIntervalLength = timeRecord[tt] - timeRecord[tt-1];
	  currentIntervalLength = 0.;
	  status = indicator[ii][tt];
	}	
      }
    }
    intervalLengths[ii].push_back (currentIntervalLength * status);
    // if (ii == 2623){
    //   cout << "here" << endl;}
  }
  vector<vector<double > > largeIntervalLengths (indicator.size());
  for (unsigned ii = 0; ii < indicator.size(); ++ii){
    for (unsigned jj = 0; jj < intervalLengths[ii].size(); ++jj){
      if (fabs(intervalLengths[ii][jj]) > intervalStay){
	largeIntervalLengths[ii].push_back (intervalLengths[ii][jj]);
      }
    }
  }

  for (unsigned ii = 0; ii < indicator.size(); ++ii){
    if (largeIntervalLengths[ii].size() <= 1) continue;
    int startIdx = 0;
    bool findOut = false;
    for (unsigned jj = 0; jj < largeIntervalLengths[ii].size(); ++jj){
      if (largeIntervalLengths[ii][jj] < 0){
	startIdx = jj;
	findOut = true;
	break;
      }
    }
    if (findOut == true){
      for (unsigned jj = startIdx+1; jj < largeIntervalLengths[ii].size(); ++jj){
	if (largeIntervalLengths[ii][jj] > 0){
	  fprintf (fpout, "%d ", ii);
	  for (unsigned kk = 0; kk < largeIntervalLengths[ii].size(); ++kk){
	    fprintf (fpout, "%f ", largeIntervalLengths[ii][kk]);
	  }
	  fprintf (fpout, "\n");
	  break;
	}
      }
    }
    // double status = largeIntervalLengths[ii][0];
    // for (unsigned jj = 1; jj < largeIntervalLengths[ii].size(); ++jj){
    //   if (largeIntervalLengths[ii][jj] * status < 0) {
    // 	fprintf (fpout, "%d ", ii);
    // 	for (unsigned kk = 0; kk < largeIntervalLengths[ii].size(); ++kk){
    // 	  fprintf (fpout, "%f ", largeIntervalLengths[ii][kk]);
    // 	}
    // 	fprintf (fpout, "\n");
    // 	break;
    //   }
    // }
  }
}


int main(int argc, char * argv[])
{
  float begin, end, x0, x1, yz0, yz1;
  std::string ifile, ofile, method;
  double minIntervalSize;
  double intervalOfStay;
  
  po::options_description desc ("Allow options");
  desc.add_options()
    ("help,h", "print this message")
    ("begin,b", po::value<float > (&begin)->default_value(0.f), "start time")
    ("end,e",   po::value<float > (&end  )->default_value(0.f), "end   time")
    ("x0", po::value<float > (&x0)->default_value(0.f), "lower bound of the interval")
    ("x1", po::value<float > (&x1)->default_value(1.f), "upper bound of the interval")
    ("yz0", po::value<float > (&yz0)->default_value(0.f), "lower bound of the interval")
    ("yz1", po::value<float > (&yz1)->default_value(1.f), "upper bound of the interval")
    ("min-time-interval", po::value<double > (&minIntervalSize)->default_value(1.), "minial time interval of interest")
    ("time-of-stay", po::value<double > (&intervalOfStay)->default_value(300.), "minial Time Interval Of Interest")
    ("method,m",  po::value<std::string > (&method)->default_value ("adress"), "type of simulation to analyze")
    ("input,f",   po::value<std::string > (&ifile)->default_value ("traj.xtc"), "the input .xtc file")
    ("output,o",  po::value<std::string > (&ofile)->default_value ("countEnter.out"), "the output file");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify (vm);
  if (vm.count("help")){
    std::cout << desc<< "\n";
    return 0;
  }

  if (x0 > x1){
    float tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }
  if (yz0 > yz1){
    float tmpyz = yz0;
    yz0 = yz1;
    yz1 = tmpyz;
  }
  
  std::cout << "###################################################" << std::endl;
  std::cout << "# begin->end: " << begin << " " << end << std::endl;
  std::cout << "# [x0,  x1 ]: " << x0 << " " << x1 << std::endl;
  std::cout << "# [yz0, yz1]: " << yz0 << " " << yz1 << std::endl;
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
  
  int countread = 0;
  std::vector<double > time_counts;
  std::vector<double > timeRecord;
  std::vector<std::vector<int > > trajIndicator;
  
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
    if (countread++ % 100 == 0){
      printf ("# load frame at time: %.1f ps\r", time);
      fflush (stdout);
    }
    
    int count = 0;
    if (method == std::string ("adress")){
      int nmol = natoms / 4;
      if (trajIndicator.size() == 0) trajIndicator.resize(nmol);
      for (int i = 0; i < nmol; ++i){
	if      (xx[i*4+3][0] <  0        ) xx[i*4+3][0] += box[0][0];
	else if (xx[i*4+3][0] >= box[0][0]) xx[i*4+3][0] -= box[0][0];
	if      (xx[i*4+3][1] <  0        ) xx[i*4+3][1] += box[1][1];
	else if (xx[i*4+3][1] >= box[1][1]) xx[i*4+3][1] -= box[1][1];
	if      (xx[i*4+3][2] <  0        ) xx[i*4+3][2] += box[2][2];
	else if (xx[i*4+3][2] >= box[2][2]) xx[i*4+3][2] -= box[2][2];
	if (xx[i*4+3][0] >= x0 && xx[i*4+3][0] < x1 &&
	    xx[i*4+3][1] >= yz0 && xx[i*4+3][1] < yz1 &&
	    xx[i*4+3][2] >= yz0 && xx[i*4+3][2] < yz1){
	  trajIndicator[i].push_back(1);
	}
	else {
	  trajIndicator[i].push_back(-1);
	}
      }
    }
    else if (method == std::string ("atom")){
      int nmol = natoms / 3;
      if (trajIndicator.size() == 0) trajIndicator.resize(nmol);
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
	if (com[0] >= x0 && com[0] < x1 &&
	    com[1] >= yz0 && com[1] < yz1 &&
	    com[2] >= yz0 && com[2] < yz1
	    ){
	  trajIndicator[i].push_back(1);
	}
	else {
	  trajIndicator[i].push_back(-1);
	}
      }
    }
    else if (method == std::string ("cg")){
      int nmol = natoms;
      if (trajIndicator.size() == 0) trajIndicator.resize(nmol);
      for (int i = 0; i < nmol; ++i){
	if      (xx[i][0] <  0        ) xx[i][0] += box[0][0];
	else if (xx[i][0] >= box[0][0]) xx[i][0] -= box[0][0];
	if      (xx[i][1] <  0        ) xx[i][1] += box[1][1];
	else if (xx[i][1] >= box[1][1]) xx[i][1] -= box[1][1];
	if      (xx[i][2] <  0        ) xx[i][2] += box[2][2];
	else if (xx[i][2] >= box[2][2]) xx[i][2] -= box[2][2];
	if (xx[i][0] >= x0 && xx[i][0] < x1 &&
	    xx[i][1] >= yz0 && xx[i][1] < yz1 &&
	    xx[i][2] >= yz0 && xx[i][2] < yz1){
	  trajIndicator[i].push_back(1);
	}
	else {
	  trajIndicator[i].push_back(-1);
	}
      }
    }

    // fprintf (fout, "%f %d\n", time, count);
    timeRecord.push_back (time);
    time_counts.push_back (double(count));
  }
  printf ("\n");

  trajAnalyzerInOut (timeRecord, trajIndicator, minIntervalSize, fout);
  trajAnalyzerStay  (timeRecord, trajIndicator, intervalOfStay,  stdout);
  
  xdrfile_close (fp);
  free (xx);
  fclose (fout);
  
  return 0;
}

