#include "json_mini/patch.hpp"
#include <sstream>
#include <stdexcept>

namespace json_mini {

JsonPointer::JsonPointer(const std::string& pointer_str) {
    if (pointer_str.empty()) return;
    if (pointer_str[0] != '/') {
        throw std::runtime_error("Invalid JSON Pointer: must start with /");
    }

    std::stringstream ss(pointer_str.substr(1));
    std::string token;
    while (std::getline(ss, token, '/')) {
        // Decode ~1 -> / and ~0 -> ~
        std::string decoded;
        for (size_t i = 0; i < token.length(); ++i) {
            if (token[i] == '~' && i + 1 < token.length()) {
                if (token[i + 1] == '1') decoded += '/';
                else if (token[i + 1] == '0') decoded += '~';
                i++;
            } else {
                decoded += token[i];
            }
        }
        tokens_.push_back(decoded);
    }
}

const JsonValue* JsonPointer::evaluate(const JsonValue& root) const {
    const JsonValue* curr = &root;
    for (const auto& token : tokens_) {
        if (curr->is_object()) {
            const auto& obj = curr->as_object();
            auto it = obj.find(token);
            if (it == obj.end()) return nullptr;
            curr = &(it->second);
        } else if (curr->is_array()) {
            const auto& arr = curr->as_array();
            size_t idx = std::stoul(token);
            if (idx >= arr.size()) return nullptr;
            curr = &arr[idx];
        } else {
            return nullptr;
        }
    }
    return curr;
}

JsonValue JsonPatch::apply(const JsonValue& target, const std::vector<Operation>& patch_ops) {
    JsonValue result = target;
    for (const auto& op : patch_ops) {
        if (op.op == "test") {
            JsonPointer ptr(op.path);
            const JsonValue* val = ptr.evaluate(result);
            if (!val || val->dump() != op.value.dump()) {
                throw std::runtime_error("JSON Patch 'test' assertion failed at " + op.path);
            }
        }
    }
    return result;
}

} // namespace json_mini
