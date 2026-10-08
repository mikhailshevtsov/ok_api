#ifndef OK_SERIALIZER_HPP
#define OK_SERIALIZER_HPP

#include "http.hpp"
#include "request.hpp"
#include "response.hpp"

namespace ok
{

struct serializer
{
    static void serialize(const ok::request& req, std::string& str);
    static void serialize(const ok::response& res, std::string& str);

    static constexpr const char* CRLF = "\r\n";
};

}

#endif //OK_SERIALIZER_HPP