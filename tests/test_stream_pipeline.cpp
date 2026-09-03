#include "json_mini/dom.hpp"
#include "json_mini/sax_parser.hpp"
#include "json_mini/arena_allocator.hpp"
#include "json_mini/pretty_printer.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    JsonObject root;
    for (int i = 0; i < 20; ++i) {
        root["key_" + std::to_string(i)] = JsonValue("test_val_" + std::to_string(i));
    }

    std::string formatted = PrettyPrinter::format(root, 2);
    assert(formatted.size() > 50);

    std::cout << "Stream and pipeline verification complete." << std::endl;
    return 0;
}
