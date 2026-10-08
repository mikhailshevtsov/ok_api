#ifndef OK_HANDLER_HPP
#define OK_HANDLER_HPP

#include "request.hpp"
#include "response.hpp"
#include "types.hpp"

#include <functional>


namespace ok
{

using handler = std::function<ok::result(ok::request&, ok::response&)>;

}

#endif //OK_HANDLER_HPP