#ifndef OK_WORKER_HPP
#define OK_WORKER_HPP

#include "request.hpp"
#include "response.hpp"
#include "handler.hpp"

#include <thread>

namespace ok
{

class api;

class call_next_t
{
public:
    call_next_t(ok::api& _api, ok::request& req, ok::response& res, ok::handler& han, std::size_t index = 0);

    void operator()();

private:
    ok::api& _api;
    ok::request& _req;
    ok::response& _res;
    ok::handler& _han;
    std::size_t _index = 0;
};

class worker
{
public:
    worker(ok::api& _api, std::size_t index);

    void work();

private:
    ok::api& _api;
    std::size_t _index = 0;
    std::jthread _th;
};

}

#endif //OK_WORKER_HPP