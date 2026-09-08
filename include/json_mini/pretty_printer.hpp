#pragma once
#include <string>
#include <sstream>

namespace json_mini {

class PrettyPrinter {
public:
    explicit PrettyPrinter(int indent_spaces = 2) : indent_width_(indent_spaces) {}

    std::string format(const std::string& compact_json) const {
        std::ostringstream out;
        int level = 0;
        bool in_string = false;
        bool escape = false;

        for (size_t i = 0; i < compact_json.size(); ++i) {
            char c = compact_json[i];
            if (escape) {
                out << c;
                escape = false;
                continue;
            }
            if (c == '\\' && in_string) {
                escape = true;
                out << c;
                continue;
            }
            if (c == '"') {
                in_string = !in_string;
                out << c;
                continue;
            }
            if (in_string) {
                out << c;
                continue;
            }

            if (c == '{' || c == '[') {
                out << c;
                if (i + 1 < compact_json.size() && (compact_json[i+1] == '}' || compact_json[i+1] == ']')) {
                    continue;
                }
                out << '\n';
                ++level;
                write_indent(out, level);
            } else if (c == '}' || c == ']') {
                if (i > 0 && (compact_json[i-1] != '{' && compact_json[i-1] != '[')) {
                    out << '\n';
                    --level;
                    write_indent(out, level);
                }
                out << c;
            } else if (c == ',') {
                out << ",\n";
                write_indent(out, level);
            } else if (c == ':') {
                out << ": ";
            } else if (!std::isspace(static_cast<unsigned char>(c))) {
                out << c;
            }
        }
        return out.str();
    }

private:
    int indent_width_;
    void write_indent(std::ostringstream& out, int level) const {
        for (int i = 0; i < level * indent_width_; ++i) {
            out << ' ';
        }
    }
};

} // namespace json_mini
