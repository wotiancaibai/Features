/*
 * ncutW: normalized cuts on a general graph
 *
 * translated from Shi et al's matlab implementation
 *
 * Evan Herbst
 * 10 / 13 / 11
 */
//#include <iomanip>
#include <cassert>
#include <cmath>
//#include <iostream>
//#include <fstream>
#include <algorithm>
#include <stdexcept>
#include "Eigen/Eigenvalues"
#include "arpack++/arssym.h" //arpack++ ARSymStdEig
#include "matrixUtils.h"
#include "ncutW.h"
#include "discretisation.h"
#include "sparsifyc.h"
#include "spmtimesd.h"
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using namespace Eigen;

double getSign(double x)
{
	return (x < 0) ? -1 : (x > 0) ? 1 : 0;
}

/*
 * compute the value of the normalized cuts objective for the given clustering (asgns[i] in [0, numClusters))
 */

double ncutObjective(const SparseMatrix<double>& W, const unsigned int numClusters, const std::vector<unsigned int>& asgns)
{
	assert(W.rows() == W.cols());
	assert(asgns.size() == W.rows());
	for(auto i = asgns.begin(); i != asgns.end(); i++) assert(*i < numClusters);

	vector<double> nums(numClusters, 0), denoms(numClusters, 0);
	for(unsigned int j = 0; j < W.cols(); j++)
		for(SparseMatrix<double>::InnerIterator it(W, j); it; ++it)
		{
			const unsigned int i = it.row();
			assert(it.value() >= 0);
			if(asgns[i] != asgns[j]) nums[asgns[i]] += it.value();
			denoms[asgns[i]] += it.value();
		}
	double ncut = 0;
	for(unsigned int i = 0; i < numClusters; i++)
		if(denoms[i] > 0)
			ncut += nums[i] / denoms[i];
	return ncut;
}

//template <typename SparseMatrixT>
class arpackWeightMatrixMultiplierFunctor
{
	public:

		arpackWeightMatrixMultiplierFunctor(const /*SparseMatrixT*/SparseMatrix<double>& w) : m(w)
		{}
		virtual ~arpackWeightMatrixMultiplierFunctor() {}

		/*
		 * b <- A * x
		 */
		void operator () (double* const x, double* const b)
		{
			//TODO make use of sparse
			/*
			VectorXd xv(m.cols());
			for(unsigned int i = 0; i < m.cols(); i++) 
                xv[i] = x[i];
			VectorXd bv = m.selfadjointView<Lower>() * xv;
			for(unsigned int i = 0; i < m.rows(); i++) 
                b[i] = bv[i];
			*/

			Map<VectorXd> xv(x, m.cols());
			Map<VectorXd> bv(b, m.rows());
			bv = m.selfadjointView<Lower>() * xv;
			
		}

	private:
		/*SparseMatrixT*/SparseMatrix<double> m;
};

/*
 * %     dataNcut= optional parameters for ncut
%
%     default parameters for dataNcut:
%     dataNcut.offset = 5e-1; offset in the diagonal of W
%     dataNcut.verbose = 0; 0 for verbose off mode, 1,2,3 for verbose on modes
%     dataNcut.maxiterations = 100; max number of iterations in eigensolver
%     dataNcut.eigsErrorTolerance = 1e-6; error tolerance in eigensolver
%     dataNcut.valeurMin=1e-6; % truncates any values in W less than valeurMin
 */
struct ncutParams
{
	ncutParams() : offset(.5), verbose(0), maxiterations(100), eigsErrorTolerance(1e-6), valeurMin(1e-6)
	{}

	double offset;
	unsigned int verbose;
	unsigned int maxiterations;
	double eigsErrorTolerance;
	double valeurMin;
};

/*
 * compute continuous eigenvectors for ncuts
 *
 * Input:
%     W= symmetric similarity matrix
%     nbEigenValues=  number of Ncut eigenvectors computed
%     dataNcut= optional parameters

% Output:
%    Eigenvectors= continuouse Ncut eigenvectos, size = length(W) x nbEigenValues
%    Eigenvalues= Ncut eigenvalues, size = 1x nbEigenValues
 */


