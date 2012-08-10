#ifndef __Rdf3_h_wanghan__
#define __Rdf3_h_wanghan__

#include "Defines.h"
#include "CellList.h"
#include "Distribution.h"

using namespace std;

class Rdf3
{
  int nbins;
  ValueType rup;
  ValueType binSize;
  ValueType offset;
  int nframe;
  ValueType rho;
  ValueType natom;
  ValueType x0, x1;
  vector<ValueType > hist;
  double h1low, h1up;
  double h1extend;
  double h2extend;
  int nh1, nh2;
  vector<Distribution_Cylinder > dists;
public:
  const ValueType & getValue (const int & i) const {return hist[i];}
  unsigned getN () const {return hist.size();}
  void reinit (const ValueType rup,
	       const ValueType refh,
	       const ValueType x0,
	       const ValueType x1,
	       const double & h1extend,
	       const double & h2extend);
  void deposit (const std::vector<std::vector<ValueType> > & coord,
		const VectorType & box,
		const CellList & clist);
  void calculate ();
  void print (const string & fileBaseName);
}
    ;


#endif
