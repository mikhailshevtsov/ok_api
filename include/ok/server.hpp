#ifndef OK_SERVER_HPP
#define OK_SERVER_HPP

#include "api.hpp"
#include "context.hpp"

#include <byte_talk/server.hpp>
#include <byte_talk/client.hpp>

#include <mx/parser.hpp>


namespace ok
{

class server
{
public:
    friend class client;
    
    static constexpr short http_port = 8080;

public:
    explicit server(const bt::config& cfg, ok::api& _api);

public:
    int run();

private:
    void set_parser(ok::context* ctx);

private:
    ok::api& _api;
    bt::server _server;
    std::vector<ok::context> _contexts;
    mx::parser::limits _limits;
};

}

#endif //OK_SERVER_HPP