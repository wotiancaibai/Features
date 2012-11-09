
#include <math.h>
#include <fftw3.h>
#include "computeEdges.h"

//#include <iostream>

/*
template<typename Scalar>
struct CwiseExp {
	CwiseExp(double index) : m_index(index) {}
	const Scalar operator()(const Scalar& x) const {
    	return exp(m_index * x); 
	}
	double m_index;
};

template<typename Scalar>
struct CwiseCos {
    const Scalar operator()(const Scalar& x) const {
        return cos(x);
    }
};

template<typename Scalar>
struct CwiseSin {
    const Scalar operator()(const Scalar& x) const {
        return sin(x);
    }
};
*/

template<typename Scalar>
struct CwiseLargerThan {
    CwiseLargerThan(const Scalar& inf) : m_inf(inf) {}
    const Scalar operator()(const Scalar& x) const {
		return x > m_inf ? 1 : 0;
    }
    Scalar m_inf;
};

template<typename Scalar>
struct CwiseLessThan {
	CwiseLessThan(const Scalar& sup) : m_sup(sup) {}
    const Scalar operator()(const Scalar& x) const {
        return x < m_sup ? 1 : 0;
    }
    Scalar m_sup;
};


double rcond(const MatrixXd& x) {
    double norm1 = x.cwiseAbs().colwise().sum().maxCoeff();
    double norm2 = x.inverse().cwiseAbs().colwise().sum().maxCoeff();
    
    return 1.0/norm1/norm2;
}

MatrixXd gaussian(const MatrixXd& x, const MatrixXd& m, const MatrixXd& C) { // 看是否可以都加const
    int d = m.cols() * m.rows();
	/*
    if (x.rows() != d) {
        x.transposeInPlace();
    }
	*/
    int N = x.cols();
    double detC = C.determinant();
    double eps = 2.2204e-16;
    MatrixXd p(N, 1);
    if (rcond(C) < eps) {
        p.fill(0);
    }
    else {
        //m.resize(d, 1);
        MatrixXd M = m * MatrixXd::Ones(1, N);
        double denom = pow(2*M_PI, d/2.0) * sqrt(fabs(detC));
        MatrixXd mahal_tmp1 = (x - M).transpose();
        //MatrixXd mahal_tmp2 = mahal_tmp1*C.inverse();
        MatrixXd mahal = (mahal_tmp1*C.inverse()).cwiseProduct(mahal_tmp1).rowwise().sum();
        /*
        MatrixXd numer(N, 1);
        for (int i = 0; i < N; ++i) {
            numer(i, 0) = exp(-0.5 * mahal(i, 0));
        }
        */

        //p = mahal.unaryExpr(CwiseExp<double>(-0.5)) / denom;
		p = (-0.5 * mahal).array().exp().matrix() / denom;
    }
    
    return p;
    
}

pair<MatrixXd, MatrixXd> meshgrid(const VectorXd& lsp_x, const VectorXd& lsp_y) {
    
    int d1 = lsp_x.rows() * lsp_x.cols(),
        d2 = lsp_y.rows() * lsp_y.cols();
    
    pair<MatrixXd, MatrixXd> XY;
    
    XY.first.resize(d2, d1);
    XY.second.resize(d2, d1);
    
    for (int i = 0; i < d1; ++i) {
        XY.first.col(i).fill(lsp_x[i]);
    }
    for (int i = 0; i < d2; ++i) {
        XY.second.row(i).fill(lsp_y[i]);
    }
    
    return XY;
}

