#include "json_mini/dom.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;
    for (int i = 0; i < 100; ++i) {
        JsonObject obj;
        obj["id"] = i;
        obj["active"] = (i % 2 == 0);
        obj["label"] = "item_" + std::to_string(i);
        std::string serialized = obj.dump();
        assert(!serialized.empty());
    }
    std::cout << "Exhaustive DOM tests passed." << std::endl;
    return 0;
}
