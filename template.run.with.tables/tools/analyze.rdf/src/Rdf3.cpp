#include "Rdf3.h"
#include <iostream>
#include <stdio.h>

void Rdf3::
reinit (const ValueType rup_,
	const ValueType refh_,
	const ValueType x0_,
	const ValueType x1_,
	const double & h1extend_,
	const double & h2extend_)
{
  rup = rup_;
  binSize = refh_;
  x0 = x0_;
  x1 = x1_;
  if (x0 > x1) {
    ValueType tmpx = x0;
    x0 = x1;
    x1 = tmpx;
  }  
  nbins = rup / refh_ + 1;
  rup = binSize * (nbins - .5);
  offset = .5 * binSize;
  hist.resize (nbins);
  std::fill (hist.begin(), hist.end(), 0.);
  nframe = 0;
  rho = 0.;
  natom = 0.;

  h1extend = (int(h1extend_ / binSize)) * binSize;
  h2extend = (int(h2extend_ / binSize)) * binSize;
  h1low = -0.5 * binSize - h1extend;
  h1up  = rup + h1extend;
  nh1 = int((h1up - h1low + 0.5 * binSize) / binSize);
  nh2 = int((h2extend + 0.5 * binSize) / binSize);
  dists.resize (nbins);
  for (int ii = 0; ii < nbins; ++ii){
    dists[ii].reinit (h1low, h1up, nh1, 0., h2extend, nh2);
  }
}



