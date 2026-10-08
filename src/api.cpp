#include <ok/api.hpp>

namespace ok
{

api::api(const ok::config& cfg, const ok::router& rtr)
    : _cfg(cfg)
    , _rtr(rtr)
{
    _workers.reserve(_cfg.workers);
    for (std::size_t i = 0; i < _cfg.workers; ++i)
        _workers.emplace_back(*this, i);
}

const ok::config& api::config() const
{
    return _cfg;
}

const ok::router& api::router() const
{
    return _rtr;
}

std::size_t api::use(const ok::middleware& mw)
{
    _mws.push_back(mw);
    return _mws.size() - 1;
}

const ok::middleware& api::middleware(std::size_t index) const
{
    return _mws.at(index);
}

std::size_t api::middleware_count() const
{
    return _mws.size();
}

void api::push(const request& req)
{
    _reqs.push(req);
}

void api::push(request&& req)
{
    _reqs.push(std::move(req));
}

request api::next()
{
    return _reqs.next();
}


}