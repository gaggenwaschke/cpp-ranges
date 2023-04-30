cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(FetchContent)
fetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2
)
fetchContent_MakeAvailable(Catch2)

add_executable(cpp-ranges_unit_test
    multi_type_join.cpp)
target_include_directories(cpp-ranges_unit_test PRIVATE ./)
target_link_libraries(cpp-ranges_unit_test PRIVATE
    cpp-ranges::cpp-ranges
    Catch2::Catch2WithMain)

add_executable(cpp-ranges::unit_test ALIAS cpp-ranges_unit_test)
