#include "json_mini/patch.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;

    JsonObject obj;
    obj["name"] = "Alice";
    JsonObject address;
    address["city"] = "Sunnyvale";
    obj["address"] = address;

    JsonPointer ptr("/address/city");
    const JsonValue* city = ptr.evaluate(obj);
    assert(city != nullptr && city->as_string() == "Sunnyvale");

    std::cout << "JSON Pointer and Patch tests passed successfully." << std::endl;
    return 0;
}
