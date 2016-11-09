#ifndef CIRCUIT_EXPORTER_H
#define CIRCUIT_EXPORTER_H

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>
namespace morpho{

class circuit_exporter
{
public:
    circuit_exporter(const std::string & circuit_filename);

    std::vector<morpho_tree> getAllPositions();

    branch::mat_points transform(branch & br, const double[] pos, const MVD3::Rotations rot);


private:
    MVD3::MVD3File file;
};
}

#endif // CIRCUIT_EXPORTER_H
