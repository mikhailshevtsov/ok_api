#include <ok/convert.hpp>

namespace ok
{

bool convert(ok::json& result, ok::request& req, std::string_view key, const ok::json& value)
{
    result = value;
    return true;
}

bool convert(ok::json& result, ok::request& req, std::string_view key, std::string_view value)
{
    result = ok::json::parse(value);
    return true;
}

bool convert(ok::path& result, ok::request& req, std::string_view key, const ok::json& value)
{
    result = ok::path(value.dump());
    return true;
}

bool convert(ok::path& result, ok::request& req, std::string_view key, std::string_view value)
{
    result = ok::path(value);
    return true;
}

bool convert(ok::text& result, ok::request& req, std::string_view key, const ok::json& value)
{
    result = value.dump().substr(1, value.dump().size() - 2);
    return true;
}

bool convert(ok::text& result, ok::request& req, std::string_view key, std::string_view value)
{
    result = ok::text(value);
    return true;
}

bool convert(std::string_view& result, ok::request& req, std::string_view key, const ok::json& value)
{
    result = value.dump().substr(1, value.dump().size() - 2);
    return true;
}

bool convert(std::string_view& result, ok::request& req, std::string_view key, std::string_view value)
{
    result = value;
    return true;
}

}