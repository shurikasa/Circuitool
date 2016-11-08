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

// Take a matrix of points and apply the specified translation and rotation
// to each point.
branch::mat_points transform(branch & branche, const MVD3::Positions pos, const MVD3::Rotations rot){
  int rows = branche.get_points().size1();
  branch::mat_points transformed(rows,branche.get_points().size2());
  for(int i = 0; i< rows; i = i+1){
    branch::point point = branche.get_point(i);
    hg::rotate<double>(rot[i],point);
    transformed.insert_element(i,0,hg::cartesian::get_x(point)+pos[i][0]);
    transformed.insert_element(i,1,hg::cartesian::get_y(point)+pos[i][1]);
    transformed.insert_element(i,2,hg::cartesian::get_z(point)+pos[i][2]);
  }

  return transformed;
}

// Obtains a vector of morpho-trees from a circuit file
std::vector<morpho_tree> circuit_exporter::getAllPositions(const std::string & filename){
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

      // Remplace the points matrix with the transformed one.
      br.set_points(transform(br,positions[i],rotations[i]),br.get_distances());
    }

    morpho_trees.insert(morpho_trees.end(), tree);
  }

  return morpho_trees;
}
}
