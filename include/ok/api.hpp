#ifndef OK_API_HPP
#define OK_API_HPP

#include "config.hpp"
#include "router.hpp"
#include "worker.hpp"
#include "queue.hpp"
#include "convert.hpp"

#include <vector>

#define ok_handler [&](ok::request& req, ok::response& res) -> ok::result
#define ok_handler_f(NAME) ok::result NAME(ok::request& req, ok::response& res)

#define ok_middleware [&](ok::request& req, ok::response& res, ok::call_next_t& call_next)
#define ok_middleware_f(NAME) void NAME(ok::request& req, ok::response& res, ok::call_next_t& call_next)

namespace ok
{

using middleware = std::function<void(ok::request&, ok::response&, ok::call_next_t&)>;

class api
{
public:
    api(const ok::config& cfg, const ok::router& rtr);

public:
    const ok::config& config() const;
    const ok::router& router() const;

public:
    std::size_t use(const ok::middleware& mw);
    const ok::middleware& middleware(std::size_t index) const;
    std::size_t middleware_count() const;

    void push(const request& req);
    void push(request&& req);
    request next();
    
private:
    ok::config _cfg;
    ok::router _rtr;
    std::vector<ok::middleware> _mws;
    std::vector<ok::worker> _workers;
    ok::queue<ok::request> _reqs;
};

}

#endif //OK_API_HPP