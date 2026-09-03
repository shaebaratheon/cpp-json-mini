#include "json_mini/sax_parser.hpp"
#include <cctype>

namespace json_mini {

SaxParser::SaxParser(std::istream& is) : stream_(is) {}

bool SaxParser::parse(JsonSaxHandler& handler) {
    char c;
    while (stream_ >> c) {
        if (c == '{') {
            if (!handler.on_start_object()) return false;
        } else if (c == '}') {
            if (!handler.on_end_object()) return false;
        } else if (c == '[') {
            if (!handler.on_start_array()) return false;
        } else if (c == ']') {
            if (!handler.on_end_array()) return false;
        } else if (c == '"') {
            std::string str;
            char sc;
            while (stream_.get(sc) && sc != '"') {
                str += sc;
            }
            if (!handler.on_string(str)) return false;
        }
    }
    return true;
}

} // namespace json_mini
