cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

add_library(cpp-ranges_cpp-ranges INTERFACE)
target_compile_features(cpp-ranges_cpp-ranges INTERFACE cxx_std_20)
target_include_directories(cpp-ranges_cpp-ranges
    INTERFACE
    ./)
    
add_library(cpp-ranges::cpp-ranges ALIAS cpp-ranges_cpp-ranges)
