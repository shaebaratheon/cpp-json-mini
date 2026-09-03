#pragma once

#include <string_view>
#include <vector>

namespace json_mini {

class StringViewTokenizer {
public:
    explicit StringViewTokenizer(std::string_view sv) : view_(sv) {}
    size_t count_tokens() const {
        size_t count = 0;
        for (char c : view_) {
            if (c == { || c == } || c == [ || c == ] || c == :) count++;
        }
        return count;
    }
private:
    std::string_view view_;
};

} // namespace json_mini
