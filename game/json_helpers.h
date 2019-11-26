#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <type_traits>

template <typename Class, typename T>
struct JsonProperty {
    constexpr JsonProperty(T Class::*member,
        const char16_t* name)
        : member { member }
        , name { name } {};

    T Class::*member;
    const char16_t* name;
};

template <typename F, std::size_t... S>
constexpr void static_for_impl(F&& function, std::index_sequence<S...>)
{
    int unpack[] = { 0,
        (void(function(std::integral_constant<std::size_t, S> {})), 0)... };

    (void)unpack;
}

template <typename Tuple>
constexpr std::size_t tuple_size(const Tuple&)
{
    return std::tuple_size<Tuple>::value;
}

template <auto N, typename F>
constexpr void static_for(F&& function)
{
    static_for_impl(std::forward<F>(function), std::make_index_sequence<tuple_size(N())>());
}

#endif
