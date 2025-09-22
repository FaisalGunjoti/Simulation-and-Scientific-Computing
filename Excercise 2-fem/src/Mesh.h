#ifndef MESH_H
#define MESH_H

#include <vector>
#include <array>
#include <string>

// Simple struct to hold an unstructured triangle mesh on the unit circle
// (or any 2D domain).  We read from data/unit_circle.txt, whose format is:
//
//   // Mesh‐file format assumption:
//   // ---------------------------
//   // First come lines of the form:
//   //    idx   x_i   y_i
//   // where idx is an integer (0‐based or 1‐based), x_i,y_i are doubles.
//   //
//   // After all vertices are listed, we list triangles as triplets of
//   // vertex‐indices (integers).  We detect “triangle lines” by
//   // noticing that all three tokens are integers without a decimal point.
//   //
//   // Example (illustrative):
//   //    0  0.00000  0.00000
//   //    1  1.00000  0.00000
//   //    2  0.00000  1.00000
//   //    …
//   //    0 1 2
//   //    1 2 3
//   //
//   // This file does not explicitly give “Nverts Ntria” on the first line.
//   // We read until all lines with (idx, x, y) are consumed; the rest are triangles.
//

struct MeshData {
    std::vector<double>       x, y;         // x[i], y[i], i = 0..nverts-1
    std::vector<std::array<int,3>> triangles;  // triangles[k] = {i,j,k} (0-based)

    // Read mesh from "filename".  Exits on failure.
    void read(const std::string &filename);

    // Number of vertices:
    int nVertices() const { return static_cast<int>(x.size()); }

    // Number of triangles:
    int nTriangles() const { return static_cast<int>(triangles.size()); }
};

#endif // MESH_H
