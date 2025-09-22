#include "lbm.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <cstdlib> 

// --- LBM Constants (D2Q9 model) ---
const int Q = 9;
const double w[] = { 4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0 };
const int cx[] = { 0, 1, 0, -1, 0,  1, -1, -1,  1 };
const int cy[] = { 0, 0, 1,  0, -1, 1,  1, -1, -1 };
const int opposite[] = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };

// --- Constructor and Initialization ---
LBM::LBM(const Parameters& params) : p(params) {
    if (!p.geometry_file.empty()) {
        // Assume geometry file is relative to data/ directory
        loadPGM("data/" + p.geometry_file);
    } else {
        this->Nx = p.sizex;
        this->Ny = p.sizey;
    }
    
    grid_width = Nx;
    grid_height = Ny;
    
    f.resize(grid_width * grid_height * Q);
    f_new.resize(grid_width * grid_height * Q);
    flags.resize(grid_width * grid_height);

    //Use Ny (p.sizey) as characteristic length
    double nu = p.u_in * p.sizey / p.reynolds;
    tau = 3.0 * nu + 0.5;
    
    std::cout << "--- Simulation Parameters ---" << std::endl;
    std::cout << "Grid size: " << Nx << " x " << Ny << std::endl;
    std::cout << "Reynolds number: " << p.reynolds << std::endl;
    std::cout << "Inflow velocity (u_in): " << p.u_in << std::endl;
    std::cout << "Kinematic viscosity (nu): " << nu << std::endl;
    std::cout << "Relaxation time (tau): " << tau << std::endl;
    std::cout << "---------------------------" << std::endl;

    if (tau <= 0.51) {
        std::cerr << "Warning: Relaxation time tau (" << tau << ") is close to or below the stability limit of 0.51." << std::endl;
    }
}

void LBM::loadPGM(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Could not open PGM file: " + filename);
    
    std::string line, magic_number;
    std::getline(file, magic_number); // P2
    while (std::getline(file, line) && line[0] == '#');

    std::stringstream ss(line);
    ss >> this->Nx >> this->Ny;

    int max_val;
    file >> max_val;
    
    p.sizex = this->Nx; p.sizey = this->Ny;

    flags.assign(Nx * Ny, FLUID);
    f.assign(Nx * Ny * Q, 0.0);
    f_new.assign(Nx * Ny * Q, 0.0);

    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            int pixel_val;
            file >> pixel_val;
            int y = (Ny - 1) - j; 
            if (pixel_val < 255) {
                flags[y * Nx + i] = OBSTACLE;
            }
        }
    }
    file.close();
    std::cout << "Loaded geometry from " << filename << std::endl;
}

void LBM::initialize() {
    // If we have a geometry file, flags are already set for the obstacle.
    // If not, create the circular obstacle.
    if (p.geometry_file.empty()) {
        double r2 = (p.diameter / 2.0) * (p.diameter / 2.0);
        for (int y = 0; y < Ny; ++y) {
            for (int x = 0; x < Nx; ++x) {
                double dx = x - p.spherex;
                double dy = y - p.spherey;
                flags[idx(x, y)] = (dx * dx + dy * dy <= r2) ? OBSTACLE : FLUID;
            }
        }
    }
    
    // Set wall, inlet, and outlet flags, being careful not to overwrite obstacle cells
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            if (flags[idx(x, y)] == OBSTACLE) continue; // Don't overwrite the obstacle

            if (y == 0 || y == Ny - 1) {
                flags[idx(x, y)] = NO_SLIP;
            } else if (x == 0) {
                flags[idx(x, y)] = VELOCITY;
            } else if (x == Nx - 1) {
                flags[idx(x, y)] = DENSITY;
            }
        }
    }

    // Initialize all cells to equilibrium state
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            double rho = 1.0;
            // Set initial velocity for inlet cells
            double ux = (flags[idx(x, y)] == VELOCITY) ? p.u_in : 0.0;
            double uy = 0.0;
            
            double u_sq = ux * ux + uy * uy;
            for (int k = 0; k < Q; ++k) {
                double cu = cx[k] * ux + cy[k] * uy;
                double feq = w[k] * rho * (1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u_sq);
                f[idx(x, y) * Q + k] = feq;
            }
        }
    }
}


void LBM::collide() {
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            if (flags[idx(x, y)] != FLUID) continue;

            double rho = 0.0, ux = 0.0, uy = 0.0;
            
            for (int k = 0; k < Q; ++k) {
                double fk = f[idx(x, y) * Q + k];
                rho += fk;
                ux += cx[k] * fk;
                uy += cy[k] * fk;
            }

            ux /= rho;
            uy /= rho;
            
            double u_sq = ux * ux + uy * uy;
            for (int k = 0; k < Q; ++k) {
                double cu = cx[k] * ux + cy[k] * uy;
                double feq = w[k] * rho * (1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u_sq);
                int current_idx = idx(x, y) * Q + k;
                f[current_idx] = f[current_idx] * (1.0 - 1.0 / tau) + feq * (1.0 / tau);
            }
        }
    }
}

