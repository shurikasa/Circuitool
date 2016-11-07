#include "circuit_exporter.hpp"

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>

using namespace std;
namespace morpho{

circuit_exporter::circuit_exporter(const std::string & circuit_filename) :
    file(circuit_filename)
{
}

// Take a matrix of points and apply the specified translation and rotation
// to each point.
branch::mat_points transform(branch::mat_points points,const std::size_t size, const MVD3::Positions pos, const MVD3::Rotations rot){
  for(int i = 0; i< size; i = i+1){
    boost::geometry::concept::Point point = point3(points[i][0],points[i][1],points[i][2]);
    hadoken::geometry::rotate<double>(rot,point);
    for(int j=0; j < 3; ++j){
      point[j] += pos[j];
    }
  }
  return points;
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
    morpho_reader mread(prefix + morphologies[i]+".h5");
    morpho_tree tree = mread.create_morpho_tree();

    for (int j=0; j<tree.get_tree_size(); j = j + 1){
      // Getting every branch of the tree
      branch br = get_branch(j);

      // Remplace the points matrix with the transformed one.
      br.set_points(transform(br.get_points(),br.get_size(),positions[i],rotations[i]),br.get_distances());
    }

    morpho_trees.insert(morpho_trees.end(), tree);
  }

  return morpho_trees;
}
}
