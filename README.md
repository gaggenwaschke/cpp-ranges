[![gcc-11](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/gcc-11.yaml/badge.svg)](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/gcc-11.yaml)
[![gcc-12](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/gcc-12.yaml/badge.svg)](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/gcc-12.yaml)
[![clang-13](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/clang-13.yaml/badge.svg)](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/clang-13.yaml)
[![clang-14](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/clang-14.yaml/badge.svg)](https://github.com/gaggenwaschke/cpp-ranges/actions/workflows/clang-14.yaml)

# cpp-ranges

This repository contains ranges and views that are not present in the cpp standard but dearly needed.

## views::multi_type_join

Much like the std::ranges::join, but it can join ranges of different types together.
The pre condition is, that the resulting references returned from all joined ranges have a common type.

The view can be used owning, non-owning and mixed:
```cpp
#include <array>
#include <vector>
#include <views/multi_type_join.hpp>

// owning
std::array<int, 3> array{1, 2, 3};
std::vector<int> vector{4, 5};
views::multi_type_join join_view{array, vector};

// non-owning
views::multi_type_join join_view{std::array<int, 3>{1, 2, 3}, std::vector<int>{4, 5}};

// mixed
views::multi_type_join join_view{array, std::vector<int>{4, 5}};
```