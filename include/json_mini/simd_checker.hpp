#pragma once

#include <string>
#include <cstdint>

namespace json_mini {

class SimdUtf8Checker {
public:
    static bool validate_ascii(const char* data, size_t length) {
        size_t i = 0;
        // Fast 64-bit word check
        while (i + 8 <= length) {
            uint64_t chunk = *reinterpret_cast<const uint64_t*>(data + i);
            if (chunk & 0x8080808080808080ULL) {
                return false;
            }
            i += 8;
        }
        while (i < length) {
            if (static_cast<unsigned char>(data[i]) >= 0x80) return false;
            i++;
        }
        return true;
    }
};

} // namespace json_mini
