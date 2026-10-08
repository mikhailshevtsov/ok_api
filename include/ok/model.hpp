#ifndef OK_MODEL_HPP
#define OK_MODEL_HPP

#include <jarl.hpp>

#define ok_macro JARL_MACRO
#define ok_field JARL_FIELD

#define ok_define_model JARL_DEFINE_STRUCT

#define ok_model JARL_STRUCT

namespace ok
{

template <char... Cs>
using static_string = jarl::static_string<Cs...>;

template <typename T>
concept model = jarl::meta_struct<T>;

template <model T>
using meta = jarl::meta<T>;

template <model T, std::size_t I>
using field = jarl::field<T, I>;

template <model T, std::size_t I>
using field_type_t = jarl::field_type_t<T, I>;

template <std::size_t I, model T>
constexpr decltype(auto) get(T&& object) noexcept { return jarl::get<I>(std::forward<T>(object)); }

template <std::size_t I, model T>
constexpr decltype(auto) get(T&& object, field<T, I> f) noexcept { return jarl::get(std::forward<T>(object), f); }

template <model T, char... Cs>
constexpr decltype(auto) get(T&& object, static_string<Cs...> s = {}) noexcept { return jarl::get(std::forward<T>(object), s); }

template <model T, typename V>
constexpr auto visit(V&& vis, T&& object, std::size_t index) { return jarl::visit(std::forward<V>(vis), std::forward<T>(object), index); }

template <model T, typename V>
constexpr auto visit(V&& vis, T&& object, std::string_view name) { return jarl::visit(std::forward<V>(vis), std::forward<T>(object), name); }

template <model T, typename V>
constexpr auto for_each(V&& vis, T&& object) { return jarl::for_each(std::forward<V>(vis), std::forward<T>(object)); }

template <model T, typename V>
constexpr auto for_each_field(V&& vis)  { return jarl::for_each_field<T>(std::forward<V>(vis)); }

}

#endif //OK_MODEL_HPP