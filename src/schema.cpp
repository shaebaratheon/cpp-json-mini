#include "json_mini/schema.hpp"
#include <iostream>

namespace json_mini {

JsonSchemaValidator::JsonSchemaValidator(JsonValue schema) : schema_(std::move(schema)) {}

bool JsonSchemaValidator::validate(const JsonValue& instance, std::vector<std::string>& errors) const {
    if (!schema_.is_object()) return true;

    const auto& schema_obj = schema_.as_object();

    // 1. "type" keyword check
    if (schema_obj.count("type")) {
        const auto& type_val = schema_obj.at("type");
        if (type_val.is_string()) {
            const std::string& expected_type = type_val.as_string();
            if (expected_type == "string" && !instance.is_string()) {
                errors.push_back("Expected string type");
            } else if (expected_type == "number" && !instance.is_number()) {
                errors.push_back("Expected number type");
            } else if (expected_type == "object" && !instance.is_object()) {
                errors.push_back("Expected object type");
            } else if (expected_type == "array" && !instance.is_array()) {
                errors.push_back("Expected array type");
            } else if (expected_type == "boolean" && !instance.is_bool()) {
                errors.push_back("Expected boolean type");
            }
        }
    }

    // 2. "required" keyword check
    if (schema_obj.count("required") && instance.is_object()) {
        const auto& req_val = schema_obj.at("required");
        if (req_val.is_array()) {
            const auto& instance_obj = instance.as_object();
            for (const auto& required_prop : req_val.as_array()) {
                if (required_prop.is_string()) {
                    if (!instance_obj.count(required_prop.as_string())) {
                        errors.push_back("Missing required property: " + required_prop.as_string());
                    }
                }
            }
        }
    }

    return errors.empty();
}

} // namespace json_mini