std::tuple<MatrixXd, VectorXd> ncut(const SparseMatrix<double>& _W, const unsigned int nbEigenValues, const ncutParams& params = ncutParams())
{
	assert(nbEigenValues <= _W.rows());
	const unsigned int n = _W.rows();
	const double offset = params.offset;


	SparseMatrix<double> W = sparsify(_W, params.valeurMin);

	//check for matrix symmetry
    //实际中可省略！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	//if(!isApproxSymmetric(W, 1e-7))
	//	throw std::runtime_error("W not symmetric");

	//degrees and regularization
	VectorXd d = rowwiseAbsSums(W);
	VectorXd dr = 0.5 * (d - rowwiseSums(W));

	d += MatrixXd::Ones(d.rows(), d.cols()) * (offset * 2);
	dr += MatrixXd::Ones(dr.rows(), dr.cols()) * offset;
	W += sparseFromDiag(dr);
	
	const double eps = 1e-7;
	const VectorXd Dinvsqrt = (ArrayXd::Ones(d.rows(), d.cols()) / (d + MatrixXd::Ones(d.rows(), d.cols()) * eps).array().sqrt()).matrix();
	const SparseMatrix<double> P = spmtimesd(W, Dinvsqrt, Dinvsqrt);

	/*
	 * call arpack
	 */
	const unsigned int numData = W.rows();
	MatrixXd eigenvectors(numData, nbEigenValues); //one per column
	VectorXd eigvals(nbEigenValues);
	
{
	const unsigned int maxIters = params.maxiterations;
	const char* const whichEigvals = "LA"; //LA = largest algebraic
//	const unsigned int numArnoldiVectors = MIN(2 * numEigenvals + 1, numData - 1); //value suggested by arpack
	const unsigned int numArnoldiVectors = std::min(std::max(35u, 2 * nbEigenValues), n); //saupp.h recommends to increase ncv if you continually get the "no shifts could be applied" error msg (but that does mean increasing space & time)
	const double relativeTol = params.eigsErrorTolerance;

	arpackWeightMatrixMultiplierFunctor/*<decltype(P)>*/ wmmf(P);
	
	ARSymStdEig<double, decltype(wmmf)> solver(
			P.rows(),
			nbEigenValues, 
			&wmmf,
			&arpackWeightMatrixMultiplierFunctor/*<decltype(P)>*/::operator (),
			const_cast<char*>(whichEigvals), 
			numArnoldiVectors,
			relativeTol,
			maxIters
	);

	//std::cout << "before find eigen vectors" << std::endl;
	solver.FindEigenvectors();
	//std::cout << "after find eigen vectors" << std::endl;
	
	assert(solver.EigenvaluesFound());
	assert(solver.ConvergedEigenvalues() >= nbEigenValues);

//	cout << "eigenvalues (want " << nbEigenValues << "): <";
	for(unsigned int i = 0; i < nbEigenValues; i++)
	{
//		cout << " " << solver.Eigenvalue(i);
		eigvals[i] = solver.Eigenvalue(i);
	}
//	cout << " >" << endl;

	assert(solver.EigenvectorsFound());

	//cout << "before find eigen vector" << endl;
	const vector<double>* concatenatedEigvecs = solver.StlEigenvectors(); //all concatenated into one vector
	//cout << "after find eigen vector" << endl;
	
	for(unsigned int i = 0; i < nbEigenValues; i++)
	{
		std::copy(concatenatedEigvecs->begin() + i * numData, concatenatedEigvecs->begin() + (i + 1) * numData, eigenvectors.col(i).data());
//		cout << "eigenvec " << eigenvectors.col(i) << endl;
	}
	delete concatenatedEigvecs;
}

	//fout << "eigenvectors:" << endl << eigenvectors << endl;
	/*
	 * sort eigvals and rearrange eigvecs
	 */
	vector<pair<double, unsigned int>> sds(eigvals.rows());
	for(unsigned int i = 0; i < eigvals.rows(); i++)
	{
		sds[i].first = eigvals[i];
		sds[i].second = i;
	}

	//cout << "before sort" << endl;
	std::sort(sds.rbegin(), sds.rend());
	//cout << "after sort" << endl;
	
	VectorXd Eigenvalues(eigvals.rows());
	for(unsigned int i = 0; i < eigvals.rows(); i++) 
		Eigenvalues[i] = sds[i].first;

	//cout << "vbar size " << eigenvectors.rows() << " x " << nbEigenValues << endl;
	MatrixXd vbar(eigenvectors.rows(), nbEigenValues); //rearranged eigenvectors
	for(unsigned int i = 0; i < eigvals.rows(); i++) 
		vbar.col(i) = eigenvectors.col(sds[i].second);
	
	//cout << "vbar size:" << endl << vbar.rows() << endl << vbar.cols() << endl;

	MatrixXd Eigenvectors = /*DiagonalMatrix<double, Dynamic, Dynamic>(Dinvsqrt)*/Dinvsqrt.asDiagonal() * vbar;

	//std::ofstream fout("out.txt");
	//fout << Dinvsqrt;
	for(unsigned int i = 0; i < nbEigenValues; i++)
	{
		 Eigenvectors.col(i) *= sqrt(n) / Eigenvectors.col(i).norm();
		 double sign = -getSign(Eigenvectors(0, i)); 
		 if(Eigenvectors(0, i) != 0)
			  Eigenvectors.col(i) *= sign;
	}

	return std::make_tuple(Eigenvectors, Eigenvalues);
}

/*
 * run normalized cuts on a similarity matrix W
 */

int* ncutW(const SparseMatrix<double>& W, const unsigned int nbcluster)
{
	assert(nbcluster >= 2 && nbcluster <= W.rows());

	//compute continuous Ncut eigenvectors
	MatrixXd eigenvectors;
	VectorXd eigenvalues;
	std::tie(eigenvectors, eigenvalues) = std::move(ncut(W, nbcluster));

	//std::cout << "nuct done" << endl;
	//compute discretized Ncut vectors
	int* discreteAsgns = discretisation(eigenvectors);
	return discreteAsgns;
}
