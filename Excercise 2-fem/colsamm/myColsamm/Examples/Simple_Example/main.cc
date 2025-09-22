/*  Computing the local stencils of the discretization 
 *  of Poisson's equation. 
 *
 *  This equation contains two stencils, the stencil
 *  of the Laplacian operator (Dv * Dw) and of the 
 *  Helmholtz operator (v*w). Hereby, v and w are 
 *  elements of the space V containing the basis 
 *  functions, in this first example piece-wise
 *  linear functions.
 */ 
 #include <iostream>
/*  Including the Colsamm library. Since we want to 
 *  present an easier notation, we use the entire 
 *  namespace _COLSAMM_
 */
 #include <Colsamm.h>
 using namespace ::_COLSAMM_;

 int main(int argc , char **argv)
    {
/*
              (0,1)
                 |\
                 |  \
                 |____\
              (0,0)   (1,0)
*/
      std::vector < double > corners (6,0.);
      corners[0] = 1.; corners[1] = 0.;
      corners[2] = 0.; corners[3] = 1.;
      corners[4] = 0.; corners[5] = 0.;

      ELEMENTS::Triangle Element; 
 
     int sizeV = Element.size_Set1(), sizeW = Element.size_Set1();
    
     std::vector<std::vector<double> > Laplacian_Stencil ;
     std::vector<std::vector<double> > Helmholtz_Stencil ;
////////////////////////////////////////////////////////////////////////////////
//                    Printing some information                               //
////////////////////////////////////////////////////////////////////////////////
     std::cout << "\n\n"\
     " *********************************************************************\n"\
     " *   This example computes the local stencils of the Laplacian and   *\n"\
     " *   the Helmholtz operator. The vertices of the used element are:   *\n"\
     " *********************************************************************\n\n  ";

         std::cout << "(" << corners[0] << "," << corners[1]  << ") " ;
         std::cout << "(" << corners[2] << "," << corners[3]  << ") " ;
         std::cout << "(" << corners[4] << "," << corners[5]  << ") " ;
        
     std::cout << "\n\n"\
     " *********************************************************************\n"\
     " *  The following matrices contain the local finite element stencils *\n"\
     " *  which denote the value of the integral over the cube above while *\n"\
     " *  combining the i-th basis function with the j-th basis function!  *\n" ; 
     std::cout << 
     " *  In order to yield the stenicls and the entries in the discreti-  *\n"\
     " *  zation matrix, one has to sum up the those integrals of basis    *\n"\
     " *  functions that do not vanish, e.g. those basisfunctions that     *\n"\
     " *  have a common support.                                           *\n"\
     " *********************************************************************\n"\
     << std::endl;
////////////////////////////////////////////////////////////////////////////////
//			Computation of the Stenicls                           //
////////////////////////////////////////////////////////////////////////////////
          Laplacian_Stencil = Element(corners).integrate(grad(v_())*grad(w_()) );
          Helmholtz_Stencil = Element.integrate(v_() * w_() );
////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n ******************************************\n"\
                      " *  Local Stencil of the Laplace Operator *\n"\
                      " ******************************************\n"\
                 << std::endl ;
       Element.printStencil(Laplacian_Stencil,sizeV,sizeW);

////////////////////////////////////////////////////////////////////////////////
//			Stencil Output 
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n ********************************************\n"\
                      " *  Local Stencil of the Helmholtz Operator *\n"\
                      " ********************************************\n"\
                 << std::endl ;
       Element.printStencil(Helmholtz_Stencil,sizeV,sizeW);
////////////////////////////////////////////////////////////////////////////////
  } 
