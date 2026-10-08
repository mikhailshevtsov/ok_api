#include <ok/worker.hpp>
#include <ok/api.hpp>
#include <ok/make_response.hpp>
#include <ok/context.hpp>
#include <ok/http_error.hpp>

namespace ok
{

call_next_t::call_next_t(ok::api& _api, ok::request& req, ok::response& res, ok::handler& han, std::size_t index)
    : _api(_api)
    , _req(req)
    , _res(res)
    , _han(han)
    , _index(index)
{}

void call_next_t::operator()()
{
    if (_index < _api.middleware_count())
        _api.middleware(_index++)(_req, _res, *this);
    else
        make_response(_req, _res, _han(_req, _res));
}

worker::worker(ok::api& _api, std::size_t index)
    : _api(_api)
    , _index(index)
    , _th(&worker::work, this)
{}

void worker::work()
{
    while (true)
    {
        ok::request req = _api.next();

        if (!req.client.alive())
            continue;

        ok::context& ctx = req.client.context();
        ok::response& res = ctx.res;
        res.client = req.client;
        res.seq_num = req.seq_num;
        res.version = req.version;
        res.ok();

        if (req.status == 200)
        {
            ok::handler han = _api.router().route(req.method, req.path, [&req](std::string_view key, std::string_view value)
            {
                req.path_params[std::string(key)] = std::string(value);
            });

            if (han)
            {
                try
                {
                    ok::call_next_t call_next(_api, req, res, han);
                    call_next();
                }
                catch (const ok::http_error& e)
                {
                    res.internal_server_error(e.what(), e.status());
                }
                catch (const std::exception& e)
                {
                    res.internal_server_error(e.what());
                }
            }
            else
                res.not_found("No handler found for " + ok::http::to_string(req.method) + " " + req.path);
        }
        else
            res.bad_request(req.detail, req.status);

        ctx.bytes_sent = 0;
        if (res.body_type == ok::http::body_type::file)
            ctx.total_bytes = res.file.size();
        else
            ctx.total_bytes = res.body.size();

        if (ctx.total_bytes > 0)
            res.headers["Content-Length"] = std::to_string(ctx.total_bytes);

        req.client.ready();
    }
}


}