/*
 * matrixUtils: stuff Eigen ought to have
 *
 * Evan Herbst
 * 10 / 20 / 11
 */

#include "matrixUtils.h"
using namespace Eigen;

/*
 * create a diagonal SparseMatrix
 */
SparseMatrix<double> sparseFromDiag(const VectorXd& d)
{
	int rows = d.rows();
	SparseMatrix<double> result(rows, rows);
	result.reserve(VectorXi::Constant(rows,1));
	for(unsigned int j = 0; j < rows; j++)
	{
		result.insert(j, j) = d[j];
	}
	result.makeCompressed();
	return result;
}

/*
 * m.array().abs().rowwise().sum().matrix()
 */
VectorXd rowwiseAbsSums(const SparseMatrix<double>& m)
{
	VectorXd result(m.rows());
	result.fill(0);
	for(unsigned int j = 0; j < m.cols(); j++)
		for(SparseMatrix<double>::InnerIterator it(m, j); it; ++it)
			result[it.row()] += fabs(it.value());
	return result;
}

/*
 * m.rowwise().sum()
 */
VectorXd rowwiseSums(const SparseMatrix<double>& m)
{
	VectorXd result(m.rows());
	result.fill(0);
	for(unsigned int j = 0; j < m.cols(); j++)
		for(SparseMatrix<double>::InnerIterator it(m, j); it; ++it)
			result[it.row()] += it.value();
	return result;
}

bool isApproxSymmetric(const SparseMatrix<double>& m, const double eps)
{
	for(unsigned int j = 0; j < m.cols(); j++)
		for(SparseMatrix<double>::InnerIterator it(m, j); it; ++it)
			for(SparseMatrix<double>::InnerIterator it2(m, it.row()); it2; ++it2)
				if(it2.row() == j)
					if(fabs(it.value() - it2.value()) > eps)
						return false;
	return true;
}
