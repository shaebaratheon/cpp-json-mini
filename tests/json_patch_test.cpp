#include "json_mini/json_patch.hpp"
#include <cassert>
#include <iostream>

void test_patch_operations() {
    std::map<std::string, std::string> doc = {{"/title", "Hello"}, {"/author", "John"}};
    std::vector<json_mini::PatchOperation> ops = {
        {"replace", "/title", "World"},
        {"add", "/version", "1.0"},
        {"remove", "/author", ""}
    };
    auto result = json_mini::JsonPatchEngine::apply(doc, ops);
    assert(result["/title"] == "World");
    assert(result["/version"] == "1.0");
    assert(result.find("/author") == result.end());
}

int main() {
    test_patch_operations();
    std::cout << "JSON Patch tests passed!\n";
    return 0;
}
