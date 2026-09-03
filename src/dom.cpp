#include "json_mini/dom.hpp"
#include <iomanip>

namespace json_mini {

std::string JsonValue::dump(int indent) const {
    if (is_null()) return "null";
    if (is_bool()) return std::get<JsonBool>(storage_) ? "true" : "false";
    if (is_number()) {
        std::ostringstream ss;
        ss << std::get<JsonNumber>(storage_);
        return ss.str();
    }
    if (is_string()) {
        return "\"" + std::get<JsonString>(storage_) + "\"";
    }
    if (is_array()) {
        const auto& arr = as_array();
        std::string out = "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            out += arr[i].dump(indent);
            if (i + 1 < arr.size()) out += ", ";
        }
        out += "]";
        return out;
    }
    if (is_object()) {
        const auto& obj = as_object();
        std::string out = "{";
        size_t idx = 0;
        for (const auto& [k, v] : obj) {
            out += "\"" + k + "\": " + v.dump(indent);
            if (++idx < obj.size()) out += ", ";
        }
        out += "}";
        return out;
    }
    return "";
}

} // namespace json_mini
