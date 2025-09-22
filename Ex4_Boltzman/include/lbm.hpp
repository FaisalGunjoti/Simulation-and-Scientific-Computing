#ifndef LBM_HPP
#define LBM_HPP

#include <vector>
#include <string>

// Enum for cell types to make the code more readable
enum CellType {
    FLUID = 0,
    NO_SLIP = 1,
    VELOCITY = 2,
    DENSITY = 3,
    OBSTACLE = 4 // For the bonus task
};

// A simple structure to hold all simulation parameters
struct Parameters {
    int sizex = 400;
    int sizey = 80;
    int timesteps = 50000;
    double u_in = 0.02;
    double reynolds = 40.0;
    int spherex = 100;
    int spherey = 40;
    int diameter = 20;
    std::string vtk_file = "output";
    int vtk_step = 500;
    std::string geometry_file = ""; // For bonus task

    void loadFromFile(const std::string& filename);
};

// The main LBM simulation class
class LBM {
public:
    LBM(const Parameters& params);
    void run();

private:
    // Core simulation steps (Corrected Declarations)
    void initialize();
    void collide();
    void stream();
    void computeForces();
    void writeVTK(int timestep);

    // Helper functions
    void loadPGM(const std::string& filename);

    Parameters p; // Simulation parameters
    
    // Grids
    std::vector<double> f, f_new;
    std::vector<int> flags;       

    // Lattice properties (D2Q9 model)
    static constexpr int Q = 9;

    int Nx, Ny;         // Dimensions of the fluid domain
    int grid_width, grid_height; // Total grid dimensions
    double tau;         // Relaxation time

    // Helper to get index for our 1D arrays
    inline int idx(int x, int y) const { return y * grid_width + x; }
};

#endif // LBM_HPP