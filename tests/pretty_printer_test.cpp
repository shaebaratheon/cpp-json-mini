#include "json_mini/pretty_printer.hpp"
#include <cassert>
#include <iostream>

void test_empty_containers() {
    json_mini::PrettyPrinter printer(2);
    assert(printer.format("{}") == "{}");
    assert(printer.format("[]") == "[]");
}

void test_nested_object() {
    json_mini::PrettyPrinter printer(2);
    std::string compact = "{\"a\":1,\"b\":[2,3]}";
    std::string formatted = printer.format(compact);
    assert(formatted.find("\n  \"a\": 1,\n") != std::string::npos);
}

int main() {
    test_empty_containers();
    test_nested_object();
    std::cout << "Pretty printer tests passed!\n";
    return 0;
}
