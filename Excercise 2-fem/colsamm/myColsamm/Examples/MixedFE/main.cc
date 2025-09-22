
/*  Example 1: computing the local stencils of the  
 *  diretization of Poisson's equation. 
 *
 *  This equation contains two stencils, the stencil
 *  of the Laplacian operator (Dv * Dw) and of the 
 *  Helmholtz operator (v*w). Hereby, v and w are 
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
 using namespace Interfaces;



//-----------------------------------------------------------------------------
template < IntegrationAccuracy accuracy = Gauss2>
 struct Mixed_Triangle_
  : public _Domain_<3,D2,ELEMENTS::Triangle_Transformation,interior,6,3,triangle,accuracy>
    {
      Mixed_Triangle_()
        {
          this->Set ( BasisSet_1, 2 * (1 - X_(1) - Y_(1) )  * ( 0.5 - X_(1) - Y_(1) ) ) ;
          this->Set ( BasisSet_1, 2 * ( X_(1)  * (  X_(1) - 0.5 ) ) ) ;
          this->Set ( BasisSet_1, 2 * ( Y_(1)  * (  Y_(1) - 0.5 ) ) ) ;
          this->Set ( BasisSet_1, 4 * (1 - X_(1) - Y_(1) ) * X_(1) ) ;
          this->Set ( BasisSet_1, 4 * ( X_(1) * Y_(1) ) ) ;
          this->Set ( BasisSet_1, 4 * (1 - X_(1) - Y_(1) ) * Y_(1) ) ;

          this->Set ( BasisSet_2, 1. - X_(1) - Y_(1) ) ;
          this->Set ( BasisSet_2, X_(1) ) ;
          this->Set ( BasisSet_2, Y_(1)  ) ;
        }
    } ;
//-----------------------------------------------------------------------------

 int main(int argc, char **argv)
    {

      typedef double bTYPE;
      typedef double TYPE;

     Mixed_Triangle_<Gauss2> Element; 

      int dim = Element.dimension(), 
          num = Element.getNumberOfCorners(), 
          check, iteration;
      int sizeV = Element.size_Set1(), 
          sizeW = Element.size_Set1(),
          sizeP = Element.size_Set2();
      
      std::ifstream PARAMETER;
      PARAMETER.open("triangle.dat",std::ios :: in);
      if (!PARAMETER)
        {
          std::cout << "Parameter file missing .... " << std::endl;
          exit(0);
        }
   // reading some further parameters ...
      PARAMETER >> iteration;
      PARAMETER >> check;

   // reading of vertices   
      std::vector<bTYPE> data(num*dim,0.);
      for (int i=0; i < num*dim; ++i)
         {
           PARAMETER >> data[i];
         }
   
   // reading some testing point, not used in this example
      std::vector<TYPE> tester (dim,0.);
      for (int i=0; i < dim; ++i)
         {
           PARAMETER >> tester[i] ;
         }
      PARAMETER.close();
   // reading done

     std::vector<std::vector<TYPE> > Stencil (sizeP,std::vector<TYPE>(sizeW,0.)); 
     std::vector<std::vector<TYPE> > Laplacian_Stencil (sizeV,std::vector<TYPE>(sizeW,0.)); 
     std::vector<std::vector<TYPE> > Helmholtz_Stencil (sizeV,std::vector<TYPE>(sizeW,0.)); 
////////////////////////////////////////////////////////////////////////////////
//                    Printing some information                               //
////////////////////////////////////////////////////////////////////////////////
#if 0
     std::cout << "\n\n"\
     " *********************************************************************\n"\
     " *   This example computes the local stencils of the Laplacian and   *\n"\
     " *   the Helmholtz operator. The vertices of the used cuboid are:    *\n"\
     " *********************************************************************\n\n  ";
#endif        
#if 1
       for(int i=0; i < num; i++)
          {
            std::cout << "(";
            int j = 0;
            for (; j < dim-1; ++j)
               {
                 std::cout << data[dim*i+j] << ","; 
               }
            std::cout << data[dim*i+j] << ") "; 
          }
#endif        
#if 1
       std::cout << std::endl << std::endl << " Point to be tested: " << std::endl;
       std::cout << "\t(";
       for(int i=0; i < dim-1; ++i)
          {
            std::cout << tester[i] << ", ";
          }
       std::cout << tester[dim-1] << ")" << std::endl;
#endif        
////////////////////////////////////////////////////////////////////////////////
//			Computation of the Stenicls                           //
////////////////////////////////////////////////////////////////////////////////
     // initialize the finite element with the vertex data of the working element
        Define_Integrand( C_(1) & Sin(M_PI * x_<1>()) & C_(1), A );
        Element(data);
        Element.integrate(Laplacian_Stencil, -grad(v_()) * grad(w_()) );
        Stencil = Element.integrate( d_dx(v_()) * p_() );
        Element.integrate(Helmholtz_Stencil, v_()*w_());
        std::cout << "Volume of element: " <<   Element.integrate(C_(1.)) << std::endl;
////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ******************************************\n"\
                        " *  Local Stencil of the Laplace Operator *\n"\
                        " ******************************************\n"\
                 << std::endl ;
       Element.printStencil(Laplacian_Stencil,sizeV,sizeW);

#if 1
////////////////////////////////////////////////////////////////////////////////
//			Stencil Output 
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ********************************************\n"\
                        " *  Local Stencil of the Helmholtz Operator *\n"\
                        " ********************************************\n"\
                 << std::endl ;
       Element.printStencil(Helmholtz_Stencil,sizeV,sizeW);
////////////////////////////////////////////////////////////////////////////////
#endif
#if 1
////////////////////////////////////////////////////////////////////////////////
//			Stencil Output 
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ********************************************\n"\
                        " *  Local Stencil of the left hand Operator *\n"\
                        " ********************************************\n"\
                 << std::endl ;
       Element.printStencil(Stencil,sizeP,sizeW);
////////////////////////////////////////////////////////////////////////////////
#endif
  } 