void LBM::stream() {
    // --- Stream from f to f_new (Pull) ---
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            for (int k = 0; k < Q; ++k) {
                // Calculate source cell coordinates
                int src_x = x - cx[k];
                int src_y = y - cy[k];

                // Handle boundaries using special conditions
                // Note: The logic here is for the destination cell (x,y)
                if (flags[idx(x,y)] == NO_SLIP || flags[idx(x,y)] == OBSTACLE) {
                    // On-grid bounce-back: pull from the opposite direction of the *same* node
                    f_new[idx(x, y) * Q + k] = f[idx(x, y) * Q + opposite[k]];
                } else if (src_x < 0) { // West Velocity Inlet
                    // These are the populations streaming from the west (k=1,5,8)
                    // We must calculate them based on the inlet velocity
                    if (flags[idx(x,y)] == VELOCITY) { // should be x=0
                        double rho0 = 1.0;
                        double u_sq = p.u_in * p.u_in;
                        // Calculate equilibrium for u_in, rho=1
                        double feq = w[k] * rho0 * (1.0 + 3.0 * (cx[k] * p.u_in) + 4.5 * pow(cx[k] * p.u_in, 2) - 1.5 * u_sq);
                        f_new[idx(x,y) * Q + k] = feq;
                    }
                } else if (src_x >= Nx) { // East Density Outlet
                    // Extrapolate distributions from the cell to the left
                    if (flags[idx(x,y)] == DENSITY) { // should be x=Nx-1
                        f_new[idx(x, y) * Q + k] = f[idx(x - 1, y) * Q + k];
                    }
                } else if (src_y < 0 || src_y >= Ny) { // Top/Bottom No-Slip Walls
                    // This is handled by the NO_SLIP flag check at the beginning
                    f_new[idx(x, y) * Q + k] = f[idx(x, y) * Q + opposite[k]];
                } else {
                    // Regular pull stream
                    f_new[idx(x, y) * Q + k] = f[idx(src_x, src_y) * Q + k];
                }
            }
        }
    }

    f.swap(f_new);
}

void LBM::computeForces() {
    double force_x = 0.0;
    double force_y = 0.0;

    for (int y = 1; y < Ny - 1; ++y) {
        for (int x = 1; x < Nx - 1; ++x) {
            // Check neighbors of fluid cells
            if (flags[idx(x, y)] == FLUID) {
                for (int k = 1; k < Q; ++k) {
                    int neighbor_x = x + cx[k];
                    int neighbor_y = y + cy[k];
                    // If neighbor is an obstacle, this link contributes to the force
                    if (flags[idx(neighbor_x, neighbor_y)] == OBSTACLE) {
                        // Momentum exchange uses the post-collision, pre-stream distribution
                        // that streams from the fluid node to the solid node.
                        force_x += 2.0 * f[idx(x, y) * Q + k] * cx[k];
                        force_y += 2.0 * f[idx(x, y) * Q + k] * cy[k];
                    }
                }
            }
        }
    }
    std::cout << "Drag: " << std::scientific << force_x << "\tLift: " << force_y << std::endl;
}

void LBM::writeVTK(int timestep) {
    if (p.vtk_step <= 0) return;
    
    //Construct the full path including the new subdirectory
    std::string subdir = p.vtk_file;
    std::string base_path = "output/" + subdir;

    std::stringstream ss;
    ss << p.vtk_file << std::setw(6) << std::setfill('0') << timestep << ".vtk";
    std::string full_path = base_path + "/" + ss.str();

    std::ofstream vtk_file(full_path);

    if (!vtk_file) {
        std::cerr << "Error: Could not open VTK file for writing: " << full_path << std::endl;
        return;
    }

    vtk_file << "# vtk DataFile Version 3.0\n";
    vtk_file << "LBM Simulation\n";
    vtk_file << "ASCII\n";
    vtk_file << "DATASET STRUCTURED_POINTS\n";
    vtk_file << "DIMENSIONS " << Nx << " " << Ny << " 1\n";
    vtk_file << "ORIGIN 0 0 0\n";
    vtk_file << "SPACING 1 1 1\n";
    vtk_file << "POINT_DATA " << Nx * Ny << "\n";

    vtk_file << "VECTORS velocity double\n";
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            int i = idx(x, y);
            if (flags[i] == FLUID) {
                double rho = 0.0, ux = 0.0, uy = 0.0;
                for (int k = 0; k < Q; ++k) {
                    double fk = f[i * Q + k];
                    rho += fk; ux += cx[k] * fk; uy += cy[k] * fk;
                }
                ux /= rho; uy /= rho;
                vtk_file << ux << " " << uy << " 0.0\n";
            } else {
                vtk_file << "0.0 0.0 0.0\n";
            }
        }
    }
    
    vtk_file << "SCALARS density double 1\n";
    vtk_file << "LOOKUP_TABLE default\n";
    for (int y = 0; y < Ny; ++y) {
        for (int x = 0; x < Nx; ++x) {
            int i = idx(x, y);
            double rho = 0;
            if (flags[i] == FLUID || flags[i] == VELOCITY || flags[i] == DENSITY) {
                for (int k = 0; k < Q; ++k) {
                    rho += f[i * Q + k];
                }
                vtk_file << rho << "\n";
            } else {
                vtk_file << 1.0 << "\n";
            }
        }
    }
    vtk_file.close();
}


void LBM::run() {
    //Create a specific subdirectory for the output
    std::string command = "mkdir -p output/" + p.vtk_file;
    system(command.c_str()); 
    
    initialize();
    
    if (p.vtk_step > 0) {
        writeVTK(0);
    }

    for (int t = 1; t <= p.timesteps; ++t) {
        stream();
        collide();

        if (p.vtk_step > 0 && (t % p.vtk_step == 0)) {
            std::cout << "Timestep: " << t << "/" << p.timesteps << " | ";
            computeForces();
            writeVTK(t);
        } else if (t % 5000 == 0) { 
             std::cout << "Timestep: " << t << "/" << p.timesteps << std::endl;
        }
    }
}