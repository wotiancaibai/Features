
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "computeW.h"

vector<Matrix<unsigned long,Dynamic,1>> cimgnbmap(const VectorXi& imgSize, const VectorXi& sampleRadius, double sample_rate) {
    /* declare variables */
    int nr, nc, np, nb, total;
    int r_i, r_j, a1, a2, b1, b2, self, neighbor;
    int i, j, k, s, t, nsamp, th_rand, no_sample;
    unsigned long *p;
    
    /* get image size */
    i = imgSize.rows();
    j = imgSize.cols();
    nr = imgSize(0);
    if (j>1 || i>1) {
        nc = imgSize(1);
    } else {
        nc = nr;
    }
    np = nr * nc;
    
    /* get neighbourhood size */
    i = sampleRadius.rows();
    j = sampleRadius.cols();
    r_i = sampleRadius(0);
    if (j>1 || i>1) {
        r_j = sampleRadius(1);
    } else {
        r_j = r_i;
    }
    if (r_i<0) { r_i = 0; }
	if (r_j<0) { r_j = 0; }
	/* get sample rate */
	if (sample_rate <= 0) {		
		sample_rate = 1;
    }
	/* prepare for random number generator */
	if (sample_rate<1) {
        srand( /*(unsigned)time( NULL )*/0 );
        th_rand = (int)ceil((double)RAND_MAX * sample_rate);
        no_sample = 0;
    } else {
		sample_rate = 1;
        th_rand = RAND_MAX;
        no_sample = 1;
    }
    
	/* figure out neighbourhood size */

    nb = (r_i + r_i + 1) * (r_j + r_j + 1); 
    if (nb>np) {
        nb = np;
    }
    nb = (int)ceil((double)nb * sample_rate);    

	/* intermediate data structure */
	p = (unsigned long *)malloc(np * (nb+1) * sizeof(unsigned long));
	memset(p, 0, np * (nb+1) * sizeof(unsigned long));
	if (p==NULL) {
		printf("Not enough space for computation.\n");
		exit(1);
	}
	
    /* computation */    
	total = 0;
    for (j=0; j<nc; j++) {
		for (i=0; i<nr; i++) {

			self = i + j * nr;

			/* put self in, otherwise the index is not ordered */
			p[self] = p[self] + 1;
			p[self + p[self]*np] = self;

			/* j range */
			b1 = j;
			b2 = j + r_j;
			if (b2>=nc) { b2 = nc-1; }                

			/* i range */
			a1 = i - r_i;
			if (a1<0) { a1 = 0; }
			a2 = i + r_i;
			if (a2>=nr) { a2 = nr-1; }

			/* number of more samples needed */
			nsamp = nb - p[self];

			k = 0;		
			t = b1;
			s = i + 1;
			if (s>a2) {
				s = a1;
				t = t + 1;
			}
			while (k<nsamp && t<=b2) {
				if (no_sample || (rand()<th_rand)) {
					k = k + 1;
					neighbor = s + t * nr;

					p[self] = p[self] + 1;					
					p[self+p[self]*np] = neighbor;

					p[neighbor] = p[neighbor] + 1;
					p[neighbor+p[neighbor]*np] = self;
				}
				s = s + 1;
				if (s>a2) {
					s = a1;
					t = t + 1;
				}
			} /* k */

			total = total + p[self];
		} /* i */
    } /* j */
    
    /* i, j */

	vector<Matrix<unsigned long,Dynamic,1>> out;
	Matrix<unsigned long, Dynamic, 1> w_i(total, 1);
	Matrix<unsigned long, Dynamic, 1> w_j(np+1, 1);
	out.push_back(w_i);
	out.push_back(w_j);
	Matrix<unsigned long, Dynamic, 1>& qi = out.at(0);
	Matrix<unsigned long, Dynamic, 1>& qj = out.at(1);

	total = 0;
    for (j=0; j<np; j++) {
		qj(j, 0) = total;
		s = j + np;
		for (t=0; t<p[j]; t++) {
		    qi(total, 0) = p[s];
			total = total + 1;
			s = s + np;
		}
    }
	qj(np, 0) = total;

	free(p);

	return out;
}  

