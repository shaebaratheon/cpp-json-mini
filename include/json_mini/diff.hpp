#pragma once

#include "dom.hpp"
#include "patch.hpp"
#include <vector>

namespace json_mini {

class JsonDiff {
public:
    static std::vector<JsonPatch::Operation> compute(const JsonValue& source, const JsonValue& target) {
        std::vector<JsonPatch::Operation> ops;
        diff_recursive("", source, target, ops);
        return ops;
    }

private:
    static void diff_recursive(const std::string& path, const JsonValue& src, const JsonValue& dst, std::vector<JsonPatch::Operation>& ops) {
        if (src.dump() == dst.dump()) return;

        if (src.is_object() && dst.is_object()) {
            const auto& src_obj = src.as_object();
            const auto& dst_obj = dst.as_object();

            for (const auto& [k, v] : dst_obj) {
                std::string field_path = path + "/" + k;
                if (!src_obj.count(k)) {
                    ops.push_back({"add", field_path, v});
                } else {
                    diff_recursive(field_path, src_obj.at(k), v, ops);
                }
            }

            for (const auto& [k, v] : src_obj) {
                if (!dst_obj.count(k)) {
                    ops.push_back({"remove", path + "/" + k, JsonNull{}});
                }
            }
        } else {
            ops.push_back({"replace", path, dst});
        }
    }
};

} // namespace json_mini
