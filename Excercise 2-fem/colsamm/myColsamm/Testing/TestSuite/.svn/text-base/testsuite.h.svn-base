#include <iostream>
#include <fstream>

template <typename Type>
class StencilManager {
   private:
     std::ifstream PARAMETER;
     char* filename;
     int counter;
     Type** actual_stencil; double *vertexSet;
     unsigned int dimension, stencil_dimension, stencil_size, number_stencils, number_vertices;
     bool info;
   public:
     StencilManager (char * name);
     ~StencilManager ();
     
     double* nextVertexSet();
     int size() const { return number_stencils;}
   
     template <class T>
     void compareStencil( const T& vect ) const;
  
 };


template <class Type>
StencilManager<Type>::StencilManager(char* name) : filename(name){
      PARAMETER.open(filename,std::ios :: in);
      if (!PARAMETER)
        {
          std::cout << "Parameter file " << filename << " missing ... exiting" << std::endl;
          exit(0);
        }
       char buff[80];
       PARAMETER.getline(buff,80);
       std::cout << buff << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> dimension;
       std::cout << " -- " << buff << ": "<< dimension << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> stencil_dimension;
       std::cout << " -- " << buff << ": "<< stencil_dimension << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> stencil_size;
       std::cout << " -- " << buff << ": "<< stencil_size << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> number_stencils;
       std::cout << " -- " << buff << ": "<< number_stencils << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> number_vertices;
       std::cout << " -- " << buff << ": "<< number_vertices << std::endl;
       PARAMETER.getline(buff,80);
       PARAMETER.getline(buff,80, ':');
       PARAMETER >> info;
       std::cout << " -- " << buff << ": ";
       if(info)
         std::cout <<"yes";
       else 
         std::cout << "no" << std::endl;
       std::cout << std::endl;
      
   // Initialization of the arrays
      actual_stencil = new Type* [stencil_size];
      for(int i=0; i < stencil_size; ++i) {
          actual_stencil[i] = new Type [stencil_size];
          for(int j=0; j < stencil_size; ++j)
            actual_stencil[i][j] = 0.;
        }
      vertexSet = new double [dimension*number_vertices];
      counter = 0;
  }

template <class Type>
double* StencilManager<Type>::nextVertexSet() {
      counter++;
      char buff[80];
      PARAMETER.getline(buff,80);
      PARAMETER.getline(buff,80);
      PARAMETER.getline(buff,80);
      PARAMETER.getline(buff,80);
      for (int i=0; i < number_vertices*dimension; ++i)
         {
           PARAMETER >> vertexSet[i];
         }
#if 1
       std::cout << std::endl << "Testing suite example #"<< counter << std::endl;
       std::cout << "Vertices of the actual element: " << std::endl;
       for(int i=0; i < number_vertices; i++)
          {
            std::cout << "(";
            int j = 0;
            for (; j < dimension-1; ++j)
               {
                 std::cout << vertexSet[dimension*i+j] << ","; 
               }
            std::cout << vertexSet[dimension*i+j] << ") "; 
          }
#endif        
      std::cout << std::endl;
      PARAMETER.getline(buff,80);
      PARAMETER.getline(buff,80);
      for (int i=0; i < stencil_size; ++i)
         for (int j=0; j < stencil_size; ++j)
            {
              PARAMETER >> actual_stencil[i][j];
            }
      return vertexSet;
   }

template <class Type>
template <class T>
void StencilManager<Type>::compareStencil (const T& vec) const {
      for (int i=0; i < stencil_size; ++i)
         for (int j=0; j < stencil_size; ++j)
            {
               if (info)
                   std::cout << vec[i][j] << " " << actual_stencil[i][j] << std::endl;
               if (fabs(vec[i][j]) < 1.e-4 )
                  assert ( fabs(vec[i][j]-actual_stencil[i][j]) < 1.e-4 );
               else 
                  assert ( fabs(vec[i][j]-actual_stencil[i][j])/fabs(vec[i][j]) < 1.e-4 );
                  //assert ( fabs(fabs(vec[i][j])-fabs(actual_stencil[i][j]))/fabs(vec[i][j]) < 1.e-4 );
            }
     std::cout << " .. check went just fine! " << std::endl;
  }


template <class Type>
StencilManager<Type>::~StencilManager() {
     for (int i=0; i< stencil_size; ++i)
       {
           delete [] actual_stencil[i];
       }
     delete [] actual_stencil;
     delete [] vertexSet;

     PARAMETER.close();
   }