MatrixXd doog1(int sig, int r, double th, int N) {

	pair<MatrixXd, MatrixXd> xy = meshgrid(VectorXd::LinSpaced(Sequential, N, -(N/2.0)+0.5, (N/2.0)-0.5), VectorXd::LinSpaced(Sequential, N, -(N/2.0)+0.5, (N/2.0)-0.5));

	MatrixXd X(2, N*N);
	xy.first.resize(1, N*N);	
	xy.second.resize(1, N*N);
	X.row(0) = xy.first.row(0);
	X.row(1) = xy.second.row(0);

	double phi = M_PI * th / 180.0;
	int sigy = sig;
	int sigx = r*sig;
	Rotation2Dd R(phi);
	MatrixXd C = R * Vector2d(sigx, sigy).asDiagonal() * R.inverse();
	
	MatrixXd m = R * Vector2d(0, sig) / 2;
	
	/*
	cout << "X:" << endl << X << endl;
	cout << "C:" << endl << C << endl;
	cout << "m:" << endl << m << endl;
	*/
	
	MatrixXd Ga = gaussian(X, m, C);
	Ga.resize(N, N);
	
	//cout << "Ga:" << endl << Ga << endl;
	
	return Ga - Ga.reverse()/* Gb */;
}

MatrixXd doog2(int sig, int r, double th, int N) {
	
	pair<MatrixXd, MatrixXd> xy = meshgrid(VectorXd::LinSpaced(Sequential, N, -(N/2.0)+0.5, (N/2.0)-0.5), VectorXd::LinSpaced(Sequential, N, -(N/2.0)+0.5, (N/2.0)-0.5));

	MatrixXd X(2, N*N);
	xy.first.resize(1, N*N);	
	xy.second.resize(1, N*N);
	X.row(0) = xy.first.row(0);
	X.row(1) = xy.second.row(0);

	double phi = M_PI * th / 180.0;
	int sigy = sig;
	int sigx = r*sig;
	Rotation2Dd R(phi);
	MatrixXd C = R * Vector2d(sigx, sigy).asDiagonal() * R.inverse();
	
	MatrixXd m = R * Vector2d(0, sig);
	MatrixXd Ga = gaussian(X, m, C);
	Ga.resize(N, N);

	m.fill(0);
	MatrixXd Gb = gaussian(X, m, C);
	Gb.resize(N, N);
	
	return -Ga + 2*Gb - Ga.reverse()/* Gc */;
}

vector<MatrixXd> make_filterbank(bool is_doog1, int num_ori, int filter_scales, int wsz, int enlong) {

	enlong *= 2;
	int M1 = wsz, M2 = wsz;
	double ori_incr = 180.0 / num_ori;
	double ori_offset = ori_incr / 2.0;
	
	vector<MatrixXd> FB;
	MatrixXd FB_element;
	
	for (int n = 0; n < num_ori; ++n) {
		if (is_doog1) {
			FB_element = doog1(filter_scales, enlong, ori_offset + n*ori_incr, M1);
			//cout << FB_element << endl;
		} else {
			FB_element = doog2(filter_scales, enlong, ori_offset + n*ori_incr, M1);	
		}
		double a = FB_element.cwiseAbs().sum();
		FB_element /= a;
		//cout << "FB:" << endl << FB_element << endl;
		FB.push_back(FB_element);
	}
	
	return FB;	
}

MatrixXd fliplr(const MatrixXd& x) {
	int rows = x.rows(), cols = x.cols();
	MatrixXd y(rows, cols);
	for (int i = 0; i < cols; ++i) {
		y.col(i) = x.col(cols - i - 1);
	}

	return y;
}

MatrixXd flipud(const MatrixXd& x) {
	int rows = x.rows(), cols = x.cols();
	MatrixXd y(rows, cols);
	for (int i = 0; i < rows; ++i) {
		y.row(i) = x.row(rows - i -1);
	}

	return y;
}

