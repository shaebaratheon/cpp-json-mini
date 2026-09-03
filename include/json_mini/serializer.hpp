#pragma once
#include "json_mini/value.hpp"
#include <string>

namespace json_mini {

class Serializer {
public:
    static std::string serialize(const Value& val, int indent = -1);

private:
    static void format(const Value& val, std::string& out, int indent, int current_indent);
    static void escape_string(const std::string& str, std::string& out);
};

} // namespace json_mini
