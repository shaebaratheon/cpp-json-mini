#include "json_mini/schema.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    JsonObject schema;
    schema["type"] = "object";
    schema["required"] = JsonArray{JsonValue("name"), JsonValue("id")};

    JsonSchemaValidator validator(schema);

    JsonObject valid_doc;
    valid_doc["name"] = "Alice";
    valid_doc["id"] = 123;

    std::vector<std::string> errors;
    bool ok = validator.validate(valid_doc, errors);
    assert(ok && errors.empty());

    JsonObject invalid_doc;
    invalid_doc["name"] = "Bob";
    bool bad_ok = validator.validate(invalid_doc, errors);
    assert(!bad_ok && !errors.empty());

    std::cout << "JSON Schema Validator tests passed successfully." << std::endl;
    return 0;
}
