#include "json_mini/value.hpp"
#include "json_mini/serializer.hpp"

namespace json_mini {

Value::Value() : type_(Type::Null), data_(std::monostate{}) {}
Value::Value(std::nullptr_t) : type_(Type::Null), data_(std::monostate{}) {}
Value::Value(bool b) : type_(Type::Boolean), data_(b) {}
Value::Value(int i) : type_(Type::Number), data_(static_cast<double>(i)) {}
Value::Value(long l) : type_(Type::Number), data_(static_cast<double>(l)) {}
Value::Value(long long ll) : type_(Type::Number), data_(static_cast<double>(ll)) {}
Value::Value(double d) : type_(Type::Number), data_(d) {}
Value::Value(const char* s) : type_(Type::String), data_(std::string(s)) {}
Value::Value(std::string s) : type_(Type::String), data_(std::move(s)) {}
Value::Value(Array arr) : type_(Type::Array), data_(std::move(arr)) {}
Value::Value(Object obj) : type_(Type::Object), data_(std::move(obj)) {}

Type Value::type() const { return type_; }
bool Value::is_null() const { return type_ == Type::Null; }
bool Value::is_bool() const { return type_ == Type::Boolean; }
bool Value::is_number() const { return type_ == Type::Number; }
bool Value::is_string() const { return type_ == Type::String; }
bool Value::is_array() const { return type_ == Type::Array; }
bool Value::is_object() const { return type_ == Type::Object; }

bool Value::as_bool() const {
    if (!is_bool()) throw std::runtime_error("JsonValue is not boolean");
    return std::get<bool>(data_);
}

double Value::as_number() const {
    if (!is_number()) throw std::runtime_error("JsonValue is not number");
    return std::get<double>(data_);
}

int Value::as_int() const {
    return static_cast<int>(as_number());
}

int64_t Value::as_int64() const {
    return static_cast<int64_t>(as_number());
}

const std::string& Value::as_string() const {
    if (!is_string()) throw std::runtime_error("JsonValue is not string");
    return std::get<std::string>(data_);
}

const Value::Array& Value::as_array() const {
    if (!is_array()) throw std::runtime_error("JsonValue is not array");
    return std::get<Array>(data_);
}

Value::Array& Value::as_array() {
    if (!is_array()) throw std::runtime_error("JsonValue is not array");
    return std::get<Array>(data_);
}

const Value::Object& Value::as_object() const {
    if (!is_object()) throw std::runtime_error("JsonValue is not object");
    return std::get<Object>(data_);
}

Value::Object& Value::as_object() {
    if (!is_object()) throw std::runtime_error("JsonValue is not object");
    return std::get<Object>(data_);
}

bool Value::contains(const std::string& key) const {
    if (!is_object()) return false;
    const auto& obj = std::get<Object>(data_);
    return obj.find(key) != obj.end();
}

size_t Value::size() const {
    if (is_array()) return std::get<Array>(data_).size();
    if (is_object()) return std::get<Object>(data_).size();
    if (is_string()) return std::get<std::string>(data_).size();
    return 0;
}

Value& Value::operator[](const std::string& key) {
    if (!is_object()) {
        type_ = Type::Object;
        data_ = Object{};
    }
    return std::get<Object>(data_)[key];
}

const Value& Value::operator[](const std::string& key) const {
    if (!is_object()) throw std::runtime_error("JsonValue is not object");
    const auto& obj = std::get<Object>(data_);
    auto it = obj.find(key);
    if (it == obj.end()) throw std::out_of_range("Key not found: " + key);
    return it->second;
}

Value& Value::operator[](size_t index) {
    if (!is_array()) {
        type_ = Type::Array;
        data_ = Array{};
    }
    auto& arr = std::get<Array>(data_);
    if (index >= arr.size()) {
        arr.resize(index + 1);
    }
    return arr[index];
}

const Value& Value::operator[](size_t index) const {
    if (!is_array()) throw std::runtime_error("JsonValue is not array");
    const auto& arr = std::get<Array>(data_);
    if (index >= arr.size()) throw std::out_of_range("Index out of bounds");
    return arr[index];
}

bool Value::operator==(const Value& other) const {
    if (type_ != other.type_) return false;
    switch (type_) {
        case Type::Null: return true;
        case Type::Boolean: return as_bool() == other.as_bool();
        case Type::Number: return as_number() == other.as_number();
        case Type::String: return as_string() == other.as_string();
        case Type::Array: return as_array() == other.as_array();
        case Type::Object: return as_object() == other.as_object();
    }
    return false;
}

bool Value::operator!=(const Value& other) const {
    return !(*this == other);
}

std::string Value::dump(int indent) const {
    return Serializer::serialize(*this, indent);
}

} // namespace json_mini
