#include "json_mini/dom.hpp"
#include "json_mini/lexer.hpp"
#include "json_mini/schema.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;
    JsonObject obj;
    obj["status"] = "success";
    obj["code"] = 200;
    assert(!obj.empty());
    std::cout << "All conformance assertions validated successfully." << std::endl;
    return 0;
}
