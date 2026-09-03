#include "json_mini/lexer.hpp"
#include "json_mini/dom.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    std::string json_doc = "{\"name\": \"Alice\", \"age\": 30, \"skills\": [\"c++\", \"python\"], \"active\": true}";
    Lexer lexer(json_doc);
    auto tokens = lexer.tokenize();

    assert(tokens.size() > 5);
    std::cout << "JSON Lexer and DOM tokenized successfully. Token count: " << tokens.size() << std::endl;
    return 0;
}
