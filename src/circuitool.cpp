#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <chrono>

#include <boost/program_options.hpp>
#include <boost/geometry.hpp>

#include <hadoken/format/format.hpp>

#include <mesh_exporter.hpp>

#include "circuit_exporter.hpp"

using namespace std;
using namespace morpho;
using namespace std::chrono;

namespace fmt = hadoken::format;
namespace po = boost::program_options;

po::parsed_options parse_args(int argc, char** argv,
                         po::variables_map & res,
                         std::string & help_msg
                             ){
    po::options_description general("Commands:\n"
                                    "\t\t\n"
                                    "  export gmsh [circuit-file] [geo-file]:\texport circuit file to .geo file format\n"
                                    "\n\n"
                                    "Options");
    general.add_options()
        ("help", "produce a help message")
        ("command", po::value<std::string>(), "command to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for command");
        ;
    po::positional_options_description pos;
    pos.add("command", 1)
       .add("subargs", -1)
            ;

    auto opts = po::command_line_parser(argc, argv )
            .options(general)
            .positional(pos)
            .allow_unregistered()
            .run();
    po::store(opts, res);
    if(res.count("help")){
        fmt::scat(std::cout, general, "\n");
        exit(0);
    }
    help_msg = fmt::scat(general);
    return opts;
}



void export_circuit_to_gmsh(const std::string & filename_circuit, const std::string & filename_geo,
                          po::variables_map & options){

    circuit_exporter circuit(filename_circuit);

    gmsh_exporter::exporter_flags flags = 0;
    flags |= gmsh_exporter::exporter_single_soma;
    flags |= gmsh_exporter::exporter_write_dmg;
    flags |= gmsh_exporter::exporter_bounding_box;
   high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::vector<morpho_tree> morpho_trees = circuit.getAllPositions();
   high_resolution_clock::time_point t2 = high_resolution_clock::now();
auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
   fmt::scat(std::cout, "\nDuration: ", duration, "\n\n");    
gmsh_exporter exporter(std::move(morpho_trees), filename_geo, flags);
 fmt::scat(std::cout, "\nEXPORTING\n\n");
    exporter.export_to_wireframe();
   high_resolution_clock::time_point t3 = high_resolution_clock::now();
auto duration2 = duration_cast<milliseconds>( t3 - t2 ).count();
   fmt::scat(std::cout, "\nDuration: ", duration2, "\n\n");

    fmt::scat(std::cout, "\nConverting ", filename_circuit, " to gmsh file format ... ", filename_geo, "\n\n");
}

int main(int argc, char** argv){
    po::variables_map options;
    std::string help_string;
    try{
        auto parsed_options = parse_args(argc, argv, options, help_string);

        if(options.count("command") && options.count("subargs") ){
            std::string command = options["command"].as<std::string>();
            std::vector<std::string> subargs = po::collect_unrecognized(parsed_options.options, po::include_positional);
            if(command == "export" ){
                if(subargs.size() == 3){
                    export_circuit_to_gmsh(subargs[1], subargs[2], options);
                    return 0;
                }
            };
        }

        fmt::scat(std::cout, "\nWrong command usage, see --help for details\n\n");
    }catch(std::exception & e){
        fmt::scat(std::cerr,
                  argv[0], "\n",
                "Error ", e.what(), "\n");
        exit(-1);
    }
    return 0;
}
