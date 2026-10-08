#ifndef OK_ROUTER_HPP
#define OK_ROUTER_HPP

#include "http.hpp"
#include "handler.hpp"
#include "make_response.hpp"

#include <mx/router.hpp>

#include <array>
#include <type_traits>

namespace ok
{

class router
{
private:
    struct path_object
    {
        std::array<ok::handler, ok::http::methods_count> handlers;
    };

    router(const mx::router<path_object>& _router);

    ok::handler& fallback(ok::http::method method);
    const ok::handler& fallback(ok::http::method method) const;

public:
    using on_path_param_t = mx::router<path_object>::on_path_param_t;

public:
    router();

    ok::router& add(ok::http::method method, std::string_view path, const ok::handler& han);
    ok::handler route(ok::http::method method, std::string_view path, const on_path_param_t& on_path_param) const;

    ok::router mount(std::string_view path, ok::router other);
    ok::router unmount(std::string_view path);

    ok::router& get(std::string_view path, const ok::handler& han);
    ok::router& post(std::string_view path, const ok::handler& han);
    ok::router& put(std::string_view path, const ok::handler& han);
    ok::router& patch(std::string_view path, const ok::handler& han);
    ok::router& delete_(std::string_view path, const ok::handler& han);
    ok::router& head(std::string_view path, const ok::handler& han);

private:
    mx::router<path_object> _router;
    path_object _fallback;
};

}

#endif //OK_ROUTER_HPP