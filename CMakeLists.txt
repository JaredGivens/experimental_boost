cmake_minimum_required(VERSION 3.13.4)

project(experimental_boost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package( Boost )
message(Boost_INCLUDE_DIR="${Boost_INCLUDE_DIR}")
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(${PROJECT_NAME} main.cc)

add_subdirectory(src/Addition)

target_link_libraries(${PROJECT_NAME} Addition ${Boost_LIBRARIES} )