vector<MatrixXd> fft_filt_2(const MatrixXd& V, vector<MatrixXd>& FB) {
	int N3 = FB.size();
	int M1 = FB.at(0).rows(), M2 = FB.at(0).cols();
	int N1 = V.rows(), N2 = V.cols();

	int rows = N1 + M1 - 1, cols = N2 + M2 - 1;
	int rc = rows * cols;
	MatrixXd I(rows, cols);
	I.fill(0);
	I.block(0, 0, N1, N2) = V;

	//cout << I << endl;
	
	fftw_complex *in1 = (fftw_complex *) fftw_malloc(rc*sizeof(fftw_complex));
	fftw_complex *in2 = (fftw_complex *) fftw_malloc(rc*sizeof(fftw_complex));
	fftw_complex *out1 = (fftw_complex *) fftw_malloc(rc*sizeof(fftw_complex));	
	fftw_complex *out2 = (fftw_complex *) fftw_malloc(rc*sizeof(fftw_complex));

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			in1[i*cols+j][0] = I(i, j);
			in1[i*cols+j][1] = 0;
		}
	}

	fftw_plan fft1 = fftw_plan_dft_2d(rows, cols, in1, out1, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(fft1);
	fftw_destroy_plan(fft1);

	vector<MatrixXd> FI;
	
	// initialize for the next loop
	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N2; ++j) {
			in1[i*cols+j][0] = 0;
		}
	}	
	
	for (int n = 0; n < N3; ++n) {
		MatrixXd f = FB.at(n).reverse();

		// cout << "f:" << endl << f; //ok

		for (int i = 0; i < M1; ++i) {
			for (int j = 0; j < M2; ++j) {
				in1[i*cols+j][0] = f(i, j);
			}
		}
		/*
		double tmp1[6][6];
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				tmp1[i][j] = in1[i*cols+j][0];
			}
		}
		*/
		fftw_plan fft2 = fftw_plan_dft_2d(rows, cols, in1, out2, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(fft2);
		fftw_destroy_plan(fft2);	
		/*
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				tmp1[i][j] = out2[i*cols+j][0];
			}
		}
		*/
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				in2[i*cols+j][0] = out1[i*cols+j][0]*out2[i*cols+j][0] - out1[i*cols+j][1]*out2[i*cols+j][1];
				in2[i*cols+j][1] = out1[i*cols+j][0]*out2[i*cols+j][1] + out1[i*cols+j][1]*out2[i*cols+j][0];
			}
		}

		fftw_plan fft3 = fftw_plan_dft_2d(rows, cols, in2, out2, FFTW_BACKWARD, FFTW_ESTIMATE);
		fftw_execute(fft3);
		fftw_destroy_plan(fft3);	
		MatrixXd If(rows, cols);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				If(i, j) = out2[i*cols+j][0] / rc; ////////////////////
			}
		}
		
		
		FI.push_back(If.block(M1/2, M2/2, N1, N2));	
		
	}	
	
	fftw_free(in1);
	fftw_free(in2);
	fftw_free(out1);
	fftw_free(out2);
	
	return FI;
}

