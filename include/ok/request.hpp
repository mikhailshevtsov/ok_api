#ifndef OK_REQUEST_HPP
#define OK_REQUEST_HPP

#include "client.hpp"
#include "http.hpp"
#include "file.hpp"

#include <string>
#include <unordered_map>

namespace ok
{

struct request
{
    ok::client client;
    std::size_t seq_num = 0;

    ok::http::method method;
    std::string path;
    ok::http::version version;

    std::unordered_map<std::string, std::string> path_params;
    std::unordered_map<std::string, std::string> query_params;

    std::unordered_map<std::string, std::string> headers;

    ok::http::body_type body_type = ok::http::body_type::text;
    std::string body;
    ok::file file;

    int status = 200;
    std::string detail;

    void reset();
};

}

#endif //OK_REQUEST_HPP