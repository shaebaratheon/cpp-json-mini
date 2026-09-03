#include "json_mini/arena_allocator.hpp"
#include "json_mini/base64.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;
    MemoryArena arena(4096);
    void* p1 = arena.allocate(128);
    void* p2 = arena.allocate(256);
    assert(p1 != nullptr && p2 != nullptr);

    std::string b64 = Base64::encode(reinterpret_cast<const unsigned char*>("Hello JSON!"), 11);
    assert(!b64.empty());
    std::cout << "Arena and Base64 tests passed. Encoded: " << b64 << std::endl;
    return 0;
}
