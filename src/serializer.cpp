#include <ok/serializer.hpp>

namespace ok
{

void serializer::serialize(const ok::request& req, std::string& str)
{
    str += std::string(ok::http::to_string(req.method)) + " " + req.path + " " + ok::http::to_string(req.version) + CRLF;
    for (const auto& [key, value] : req.headers)
        str += key + ": " + value + CRLF;
    str += CRLF;
}

void serializer::serialize(const ok::response& res, std::string& str)
{
    str += ok::http::to_string(res.version) + " " + std::to_string(res.status) + " " + res.reason + CRLF;
    for (const auto& [key, value] : res.headers)
        str += key + ": " + value + CRLF;
    str += CRLF;
}

}