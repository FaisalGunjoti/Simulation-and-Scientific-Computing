#include "lbm.hpp"
#include <fstream>
#include <sstream>
#include <map>

void Parameters::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open parameter file: " + filename);
    }

    std::map<std::string, std::string> params_map;
    std::string line, key, value;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        if (ss >> key >> value) {
            params_map[key] = value;
        }
    }
    file.close();

    // Assign values, handling both standard and bonus cases
    if (params_map.count("sizex")) sizex = std::stoi(params_map["sizex"]);
    if (params_map.count("sizey")) sizey = std::stoi(params_map["sizey"]);
    if (params_map.count("timesteps")) timesteps = std::stoi(params_map["timesteps"]);
    if (params_map.count("uin")) u_in = std::stod(params_map["uin"]);
    if (params_map.count("Re")) reynolds = std::stod(params_map["Re"]);
    if (params_map.count("spherex")) spherex = std::stoi(params_map["spherex"]);
    if (params_map.count("spherey")) spherey = std::stoi(params_map["spherey"]);
    if (params_map.count("diameter")) diameter = std::stoi(params_map["diameter"]);
    if (params_map.count("vtk_file")) vtk_file = params_map["vtk_file"];
    if (params_map.count("vtk_step")) vtk_step = std::stoi(params_map["vtk_step"]);
    if (params_map.count("geometry")) geometry_file = params_map["geometry"];
}