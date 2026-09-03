#pragma once

#include "dom.hpp"
#include <vector>
#include <cstdint>

namespace json_mini {

class MsgPackSerializer {
public:
    static std::vector<uint8_t> pack(const JsonValue& val) {
        std::vector<uint8_t> buffer;
        pack_impl(val, buffer);
        return buffer;
    }

private:
    static void pack_impl(const JsonValue& val, std::vector<uint8_t>& buf) {
        if (val.is_null()) {
            buf.push_back(0xc0);
        } else if (val.is_bool()) {
            buf.push_back(val.dump() == "true" ? 0xc3 : 0xc2);
        } else if (val.is_number()) {
            buf.push_back(0xcb); // double-precision float
            double n = val.as_number();
            uint64_t raw;
            std::memcpy(&raw, &n, sizeof(double));
            for (int i = 7; i >= 0; --i) buf.push_back((raw >> (i * 8)) & 0xFF);
        } else if (val.is_string()) {
            const auto& str = val.as_string();
            if (str.length() < 32) {
                buf.push_back(0xa0 | static_cast<uint8_t>(str.length()));
            } else {
                buf.push_back(0xda);
                buf.push_back((str.length() >> 8) & 0xFF);
                buf.push_back(str.length() & 0xFF);
            }
            buf.insert(buf.end(), str.begin(), str.end());
        }
    }
};

} // namespace json_mini
