#include "json_mini/flattener.hpp"
#include <cassert>
#include <iostream>

void test_key_join_and_split() {
    assert(json_mini::JsonFlattener::join_key("user", "name") == "user.name");
    assert(json_mini::JsonFlattener::join_key("", "root") == "root");
    
    auto [first, rest] = json_mini::JsonFlattener::split_first_key("user.address.zip");
    assert(first == "user");
    assert(rest == "address.zip");
}

int main() {
    test_key_join_and_split();
    std::cout << "Flattener tests passed!\n";
    return 0;
}