vector<MatrixXd> quadedgep(const MatrixXd& I, const edgeParams par) {
	
	int n_filter = par.n_filter, n_scale = par.n_scale, winsz = par.winsz, enlong = par.enlong;
	if ((winsz % 2) == 0)	
		++winsz;
	
    int r = I.rows(), c = I.cols();
    vector<MatrixXd> FBo = make_filterbank(true, n_filter, n_scale, winsz, enlong);
    vector<MatrixXd> FBe = make_filterbank(false, n_filter, n_scale, winsz, enlong);
	
	//cout << "FBe:" << endl << FBe.at(0) << endl; // OK
	
	
    int n = (winsz+1) / 2;
    MatrixXd f_tmp(r, 2*n+c);
    f_tmp.block(0,   0, r, n) = fliplr(I.middleCols(1, n));
    f_tmp.block(0,   n, r, c) = I;
	f_tmp.block(0, n+c, r, n) = fliplr(I.middleCols(c-n-1, n));

	MatrixXd f(2*n+r, 2*n+c);
	f.block(  0, 0, n, 2*n+c) = flipud(f_tmp.middleRows(1, n));
	f.block(  n, 0, r, 2*n+c) = f_tmp;
	f.block(n+r, 0, n, 2*n+c) = flipud(f_tmp.middleRows(r-n-1, n));
	
	//cout << f << endl; // ok
	
	vector<MatrixXd> FIo = fft_filt_2(f, FBo);
	vector<MatrixXd> FIe = fft_filt_2(f, FBe);

	//cout << "FIo:" << endl << FIo.at(0) << endl;
	
	int size = FIo.size();
	for (int i = 0; i < size; ++i) {
		FIo.at(i) = FIo.at(i).block(n, n, r, c).eval();
		FIe.at(i) = FIe.at(i).block(n, n, r, c).eval();
	}
	
	MatrixXd mag(r, c); 
	vector<MatrixXd> mag_a;
	MatrixXd sum = MatrixXd::Zero(r, c);
	for (int i = 0; i < size; ++i) {
		sum += FIo.at(i).cwiseAbs2();
		sum += FIe.at(i).cwiseAbs2();
		mag_a.push_back((FIo.at(i).cwiseAbs2()+FIe.at(i).cwiseAbs2()).cwiseSqrt());
	}
	mag = sum.cwiseSqrt(); 

	//cout << mag << endl;
	/*
	cout << "mag_a(0):" << endl << mag_a.at(0) << endl;
	cout << "mag_a(1):" << endl << mag_a.at(1) << endl;
	cout << "mag_a(2):" << endl << mag_a.at(2) << endl;
	cout << "mag_a(3):" << endl << mag_a.at(3) << endl;
	*/

	MatrixXd max_id = MatrixXd::Zero(r, c), max_tmp = mag_a.at(0);
	for (int n = 1; n < size; ++n) {
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				MatrixXd& r_mat = mag_a.at(n);
				if (r_mat(i, j) > max_tmp(i, j)) {
					max_tmp(i, j) = mag_a.at(n)(i, j);
					max_id(i, j) = n;
				}
			}
		}
	}
	
	MatrixXd mage(r, c);
    MatrixXd mago(r, c);
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			mage(i, j) = FIe.at(max_id(i, j))(i, j);
		}
	}

	//cout << "max_id:" << endl << max_id << endl;
	
	for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            mago(i, j) = FIo.at(max_id(i, j))(i, j);
        }
    }

	mage = mage.unaryExpr(CwiseLargerThan<double>(0)) - mage.unaryExpr(CwiseLessThan<double>(0));

	/*
    double ori_incr = M_PI / n_filter;
    double ori_offset = ori_incr / 2;
	VectorXd theta(n_filter);    
    for (int n = 0; n < n_filter; ++n) {
        theta(n) = ori_offset + n * ori_incr;
    }
    
    MatrixXd ori(r, c);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            ori(i, j) = theta(max_id(i, j));
        }
    }

    ori = ori.cwiseProduct(mago.unaryExpr(CwiseLargerThan<double>(0))) + (ori.array()+M_PI).matrix().cwiseProduct(mago.unaryExpr(CwiseLessThan<double>(0)));

    MatrixXd gy(r, c), gx(r, c);
    gy = mag.cwiseProduct(ori.array().cos().matrix());
    gx = - mag.cwiseProduct(ori.array().sin().matrix());
    */
	
	vector<MatrixXd> out;
    out.push_back(mag);
    out.push_back(mage);

    return out;
}

/*
MatrixXd cannyEdge(MatrixXd imageX) {

	CvSize imSize = cvSize(imageX.cols(), imageX.rows());
	IplImage* pCannyImg = cvCreateImage(imSize, IPL_DEPTH_8U, 1);
	IplImage* grayImage = cvCreateImage(imSize, IPL_DEPTH_8U, 1);
	
	int width = pCannyImg->width;
  	int height = pCannyImg->height;
  	int widthStep = pCannyImg->widthStep;	

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			((unsigned char*)grayImage->imageData)[i*widthStep+j] = imageX(i, j);
		}
	}
	
    cvCanny(grayImage, pCannyImg, 70, 150, 3);

	
	
	MatrixXd edge(height, width);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			edge(i, j) = (double)((unsigned char*)pCannyImg->imageData)[i*widthStep+j]/255;
		}
	}

    cvReleaseImage(&grayImage);
    cvReleaseImage(&pCannyImg);

	return edge;
}

MatrixXd computeEdges(MatrixXd imageX, const edgeParams& parametres, double threshold) {
	vector<MatrixXd> quad = quadedgep(imageX);
	MatrixXd edges2 = quad.at(0).cwiseProduct(cannyEdge(imageX));

	edges2 = edges2.cwiseProduct(edges2.unaryExpr(CwiseLargerThan<double>(threshold))).eval();//the eval() may be elimite;
	//TODO: complete the function, not needed for the ncut
}
*/
