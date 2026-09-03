#include "json_mini/diff.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    JsonObject v1;
    v1["name"] = "Alice";
    v1["city"] = "NYC";

    JsonObject v2;
    v2["name"] = "Alice";
    v2["city"] = "SF";
    v2["role"] = "admin";

    auto patch = JsonDiff::compute(v1, v2);
    assert(patch.size() == 2);
    std::cout << "JSON Diff calculated patch operations count: " << patch.size() << std::endl;
    return 0;
}