SparseMatrix<double> affinityic(const MatrixXd& m_emag, const MatrixXd& m_ephase, const Matrix<unsigned long, Dynamic, 1>& m_pi, const Matrix<unsigned long, Dynamic, 1>& m_pj, double sigma) {
    
    /* declare variables */
    int nr, nc, np, total;
    int i, j, k, ix, iy, jx, jy, ii, jj, iip1, jjp1, iip2, jjp2, step;
    double di, dj, a, z, maxori, phase1, phase2, slope;
	const unsigned long *pi, *pj;
	const double *emag, *ephase;
    
    /* get edgel information */
	nr = m_emag.rows();
	nc = m_emag.cols();
	if ( nr*nc ==0 || nr != m_ephase.rows() || nc != m_ephase.cols() ) {
	    printf("Edge magnitude and phase shall be of the same image size");
		exit(1);
	}
    emag = m_emag.data();
    ephase = m_ephase.data();
    np = nr * nc;
    
    /* get new index pair */
    if ((m_pj.rows() * m_pj.cols()) != np + 1) {
        printf("Wrong index representation\n");
		exit(1);
    }
    pi = m_pi.data();
    pj = m_pj.data();

    /* find my sigma */
	if (sigma <= 0) {
    	for (k=0; k<np; k++) { 
    	    if (emag[k]>sigma) { sigma = emag[k]; }
    	}
    	sigma = sigma / 6;
    	printf("sigma = %6.5f",sigma);
	} 
	a = 0.5 / (sigma * sigma);
	
    /* create output */
	SparseMatrix<double> W(np, np);
	vector<Triplet<double>> tripletList;
	tripletList.reserve(pj[np]);

    /* computation */ 
    total = 0;
    for (j=0; j<np; j++) {            
        jx = j / nr; /* col */
        jy = j % nr; /* row */
        for (k=pj[j]; k<pj[j+1]; k++) {
            i = pi[k];
            if (i==j) {
                maxori = 1;
            } else {
                ix = i / nr; 
                iy = i % nr;
                /* scan */            
                di = (double) (iy - jy);
                dj = (double) (ix - jx);
            
                maxori = 0.;
	            phase1 = ephase[j];
	               
                /* sample in i direction */
                if (abs(di) >= abs(dj)) {  
            	    slope = dj / di;
            	    step = (iy>=jy) ? 1 : -1;
            	
              	    iip1 = jy;
            	    jjp1 = jx;
	
	                for (ii=0;ii<abs(di);ii++){
	                    iip2 = iip1 + step;
	                    jjp2 = (int)(0.50001 + slope*(iip2-jy) + jx);
	  	  
	                    phase2 = ephase[iip2+jjp2*nr];
               
	                    if (phase1 != phase2) {
	                        z = (emag[iip1+jjp1*nr] + emag[iip2+jjp2*nr]);
	                        if (z > maxori){
	                            maxori = z;
	                        }
	                    } 
	             
	                    iip1 = iip2;
	                    jjp1 = jjp2;
	                    phase1 = phase2;
	                }
	            
	            /* sample in j direction */    
                } else { 
	                slope = di / dj;
	                step =  (ix>=jx) ? 1: -1;

    	            jjp1 = jx;
	                iip1 = jy;	           
	    
	 
	                for (jj=0;jj<abs(dj);jj++){
	                    jjp2 = jjp1 + step;
	                    iip2 = (int)(0.50001+ slope*(jjp2-jx) + jy);
	  	  
	                    phase2 = ephase[iip2+jjp2*nr];
	     
	                    if (phase1 != phase2){
	                        z = (emag[iip1+jjp1*nr] + emag[iip2+jjp2*nr]);
	                        if (z > maxori){ 
	                            maxori = z; 
	                        }
	                        
	                    }
	  
	                    iip1 = iip2;
	                    jjp1 = jjp2;
	                    phase1 = phase2;
	                }
                }            
            
                maxori = 0.5 * maxori;
                maxori = exp(-maxori * maxori * a);
            }       
		    total = total + 1;
			tripletList.push_back(Triplet<double>(i, j, maxori));	
			
		} /* i */
    } /* j */
	
	W.setFromTriplets(tripletList.begin(), tripletList.end());
    	
	return W;
}

SparseMatrix<double> computeW(const MatrixXd& imageX, const dataW dataWPar, const MatrixXd& emag, const MatrixXd& ephase) {
    
	int p = imageX.rows();
	int q = imageX.cols();
	Vector2i imageSize(p, q);
	Vector2i sampleRadius(dataWPar.sampleRadius, dataWPar.sampleRadius);
	double sigma = emag.maxCoeff() * dataWPar.edgeVariance;

	vector<Matrix<unsigned long,Dynamic,1>> w = cimgnbmap(imageSize, sampleRadius, dataWPar.sample_rate);
	
	SparseMatrix<double> W = affinityic(emag, ephase, w.at(0), w.at(1), sigma);

	return W;
}
