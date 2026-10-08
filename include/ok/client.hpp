#ifndef OK_CLIENT_HPP
#define OK_CLIENT_HPP

#include <byte_talk/client.hpp>

namespace ok
{

class context;

class client
{
public:
    friend class server;

    client();

    bool alive() const;
    ok::context& context();
    void ready();

private:
    client(bt::client _client);

    bt::client _client;
};

}
#endif //OK_CLIENT_HPP