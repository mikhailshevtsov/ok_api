#ifndef OK_CONFIG_HPP
#define OK_CONFIG_HPP

#include "model.hpp"
#include "types.hpp"

#include <byte_talk/timeout.hpp>

namespace ok
{

using namespace std::chrono_literals;

ok_model(
    config,
    ok_field(workers, std::size_t, 16)
    ok_field(timeout, bt::timeout_t, 30s)
    ok_field(base_dir, ok::path)
    ok_field(temp_dir, ok::path)
    ok_field(html_dir, ok::path, base_dir/"html")
    ok_field(css_dir, ok::path, base_dir/"css")
    ok_field(js_dir, ok::path, base_dir/"js")
    ok_field(img_dir, ok::path, base_dir/"img")
);

}

#endif //OK_CONFIG_HPP