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

using namespace std;

int main(int argc, char * argv[])
{
  if (argc != 2){
    cerr << "usage:" << endl;
    cerr << argv[0] << " filename" << endl;
    return 1;
  }
  
  FILE * fp = fopen (argv[1],"r");
  if (fp == NULL){
    cerr << "cannot open file " << argv[1] << endl;
    return 1;
  }
  vector<double > xx;
  vector<double > yy;
  double tmpx, tmpy;
  while (2 == fscanf (fp, "%lf %lf", &tmpx, &tmpy)){
    xx.push_back (tmpx);
    yy.push_back (tmpy);
  }
  fclose (fp);

  for (unsigned ii = 1; ii < yy.size()-1; ++ii){
    if (yy[ii-1] < 0.5 && yy[ii+1] < 0.5 && yy[ii] > 1.5){
      yy[ii] = 0.5 * (yy[ii-1] +  yy[ii+1]);
    }
  }
  
  fp = fopen (argv[1],"w");

  for (unsigned ii = 0; ii < yy.size(); ++ii){
    fprintf (fp, "%f %f\n", xx[ii], yy[ii]);
  }
  fclose (fp);

  return 0;
}
