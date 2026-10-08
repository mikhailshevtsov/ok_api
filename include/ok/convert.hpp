#ifndef OK_CONVERTER_HPP
#define OK_CONVERTER_HPP

#include "request.hpp"
#include "types.hpp"
#include "model.hpp"

#include <charconv>
#include <concepts>
#include <string_view>
#include <string>

#define ok_convert(WHAT, NAME, TYPE, ...) \
TYPE NAME{}; \
ok::convert(NAME, req, #NAME, std::string_view(req.WHAT[(#NAME __VA_OPT__(, #__VA_ARGS__))]));

#define ok_path_param(NAME, TYPE, ...) ok_convert(path_params, NAME, TYPE __VA_OPT__(, __VA_ARGS__))
#define ok_query_param(NAME, TYPE, ...) ok_convert(query_params, NAME, TYPE __VA_OPT__(, __VA_ARGS__))
#define ok_header(NAME, TYPE, ...) ok_convert(headers, NAME, TYPE __VA_OPT__(, __VA_ARGS__))
#define ok_body(NAME, TYPE) \
TYPE NAME{}; \
ok::convert(NAME, req, #NAME, std::string_view(req.body));


namespace ok
{

bool convert(ok::json& result, ok::request& req, std::string_view key, const ok::json& value);
bool convert(ok::json& result, ok::request& req, std::string_view key, std::string_view value);

bool convert(ok::path& result, ok::request& req, std::string_view key, const ok::json& value);
bool convert(ok::path& result, ok::request& req, std::string_view key, std::string_view value);

bool convert(ok::text& result, ok::request& req, std::string_view key, const ok::json& value);
bool convert(ok::text& result, ok::request& req, std::string_view key, std::string_view value);

bool convert(std::string_view& result, ok::request& req, std::string_view key, const ok::json& value);
bool convert(std::string_view& result, ok::request& req, std::string_view key, std::string_view value);

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <arithmetic T>
bool convert(T& result, ok::request& req, std::string_view key, const ok::json& value)
{
    std::string str_value = value.dump();
    return convert(result, req, key, std::string_view(str_value));
}

template <arithmetic T>
bool convert(T& result, ok::request& req, std::string_view key, std::string_view value)
{
    std::from_chars(value.data(), value.data() + value.size(), result);
    return true;
}

template <ok::model T>
bool convert(T& result, ok::request& req, std::string_view key, const ok::json& value)
{
    for (const auto& [k, v]: value.items())
    {
        ok::visit(
            [&](auto& val) { ok::convert(val, req, k, v); },
            result,
            k
        );
    }
    return true;
}

template <ok::model T>
bool convert(T& result, ok::request& req, std::string_view key, std::string_view value)
{
    return convert(result, req, key, ok::json::parse(value));
}

}

#endif //OK_CONVERTER_HPP