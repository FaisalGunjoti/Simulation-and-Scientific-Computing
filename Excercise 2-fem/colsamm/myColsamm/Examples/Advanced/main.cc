
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

#if 0
std::complex<double> f_(std::complex<double> x, std::complex<double> y, std::complex<double> z){
#else
double f_(double x, double y, double z){
#endif
      return x+y+z;
 }

 int main(int argc, char **argv)
    {

#if 0
     typedef std::complex<double> TYPE;
     typedef double bTYPE;
#else
      typedef double bTYPE;
      typedef double TYPE;
#endif
     ELEMENTS::_Cuboid_<Gauss2,TYPE> Element; 

      int dim = Element.dimension(), 
          num = Element.getNumberOfCorners(), 
          check, iteration;
      int sizeV = Element.size_Set1(), 
          sizeW = Element.size_Set1();
      
      std::ifstream PARAMETER;
      PARAMETER.open("cuboid.dat",std::ios :: in);
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

#if 1
     std::vector<std::vector<TYPE> > Stencil_1 (sizeV,std::vector<TYPE>(sizeW,0.)); 
     std::vector<std::vector<TYPE> > Stencil_2 (sizeV,std::vector<TYPE>(sizeW,0.)); 
#else
     TYPE** Stencil_1 = new TYPE*[sizeV]; 
     for (int i= 0; i < sizeV; ++i)
        {
          Stencil_1[i] = new TYPE[sizeW];
        }
     TYPE** Stencil_2 = new TYPE*[sizeV]; 
     for (int i= 0; i < sizeV; ++i)
        {
          Stencil_2[i] = new TYPE[sizeW];
        }
#endif
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

        Element(data).integrate(Stencil_1, 
           - grad(v_()) * grad(w_()) + A * grad(v_())*w_() + v_()*w_() );

        Element.integrate(Stencil_2[0],   func(f_) *v_());




        std::cout << "Volume of element: " <<   Element.integrate(C_(1.)) << std::endl;



////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ******************************************\n"\
                        " *        Local Stencil of operator 1     *\n"\
                        " ******************************************\n"\
                 << std::endl ;
       Element.printStencil(Stencil_1,sizeV,sizeW);

#if 1
////////////////////////////////////////////////////////////////////////////////
//			Stencil Output 
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ********************************************\n"\
                        " *        Local Stencil of operator 2       *\n"\
                        " ********************************************\n"\
                 << std::endl ;
    // Element.printStencil(Stencil_2,sizeV,sizeW);
       Element.printStencil(Stencil_2[0],sizeV);
////////////////////////////////////////////////////////////////////////////////
#endif
  } 

