
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

 int main(int argc , char **argv)
    {

      typedef double bTYPE;
      typedef double TYPE;

      ELEMENTS::Vector_Tetrahedron_<Gauss2> Element; 

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
#if 0
      std::vector<TYPE> FunctionValues(sizeV,0.); 
      for (int i=0; i < sizeV; ++i)
         {
           PARAMETER >> FunctionValues[i] ;
         }
#endif
      PARAMETER.close();
   // reading done

#if 1
     std::vector<std::vector<TYPE> > Laplacian_Stencil (sizeV,std::vector<TYPE>(sizeW,0.)); 
     std::vector<std::vector<TYPE> > Helmholtz_Stencil (sizeV,std::vector<TYPE>(sizeW,0.)); 
     std::vector<std::vector<TYPE> > BasisFunctionValues; 
#else
     TYPE** Laplacian_Stencil = new TYPE*[sizeV]; 
     for (int i= 0; i < sizeV; ++i)
        {
          Laplacian_Stencil[i] = new TYPE[sizeW];
        }
     TYPE** Helmholtz_Stencil = new TYPE*[sizeV]; 
     for (int i= 0; i < sizeV; ++i)
        {
          Helmholtz_Stencil[i] = new TYPE[sizeW];
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
#if 1
    Define_Integrand (v_vec3D(), cV);
    Define_Integrand (w_vec3D(), cW);
#endif
    Element(data);

    Element.integrate(Laplacian_Stencil, - ( curl(cV) * curl(cW) ) );
     Element.integrate(Helmholtz_Stencil, cV * cW);

////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ******************************************\n"\
                        " *  Local Stencil of the Laplace Operator *\n"\
                        " ******************************************\n"\
                 << std::endl ;
       Element.printStencil(Laplacian_Stencil,sizeV,sizeW);

////////////////////////////////////////////////////////////////////////////////
//			Stencil Output 
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ********************************************\n"\
                        " *  Local Stencil of the Helmholtz Operator *\n"\
                        " ********************************************\n"\
                 << std::endl ;
       Element.printStencil(Helmholtz_Stencil,sizeV,sizeW);
////////////////////////////////////////////////////////////////////////////////
  } 

