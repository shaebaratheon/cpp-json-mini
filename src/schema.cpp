#include "json_mini/schema.hpp"
#include <algorithm>

namespace json_mini {

SchemaValidator::SchemaValidator(Value schema) : schema_(std::move(schema)) {}

bool SchemaValidator::is_valid(const Value& instance) const {
    std::vector<ValidationError> errors;
    return validate(instance, errors);
}

bool SchemaValidator::validate(const Value& instance, std::vector<ValidationError>& errors) const {
    return validate_internal(schema_, instance, "#", errors);
}

bool SchemaValidator::check_type(const std::string& expected, const Value& instance) const {
    if (expected == "null") return instance.is_null();
    if (expected == "boolean") return instance.is_bool();
    if (expected == "number") return instance.is_number();
    if (expected == "integer") return instance.is_number() && (instance.as_number() == instance.as_int64());
    if (expected == "string") return instance.is_string();
    if (expected == "array") return instance.is_array();
    if (expected == "object") return instance.is_object();
    return false;
}

bool SchemaValidator::validate_internal(const Value& schema, const Value& instance, const std::string& path, std::vector<ValidationError>& errors) const {
    if (!schema.is_object()) return true;

    bool valid = true;

    // Check "type"
    if (schema.contains("type")) {
        const auto& t = schema["type"];
        if (t.is_string()) {
            if (!check_type(t.as_string(), instance)) {
                errors.push_back({path, "Expected type " + t.as_string() + " but got different type"});
                valid = false;
            }
        }
    }

    // Number constraints: minimum, maximum
    if (instance.is_number()) {
        double num = instance.as_number();
        if (schema.contains("minimum") && schema["minimum"].is_number()) {
            double min_val = schema["minimum"].as_number();
            if (num < min_val) {
                errors.push_back({path, "Value " + std::to_string(num) + " is less than minimum " + std::to_string(min_val)});
                valid = false;
            }
        }
        if (schema.contains("maximum") && schema["maximum"].is_number()) {
            double max_val = schema["maximum"].as_number();
            if (num > max_val) {
                errors.push_back({path, "Value " + std::to_string(num) + " is greater than maximum " + std::to_string(max_val)});
                valid = false;
            }
        }
    }

    // String constraints: minLength, maxLength
    if (instance.is_string()) {
        size_t len = instance.as_string().size();
        if (schema.contains("minLength") && schema["minLength"].is_number()) {
            size_t min_len = static_cast<size_t>(schema["minLength"].as_int());
            if (len < min_len) {
                errors.push_back({path, "String length " + std::to_string(len) + " is less than minLength " + std::to_string(min_len)});
                valid = false;
            }
        }
        if (schema.contains("maxLength") && schema["maxLength"].is_number()) {
            size_t max_len = static_cast<size_t>(schema["maxLength"].as_int());
            if (len > max_len) {
                errors.push_back({path, "String length " + std::to_string(len) + " exceeds maxLength " + std::to_string(max_len)});
                valid = false;
            }
        }
    }

    // Array constraints: minItems, maxItems, items
    if (instance.is_array()) {
        const auto& arr = instance.as_array();
        if (schema.contains("minItems") && schema["minItems"].is_number()) {
            size_t min_items = static_cast<size_t>(schema["minItems"].as_int());
            if (arr.size() < min_items) {
                errors.push_back({path, "Array size " + std::to_string(arr.size()) + " is less than minItems " + std::to_string(min_items)});
                valid = false;
            }
        }
        if (schema.contains("items")) {
            const auto& item_schema = schema["items"];
            for (size_t i = 0; i < arr.size(); ++i) {
                std::string item_path = path + "/" + std::to_string(i);
                if (!validate_internal(item_schema, arr[i], item_path, errors)) {
                    valid = false;
                }
            }
        }
    }

    // Object constraints: required, properties
    if (instance.is_object()) {
        if (schema.contains("required") && schema["required"].is_array()) {
            for (const auto& req : schema["required"].as_array()) {
                if (req.is_string()) {
                    const std::string& prop = req.as_string();
                    if (!instance.contains(prop)) {
                        errors.push_back({path, "Missing required property: " + prop});
                        valid = false;
                    }
                }
            }
        }
        if (schema.contains("properties") && schema["properties"].is_object()) {
            const auto& props = schema["properties"].as_object();
            for (const auto& [prop_name, prop_schema] : props) {
                if (instance.contains(prop_name)) {
                    std::string prop_path = path + "/" + prop_name;
                    if (!validate_internal(prop_schema, instance[prop_name], prop_path, errors)) {
                        valid = false;
                    }
                }
            }
        }
    }

    return valid;
}

} // namespace json_mini
