#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

namespace json_mini {

struct PatchOperation {
    std::string op;
    std::string path;
    std::string value;
};

class JsonPatchEngine {
public:
    static std::map<std::string, std::string> apply(
        std::map<std::string, std::string> doc,
        const std::vector<PatchOperation>& ops) {
        
        for (const auto& item : ops) {
            if (item.op == "add" || item.op == "replace") {
                doc[item.path] = item.value;
            } else if (item.op == "remove") {
                auto it = doc.find(item.path);
                if (it == doc.end()) {
                    throw std::runtime_error("Path not found: " + item.path);
                }
                doc.erase(it);
            } else {
                throw std::invalid_argument("Unsupported operation: " + item.op);
            }
        }
        return doc;
    }
};

} // namespace json_mini
