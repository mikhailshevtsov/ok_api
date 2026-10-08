#include <ok/content_types.hpp>

#include <algorithm>

namespace ok
{

const std::vector<std::pair<std::string_view, std::string_view>>& content_types()
{
    static std::vector<std::pair<std::string_view, std::string_view>> _content_types
    {
        {".txt",  "text/plain; charset=utf-8"},
        {".html", "text/html; charset=utf-8"},
        {".htm",  "text/html; charset=utf-8"},
        {".css",  "text/css"},
        {".js",   "text/javascript"},
        {".csv",  "text/csv; charset=utf-8"},
        {".json", "application/json"},
        {".xml",  "application/xml"},
        {".pdf",  "application/pdf"},
        {".zip",  "application/zip"},
        {".wasm", "application/wasm"},
        {".jpeg", "image/jpeg"},
        {".jpg",  "image/jpeg"},
        {".png",  "image/png"},
        {".gif",  "image/gif"},
        {".webp", "image/webp"},
        {".svg",  "image/svg+xml"},
        {".ico",  "image/x-icon"},
        {".mp3",  "audio/mpeg"},
        {".wav",  "audio/wav"},
        {".mp4",  "video/mp4"},
        {".webm", "video/webm"},
        {".woff", "font/woff"},
        {".woff2","font/woff2"},
        {"?",     "appllication/octet-stream"}
    };
    return _content_types;
}

std::string_view get_extension(std::string_view content_type)
{
    auto it = std::find_if(std::cbegin(content_types()), std::cend(content_types()), [content_type](const auto& kv) { return kv.second == content_type; });
    if (it != std::cend(content_types()))
        return it->first;
    return {};
}

std::string_view get_content_type(std::string_view extension)
{
    auto it = std::find_if(std::cbegin(content_types()), std::cend(content_types()), [extension](const auto& kv) { return kv.first == extension; });
    if (it != std::cend(content_types()))
        return it->second;
    return {};
}

}