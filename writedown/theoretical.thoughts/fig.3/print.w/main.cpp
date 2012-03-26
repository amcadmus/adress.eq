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


int main(int argc, char * argv[])
{
  double dex, dhy, dcg;
  double rc;
  dex = 0.5;
  dhy = 2.75;
  dcg = 0.5;
  rc = 0.9;
  int num = 1000;
  double refh = dhy / double(num);
  
  FILE * fp;
  
  fp = fopen ("old.w.out", "w");
  fprintf (fp, "%f %f\n", 0., 1.);
  fprintf (fp, "%f %f\n", dex, 1.);
  for (int i = 0; i < num; ++i){
    double xx = (i+1) * refh + dex;
    double value = cos (M_PI / (2. * dhy) * (xx - dex));
    fprintf (fp, "%f %f\n", xx, value * value);
  }
  fprintf (fp, "%f %f\n", dex + dhy + dcg , 0.);
  fclose (fp);

  fp = fopen ("new.w.out", "w");
  fprintf (fp, "%f %f\n", 0., 1.);
  fprintf (fp, "%f %f\n", dex, 1.);
  for (int i = 0; i < num; ++i){
    double xx = (i+1) * refh + dex;
    double value;
    if (xx < dex + rc){
      value = 1.;
    }
    else {
      value = cos (M_PI / (2. * (dhy - rc)) * (xx - dex - rc));
    }
    fprintf (fp, "%f %f\n", xx, value * value);
  }
  fprintf (fp, "%f %f\n", dex + dhy + dcg , 0.);
  fclose (fp);

  
}
