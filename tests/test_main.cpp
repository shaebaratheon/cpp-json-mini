#include "json_mini/value.hpp"
#include "json_mini/parser.hpp"
#include "json_mini/serializer.hpp"
#include "json_mini/schema.hpp"
#include "json_mini/patch.hpp"
#include <iostream>
#include <cassert>

using namespace json_mini;

void test_parse_and_dump() {
    std::cout << "[TEST] Parsing and serialization..." << std::endl;
    std::string raw = R"({
        "project": "cpp-json-mini",
        "version": 1.2,
        "features": ["parsing", "serializing", "schema", "patch"],
        "active": true,
        "metadata": {
            "author": "Google SWE",
            "license": "Apache 2.0"
        }
    })";

    Value doc = Parser::parse(raw);
    assert(doc.is_object());
    assert(doc["project"].as_string() == "cpp-json-mini");
    assert(doc["version"].as_number() == 1.2);
    assert(doc["features"].is_array());
    assert(doc["features"].size() == 4);
    assert(doc["features"][0].as_string() == "parsing");
    assert(doc["active"].as_bool() == true);
    assert(doc["metadata"]["author"].as_string() == "Google SWE");

    std::string minified = doc.dump(-1);
    assert(minified.find('\n') == std::string::npos);

    std::string pretty = doc.dump(2);
    assert(pretty.find('\n') != std::string::npos);

    Value re_parsed = Parser::parse(pretty);
    assert(re_parsed == doc);
    std::cout << "  Passed parse and dump test." << std::endl;
}

void test_schema_validator() {
    std::cout << "[TEST] Schema validation..." << std::endl;
    std::string schema_str = R"({
        "type": "object",
        "required": ["id", "username", "score"],
        "properties": {
            "id": { "type": "integer", "minimum": 1 },
            "username": { "type": "string", "minLength": 3, "maxLength": 20 },
            "score": { "type": "number", "minimum": 0, "maximum": 100 },
            "tags": {
                "type": "array",
                "minItems": 1,
                "items": { "type": "string" }
            }
        }
    })";

    Value schema_val = Parser::parse(schema_str);
    SchemaValidator validator(schema_val);

    // Valid instance
    std::string valid_instance = R"({
        "id": 42,
        "username": "alice",
        "score": 95.5,
        "tags": ["admin", "developer"]
    })";
    assert(validator.is_valid(Parser::parse(valid_instance)));

    // Missing required field
    std::string missing_req = R"({
        "id": 42,
        "username": "alice"
    })";
    std::vector<ValidationError> errors;
    assert(!validator.validate(Parser::parse(missing_req), errors));
    assert(!errors.empty());

    // Out of range number
    std::string out_of_range = R"({
        "id": 42,
        "username": "alice",
        "score": 150
    })";
    assert(!validator.is_valid(Parser::parse(out_of_range)));

    std::cout << "  Passed schema validation test." << std::endl;
}

void test_json_patch() {
    std::cout << "[TEST] RFC 6902 JSON Patch..." << std::endl;
    std::string doc_str = R"({
        "title": "Document",
        "items": [10, 20, 30],
        "owner": { "name": "Bob" }
    })";

    Value doc = Parser::parse(doc_str);

    std::string patch_str = R"([
        { "op": "test", "path": "/title", "value": "Document" },
        { "op": "replace", "path": "/owner/name", "value": "Charlie" },
        { "op": "add", "path": "/items/-", "value": 40 },
        { "op": "remove", "path": "/items/0" }
    ])";

    Value patch = Parser::parse(patch_str);
    std::string err;
    bool ok = JsonPatch::apply(doc, patch, err);
    if (!ok) {
        std::cerr << "Patch failed: " << err << std::endl;
    }
    assert(ok);

    assert(doc["owner"]["name"].as_string() == "Charlie");
    assert(doc["items"].size() == 3);
    assert(doc["items"][0].as_int() == 20);
    assert(doc["items"][1].as_int() == 30);
    assert(doc["items"][2].as_int() == 40);

    std::cout << "  Passed JSON patch test." << std::endl;
}

int main() {
    std::cout << "=== Running cpp-json-mini Test Suite ===" << std::endl;
    test_parse_and_dump();
    test_schema_validator();
    test_json_patch();
    std::cout << "=== All cpp-json-mini tests PASSED! ===" << std::endl;
    return 0;
}
