#pragma once

#include <iterator>
#include <ranges>
#include <tuple>
#include <utility>
#include <variant>
#include <type_traits>

namespace iterators
{
    struct sentinel
    {
    };

    template <std::ranges::range... range_types>
    class multi_type_join
    {
    private:
        constexpr static auto size{sizeof...(range_types)};
        constexpr static auto is_const{(std::is_const_v<range_types> || ...)};

        using iterator_sentinel_pairs_type = std::tuple<
            std::pair<
                std::ranges::iterator_t<range_types>,
                std::ranges::sentinel_t<range_types>>...>;
        using size_type = std::remove_cvref_t<decltype(size)>;

    public:
        static_assert((!std::is_reference_v<range_types> && ...));

        using ranges_tuple_type = std::tuple<std::add_lvalue_reference_t<range_types>...>;

        constexpr multi_type_join(range_types &...ranges)
            : index{0}, iterator_sentinel_pairs{std::make_pair(std::ranges::begin(ranges), std::ranges::end(ranges))...}
        {
            increment_while_invalid();
        }

        constexpr multi_type_join &operator++()
        {
            visit([this](auto &iterator, const auto &sentinel)
                  {
            ++iterator;
            if (iterator == sentinel)
            {
                ++index;
            } });
            increment_while_invalid();
            return *this;
        }

        constexpr multi_type_join operator++(int)
        {
            multi_type_join copy{*this};
            operator++();
            return copy;
        }

        constexpr decltype(auto) operator*() const
        {
            if constexpr (is_const)
            {
                return visit([](auto &iterator, auto &sentinel) -> decltype(auto)
                             { return std::as_const(*(iterator)); });
            }
            else
            {
                return visit([](auto &iterator, auto &sentinel) -> decltype(auto)
                             { return *(iterator); });
            }
        }

        friend constexpr bool operator==(const multi_type_join &join_range, sentinel)
        {
            return join_range.index == join_range.size;
        }

    private:
        template <size_type recurse_index = 0>
        struct visiter
        {
            constexpr decltype(auto) operator()(auto &instance, const auto &functor) const
            {
                if (instance.index == recurse_index)
                {
                    return functor(
                        std::get<recurse_index>(instance.iterator_sentinel_pairs).first,
                        std::get<recurse_index>(instance.iterator_sentinel_pairs).second);
                }
                else
                {
                    if constexpr (recurse_index + 1 < size)
                    {
                        return visiter<recurse_index + 1>{}(instance, functor);
                    }
                    else
                    {
                        throw std::out_of_range{"Attempted to visit invalid index"};
                    }
                }
            }
        };

        constexpr decltype(auto) visit(const auto &functor) const
        {
            return visiter{}(*this, functor);
        }

        constexpr decltype(auto) visit(const auto &functor)
        {
            return visiter{}(*this, functor);
        }

        constexpr void increment_while_invalid()
        {
            while (index != size && current_range_is_end())
            {
                ++index;
            }
        }

        constexpr bool current_range_is_end() const
        {
            return visit([](auto &iterator, auto &sentinel)
                         { return iterator == sentinel; });
        }

        size_type index;
        iterator_sentinel_pairs_type iterator_sentinel_pairs;
    };
} // namespace iterators

namespace std
{
    template <std::ranges::range... range_types>
    struct iterator_traits<::iterators::multi_type_join<range_types...>>
    {
        using difference_type = std::ptrdiff_t;
        using value_type = decltype(*std::declval<::iterators::multi_type_join<range_types...> &>());
        using iterator_category = std::forward_iterator_tag;
    };
}
