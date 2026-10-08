#include <ok/context.hpp>

namespace ok
{

void context::reset()
{
    parser.reset();
    req.reset();
    res.reset();
    reqs = {};
    keep_alive = closing = pending = false;
    seq_num = bytes_sent = total_bytes = 0;
    start_line.clear();
}

void context::new_request()
{
    req.seq_num = seq_num++;
    reqs.push(req);
}

}