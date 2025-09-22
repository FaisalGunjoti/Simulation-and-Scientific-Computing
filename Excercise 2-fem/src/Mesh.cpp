#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>


//reading the mesh data from a file
void MeshData::read(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: could not open mesh file \"" << filename << "\"\n";
        std::exit(1);
    }

    x.clear();
    y.clear();
    triangles.clear();

    std::string line;
    int Nverts = 0;

    //  Find and parse the line "<Nverts> vertices in the domain"
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        int maybeN;
        std::string token;
        ss >> maybeN >> token;
        if (ss && token == "vertices") {
            Nverts = maybeN;
            break;
        }
    }
    if (Nverts < 0) {
        std::cerr << "Error: expected \"<N> vertices in the domain\" at top of " 
                  << filename << "\n";
        std::exit(1);
    }

    // Skip the next header line(s) until we hit the first vertex line.
    //    We assume the first vertex line starts with an integer (the 0-based index).
 std::string firstVertexLine;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        // first non‑space char?
        const char *p = line.c_str();
        while (*p && std::isspace(static_cast<unsigned char>(*p))) ++p;
        if (*p && std::isdigit(static_cast<unsigned char>(*p))) {
            firstVertexLine = line;            // cache it
            break;
        }
        /* else: keep skipping */
    }

    if (firstVertexLine.empty()) {
        std::cerr << "Error: could not find vertex block in " << filename << "\n";
        std::exit(1);
    }

    /* ------------------------------------------------------------------
     Read exactly Nverts lines of “idx  x  y”
       ------------------------------------------------------------------ */

    x.reserve(Nverts);
    y.reserve(Nverts);

    auto readOneVertex = [&](const std::string& ln, int idx) {
        std::istringstream ss(ln);
        int vidx;      double xv, yv;
        ss >> vidx >> xv >> yv;
        if (!ss) {
            std::cerr << "Error: malformed vertex line #" << idx
                      << " (\"" << ln << "\") in " << filename << "\n";
            std::exit(1);
        }
        x.push_back(xv);
        y.push_back(yv);
    };

    readOneVertex(firstVertexLine, 0);         // the cached first line

    for (int i = 1; i < Nverts; ++i) {
        if (!std::getline(in, line)) {
            std::cerr << "Error: unexpected EOF while reading " << Nverts
                      << " vertex lines in " << filename << "\n";
            std::exit(1);
        }
        if (line.empty()) { --i; continue; }   // skip blanks
        readOneVertex(line, i);
    }

    //  After reading Nverts vertices, *treat every remaining non‐empty line as one triangle*.
    //    Each such line should contain exactly three integers (0-based indices).
        std::vector<std::array<int,3>> rawT;      // temporary storage
    rawT.reserve(2* Nverts);                  // rough guess

    bool zeroSeen = false;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        int a, b, c;
        if (!(ss >> a >> b >> c)) continue;   // skip non‑numeric lines

        if (a == 0 || b == 0 || c == 0) zeroSeen = true;

        rawT.push_back({a, b, c});
    }

    if (rawT.empty()) {
        std::cerr << "Error: no triangles found in " << filename << "\n";
        std::exit(1);
    }

    const bool oneBased = !zeroSeen;

    for (auto t : rawT) {
        int a = t[0], b = t[1], c = t[2];
        if (oneBased) { --a; --b; --c; }      // shift once, consistently

        // sanity check
        if (a < 0 || b < 0 || c < 0 ||
            a >= Nverts || b >= Nverts || c >= Nverts) {
            std::cerr << "Error: triangle index out of range after shifting in "
                      << filename << "\n";
            std::exit(1);
        }
        triangles.push_back({a, b, c});
    }

    in.close();

    if ((int)x.size() != Nverts) {
        std::cerr << "Error: read " << x.size() << " vertices but expected " 
                  << Nverts << " in " << filename << "\n";
        std::exit(1);
    }
    if (triangles.empty()) {
        std::cerr << "Error: no triangles found after reading vertices in " 
                  << filename << "\n";
        std::exit(1);
    }
    // At this point, mesh.nVertices() == Nverts, mesh.nTriangles() == triangles.size()
}
