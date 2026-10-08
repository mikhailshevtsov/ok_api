#ifndef OK_CONTEXT_HPP
#define OK_CONTEXT_HPP

#include "request.hpp"
#include "response.hpp"

#include <mx/parser.hpp>
#include <chrono>
#include <queue>

namespace ok
{

struct context
{
    using time_point = std::chrono::steady_clock::time_point;

    mx::parser parser;
    request req;
    response res;
    std::queue<request> reqs;

    bool keep_alive = false;
    bool closing = false;
    bool pending = false;

    std::size_t seq_num = 0;
    std::size_t bytes_sent = 0;
    std::size_t total_bytes = 0;

    std::string start_line;

    time_point opened;
    time_point last_request_read;
    time_point last_response_sent;
    time_point closed;
    
    void reset();
    void new_request();
};

}

#endif //OK_CONTEXT_HPP