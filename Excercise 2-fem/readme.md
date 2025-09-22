
# Beam Waveguide Eigenmode Solver

This program computes the lowest‐order eigenmode of a 2D Helmholtz‐type problem on the unit circle, using finite elements and inverse‐power iteration. Element Matrices are assembled by either using the CoLSamm header or custom 3 -point quadrature.


## 1. Usage
to change the mesh type you have to manually change the file name in waveguide.cpp (line 30)
# Standard run (uses bonus custom quadrature)
./waveguide <δ> <ε>

# Use CoLSaMM flag for CoLSaMM  assembly instead
./waveguide <δ> <ε> --Colsamm


## Arguments to match the reference solution
colsamm
fine parametes ./waveguide 0.00948 1e-12 --Colsamm
coarse parameters ./waveguide 0.01 1e-12 --Colsamm

quadrature
fine paramters ./waveguide 0.2588 1e-12
coarse paramters ./waveguide 2.638 1e-12

- Using these arguments is get the followign lambda values which can be compared with the reference values

| Mesh   | Assembly      | Computed λ         | Reference λ    | Δ = Computed − Reference  |
|--------|---------------|--------------------|----------------|---------------------------|
| Coarse | CoLSaMM       | 88.620369698620    | 88.62036968    | +0.00000001862            |
| Coarse | Quadrature    | 88.620365992610    | 88.62036968    | +0.00000368739            |
| Fine   | CoLSaMM       | 88.350844749047    | 88.35084686    | –0.00000211095            |
| Fine   | Quadrature    | 88.350846354506    | 88.35084686    | –0.00000050549            |


## File Structure and Outputs
- all main code available under root/src/ 
  - waveguide.cpp is the main file here
  - sparse matrices data structure is available under Sparse.cpp
  - CG solver is implemented in Solver.cpp
- mesh data is stored  under root/data/ 
- the refernce ouptuts are avaialbe under root/reference_outputs_
- Make file is avialbe under root/
- All the output matrices/logs .txt and plots are available under output_coarse and output_fine
- plots are done using gnuplot splot command and are available under their respective output folder.