/*
   ARPACK++ v1.2 2/20/2000
   c++ interface to ARPACK code.

   MODULE ARDSSym.h.
   Arpack++ class ARluSymStdEig definition
   (dense matrix version).

   ARPACK Authors
      Richard Lehoucq
      Danny Sorensen
      Chao Yang
      Dept. of Computational & Applied Mathematics
      Rice University
      Houston, Texas
*/

#ifndef ARDSSYM_H
#define ARDSSYM_H

#include <stddef.h>
#include "arch.h"
#include "arssym.h"
#include "ardsmat.h"


template<class ARFLOAT>
class ARluSymStdEig:
  public virtual ARSymStdEig<ARFLOAT, ARdsSymMatrix<ARFLOAT> > {

	  typedef ARSymStdEig<ARFLOAT, ARdsSymMatrix<ARFLOAT> > superT;

 public:

 // a) Public functions:

 // a.1) Functions that allow changes in problem parameters.

  virtual void ChangeShift(ARFLOAT sigmaRp);

  virtual void SetRegularMode();

  virtual void SetShiftInvertMode(ARFLOAT sigmap);

 // a.2) Constructors and destructor.

  ARluSymStdEig() { }
  // Short constructor.

  ARluSymStdEig(int nevp, ARdsSymMatrix<ARFLOAT>& A,
                char* whichp = "LM", int ncvp = 0,
                ARFLOAT tolp = 0.0, int maxitp = 0,
                ARFLOAT* residp = NULL, bool ishiftp = true);
  // Long constructor (regular mode).

  ARluSymStdEig(int nevp, ARdsSymMatrix<ARFLOAT>& A,
                ARFLOAT sigma, char* whichp = "LM", int ncvp = 0,
                ARFLOAT tolp = 0.0, int maxitp = 0,
                ARFLOAT* residp = NULL, bool ishiftp = true);
  // Long constructor (shift and invert mode).

  ARluSymStdEig(const ARluSymStdEig& other) { Copy(other); }
  // Copy constructor.

  virtual ~ARluSymStdEig() { }
  // Destructor.

 // b) Operators.

  ARluSymStdEig& operator=(const ARluSymStdEig& other);
  // Assignment operator.

}; // class ARluSymStdEig.


// ------------------------------------------------------------------------ //
// ARluSymStdEig member functions definition.                               //
// ------------------------------------------------------------------------ //


template<class ARFLOAT>
inline void ARluSymStdEig<ARFLOAT>::
ChangeShift(ARFLOAT sigmaRp)
{

	superT::sigmaR    = sigmaRp;
	superT::sigmaI    = 0.0;
	superT::mode      = 3;
	superT::iparam[7] = superT::mode;

	superT::objOP->FactorAsI(superT::sigmaR);
  superT::Restart();

} // ChangeShift.


template<class ARFLOAT>
inline void ARluSymStdEig<ARFLOAT>::SetRegularMode()
{

  ARStdEig<ARFLOAT, ARFLOAT, ARdsSymMatrix<ARFLOAT> >::
    SetRegularMode(superT::objOP, &ARdsSymMatrix<ARFLOAT>::MultMv);

} // SetRegularMode.


template<class ARFLOAT>
inline void ARluSymStdEig<ARFLOAT>::SetShiftInvertMode(ARFLOAT sigmap)
{

  ARStdEig<ARFLOAT, ARFLOAT, ARdsSymMatrix<ARFLOAT> >::
    SetShiftInvertMode(sigmap, superT::objOP, &ARdsSymMatrix<ARFLOAT>::MultInvv);

} // SetShiftInvertMode.


template<class ARFLOAT>
inline ARluSymStdEig<ARFLOAT>::
ARluSymStdEig(int nevp, ARdsSymMatrix<ARFLOAT>& A,
              char* whichp, int ncvp, ARFLOAT tolp,
              int maxitp, ARFLOAT* residp, bool ishiftp)
{

	superT::NoShift();
  DefineParameters(A.ncols(), nevp, &A, &ARdsSymMatrix<ARFLOAT>::MultMv,
                   whichp, ncvp, tolp, maxitp, residp, ishiftp);

} // Long constructor (regular mode).


template<class ARFLOAT>
inline ARluSymStdEig<ARFLOAT>::
ARluSymStdEig(int nevp, ARdsSymMatrix<ARFLOAT>& A,
              ARFLOAT sigmap, char* whichp, int ncvp, ARFLOAT tolp,
              int maxitp, ARFLOAT* residp, bool ishiftp)

{

  DefineParameters(A.ncols(), nevp, &A, &ARdsSymMatrix<ARFLOAT>::MultInvv,
                   whichp, ncvp, tolp, maxitp, residp, ishiftp);
  ChangeShift(sigmap);

} // Long constructor (shift and invert mode).


template<class ARFLOAT>
ARluSymStdEig<ARFLOAT>& ARluSymStdEig<ARFLOAT>::
operator=(const ARluSymStdEig<ARFLOAT>& other)
{

  if (this != &other) { // Stroustrup suggestion.
	  superT::ClearMem();
    Copy(other);
  }
  return *this;

} // operator=.


#endif // ARDSSYM_H

