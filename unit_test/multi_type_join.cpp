#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <array>
#include <vector>
#include <ranges/multi_type_join.hpp>

namespace mock
{
    constexpr static int expected[]{1, 2, 3, 4, 5};
}

TEST_CASE("view::multi_type_join non-owning iterates")
{
    std::array<int, 3> array{1, 2, 3};
    std::vector<int> vector{4, 5};
    ranges::multi_type_join join_view{array, vector};

    CHECK(std::ranges::range<decltype(join_view)>);
    CHECK(std::ranges::equal(join_view, mock::expected));

    for (auto &element : join_view)
    {
        ++element;
    }
    CHECK(array[0] == 2);
    CHECK(array[1] == 3);
    CHECK(array[2] == 4);
    CHECK(vector[0] == 5);
    CHECK(vector[1] == 6);
}

TEST_CASE("view::multi_type_join owning iterates")
{
    ranges::multi_type_join join_view{std::array<int, 3>{1, 2, 3}, std::vector<int>{4, 5}};

    CHECK(std::ranges::range<decltype(join_view)>);
    CHECK(std::ranges::equal(join_view, mock::expected));

    for (auto &element : join_view)
    {
        ++element;
    }
    constexpr static int expected_incremented[]{2, 3, 4, 5, 6};
    CHECK(std::ranges::equal(join_view, expected_incremented));
}

TEST_CASE("view::multi_type_join mixed ownership iterates")
{
    constexpr static std::array<int, 3> array{1, 2, 3};
    ranges::multi_type_join join_view{array, std::vector<int>{4, 5}};

    CHECK(std::ranges::range<decltype(join_view)>);
    CHECK(std::ranges::equal(join_view, mock::expected));
}

TEST_CASE("view::multi_type_join empty ranges iterates")
{
    constexpr static std::array<int, 3> array{1, 2, 3};
    const std::vector<int> empty{};
    const std::vector<int> vector{4, 5};
    ranges::multi_type_join join_view{array, empty, vector};

    CHECK(std::ranges::range<decltype(join_view)>);
    CHECK(std::ranges::equal(join_view, mock::expected));
}
