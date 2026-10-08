#include <ok/response.hpp>

namespace ok
{

void response::reset()
{
    client = {};
    seq_num = 0;
    version = {};
    status = 0;
    reason.clear();
    headers.clear();
    body_type = http::body_type::text;
    body.clear();
    file.close();
}

void response::setup(std::string_view detail, int status_, std::string_view reason_)
{
    status = status_;
    reason = reason_;
    if (detail.empty())
        detail = reason_;
    body = detail;
    headers["Content-Type"] = "text/plain";
}

void response::ok()
{
    status = 200;
    reason = "OK";
}

void response::bad_request(std::string_view detail, int status_)
{
    setup(detail, status_, "Bad Request");
}

void response::not_found(std::string_view detail, int status_)
{
    setup(detail, status_, "Not Found");
}

void response::internal_server_error(std::string_view detail, int _status)
{
    setup(detail, _status, "Internal Server Error");
}

}