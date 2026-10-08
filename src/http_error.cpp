#include <ok/http_error.hpp>

namespace ok
{

http_error::http_error(const char* detail, int status)
    : std::runtime_error(detail)
    , _status{status}
{}

int http_error::status() const
{
    return _status;
}

}