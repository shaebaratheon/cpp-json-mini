#include "json_mini/dom.hpp"
#include <cassert>
#include <iostream>

int main() {
    json_mini::JsonObject root;
    for (int i = 0; i < 50; ++i) {
        root["key_" + std::to_string(i)] = i;
    }
    assert(root.size() == 50);
    std::cout << "Final integrity verification confirmed." << std::endl;
    return 0;
}
