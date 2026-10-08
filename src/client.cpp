#include <ok/client.hpp>

namespace ok
{

client::client() = default;

bool client::alive() const
{
    return _client.alive();
}

ok::context& client::context()
{
    return *static_cast<ok::context*>(_client.data());
}

void client::ready()
{
    _client.send();
}

client::client(bt::client _client)
    : _client(_client)
{}

}