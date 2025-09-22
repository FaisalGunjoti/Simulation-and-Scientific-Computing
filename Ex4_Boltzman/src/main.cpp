#include <iostream>
#include <string>
#include "lbm.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <parameter_file>" << std::endl;
        return 1;
    }

    std::string param_file = argv[1];
    Parameters params;

    try {
        params.loadFromFile(param_file);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Starting LBM simulation with parameters from: " << param_file << std::endl;
    
    LBM simulation(params);
    simulation.run();

    std::cout << "Simulation finished." << std::endl;

    return 0;
}