#ifndef OK_HTTP_HPP
#define OK_HTTP_HPP

#include <string>
#include <string_view>

namespace ok
{

struct http
{
    enum class method
    {
        get, post, put, patch, delete_, head
    };

    struct version
    {
        int major = 0;
        int minor = 0;
    };

    static std::string to_string(http::method method);
    static bool from_string(std::string_view str, http::method& method);

    static std::string to_string(http::version version);
    static bool from_string(std::string_view str, http::version& version);

    static constexpr std::string_view str_methods[] = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD" };
    static constexpr method methods[] = { method::get, method::post, method::put, method::patch, method::delete_, method::head };
    static constexpr std::size_t methods_count = sizeof(methods);

    enum class body_type { text, file };
};

}

#endif //OK_HTTP_HPP