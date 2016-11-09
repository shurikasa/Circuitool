#include "circuit_exporter.hpp"

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>

using namespace std;
namespace bg = boost::geometry;
namespace hg = hadoken::geometry;
namespace morpho{

circuit_exporter::circuit_exporter(const std::string & circuit_filename) :
    file(circuit_filename)
{
}

// Obtains a vector of morpho-trees from a circuit file
std::vector<morpho_tree> circuit_exporter::getAllPositions(){
  std::string prefix = "./data/";

  const MVD3::Positions positions = file.getPositions();
  const MVD3::Rotations rotations = file.getRotations();
  const std::vector<std::string> morphologies = file.getMorphologies();

  assert(positions.shape[0] == rotations.shape[0] && positions.shape[0] == morphologies.size);

  std::vector<morpho_tree> morpho_trees;
  for (int i = 0; i < morphologies.size(); i = i+1 ){
    // Getting a morpho_tree for each morphology
    h5_v1::morpho_reader mread(prefix + morphologies[i]+".h5");
    morpho_tree tree = mread.create_morpho_tree();

    for (int j=0; j<tree.get_tree_size(); j = j + 1){
      // Getting every branch of the tree
      branch & br = tree.get_branch(j);

      double pos[3] = {positions[i][0],positions[i][1],positions[i][2]};
      double rot[4] = {rotations[i][0],rotations[i][1],rotations[i][2],rotations[i][3]};
      // Remplace the points matrix with the transformed one.

      int rows = br.get_points().size1();
      branch::mat_points transformed(rows,br.get_points().size2());
      for(int k = 0; k< rows; k = k+1){
        branch::point point = br.get_point(k);
        hg::rotate<double>(rotations[i],point);
        transformed.insert_element(k,0,hg::cartesian::get_x(point)+positions[i][0]);
        transformed.insert_element(k,1,hg::cartesian::get_y(point)+positions[i][1]);
        transformed.insert_element(k,2,hg::cartesian::get_z(point)+positions[i][2]);
      }
      _transformed = std::move(transformed);
      _distances = std::move(br.get_distances())
      br.set_points(_transformed,_distances;
    }

    morpho_trees.insert(morpho_trees.end(), tree);
  }

  return morpho_trees;
}
}
