#include "json_mini/simd_checker.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::string ascii = "{\"standard_ascii_key\": \"pure_ascii_value_12345\"}";
    assert(json_mini::SimdUtf8Checker::validate_ascii(ascii.data(), ascii.length()));
    std::cout << "SIMD ASCII validator verified." << std::endl;
    return 0;
}
