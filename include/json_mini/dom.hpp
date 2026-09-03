#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <variant>

namespace json_mini {

class JsonValue;
using JsonNull = std::monostate;
using JsonBool = bool;
using JsonNumber = double;
using JsonString = std::string;
using JsonArray = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

class JsonValue {
public:
    using Storage = std::variant<JsonNull, JsonBool, JsonNumber, JsonString, JsonArray, JsonObject>;

    JsonValue() : storage_(JsonNull{}) {}
    JsonValue(std::nullptr_t) : storage_(JsonNull{}) {}
    JsonValue(bool b) : storage_(b) {}
    JsonValue(double n) : storage_(n) {}
    JsonValue(int n) : storage_(static_cast<double>(n)) {}
    JsonValue(const char* s) : storage_(std::string(s)) {}
    JsonValue(std::string s) : storage_(std::move(s)) {}
    JsonValue(JsonArray arr) : storage_(std::move(arr)) {}
    JsonValue(JsonObject obj) : storage_(std::move(obj)) {}

    bool is_null() const { return std::holds_alternative<JsonNull>(storage_); }
    bool is_bool() const { return std::holds_alternative<JsonBool>(storage_); }
    bool is_number() const { return std::holds_alternative<JsonNumber>(storage_); }
    bool is_string() const { return std::holds_alternative<JsonString>(storage_); }
    bool is_array() const { return std::holds_alternative<JsonArray>(storage_); }
    bool is_object() const { return std::holds_alternative<JsonObject>(storage_); }

    double as_number() const { return std::get<JsonNumber>(storage_); }
    const std::string& as_string() const { return std::get<JsonString>(storage_); }
    const JsonArray& as_array() const { return std::get<JsonArray>(storage_); }
    const JsonObject& as_object() const { return std::get<JsonObject>(storage_); }

    std::string dump(int indent = 0) const;

private:
    Storage storage_;
};

} // namespace json_mini
