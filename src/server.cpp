#include <ok/server.hpp>
#include <ok/client.hpp>
#include <ok/serializer.hpp>
#include <ok/content_types.hpp>

namespace ok
{

server::server(const bt::config& cfg, ok::api& _api)
    : _api(_api)
    , _server(cfg)
{
    _contexts.resize(_server.config().max_conn);
    for (auto& ctx : _contexts)
        set_parser(&ctx);

    _server.on_opened += [this](bt::client _client)
    {
        ok::context* ctx = &_contexts[_client.index()];
        ctx->reset();
        _client.set_data(ctx);
        _client.set_timeout(this->_api.config().timeout);

        ctx->opened = bt::now();
        ctx->req.client = _client;
    };

    _server.on_read += [this](bt::client _client)
    {
        ok::context* ctx = static_cast<ok::context*>(_client.data());
        if (ctx->closing)
            return;

        std::string_view msg = _client.read();
        ctx->parser.parse(msg);

        if (!ctx->pending && !ctx->reqs.empty())
        {
            ctx->pending = true;
            auto req = std::move(ctx->reqs.front());
            this->_api.push(std::move(req));
            ctx->reqs.pop();
            _client.set_timeout(this->_api.config().timeout);
        }
    };

    _server.on_sent += [this](bt::client _client)
    {
        ok::context* ctx = static_cast<ok::context*>(_client.data());

        if (ctx->bytes_sent < ctx->total_bytes)
        {
            if (ctx->bytes_sent == 0)
            {
                ok::serializer::serialize(ctx->res, ctx->start_line);
                _client.send(ctx->start_line);
            }
            std::size_t bytes_sent = 0;
            if (ctx->res.body_type == ok::http::body_type::file)
            {
                ctx->res.body.resize(ctx->res.file.threshold() - _client.bytes_to_send());
                bytes_sent = ctx->res.file.read(ctx->res.body.data(), ctx->res.body.size());
                if (bytes_sent < 0)
                {
                    _client.close();
                    return;
                }
            }
            else
                bytes_sent = ctx->res.body.size();

            _client.send(ctx->res.body);
            ctx->bytes_sent += bytes_sent;

            return;
        }

        if (!_client.sent_all())
            return;

        ctx->res.file.close();
        ctx->last_response_sent = bt::now();
        ctx->pending = false;

        if (ctx->closing)
        {
            _client.close();
            return;
        }

        if (!ctx->pending && !ctx->reqs.empty())
        {
            ctx->pending = true;
            auto req = std::move(ctx->reqs.front());
            this->_api.push(std::move(req));
            ctx->reqs.pop();
            _client.set_timeout(this->_api.config().timeout);
        }
    };
}

int server::run()
{
    return _server.run();
}

void server::set_parser(ok::context* ctx)
{
    mx::parser& p = ctx->parser;
    p._data = ctx;
    p._limits = &_limits;

    p.on_method = [](mx::parser* p, std::string_view value)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        if (!ok::http::from_string(value, ctx->req.method))
        {
            ctx->req.detail = std::string("Method ") + value.data() + " is not implemented";
            return false;
        }
        return true;
    };

    p.on_path = [](mx::parser* p, std::string_view value)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);

        if (value[0] != '/')
        {
            ctx->req.detail = "Path must start with \'/\'";
            return false;
        }

        ctx->req.path = value;

        std::size_t begin{}, end{};
    
        begin = 0;
        end = value.find_first_of("?", begin);
        if (end == std::string::npos)
            return true;

        while (end < value.size())
        {
            begin = end + 1;
            end = value.find_first_of("&", begin);
            if (end == std::string::npos)
                end = value.size();

            std::string_view query(value.data() + begin, end - begin);
            std::string_view query_key;
            std::string_view query_value;
            std::size_t mid = query.find_first_of("=");
            if (mid != std::string::npos)
            {
                query_key = std::string_view(query.data(), mid);
                query_value = std::string_view(query.data() + mid + 1, query.size() - mid - 1);
            }
            if (query_key.empty() || query_value.empty())
            {
                ctx->req.detail = "Empty query";
                return false;
            }
            ctx->req.query_params[std::string(query_key)] = std::string(query_value);
        }

        return true;
    };

    p.on_version = [](mx::parser* p, std::string_view value)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        if (!ok::http::from_string(value, ctx->req.version))
        {
            ctx->req.detail = "Invalid version";
            return false;
        }
        return true;
    };

    p.on_header = [](mx::parser* p, std::string_view key, std::string_view value)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        ctx->req.headers[std::string(key)] = std::string(value);
        return true;
    };

    p.on_body = [](mx::parser* p, std::string_view chunk)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        if (ctx->req.body_type == ok::http::body_type::file)
            ctx->req.file.write(chunk.data(), chunk.size());
        else
            ctx->req.body += chunk;
        return true;
    };

    p.on_body_start = [](mx::parser* p, const char* data, std::size_t size)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        ok::request& req = ctx->req;
        if (size < req.file.threshold())
            return true;
        // req.body_type = ok::http::body_type::file;
        // std::string_view content_type = req.headers["Content-Type"];
        // std::string_view extension = ok::get_extension(content_type);
        // req.file.open(ok::path(std::string("temp") + std::string(extension)), ok::file::mode::write);
        return true;
    };

    p.on_error = [](mx::parser* p, const char* ptr, std::size_t size, mx::parser::result res)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        ctx->req.status = 400;
        ctx->closing = true;
        ctx->new_request();
        ctx->last_request_read = bt::now();
        return false;
    };

    p.on_ready = [](mx::parser* p, const char* ptr, std::size_t size)
    {
        ok::context* ctx = static_cast<ok::context*>(p->_data);
        if (ctx->seq_num == 0)
        {
            auto it = ctx->req.headers.find("Connection");
            if (it != std::end(ctx->req.headers) && it->second == "keep-alive")
                ctx->keep_alive = true;
            ctx->closing = !ctx->keep_alive;
        }
        ctx->new_request();
        ctx->last_request_read = bt::now();
        return true;
    };
}

}