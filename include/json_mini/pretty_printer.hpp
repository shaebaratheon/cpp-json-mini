#pragma once

#include "dom.hpp"
#include <string>
#include <sstream>

namespace json_mini {

class PrettyPrinter {
public:
    static std::string format(const JsonValue& val, int indent_spaces = 2) {
        std::ostringstream ss;
        format_impl(val, ss, 0, indent_spaces);
        return ss.str();
    }

private:
    static void format_impl(const JsonValue& val, std::ostringstream& ss, int current_indent, int step) {
        std::string pad(current_indent, ' ');
        if (val.is_object()) {
            const auto& obj = val.as_object();
            if (obj.empty()) { ss << "{}"; return; }
            ss << "{\n";
            size_t i = 0;
            for (const auto& [k, v] : obj) {
                ss << std::string(current_indent + step, ' ') << "\"" << k << "\": ";
                format_impl(v, ss, current_indent + step, step);
                if (++i < obj.size()) ss << ",";
                ss << "\n";
            }
            ss << pad << "}";
        } else if (val.is_array()) {
            const auto& arr = val.as_array();
            if (arr.empty()) { ss << "[]"; return; }
            ss << "[\n";
            for (size_t i = 0; i < arr.size(); ++i) {
                ss << std::string(current_indent + step, ' ');
                format_impl(arr[i], ss, current_indent + step, step);
                if (i + 1 < arr.size()) ss << ",";
                ss << "\n";
            }
            ss << pad << "]";
        } else {
            ss << val.dump();
        }
    }
};

} // namespace json_mini
