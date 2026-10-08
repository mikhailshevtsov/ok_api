#ifndef OK_CONTENTTYPE_HPP
#define OK_CONTENTTYPE_HPP

#include <vector>
#include <string_view>

namespace ok
{

const std::vector<std::pair<std::string_view, std::string_view>>& content_types();

std::string_view get_extension(std::string_view content_type);
std::string_view get_content_type(std::string_view extension);

}

#endif //OK_CONTENTTYPE_HPP