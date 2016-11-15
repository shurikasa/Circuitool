cmake_minimum_required(VERSION 3.2)
project(circuitool)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")

set(Boost_NO_BOOST_CMAKE TRUE)
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

## find dependencies
find_package(Boost 1.41 QUIET REQUIRED COMPONENTS system filesystem program_options)
find_package(HDF5 QUIET REQUIRED)


add_definitions( -std=c++11 )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/highfive/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/hadoken/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/mvd-tool/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/morpho-tool/include)


include_directories(SYSTEM ${Boost_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS})
add_definitions( -DH5_USE_BOOST -DCIRCUI_VERSION_MAJOR=\"${VERSION_MAJOR}\" -DCIRCUI_VERSION_MINOR=\"${VERSION_MINOR}\" )

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
add_executable(circuitool ${SRC_LIST} ${CIRCUI_HEADERS_BITS} ${CIRCUI_HEADERS})
target_link_libraries(circuitool ${HDF5_C_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES}
                            ${Boost_SYSTEM_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARIES})


#add_subdirectory(tests/unit)

add_subdirectory(deps/hadoken)
add_subdirectory(deps/highfive)
add_subdirectory(deps/mvd-tool)
add_subdirectory(deps/morpho-tool)
