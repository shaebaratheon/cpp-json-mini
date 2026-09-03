#pragma once

#include <string>
#include <vector>

namespace json_mini {

class Base64 {
public:
    static std::string encode(const unsigned char* data, size_t len) {
        static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string out;
        for (size_t i = 0; i < len; i += 3) {
            uint32_t b = (data[i] << 16) | ((i + 1 < len ? data[i + 1] : 0) << 8) | (i + 2 < len ? data[i + 2] : 0);
            out.push_back(tbl[(b >> 18) & 0x3F]);
            out.push_back(tbl[(b >> 12) & 0x3F]);
            out.push_back(i + 1 < len ? tbl[(b >> 6) & 0x3F] : '=');
            out.push_back(i + 2 < len ? tbl[b & 0x3F] : '=');
        }
        return out;
    }
};

} // namespace json_mini
