#include <ok/make_response.hpp>
#include <ok/content_types.hpp>

namespace ok
{

void make_response(ok::request& req, ok::response& res, const ok::void_t& value) {}

void make_response(ok::request& req, ok::response& res, const ok::json& value)
{
    res.body = value.dump();
    res.headers["Content-Type"] = "application/json";
};

void make_response(ok::request& req, ok::response& res, const ok::path& value)
{
    std::string_view content_type = ok::get_content_type(value.extension().c_str());
    res.headers["Content-Type"] = content_type;

    if (res.file.open(value, ok::file::mode::read) < 0)
    {
        res.not_found("File not found: " + value.string());
        return;
    }

    if (res.file.size() < res.file.threshold())
    {
        res.body.resize(res.file.size());
        if (res.file.read(res.body.data(), res.body.size()) < 0)
            res.not_found("File not found: " + value.string());
    }
    else
        res.body_type = ok::http::body_type::file;
}

void make_response(ok::request& req, ok::response& res, const ok::text& value)
{
    res.body = value;
    res.headers["Content-Type"] = "text/plain";
}

void make_response(ok::request& req, ok::response& res, const ok::json_model& value)
{
    make_response(req, res, value.data);
}

void make_response(ok::request& req, ok::response& res, const ok::response& value)
{
    res = value;
}

void make_response(ok::request& req, ok::response& res, const ok::result& value)
{
    std::visit([&](const auto& v) { make_response(req, res, v); }, value);
}

}