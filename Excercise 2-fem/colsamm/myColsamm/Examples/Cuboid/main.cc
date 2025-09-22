
/*  Example 1: computing the local stencils of the  
 *  diretization of a more complex equation!
 *
 *  This equation contains the computation of one stencil,
 *  that could denote the operator of a more general 
 *  eliptical operator. Hereby, v and w are again
 *  elements of the space V containing the basis 
 *  functions, in this first example piece-wise
 *  linear functions.
 */ 

// Include files for the I/O and the file I/O
 #include <iostream>
 #include <fstream>

/*  Including the Colsamm library. Since we want to 
 *  present an easier notation, we use the entire 
 *  namespace _COLSAMM_
 */
 #include "../../Source/Colsamm.h"
 using namespace _COLSAMM_;

 int main(int argc , char **argv)
    {
      int dim, num, check, iteration;

      ELEMENTS::_Quadrangle3D_<Gauss2> Element; 
      dim = Element.dimension();
      num = Element.getNumberOfCorners();

      std::ifstream PARAMETER;
      PARAMETER.open("cuboid.dat",std::ios :: in);
      if (!PARAMETER)
        {
          std::cout << "Parameter file missing .... " << std::endl;
          exit(0);
        }

   // reading the dimension and the number of vertices
      PARAMETER >> iteration >> check;
   // reading of vertices   
      std::vector<double> data(num*dim,0.);
      for (int i=0; i < num*dim; ++i)
         {
           PARAMETER >> data[i];
         }
   
   // reading some testing point, not used in this example
      double * tester = new double [dim];
      for (int i=0; i < dim; ++i)
         {
           PARAMETER >> tester[i] ;
         }
   // reading some further parameters ...
      PARAMETER.close();
   // reading done


     int sizeV = Element.size_Set1(), 
         sizeW = Element.size_Set1();
     std::vector<std::vector<double> > Stencil; 


////////////////////////////////////////////////////////////////////////////////
//                    Printing some information                               //
////////////////////////////////////////////////////////////////////////////////
     std::cout << "\n\n"\
     " *********************************************************************\n"\
     " *   This example computes the local stencils of the Laplacian and   *\n"\
     " *   the Helmholtz operator. The vertices of the used cuboid are:    *\n"\
     " *********************************************************************\n\n  ";

       for(int i=0; i < num; i++)
          {
            std::cout << "(" << data[3*i] << "," << data[3*i+1] << "," 
                      << data[3*i+2] << ") " ;
          }
        
     std::cout << "\n\n\n"\
     " *********************************************************************\n"\
     " *  The following matriced contain the local Finite Elment stencils  *\n"\
     " *  which denote the value of the boundary integral over a cube by   *\n"\
     " *  combining the i-th basisfunction with the j-th basisfunction!    *\n" ; 
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
     // initialize the finite element with the vertex data of the working element
       Element(data);

       Stencil = Element.integrate(v_()*w_());

////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ******************************************\n"\
                        " *  Local Stencil of the Laplace Operator *\n"\
                        " ******************************************\n"\
                 << std::endl << "\033[33m" ;
       for (int p=0; p < sizeV; ++p) 
          {
            for (int q=0; q < sizeW; ++q)
               {
                 std::cout.width(13);
                 std::cout << Stencil[p][q] ;
               }
            std::cout << std::endl;
          }
       std::cout << "\033[m" << std::endl;

////////////////////////////////////////////////////////////////////////////////
    delete [] tester;
  } 
