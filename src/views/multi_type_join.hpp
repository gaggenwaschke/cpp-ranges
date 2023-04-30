#pragma once

#include <ranges>
#include <type_traits>
#include <tuple>

#include "iterators/multitype_join.hpp"

namespace views
{
    template <typename... range_types>
        requires(std::ranges::range<std::remove_cvref_t<range_types>> && ...)
    class multi_type_join
    {
    public:
        template <std::ranges::range... inner_range_types>
        constexpr multi_type_join(inner_range_types &&...ranges) : ranges{ranges...}
        {
        }

        constexpr auto begin()
        {
            return std::apply([](auto &...ranges)
                              { return iterators::multi_type_join<std::remove_reference_t<range_types>...>{ranges...}; },
                              ranges);
        }

        constexpr auto begin() const
        {
            return std::apply([](auto &...ranges)
                              { return iterators::multi_type_join<std::add_const_t<std::remove_reference_t<range_types>>...>{ranges...}; },
                              ranges);
        }

        constexpr iterators::sentinel end() const
        {
            return {};
        }

    private:
        std::tuple<range_types...> ranges;
    };

    template <std::ranges::range... inner_range_types>
    multi_type_join(inner_range_types &&...ranges) -> multi_type_join<inner_range_types...>;
} // namespace ranges
