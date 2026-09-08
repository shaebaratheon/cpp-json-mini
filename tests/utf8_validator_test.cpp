#include "json_mini/utf8_validator.hpp"
#include <cassert>
#include <iostream>

void test_utf8_validity() {
    assert(json_mini::Utf8Validator::is_valid("hello world"));
    assert(json_mini::Utf8Validator::is_valid("Café au lait"));
    assert(json_mini::Utf8Validator::is_valid("你好，世界"));
    
    // Invalid sequence: lone continuation byte
    std::string invalid = "\x80\x81";
    assert(!json_mini::Utf8Validator::is_valid(invalid));
}

int main() {
    test_utf8_validity();
    std::cout << "UTF-8 validator tests passed!\n";
    return 0;
}
