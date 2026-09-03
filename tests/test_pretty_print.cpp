#include "json_mini/pretty_printer.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    JsonObject root;
    root["name"] = "Alice";
    root["scores"] = JsonArray{JsonValue(100), JsonValue(95)};

    std::string pretty = PrettyPrinter::format(root, 4);
    assert(pretty.find("\n") != std::string::npos);
    std::cout << "Pretty print output:\n" << pretty << std::endl;
    return 0;
}
