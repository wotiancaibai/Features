/* 
 * File:   computeEdges.h
 * Author: slee
 *
 * Created on October 17, 2012, 9:50 AM
 */

#ifndef COMPUTEEDGES_H
#define	COMPUTEEDGES_H

#include "Eigen/Dense"
#include <vector>

using namespace Eigen;
using namespace std;

struct edgeParams
{
	edgeParams() : n_filter(4), n_scale(3), winsz(21), enlong(3)
	{}
	edgeParams(int a, int b, int c, int d) : n_filter(a), n_scale(b), winsz(c), enlong(d) 
	{}	
	int n_filter;
	int n_scale;
	int winsz;
	int enlong;
};

double rcond(const MatrixXd& x);

MatrixXd gaussian(const MatrixXd& x, const MatrixXd& m, const MatrixXd& C);

pair<MatrixXd, MatrixXd> meshgrid(const VectorXd& lsp_x, const VectorXd& lsp_y);

MatrixXd doog1(int sig, int r, double th, int N);
MatrixXd doog2(int sig, int r, double th, int N);

vector<MatrixXd> make_filterbank(bool is_doog1, int num_ori, int filter_scales, int wsz, int enlong = 3);

MatrixXd fliplr(const MatrixXd& x);

MatrixXd flipud(const MatrixXd& x);

vector<MatrixXd> fft_filt_2(const MatrixXd& V, std::vector<MatrixXd>& FB);

vector<MatrixXd> quadedgep(const MatrixXd& I, const edgeParams par = edgeParams(4, 3, 21, 3));

/*
MatrixXd cannyEdge(MatrixXd imageX);
*/


#endif	/* COMPUTEEDGES_H */

