#pragma once

#include "dom.hpp"
#include <string>
#include <vector>

namespace json_mini {

class JsonSchemaValidator {
public:
    explicit JsonSchemaValidator(JsonValue schema);
    bool validate(const JsonValue& instance, std::vector<std::string>& errors) const;

private:
    JsonValue schema_;
};

} // namespace json_mini
