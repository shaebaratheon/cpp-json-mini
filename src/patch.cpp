#include "json_mini/patch.hpp"
#include <sstream>

namespace json_mini {

JsonPointer::JsonPointer(std::string pointer) {
    if (pointer.empty()) return;
    if (pointer[0] != '/') {
        throw std::runtime_error("Invalid JSON Pointer: must start with '/'");
    }

    size_t start = 1;
    while (start <= pointer.size()) {
        size_t end = pointer.find('/', start);
        if (end == std::string::npos) {
            tokens_.push_back(unescape(pointer.substr(start)));
            break;
        } else {
            tokens_.push_back(unescape(pointer.substr(start, end - start)));
            start = end + 1;
        }
    }
}

std::string JsonPointer::unescape(const std::string& s) {
    std::string res;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '~' && i + 1 < s.size()) {
            if (s[i + 1] == '0') {
                res += '~';
                i++;
            } else if (s[i + 1] == '1') {
                res += '/';
                i++;
            } else {
                res += s[i];
            }
        } else {
            res += s[i];
        }
    }
    return res;
}

Value* JsonPointer::resolve(Value& root) const {
    Value* curr = &root;
    for (const auto& token : tokens_) {
        if (curr->is_object()) {
            if (!curr->contains(token)) return nullptr;
            curr = &((*curr)[token]);
        } else if (curr->is_array()) {
            try {
                size_t idx = std::stoul(token);
                if (idx >= curr->size()) return nullptr;
                curr = &((*curr)[idx]);
            } catch (...) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return curr;
}

const Value* JsonPointer::resolve(const Value& root) const {
    const Value* curr = &root;
    for (const auto& token : tokens_) {
        if (curr->is_object()) {
            if (!curr->contains(token)) return nullptr;
            curr = &((*curr)[token]);
        } else if (curr->is_array()) {
            try {
                size_t idx = std::stoul(token);
                if (idx >= curr->size()) return nullptr;
                curr = &((*curr)[idx]);
            } catch (...) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return curr;
}

bool JsonPatch::apply(Value& document, const Value& patch, std::string& error_msg) {
    if (!patch.is_array()) {
        error_msg = "Patch must be an array of operation objects";
        return false;
    }

    Value copy = document;

    for (const auto& op : patch.as_array()) {
        if (!op.is_object() || !op.contains("op") || !op.contains("path")) {
            error_msg = "Invalid patch operation object";
            return false;
        }
        std::string op_type = op["op"].as_string();
        std::string path = op["path"].as_string();

        if (op_type == "add") {
            if (!op.contains("value") || !op_add(copy, path, op["value"], error_msg)) return false;
        } else if (op_type == "remove") {
            if (!op_remove(copy, path, error_msg)) return false;
        } else if (op_type == "replace") {
            if (!op.contains("value") || !op_replace(copy, path, op["value"], error_msg)) return false;
        } else if (op_type == "test") {
            if (!op.contains("value") || !op_test(copy, path, op["value"], error_msg)) return false;
        } else {
            error_msg = "Unsupported patch operation: " + op_type;
            return false;
        }
    }

    document = std::move(copy);
    return true;
}

bool JsonPatch::op_add(Value& doc, const std::string& path, const Value& value, std::string& err) {
    JsonPointer ptr(path);
    if (ptr.tokens().empty()) {
        doc = value;
        return true;
    }

    std::string last = ptr.tokens().back();
    std::string parent_path = path.substr(0, path.rfind('/'));
    JsonPointer parent_ptr(parent_path);
    Value* parent = parent_ptr.resolve(doc);

    if (!parent) {
        err = "Parent path not found: " + parent_path;
        return false;
    }

    if (parent->is_object()) {
        (*parent)[last] = value;
        return true;
    } else if (parent->is_array()) {
        if (last == "-") {
            parent->as_array().push_back(value);
            return true;
        }
        size_t idx = std::stoul(last);
        auto& arr = parent->as_array();
        if (idx > arr.size()) {
            err = "Index out of bounds for array insertion: " + last;
            return false;
        }
        arr.insert(arr.begin() + idx, value);
        return true;
    }
    err = "Parent is neither object nor array";
    return false;
}

bool JsonPatch::op_remove(Value& doc, const std::string& path, std::string& err) {
    JsonPointer ptr(path);
    if (ptr.tokens().empty()) {
        err = "Cannot remove root";
        return false;
    }

    std::string last = ptr.tokens().back();
    std::string parent_path = path.substr(0, path.rfind('/'));
    JsonPointer parent_ptr(parent_path);
    Value* parent = parent_ptr.resolve(doc);

    if (!parent) {
        err = "Parent path not found: " + parent_path;
        return false;
    }

    if (parent->is_object()) {
        auto& obj = parent->as_object();
        auto it = obj.find(last);
        if (it == obj.end()) {
            err = "Property not found: " + last;
            return false;
        }
        obj.erase(it);
        return true;
    } else if (parent->is_array()) {
        size_t idx = std::stoul(last);
        auto& arr = parent->as_array();
        if (idx >= arr.size()) {
            err = "Index out of bounds for array removal: " + last;
            return false;
        }
        arr.erase(arr.begin() + idx);
        return true;
    }
    err = "Parent is not container";
    return false;
}

bool JsonPatch::op_replace(Value& doc, const std::string& path, const Value& value, std::string& err) {
    JsonPointer ptr(path);
    Value* target = ptr.resolve(doc);
    if (!target) {
        err = "Target path does not exist for replace: " + path;
        return false;
    }
    *target = value;
    return true;
}

bool JsonPatch::op_test(const Value& doc, const std::string& path, const Value& expected, std::string& err) {
    JsonPointer ptr(path);
    const Value* target = ptr.resolve(doc);
    if (!target) {
        err = "Target path does not exist for test: " + path;
        return false;
    }
    if (*target != expected) {
        err = "Test failed: value does not match expected";
        return false;
    }
    return true;
}

} // namespace json_mini