void Rdf3::
deposit (const std::vector<std::vector<ValueType> > & coord,
	 const VectorType & box,
	 const CellList & clist)
{
  int xiter = rup / clist.getCellSize().x;
  if (xiter * clist.getCellSize().x < rup) xiter ++;
  int yiter = rup / clist.getCellSize().y;
  if (yiter * clist.getCellSize().y < rup) yiter ++;
  int ziter = rup / clist.getCellSize().z;
  if (ziter * clist.getCellSize().z < rup) ziter ++;

  IntVectorType nCell = clist.getNumCell();
  ValueType myNatom = 0.;

  printf ("\n");
  
  for (unsigned iCellIndex = 0; iCellIndex < unsigned(nCell.x * nCell.y * nCell.z); ++iCellIndex){
    // myNatom += clist.getList()[iCellIndex].size();
    printf ("## calculating cell %d in %d       \r",
	    iCellIndex, unsigned(nCell.x * nCell.y * nCell.z));
    fflush (stdout);
    std::vector<unsigned > neighborCellIndex =
	clist.neighboringCellIndex (iCellIndex, IntVectorType (xiter, yiter, ziter));
    for (unsigned iNeighborCellIndex = 0; iNeighborCellIndex < neighborCellIndex.size(); ++iNeighborCellIndex){
      unsigned jCellIndex = neighborCellIndex[iNeighborCellIndex];
      for (unsigned ii = 0; ii < clist.getList()[iCellIndex].size(); ++ii){
	VectorType icoord;
	icoord.x = coord[clist.getList()[iCellIndex][ii]][0];
	if (x1 != 0. && (!(icoord.x >= x0 && icoord.x < x1))) continue;
	if (iNeighborCellIndex == 0) myNatom += 1.;
	icoord.y = coord[clist.getList()[iCellIndex][ii]][1];
	icoord.z = coord[clist.getList()[iCellIndex][ii]][2];
	bool sameCell (iCellIndex == jCellIndex);
	for (unsigned jj = 0; jj < clist.getList()[jCellIndex].size(); ++jj){
	  if (sameCell && ii == jj) continue;	    
	  VectorType jcoord;
	  jcoord.x = coord[clist.getList()[jCellIndex][jj]][0];
	  jcoord.y = coord[clist.getList()[jCellIndex][jj]][1];
	  jcoord.z = coord[clist.getList()[jCellIndex][jj]][2];
	  VectorType diff;
	  diff.x = - icoord.x + jcoord.x;
	  diff.y = - icoord.y + jcoord.y;
	  diff.z = - icoord.z + jcoord.z;
	  if      (diff.x < -.5 * box.x) diff.x += box.x;
	  else if (diff.x >= .5 * box.x) diff.x -= box.x;
	  if      (diff.y < -.5 * box.y) diff.y += box.y;
	  else if (diff.y >= .5 * box.y) diff.y -= box.y;
	  if      (diff.z < -.5 * box.z) diff.z += box.z;
	  else if (diff.z >= .5 * box.z) diff.z -= box.z;
	  ValueType dr = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	  dr = sqrt (dr);
	  unsigned index = (dr + offset) / binSize;
	  if (dr < rup){
	    if (index >= unsigned(nbins)){
	      // printf ("# dr: %f, index: %d, rup: %f, nbins: %d\n",
	      // 	      dr, index, rup, nbins);
	      index = nbins - 1;
	    }
	    hist[index] += 1.;
	    
	    double tmpx = h1extend + rup;
	    // if (tmpx < rup) tmpx = rup;
	    double body3range = sqrt(tmpx * tmpx + h2extend * h2extend);
	    int body3xiter = body3range / clist.getCellSize().x;
	    if (body3xiter * clist.getCellSize().x < body3range) body3xiter ++;
	    int body3yiter = body3range / clist.getCellSize().y;
	    if (body3yiter * clist.getCellSize().y < body3range) body3yiter ++;
	    int body3ziter = body3range / clist.getCellSize().z;
	    if (body3ziter * clist.getCellSize().z < body3range) body3ziter ++;
	    std::vector<unsigned > body3NeighborCellIndex =
		clist.neighboringCellIndex (iCellIndex, IntVectorType (body3xiter, body3yiter, body3ziter));
	    for (unsigned ibody3Cell = 0; ibody3Cell < body3NeighborCellIndex.size(); ++ibody3Cell){
	      unsigned kCellIndex = body3NeighborCellIndex[ibody3Cell];
	      for (unsigned kk = 0; kk < clist.getList()[kCellIndex].size(); ++kk){
		if (clist.getList()[iCellIndex][ii] == clist.getList()[kCellIndex][kk] ||
		    clist.getList()[jCellIndex][jj] == clist.getList()[kCellIndex][kk] ) continue;
		VectorType kcoord;
		kcoord.x = coord[clist.getList()[kCellIndex][kk]][0];
		kcoord.y = coord[clist.getList()[kCellIndex][kk]][1];
		kcoord.z = coord[clist.getList()[kCellIndex][kk]][2];
		VectorType diff2;
		diff2.x = - icoord.x + kcoord.x;
		diff2.y = - icoord.y + kcoord.y;
		diff2.z = - icoord.z + kcoord.z;
		if      (diff2.x < -.5 * box.x) diff2.x += box.x;
		else if (diff2.x >= .5 * box.x) diff2.x -= box.x;
		if      (diff2.y < -.5 * box.y) diff2.y += box.y;
		else if (diff2.y >= .5 * box.y) diff2.y -= box.y;
		if      (diff2.z < -.5 * box.z) diff2.z += box.z;
		else if (diff2.z >= .5 * box.z) diff2.z -= box.z;
		double h1 = (diff.x * diff2.x +
			     diff.y * diff2.y + 
			     diff.z * diff2.z ) / dr;
		if (h1 <= h1low || h1 >= h1up) continue;
		double dr22 = sqrt (diff2.x * diff2.x + diff2.y * diff2.y + diff2.z * diff2.z);
		double h2 = sqrt(dr22 - h1 * h1);
		if (h2 >= h2extend) continue;
		dists[index].deposit (h1, h2);
	      }
	    }	
	  }
	}
      }
    }
  }

  printf ("\n");
  nframe ++;
  if (x1 == x0){
    rho += myNatom / (box.x * box.y * box.z);
  }
  else {
    rho += myNatom / ((x1 - x0) * box.y * box.z);
  }
  natom += myNatom;
}



void Rdf3::
calculate()
{
  rho /= double(nframe);
  for (int i = 0; i < nbins; ++i){
    hist[i] /= double(natom);
  }
  {
    double r = 0.5 * binSize;
    hist[0] /= 4. / 3. * M_PI * r * r * r * rho;
  }
  for (int i = 1; i < nbins; ++i){
    double r0 = (i-0.5) * binSize;
    double r1 = (i+0.5) * binSize;
    // double r01 = i * binSize;
    hist[i] /= 4. / 3. * M_PI * (r1*r1*r1 - r0*r0*r0) * rho;
    // hist[i] /= 4. * M_PI * r0 * r1 * (r1 - r0) * rho;
    dists[i].average(1. / (4. / 3. * M_PI * (r1*r1*r1 - r0*r0*r0) * rho * rho * double(natom)));
  }

  // for (unsigned ii = 0; ii < dists.size(); ++ii){
  //   dists[ii].average();
  // }
}


void
Rdf3::print (const string & fileBaseName)
{
  char name[2048];
  for (int ii = 0; ii < nbins; ++ii){
    double rr = ii * binSize;
    int intrr = int(rr);
    int decirr = int((rr - intrr) * 1000);
    sprintf (name, "%s.%02d.%03d.out", fileBaseName.c_str(), intrr, decirr);
    dists[ii].print_xv (string(name));
  }
}


