#include <ok/request.hpp>

namespace ok
{

void request::reset()
{
    client = {};
    seq_num = 0;
    method = {};
    path.clear();
    version = {};
    path_params.clear();
    query_params.clear();
    headers.clear();
    body_type = http::body_type::text;
    body.clear();
    file.close();
    status = 200;
    detail.clear();
}

}