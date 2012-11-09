/* 
 * File:   computeW.h
 * Author: slee
 *
 * Created on October 20, 2012, 12:37 PM
 */

#ifndef COMPUTEW_H
#define	COMPUTEW_H

#include "Eigen/Dense"
#include "Eigen/Sparse"
#include <vector>

using namespace Eigen;
using namespace std;

struct dataW {
	dataW() : sampleRadius(10), sample_rate(0.3), edgeVariance(0.1) {}
	int sampleRadius;
	double sample_rate;
	double edgeVariance;
};

vector<Matrix<unsigned long,Dynamic,1>> cimgnbmap(const VectorXi& imgSize, const VectorXi& sampleRadius, double sample_rate = 1);

SparseMatrix<double> affinityic(const MatrixXd& m_emag, const MatrixXd& m_ephase, const Matrix<unsigned long, Dynamic, 1>& m_pi, const Matrix<unsigned long, Dynamic, 1>& m_pj, double sigma = -1);

SparseMatrix<double> computeW(const MatrixXd& imageX, const dataW dataWPar, const MatrixXd& emag, const MatrixXd& ephase);


#endif	/* COMPUTEW_H */

