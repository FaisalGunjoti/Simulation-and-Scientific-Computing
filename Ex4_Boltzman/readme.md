# Lattice Boltzmann Method

This project is a C++ implementation of the Lattice Boltzmann Method (LBM) to simulate 2D fluid flow around obstacles in a virtual wind tunnel. It was completed for the "Simulation and Scientific Computing 2" course.

## Usage
- This project is a C++ implementation of the Lattice Boltzmann Method (LBM) to simulate 2D fluid flow around obstacles in a virtual wind tunnel. It was completed for the "Simulation and Scientific Computing 2" course.
- ./build/lbm <path_to_parameter_file>
- Example:
    - ./build/lbm data/params_Re40.dat
    - ./build/lbm data/params_Re500.dat
    - ./build/lbm data/params_Re100_wing.dat

## Output

- Simulation results are saved as .vtk files in the output/ directory. Each simulation run creates a unique sub-folder within output/ (e.g., output/Re40_/) to keep results organized. These files can be visualized using ParaView. I have also added the screenshot of the visualizaiton from paraview.

- For Re=500, the t(tau) value came out to be 0.5096 so i have increased the numerical resolution (params_Re500_highres.dat) and performed the simualtion again whihc gave t(tau) = 0.5384
## Project Structure
The project is organized into the following directories and files:

* **`build/`**: This directory is where the compiler places all intermediate object files and the final `lbm` executable. It is created automatically when you run `make`.
* **`data/`**: All input files for the simulation, such as the parameter files (`.dat`) and geometry files (`.pgm`), should be placed here.
* **`include/`**: Contains the C++ header files (`.hpp`), which declare the classes and functions used in the project.
* **`src/`**: Contains the C++ source files (`.cpp`), which hold the implementation of the simulation logic.
* **`output/`**: This directory is created automatically when a simulation is run. It stores the resulting `.vtk` files in subdirectories named after each simulation case.
* **`Makefile`**: The script that automates the compilation process.
* **`README.md`**: The main documentation file for the project.
