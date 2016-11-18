#include "circuit_exporter.hpp"

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>

using namespace std;
namespace bg = boost::geometry;
namespace hg = hadoken::geometry;
namespace fmt = hadoken::format;
namespace morpho{

circuit_exporter::circuit_exporter(const std::string & circuit_filename) :
    file(circuit_filename)
{
}

// Obtains a vector of morpho-trees from a circuit file
std::vector<morpho_tree> circuit_exporter::getAllPositions(){
  std::string prefix = "data/";

  const MVD3::Positions positions = file.getPositions();
  const MVD3::Rotations rotations = file.getRotations();
  const std::vector<std::string> morphologies = file.getMorphologies();


  std::vector<morpho_tree> morpho_trees;
    fmt::scat(std::cout, "\n Size ", morphologies.size(), "\n\n");  
for (unsigned int i = 0; i < morphologies.size(); i = i+1 ){
    fmt::scat(std::cout, "\nOpening file numero ",i," called ", morphologies[i], "\n\n");
    // Getting a morpho_tree for each morphology
    h5_v1::morpho_reader mread(prefix + morphologies[i]+".h5");
    fmt::scat(std::cout, "\nOpened file numero ",i," called ", morphologies[i], "\n\n");    
morpho_tree tree = mread.create_morpho_tree();

    for (unsigned int j=0; j<tree.get_tree_size(); j = j + 1){
      // Getting every branch of the tree
      branch & br = tree.get_branch(j);

      // Remplace the points matrix with the transformed one.

      int rows = br.get_points().size1();
      branch::mat_points transformed(rows,br.get_points().size2());
      for(unsigned int k = 0; k< rows; k = k+1){
        branch::point point = br.get_point(k);
        double point_val [3] = {hg::cartesian::get_x(point),hg::cartesian::get_y(point),hg::cartesian::get_z(point)};
        hg::rotate<double>(rotations[i],point_val);
        transformed.insert_element(k,0,hg::cartesian::get_x(point)+positions[i][0]);
        transformed.insert_element(k,1,hg::cartesian::get_y(point)+positions[i][1]);
        transformed.insert_element(k,2,hg::cartesian::get_z(point)+positions[i][2]);
      }
      branch::vec_double dist = br.get_distances();
      br.set_points(std::move(transformed),std::move(dist));
    }
    fmt::scat(std::cout, "\nTreated file ",i,"\n\n");

    morpho_trees.push_back(std::move(tree));
  }
    fmt::scat(std::cout, "\n FINISH TREATING \n\n");
  return morpho_trees;
}
}
