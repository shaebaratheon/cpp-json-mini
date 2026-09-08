#pragma once
#include <string>
#include <map>
#include <sstream>

namespace json_mini {

class JsonFlattener {
public:
    static std::string join_key(const std::string& parent, const std::string& child) {
        if (parent.empty()) return child;
        return parent + "." + child;
    }

    static std::pair<std::string, std::string> split_first_key(const std::string& compound) {
        size_t dot = compound.find('.');
        if (dot == std::string::npos) return {compound, ""};
        return {compound.substr(0, dot), compound.substr(dot + 1)};
    }
};

} // namespace json_mini
