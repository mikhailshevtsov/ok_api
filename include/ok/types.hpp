#ifndef OK_TYPES_HPP
#define OK_TYPES_HPP

#include <nlohmann/json.hpp>
#include <filesystem>
#include <string>
#include <string_view>
#include <variant>

#include "model.hpp"
#include "response.hpp"

namespace ok
{

struct void_t {};

using json = nlohmann::json;
using path = std::filesystem::path;
using text = std::string;

struct json_model
{
    ok::json data;

    template <ok::model T>
    json_model(T&& value)
    {
        ok::for_each_field<std::remove_cvref_t<T>>([this, &value](auto field) { data[field.name()] = ok::get(std::forward<T>(value), field); });
    }

    template <ok::model T>
    json_model& operator=(T&& value)
    {
        json_model temp(std::forward<T>(value));
        std::swap(temp, *this);
        return *this;
    }
};

using result = std::variant<ok::void_t, ok::json, ok::path, ok::text, ok::json_model, ok::response>;

}

#endif //OK_TYPES_HPP