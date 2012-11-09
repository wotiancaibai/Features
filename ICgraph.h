/* 
 * File:   ICgraph.h
 * Author: slee
 *
 * Created on October 21, 2012, 2:57 PM
 */

#ifndef ICGRAPH_H
#define	ICGRAPH_H
#include "computeEdges.h"
#include "computeW.h"

SparseMatrix<double> ICgraph(const MatrixXd& I, const dataW dataWPar = dataW(), const edgeParams dataMapPar = edgeParams());


#endif	/* ICGRAPH_H */

