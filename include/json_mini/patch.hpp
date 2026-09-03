#pragma once

#include "dom.hpp"
#include <string>
#include <vector>

namespace json_mini {

class JsonPointer {
public:
    explicit JsonPointer(const std::string& pointer_str);
    const JsonValue* evaluate(const JsonValue& root) const;

private:
    std::vector<std::string> tokens_;
};

class JsonPatch {
public:
    struct Operation {
        std::string op; // "add", "remove", "replace", "test"
        std::string path;
        JsonValue value;
    };

    static JsonValue apply(const JsonValue& target, const std::vector<Operation>& patch_ops);
};

} // namespace json_mini
