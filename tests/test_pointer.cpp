#include <cassert>
#include <iostream>
#include "json_mini/pointer.hpp"

void test_json_pointer_resolution() {
    using namespace json_mini;
    JsonObject user;
    user["name"] = JsonValue("Alice");
    JsonArray scores;
    scores.push_back(JsonValue(100.0));
    scores.push_back(JsonValue(95.0));
    user["scores"] = JsonValue(scores);
    JsonValue root(user);

    const JsonValue* name = JsonPointer::resolve(&root, "/name");
    assert(name != nullptr);
    assert(name->as_string() == "Alice");

    const JsonValue* score0 = JsonPointer::resolve(&root, "/scores/0");
    assert(score0 != nullptr);
    assert(score0->as_number() == 100.0);

    const JsonValue* invalid = JsonPointer::resolve(&root, "/non_existent");
    assert(invalid == nullptr);
    std::cout << "JsonPointer tests passed successfully!\n";
}

int main() {
    test_json_pointer_resolution();
    return 0;
}
