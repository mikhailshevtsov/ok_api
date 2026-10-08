#include <ok/http.hpp>

#include <cstring>

namespace ok
{

std::string http::to_string(http::method method)
{
    return std::string(http::str_methods[static_cast<std::size_t>(method)]);
}

bool http::from_string(std::string_view str, http::method& method)
{
    for (std::size_t i = 0; i < http::methods_count; ++i)
    {
        if (str == http::str_methods[i])
        {
            method = static_cast<http::method>(i);
            return true;
        }
    }
    return false;
}

std::string http::to_string(http::version version)
{
    return "HTTP/" + std::to_string(version.major) + "." + std::to_string(version.minor);
}

bool http::from_string(std::string_view str, http::version& version)
{
    std::size_t begin{}, end{};
    
    begin = 0;
    end = str.find_first_of("/", begin);
    if (end == std::string::npos)
        return false;
    
    begin = end + 1;
    end = str.find_first_of(".", begin);
    if (end == std::string::npos)
        return false;

    std::size_t count = 0;

    int major = std::stoi(str.substr(begin, end - begin).data(), &count);
    if (count < end - begin)
        return false;

    begin = end + 1;
    end = str.size();

    int minor = std::stoi(str.substr(begin, end - begin).data(), &count);
    if (count < end - begin)
        return false;
    
    version = {major, minor};
    return true;
}

}
