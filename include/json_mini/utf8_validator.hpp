#pragma once
#include <string_view>
#include <cstddef>

namespace json_mini {

class Utf8Validator {
public:
    static bool is_valid(std::string_view s) {
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(s.data());
        size_t len = s.size();
        size_t i = 0;

        while (i < len) {
            unsigned char b = bytes[i];
            if (b <= 0x7F) {
                ++i;
            } else if ((b >= 0xC2) && (b <= 0xDF)) {
                if (i + 1 >= len) return false;
                if ((bytes[i+1] & 0xC0) != 0x80) return false;
                i += 2;
            } else if ((b >= 0xE0) && (b <= 0xEF)) {
                if (i + 2 >= len) return false;
                if ((bytes[i+1] & 0xC0) != 0x80 || (bytes[i+2] & 0xC0) != 0x80) return false;
                i += 3;
            } else if ((b >= 0xF0) && (b <= 0xF4)) {
                if (i + 3 >= len) return false;
                if ((bytes[i+1] & 0xC0) != 0x80 || (bytes[i+2] & 0xC0) != 0x80 || (bytes[i+3] & 0xC0) != 0x80) return false;
                i += 4;
            } else {
                return false;
            }
        }
        return true;
    }
};

} // namespace json_mini
