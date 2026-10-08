#ifndef OK_RESPONSE_HPP
#define OK_RESPONSE_HPP

#include "client.hpp"
#include "http.hpp"
#include "file.hpp"

#include <string>
#include <unordered_map>

namespace ok
{

struct response
{
    ok::client client;
    std::size_t seq_num = 0;
    
    ok::http::version version;
    int status{};
    std::string reason;

    std::unordered_map<std::string, std::string> headers;

    ok::http::body_type body_type = ok::http::body_type::text;
    std::string body;
    ok::file file;

    void reset();

    void setup(std::string_view detail, int status_, std::string_view reason_);
    
    void ok();
    void bad_request(std::string_view detail, int status_ = 400);
    void not_found(std::string_view detail, int status_ = 404);
    void internal_server_error(std::string_view detail, int _status = 500);
};

}

#endif //OK_RESPONSE_HPP