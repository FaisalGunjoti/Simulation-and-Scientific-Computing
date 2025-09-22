
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
 #include <Colsamm.h>
 #include <testsuite.h>
 using namespace _COLSAMM_;

 int main(int argc , char **argv)
    {
      assert ( 1 < argc );
#if 0
     typedef std::complex<double> TYPE;
      typedef double bTYPE;
#else
      typedef double bTYPE;
      typedef double TYPE;
#endif

      StencilManager<TYPE> file(argv[1]);
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
     ELEMENTS::_Cuboid_<Gauss2,TYPE> Element; 
    // ELEMENTS::Triangle Element; 
    // ELEMENTS::_Triangle_Quadratic_<Gauss2> Element; 
    // ELEMENTS::_Edge2D_<Gauss2> Element; 
    // ELEMENTS::_Cuboid_Boundary_<Gauss2> Element; 
    // ELEMENTS::_Interval_<Gauss2> Element;
    // ELEMENTS::Triangle_C1 Element; 
#endif

      int dim = Element.dimension(), 
          num = Element.getNumberOfCorners(), 
          check = 1, iteration = 1;
      int sizeV = Element.size_Set1(), 
          sizeW = Element.size_Set1();
      
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
//			Computation of the Stenicls                           //
////////////////////////////////////////////////////////////////////////////////
     // initialize the finite element with the vertex data of the working element
       for (int i=0; i < file.size(); ++i)
         {
          Element(file.nextVertexSet());
          Element.integrate(Laplacian_Stencil,  grad(v_())*grad(w_()));
          file.compareStencil(Laplacian_Stencil);
         }
  } 

