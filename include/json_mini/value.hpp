#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <variant>
#include <ostream>

namespace json_mini {

enum class Type {
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object
};

class Value {
public:
    using Array = std::vector<Value>;
    using Object = std::map<std::string, Value>;

    Value();
    Value(std::nullptr_t);
    Value(bool b);
    Value(int i);
    Value(long l);
    Value(long long ll);
    Value(double d);
    Value(const char* s);
    Value(std::string s);
    Value(Array arr);
    Value(Object obj);

    Type type() const;
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    bool as_bool() const;
    double as_number() const;
    int as_int() const;
    int64_t as_int64() const;
    const std::string& as_string() const;
    const Array& as_array() const;
    Array& as_array();
    const Object& as_object() const;
    Object& as_object();

    bool contains(const std::string& key) const;
    size_t size() const;

    Value& operator[](const std::string& key);
    const Value& operator[](const std::string& key) const;
    Value& operator[](size_t index);
    const Value& operator[](size_t index) const;

    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;

    std::string dump(int indent = -1) const;

private:
    Type type_;
    std::variant<
        std::monostate,
        bool,
        double,
        std::string,
        Array,
        Object
    > data_;
};

} // namespace json_mini
