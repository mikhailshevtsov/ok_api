#include <ok/router.hpp>

namespace ok
{

router::router(const mx::router<path_object>& _router) : _router{_router} {}

ok::handler& router::fallback(ok::http::method method) { return _fallback.handlers[static_cast<std::size_t>(method)]; }

const ok::handler& router::fallback(ok::http::method method) const { return _fallback.handlers[static_cast<std::size_t>(method)]; }

router::router() = default;

ok::router& router::add(ok::http::method method, std::string_view path, const ok::handler& han)
{
    if (path == "*")
    {
        fallback(method) = han;
        return *this;
    }

    auto it = _router.insert(path);
    if (!*it)
        *it = path_object{};
    (*it)->handlers[static_cast<std::size_t>(method)] = han;
    return *this;
}

ok::handler router::route(ok::http::method method, std::string_view path, const on_path_param_t& on_path_param) const
{
    auto it = _router.find(path, on_path_param);
    if (!it)
        return fallback(method);
    auto han = (*it)->handlers[static_cast<std::size_t>(method)];
    if (!han)
        return fallback(method);
    return han;
}

ok::router router::mount(std::string_view path, ok::router other)
{
    return ok::router(_router.insert(path, other._router.root()));
}

ok::router router::unmount(std::string_view path)
{
    return ok::router(_router.remove(path));
}

ok::router& router::get(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::get, path, han);
}

ok::router& router::post(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::post, path, han);
}

ok::router& router::put(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::put, path, han);
}

ok::router& router::patch(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::patch, path, han);
}

ok::router& router::delete_(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::delete_, path, han);
}

ok::router& router::head(std::string_view path, const ok::handler& han)
{
    return add(ok::http::method::head, path, han);
}

}