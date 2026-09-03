#include "json_mini/sax_parser.hpp"
#include <sstream>
#include <cassert>
#include <iostream>

class CountingHandler : public json_mini::JsonSaxHandler {
public:
    int objects = 0;
    int strings = 0;

    bool on_null() override { return true; }
    bool on_bool(bool) override { return true; }
    bool on_number(double) override { return true; }
    bool on_string(const std::string&) override { strings++; return true; }
    bool on_start_object() override { objects++; return true; }
    bool on_key(const std::string&) override { return true; }
    bool on_end_object() override { return true; }
    bool on_start_array() override { return true; }
    bool on_end_array() override { return true; }
};

int main() {
    std::istringstream iss("{\"key\": \"value\"}");
    json_mini::SaxParser parser(iss);
    CountingHandler handler;
    parser.parse(handler);
    assert(handler.objects == 1);
    std::cout << "SAX streaming parser test passed." << std::endl;
    return 0;
}
