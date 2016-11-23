# Circuitool

## Description
The Circuitool is designed to extract morphologies information (name, position, rotation) out of a circuit *MVD3* file to apply those informations to each morphology. The result is returned then as a set of *morpho-trees* that can be exported thanks to the **Morpho-tool**.
Circuitool only use the options *wireframe*, *single_soma*, *write_dmg* and *bounding_box* from **Morpho-tool**:

## Installation
To install it:
```
git clone git://github.com/sv91/Circuitool.git
git submodule update --init --recursive

mkdir build
cd build

cmake3 ..
make
```

## Use
To use the Circuitool after installing, simply call:
```
./circuitool export [circuit-file] [geo-file]
```
**Important:**
The *H5* files of the used morphologies need to be located in a */data* folder in the */build* folder.

## Improvement list
* Possibility to choose options.
* Possibility to choose folder where the *H5* files are.
* Integration with **GMSH**.
