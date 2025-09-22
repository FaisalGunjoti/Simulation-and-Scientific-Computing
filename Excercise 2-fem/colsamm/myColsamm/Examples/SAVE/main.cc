
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
 #include "../../Colsamm/Colsamm.h"
 using namespace _COLSAMM_;

 int main(int argc , char **argv)
    {

#if 0
     typedef std::complex<double> TYPE;
      typedef double bTYPE;
#else
      typedef double bTYPE;
      typedef double TYPE;
#endif
#if 0
     //ELEMENTS::Vector_Cuboid_<Gauss2> Element; 
       ELEMENTS::Vector_Tetrahedron_<Gauss2> Element; 
     //ELEMENTS::Vector_Triangle_<Gauss2> Element;
     //ELEMENTS::Vector_Edge_2D_<Gauss2> Boundary; 
     //ELEMENTS::Vector_Quadrangle_<Gauss2> Element; 
#else
    // ELEMENTS::_Quadrangle_<Gauss3> Element; 
    // ELEMENTS::_Cuboid_<Gauss2> Element; 
    // ELEMENTS::_Hexahedron_<Gauss2> Element; 
    // ELEMENTS::_Hexahedron_Boundary_<Gauss2,TYPE> Element; 
    // ELEMENTS::_Cuboid_Quadratic_<Gauss3> Element; 
   //ELEMENTS::_Tetrahedron_<Gauss2,TYPE> Element; 
     ELEMENTS::_Cuboid_Boundary_<> Element; 
    // ELEMENTS::Triangle Element; 
    // ELEMENTS::_Triangle_Quadratic_<Gauss2> Element; 
    // ELEMENTS::_Edge2D_<Gauss2> Element; 
    // ELEMENTS::_Cuboid_Boundary_<Gauss2> Element; 
    // ELEMENTS::_Interval_<Gauss2> Element;
    // ELEMENTS::Triangle_C1 Element; 
#endif

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
#if 0
     std::cout << "\n\n\n"\
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
#endif
////////////////////////////////////////////////////////////////////////////////
//			Computation of the Stenicls                           //
////////////////////////////////////////////////////////////////////////////////
     // initialize the finite element with the vertex data of the working element
#if 0
    Define_Integrand (v_vec3D(), cV);
    Define_Integrand (w_vec3D(), cW);
#endif
#if 0
    Define_Integrand (Interfaces::x_<0>() & 0.*Interfaces::x_<0>(), my_vec1);
    Define_Integrand (0.*Interfaces::x_<0>() & Interfaces::x_<0>(), my_vec2);
    Define_Integrand (my_vec1^(cW^my_vec1), cW_T);
    Define_Integrand ( 0.*Interfaces::x_<0>() & (Vector_BasisFunction<BasisSet_1,1,dirY>()), test_vec);
#endif
    Define_Integrand ( 0.*Interfaces::x_<0>() & 
                       0.*Interfaces::x_<0>() & 
                       1.*Interfaces::x_<0>() , test_vec);
       for (int i=0; i < iteration; ++i)
         {
          Element(data);
#if 1
       // Element.integrate(Laplacian_Stencil,grad(v_())*grad(w_()));
          Element.integrate(Laplacian_Stencil[0],  N() * grad(v_()) );
       // Element.integrate(Laplacian_Stencil[0], Interfaces::z_<1>() *v_());
       // Element.integrate(Helmholtz_Stencil,     v_() *w_());
#else
 /*
          v_vec2D(0,1).value_corner(Element,0) * u(0) +
          v_vec2D(1,2).value_corner(Element,0) * u(1) +   
          v_vec2D(2,0).value_corner(Element,0) * u(2)   ; 
 */
      //Element.integrate(Laplacian_Stencil, curl(cV) * curl(cW));
        Element.integrate(Laplacian_Stencil, curl(cV) * curl(cW));
        Element.integrate(Helmholtz_Stencil, cV * cW);

#endif
         }
#if 1
    int iterator[1] = {0};
    std::cout << "("  << UnitNormalComponent<0>().eval(Element,iterator) ;
    std::cout << ", " << UnitNormalComponent<1>().eval(Element,iterator);
    std::cout << ", " << UnitNormalComponent<2>().eval(Element,iterator) << ")   <----------->    ";
    std::cout << "("  << NormalComponent<0>().eval(Element,iterator) ;
    std::cout << ", " << NormalComponent<1>().eval(Element,iterator);
    std::cout << ", " << NormalComponent<2>().eval(Element,iterator) << ")" << std::endl;

////////////////////////////////////////////////////////////////////////////////
//			Stencil Output                                        //
////////////////////////////////////////////////////////////////////////////////
       std::cout << "\n\n ******************************************\n"\
                        " *  Local Stencil of the Laplace Operator *\n"\
                        " ******************************************\n"\
                 << std::endl ;
       Element.printStencil(Laplacian_Stencil,sizeV,sizeW);

#endif
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
#if 0
    Boundary(data);
    Boundary.integrate(Laplacian_Stencil[0], Interfaces::y_<2>() * v_()) ;
    Boundary.printStencil(Laplacian_Stencil,1, 1);
    int iterator[1] = {0};
    std::cout << "("  << UnitNormalComponent<0>().eval(Boundary,iterator) ;
    std::cout << ", " << UnitNormalComponent<1>().eval(Boundary,iterator);
    std::cout << ", " << UnitNormalComponent<2>().eval(Boundary,iterator) << ")" << std::endl;

    Element.Point_(tester);
    Laplacian_Stencil[0] = Element.value(v_());
    Laplacian_Stencil[1] = Element.value(d_dx(v_()));
    Laplacian_Stencil[2] = Element.value(d_dy(v_()));
    Laplacian_Stencil[3] = Element.value(d_dz(v_()));
    Element.printStencil(Laplacian_Stencil[0],sizeV);
    Element.printStencil(Laplacian_Stencil[1],sizeV);
    Element.printStencil(Laplacian_Stencil[2],sizeV);
    Element.printStencil(Laplacian_Stencil[3],sizeV);
#endif
  } 

