#include "json_mini/serializer.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace json_mini {

std::string Serializer::serialize(const Value& val, int indent) {
    std::string out;
    format(val, out, indent, 0);
    return out;
}

void Serializer::escape_string(const std::string& str, std::string& out) {
    out += '"';
    for (char c : str) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                    out += buf;
                } else {
                    out += c;
                }
                break;
        }
    }
    out += '"';
}

void Serializer::format(const Value& val, std::string& out, int indent, int current_indent) {
    bool pretty = indent >= 0;
    auto new_line = [&](int level) {
        if (!pretty) return;
        out += '\n';
        out.append(level * indent, ' ');
    };

    switch (val.type()) {
        case Type::Null:
            out += "null";
            break;
        case Type::Boolean:
            out += val.as_bool() ? "true" : "false";
            break;
        case Type::Number: {
            double d = val.as_number();
            if (std::floor(d) == d && !std::isinf(d)) {
                out += std::to_string(static_cast<int64_t>(d));
            } else {
                std::ostringstream ss;
                ss << std::setprecision(15) << d;
                out += ss.str();
            }
            break;
        }
        case Type::String:
            escape_string(val.as_string(), out);
            break;
        case Type::Array: {
            const auto& arr = val.as_array();
            if (arr.empty()) {
                out += "[]";
                return;
            }
            out += '[';
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) out += ',';
                new_line(current_indent + 1);
                format(arr[i], out, indent, current_indent + 1);
            }
            new_line(current_indent);
            out += ']';
            break;
        }
        case Type::Object: {
            const auto& obj = val.as_object();
            if (obj.empty()) {
                out += "{}";
                return;
            }
            out += '{';
            size_t i = 0;
            for (const auto& [k, v] : obj) {
                if (i++ > 0) out += ',';
                new_line(current_indent + 1);
                escape_string(k, out);
                out += pretty ? ": " : ":";
                format(v, out, indent, current_indent + 1);
            }
            new_line(current_indent);
            out += '}';
            break;
        }
    }
}

} // namespace json_mini
