#ifndef JSON_MINI_POINTER_HPP
#define JSON_MINI_POINTER_HPP

#include <string>
#include <vector>
#include <sstream>
#include "json_mini/dom.hpp"

namespace json_mini {

class JsonPointer {
public:
    static std::vector<std::string> parse(const std::string& pointer_str) {
        std::vector<std::string> tokens;
        if (pointer_str.empty()) return tokens;
        if (pointer_str[0] != '/') {
            throw std::invalid_argument("JSON pointer must start with '/'");
        }
        std::stringstream ss(pointer_str.substr(1));
        std::string token;
        while (std::getline(ss, token, '/')) {
            std::string unescaped;
            for (size_t i = 0; i < token.size(); ++i) {
                if (token[i] == '~' && i + 1 < token.size()) {
                    if (token[i + 1] == '0') unescaped += '~';
                    else if (token[i + 1] == '1') unescaped += '/';
                    else unescaped += token[i + 1];
                    i++;
                } else {
                    unescaped += token[i];
                }
            }
            tokens.push_back(unescaped);
        }
        return tokens;
    }

    static const JsonValue* resolve(const JsonValue* root, const std::string& pointer_str) {
        if (!root || pointer_str.empty()) return root;
        std::vector<std::string> tokens = parse(pointer_str);
        const JsonValue* current = root;
        for (const auto& token : tokens) {
            if (current->is_object()) {
                const auto& obj = current->as_object();
                auto it = obj.find(token);
                if (it == obj.end()) return nullptr;
                current = &(it->second);
            } else if (current->is_array()) {
                const auto& arr = current->as_array();
                size_t idx = std::stoul(token);
                if (idx >= arr.size()) return nullptr;
                current = &(arr[idx]);
            } else {
                return nullptr;
            }
        }
        return current;
    }
};

} // namespace json_mini

#endif // JSON_MINI_POINTER_HPP
