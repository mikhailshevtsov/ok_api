#ifndef OK_MAKERESPONSE_HPP
#define OK_MAKERESPONSE_HPP

#include "request.hpp"
#include "response.hpp"

#include "types.hpp"

namespace ok
{

void make_response(ok::request& req, ok::response& res, const ok::void_t& value);
void make_response(ok::request& req, ok::response& res, const ok::json& value);
void make_response(ok::request& req, ok::response& res, const ok::text& value);
void make_response(ok::request& req, ok::response& res, const ok::path& value);
void make_response(ok::request& req, ok::response& res, const ok::json_model& value);
void make_response(ok::request& req, ok::response& res, const ok::response& value);
void make_response(ok::request& req, ok::response& res, const ok::result& value);

}

#endif //OK_MAKERESPONSE_HPP