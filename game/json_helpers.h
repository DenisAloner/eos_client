#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <type_traits>
#include <string>
#include <optional>

template <typename Class, typename T, typename W>
struct JsonProperty {

    typedef std::u16string (W::*custom_function_t)(T);

    constexpr JsonProperty(T Class::*member,
        const char16_t* name,
        custom_function_t custom_function)
        : member { member }
        , name { name }
        , custom_function { custom_function } {};

    constexpr JsonProperty(T Class::*member,
        const char16_t* name)
        : member { member }
        , name { name } {};

    using type = T;

    T Class::*member;
    const char16_t* name;
    std::optional<custom_function_t> custom_function;
};

template <typename Class, typename T, typename W>
constexpr auto
json_property(T Class::*member, const char16_t* name, std::u16string (W::*fn_ref)(T))
{
    return JsonProperty<Class, T, W> { member, name, fn_ref };
}

template <typename Class, typename T>
constexpr auto
json_property(T Class::*member, const char16_t* name)
{
    return JsonProperty<Class, T, Class> { member, name };
}

template <typename T, T... S, typename F>
constexpr void
for_sequence_json(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t { (static_cast<void>(f(std::integral_constant<T, S> {})), 0)..., 0 };
}

#endif
