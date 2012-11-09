
#include "ICgraph.h"


SparseMatrix<double> ICgraph(const MatrixXd& I, const dataW dataWPar, const edgeParams dataMapPar) {
	vector<MatrixXd> edgeMap = quadedgep(I, dataMapPar);
	MatrixXd& emag = edgeMap.at(0);
	MatrixXd& ephase = edgeMap.at(1);
	SparseMatrix<double> W = computeW(I, dataWPar, emag, ephase);

	return W;
}

