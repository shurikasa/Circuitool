#ifndef CIRCUIT_EXPORTER_H
#define CIRCUIT_EXPORTER_H

#include <vector>

#include <mvd/mvd3.hpp>

class gmsh_exporter
{
public:

    gmsh_exporter(const std::string & circuit_filename);


    std::vector<morpho_tree> getAllPositions(const std::string & filename);

    mat_points transform(mat_points points, const Positions pos, const Rotations rot);



private:
    MVD2::MVD3File file;
};


#endif // CIRCUIT_EXPORTER_H
