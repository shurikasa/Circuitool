#include "circuit_exporter.hpp"

#include <vector>

#include <mvd/mvd3.hpp>
#include <morpho/morpho_h5_v1.hpp>
#include <morpho/morpho_tree.hpp>

using namespace std;
namespace bg = boost::geometry;
namespace fmt = hadoken::format;
namespace morpho{

  circuit_exporter::circuit_exporter(const std::string & circuit_filename, const std::string & data_folder) :
  file(circuit_filename),
  data_folder_name(data_folder)
  {
  }

  /*  Function:     getAllPositions
  *   Description:  Generates a vector of morpho-trees from a .mvd3 circuit file
  *                 and the associated .h5 morphologies.
  *   Output:       morpho_trees : vector of morpho-trees.
  */
  std::vector<morpho_tree> circuit_exporter::getAllPositions(){
    std::string prefix = data_folder_name;
    if(prefix == ""){
      prefix = "data/";
    }

    const MVD3::Positions positions = file.getPositions();
    const MVD3::Rotations rotations = file.getRotations();
    const std::vector<std::string> morphologies = file.getMorphologies();


    std::vector<morpho_tree> morpho_trees;
    fmt::scat(std::cout, "\n Size ", morphologies.size(), "\n");
    for (unsigned int i = 0; i < morphologies.size(); i = i+1 ){
      // Getting a morpho_tree for each morphology
      h5_v1::morpho_reader mread(prefix + morphologies[i]+".h5");
      morpho_tree tree = mread.create_morpho_tree();
      for (unsigned int j=0; j<tree.get_tree_size(); j = j + 1){
        // Getting every branch of the tree
        branch & br = tree.get_branch(j);

        // Remplace the points matrix with the transformed one.

        int rows = br.get_points().size1();
        mat_points transformed(rows,br.get_points().size2());
        for(unsigned int k = 0; k< rows; k = k+1){
          point point = br.get_point(k);
          double point_val [3] = {hadoken::geometry::cartesian::get_x(point),hadoken::geometry::cartesian::get_y(point),hadoken::geometry::cartesian::get_z(point)};
          hadoken::geometry::rotate<double>(rotations[i],point_val);
          transformed.insert_element(k,0,point_val[0]+positions[i][0]);
          transformed.insert_element(k,1,point_val[1]+positions[i][1]);
          transformed.insert_element(k,2,point_val[2]+positions[i][2]);
        }

        vec_double dist = br.get_distances();
        br.set_points(std::move(transformed),std::move(dist));
      }

      morpho_trees.push_back(std::move(tree));
    }
    fmt::scat(std::cout, "FINISH TREATING: ",morphologies.size()," Neuron(s) treated. \n\n");
    return morpho_trees;
  }
}
