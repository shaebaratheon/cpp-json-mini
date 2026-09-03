#pragma once
#include "json_mini/value.hpp"
#include <string>
#include <vector>
#include <optional>

namespace json_mini {

struct ValidationError {
    std::string path;
    std::string message;
};

class SchemaValidator {
public:
    explicit SchemaValidator(Value schema);

    bool validate(const Value& instance, std::vector<ValidationError>& errors) const;
    bool is_valid(const Value& instance) const;

private:
    bool validate_internal(const Value& schema, const Value& instance, const std::string& path, std::vector<ValidationError>& errors) const;
    bool check_type(const std::string& expected_type, const Value& instance) const;

    Value schema_;
};

} // namespace json_mini
