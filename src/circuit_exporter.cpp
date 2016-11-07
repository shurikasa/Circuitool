#include "circuit_exporter.hpp"

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>

using namespace std;
namespace morpho{

circuit_exporter::circuit_exporter(const std::string & circuit_filename) :
    reader(circuit_filename)
{


}

// Take a matrix of points and apply the specified translation and rotation
// to each point.
morpho_tree::mat_points transform(morpho_tree::mat_points points, const Positions pos, const Rotations rot){
  for(int i = 0; i< points.shape[0]; i = i+1){
    Point point = point3(points[i][0],points[i][1],points[i][2])
    hadoken::geometry::rotate<double>(rotations[i],point[j]);
    for(int j=0; j < 3; ++k){
      point[j] += translation[j];
    }
  }
  return points;
}

// Obtains a vector of morpho-trees from a circuit file
std::vector<morpho_tree> circuit_exporter::getAllPositions(const std::string & filename){
  using namespace MVD3;
  std::string prefix = "./data/";

  MVD3File file(filename);
  const Positions positions = file.getPositions();
  const Rotations rotations = file.getRotations();
  const std::vector<std::string> morphologies = file.getMorphologies();

  assert(positions.shape[0] == rotations.shape[0] && positions.shape[0] == morphologies.size);

  std::vector<morpho_tree> morpho_trees;
  for (int i = 0; i < morphologies.size(); i = i+1 ){
    // Getting a morpho_tree for each morphology
    morpho_reader mread(prefix + morphologies[i]+".h5");
    morpho_tree tree = mread.create_morpho_tree();

    for (int j=0; j<tree.get_tree_size(); j = j + 1){
      // Getting every branch of the tree
      branch br = get_branch(j);

      // Remplace the points matrix with the transformed one.
      br.set_points(transform(br.get_points(),positions[i],rotations[i]),br.get_distances());
    }

    morpho_trees.insert(morpho_trees.end(), tree);
  }

  return morpho_trees;
}
}
