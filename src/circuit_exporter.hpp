#ifndef CIRCUIT_EXPORTER_H
#define CIRCUIT_EXPORTER_H

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>
namespace morpho{
namespace MVD3{

class circuit_exporter
{
public:
    circuit_exporter(const std::string & circuit_filename);

    std::vector<morpho_tree> getAllPositions(const std::string & filename);

    branch::mat_points transform(branch::mat_points points, const Positions pos, const Rotations rot);


private:
    MVD3File file;
};
}}

#endif // CIRCUIT_EXPORTER_H
