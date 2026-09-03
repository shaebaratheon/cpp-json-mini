#include "json_mini.hpp"
#include "json_mini/value.hpp"
#include "json_mini/parser.hpp"
#include "json_mini/serializer.hpp"
#include "json_mini/schema.hpp"
#include "json_mini/patch.hpp"
#include <iostream>

int main() {
    std::cout << "=== cpp-json-mini Demo ===" << std::endl;

    // 1. Legacy Tokenizer Demo
    std::string json = "{\"name\": \"MiniJSON\", \"active\": true, \"data\": null}";
    JsonTokenizer tokenizer(json);
    auto tokens = tokenizer.tokenize();
    
    std::cout << "Legacy Tokenized output:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << "Token: " << (int)token.type << " | Value: " << token.value << std::endl;
    }

    // 2. Modern DOM & AST Parser Demo
    std::string modern_json = R"({
        "service": "Brimfield Pipeline",
        "port": 8080,
        "enabled": true,
        "workers": ["node-1", "node-2", "node-3"],
        "config": {
            "timeout_seconds": 600,
            "max_batch_size": 1000
        }
    })";

    json_mini::Value val = json_mini::Parser::parse(modern_json);
    std::cout << "\nParsed Service: " << val["service"].as_string() << std::endl;
    std::cout << "Workers count: " << val["workers"].size() << std::endl;
    std::cout << "Max Batch Size: " << val["config"]["max_batch_size"].as_int() << std::endl;

    // 3. Pretty serialization
    std::cout << "\nPretty-printed JSON:\n" << val.dump(2) << std::endl;

    return 0;
}
