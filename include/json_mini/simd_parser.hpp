#pragma once

#include "dom.hpp"
#include <string_view>
#include <vector>

namespace json_mini {

class SimdStructuralIndexer {
public:
    static std::vector<size_t> find_structural_indices(std::string_view json) {
        std::vector<size_t> indices;
        for (size_t i = 0; i < json.size(); ++i) {
            char c = json[i];
            if (c == { || c == } || c == [ || c == ] || c == : || c == ,) {
                indices.push_back(i);
            }
        }
        return indices;
    }
};

} // namespace json_mini
