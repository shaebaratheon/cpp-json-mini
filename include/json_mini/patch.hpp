#pragma once
#include "json_mini/value.hpp"
#include <string>
#include <vector>

namespace json_mini {

class JsonPointer {
public:
    explicit JsonPointer(std::string pointer);

    Value* resolve(Value& root) const;
    const Value* resolve(const Value& root) const;
    const std::vector<std::string>& tokens() const { return tokens_; }

private:
    static std::string unescape(const std::string& s);
    std::vector<std::string> tokens_;
};

class JsonPatch {
public:
    // Applies RFC 6902 JSON Patch operations to document
    static bool apply(Value& document, const Value& patch, std::string& error_msg);

private:
    static bool op_add(Value& doc, const std::string& path, const Value& value, std::string& err);
    static bool op_remove(Value& doc, const std::string& path, std::string& err);
    static bool op_replace(Value& doc, const std::string& path, const Value& value, std::string& err);
    static bool op_test(const Value& doc, const std::string& path, const Value& expected, std::string& err);
};

} // namespace json_mini
